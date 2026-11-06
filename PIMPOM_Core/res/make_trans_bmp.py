"""幾何変換ウィンドウの説明図（PIMPOM.rc の IDB_TRANSFORM、res\\trans.bmp）を生成する。
左に元画像、右に仮想カメラで撮り直した変換後の画像を並べ、下にパラメータの意味を書く。
変換後の画像は、左の画像を実際に射影変換して作る。
ダイアログの描画領域（287×127 DLU、横長）に合わせた比率で、125〜150% 表示でもにじまないよう 1.5 倍で描く。
OnPaint が縦横比を保って領域に縮めて貼る。
実行: python make_trans_bmp.py（Pillow と numpy、Windows 標準の Meiryo UI フォントが必要）"""
import os
import numpy as np
from PIL import Image, ImageDraw, ImageFont

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "trans.bmp")
W, H = 516, 242          # 96dpi 基準の大きさ（描画領域の比率）
SCALE = 1.5              # 出力の倍率
SS = 3                   # なめらかに描くための拡大率（最後に縮める）
K = SCALE * SS

FONT = r"C:\Windows\Fonts\meiryo.ttc"
INK = (44, 44, 42)
SUB = (95, 94, 90)
FRAME = (24, 95, 165)
SKY = (230, 241, 251)
SUN = (133, 183, 235)
RED = (153, 60, 29)
GREEN = (15, 110, 86)
LINE = (211, 209, 199)

def font(size):
    return ImageFont.truetype(FONT, int(size * K), index=2)  # index 2 = Meiryo UI

def P(x, y):
    return (x * K, y * K)

def Pi(x, y):
    return (int(round(x * K)), int(round(y * K)))

def text(d, x, y, s, size=11, fill=INK):
    d.text(P(x, y), s, font=font(size), fill=fill)

def arrow(d, x0, y0, x1, y1, fill=INK, w=1.2):
    d.line([P(x0, y0), P(x1, y1)], fill=fill, width=int(w * K))
    v = np.array([x1 - x0, y1 - y0], float); v /= np.linalg.norm(v); n = np.array([-v[1], v[0]])
    tip = np.array([x1, y1]); a = tip - v * 7 + n * 4; b = tip - v * 7 - n * 4
    d.polygon([P(*tip), P(*a), P(*b)], fill=fill)

def scene(w, h):
    """元画像の中身（空・山・太陽）。w×h は描く画素数"""
    im = Image.new("RGB", (w, h), SKY)
    d = ImageDraw.Draw(im)
    s = w / 196
    d.ellipse([150 * s, 16 * s, 172 * s, 38 * s], fill=SUN)
    d.polygon([(0, h), (0, 96 * s), (52 * s, 40 * s), (86 * s, 76 * s), (112 * s, 52 * s), (196 * s, 104 * s), (196 * s, h)],
              fill=(181, 212, 244))
    d.line([(0, 96 * s), (52 * s, 40 * s), (86 * s, 76 * s), (112 * s, 52 * s), (196 * s, 104 * s)], fill=FRAME, width=int(2.4 * s), joint="curve")
    return im

def perspective_coeffs(dst, src):
    """dst の四隅を src の四隅へ写す係数（PIL の PERSPECTIVE は出力→入力の写像）"""
    A, b = [], []
    for (x, y), (u, v) in zip(dst, src):
        A.append([x, y, 1, 0, 0, 0, -u * x, -u * y]); b.append(u)
        A.append([0, 0, 0, x, y, 1, -v * x, -v * y]); b.append(v)
    return np.linalg.solve(np.array(A, float), np.array(b, float)).tolist()

im = Image.new("RGB", (int(W * K), int(H * K)), (255, 255, 255))
d = ImageDraw.Draw(im)

# 左: 元画像
L, T, PW, PH = 14, 28, 196, 112
text(d, L, 8, "元画像", 12)
src = scene(int(PW * K), int(PH * K))
im.paste(src, Pi(L, T))
d.rectangle([P(L, T), P(L + PW, T + PH)], outline=FRAME, width=int(1.5 * K))
cx, cy = L + PW / 2, T + PH / 2
arrow(d, cx, cy, cx + 64, cy, INK, 1.2)
arrow(d, cx, cy, cx, cy - 46, INK, 1.2)
d.ellipse([P(cx - 2.5, cy - 2.5), P(cx + 2.5, cy + 2.5)], fill=INK)
text(d, cx + 62, cy - 18, "X", 11)
text(d, cx + 5, cy - 52, "Y", 11)
text(d, L, T + PH + 4, "中心が原点、1 画素 = 1 mm", 10, SUB)

# 中央: 仮想カメラ
mx = L + PW + 12
arrow(d, mx, cy, mx + 72, cy, INK, 1.6)
d.rounded_rectangle([P(mx + 18, cy - 30), P(mx + 46, cy - 10)], radius=3 * K, fill=SUB)
d.rounded_rectangle([P(mx + 46, cy - 25), P(mx + 54, cy - 15)], radius=1.5 * K, fill=SUB)
text(d, mx + 2, cy + 8, "仮想カメラで", 10, SUB)
text(d, mx + 2, cy + 22, "撮り直す", 10, SUB)

# 右: 変換後の画像（左の画像を実際に射影変換する）
R = mx + 82
text(d, R, 8, "変換後の画像", 12)
d.rectangle([P(R, T), P(R + PW, T + PH)], fill=INK)
quad = [(R + 24, T + 12), (R + PW - 26, T + 22), (R + PW - 14, T + PH - 8), (R + 14, T + PH - 16)]
dst = [P(x, y) for x, y in quad]
sw, sh = src.size
coeffs = perspective_coeffs([(x - Pi(R, T)[0], y - Pi(R, T)[1]) for x, y in dst], [(0, 0), (sw, 0), (sw, sh), (0, sh)])
warped = src.transform((int(PW * K), int(PH * K)), Image.PERSPECTIVE, coeffs, Image.BICUBIC)
mask = Image.new("L", warped.size, 0)
ImageDraw.Draw(mask).polygon([(x - Pi(R, T)[0], y - Pi(R, T)[1]) for x, y in dst], fill=255)
im.paste(warped, Pi(R, T), mask)
d.polygon(dst, outline=SUN, width=int(1.2 * K))

# 下: パラメータの意味
y0 = T + PH + 26
d.line([P(L, y0 - 6), P(R + PW, y0 - 6)], fill=LINE, width=int(1 * K))
text(d, L, y0, "回転 θ（Z 軸）・ψ（Y 軸）・φ（X 軸）で傾ける", 11, RED)
text(d, L, y0 + 19, "移動 TX, TY [mm]／撮像素子ずれ TI, TJ [pix] でずらす", 11, GREEN)
text(d, L, y0 + 38, "焦点距離 f と撮像素子サイズ a で台形のゆがみの強さが変わる", 11, INK)
text(d, L, y0 + 57, "はみ出した画素を埋める：OFF は黒、ON は端の画素で埋める", 10, SUB)

out = im.resize((int(W * SCALE), int(H * SCALE)), Image.LANCZOS)
out.quantize(colors=256, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE).save(OUT)  # 8 ビット BMP（リソースを小さくする）
print("wrote", OUT, out.size)
