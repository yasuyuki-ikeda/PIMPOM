"""メイン画面右端のボタン用アイコン（PIMPOM.rc の IDI_BTN_*）（線画・透過付き・複数サイズ .ico）を生成する。
高解像度(256px)で描いて縮小し、16〜64px を 1 つの .ico に収める。
実行: python make_icons.py（Pillow と Windows 標準の Segoe UI フォントが必要）"""
import os, math
from PIL import Image, ImageDraw, ImageFont

OUT = os.path.dirname(os.path.abspath(__file__))
os.makedirs(OUT, exist_ok=True)
N = 256                      # 描画キャンバス
SIZES = [(16, 16), (20, 20), (24, 24), (32, 32), (40, 40), (48, 48), (64, 64)]
INK = (45, 45, 48, 255)      # 線の色
SW = 18                      # 線幅（256px 基準）
FONT_B = r"C:\Windows\Fonts\segoeuib.ttf"
FONT_SYM = r"C:\Windows\Fonts\seguisym.ttf"

def canvas():
    im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
    return im, ImageDraw.Draw(im)

def line(d, pts, w=SW, fill=INK):
    d.line(pts, fill=fill, width=w, joint="curve")
    for p in (pts[0], pts[-1]):
        d.ellipse([p[0] - w / 2, p[1] - w / 2, p[0] + w / 2, p[1] + w / 2], fill=fill)

def text(d, s, size, fill=INK, font=FONT_B, dy=0):
    f = ImageFont.truetype(font, size)
    b = d.textbbox((0, 0), s, font=f)
    w, h = b[2] - b[0], b[3] - b[1]
    d.text(((N - w) / 2 - b[0], (N - h) / 2 - b[1] + dy), s, font=f, fill=fill)

def dot(d, color, r=78, cx=N / 2, cy=N / 2):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=color)

def save(name, im):
    im.save(os.path.join(OUT, name + ".ico"), sizes=SIZES)

ICONS = {}
def icon(name):
    def deco(f):
        ICONS[name] = f
        return f
    return deco

# --- 動画（3D）---------------------------------------------------------
@icon("play")
def _(d): d.polygon([(84, 56), (204, 128), (84, 200)], fill=INK)
@icon("next")
def _(d): line(d, [(100, 56), (172, 128), (100, 200)], 22)
@icon("prev")
def _(d): line(d, [(156, 56), (84, 128), (156, 200)], 22)
@icon("stop")
def _(d): d.rounded_rectangle([70, 70, 186, 186], 14, fill=INK)

# --- 複素数・XY・XYZ ---------------------------------------------------
@icon("re")
def _(d): text(d, "Re", 130)
@icon("im")
def _(d): text(d, "Im", 130)
@icon("abs")
def _(d): text(d, "|z|", 140)
@icon("arg")
def _(d):
    line(d, [(56, 196), (204, 196)]); line(d, [(56, 196), (176, 68)])
    d.arc([0, 140, 112, 252], 300, 360, fill=INK, width=SW)
@icon("x")
def _(d): text(d, "X", 170)
@icon("y")
def _(d): text(d, "Y", 170)
@icon("z")
def _(d): text(d, "Z", 170)
@icon("cloud")
def _(d):
    # 立方体＋点群
    f, b = [(64, 104), (160, 104), (160, 200), (64, 200)], [(104, 64), (200, 64), (200, 160), (104, 160)]
    for i in range(4):
        line(d, [f[i], f[(i + 1) % 4]], 12); line(d, [b[i], b[(i + 1) % 4]], 12); line(d, [f[i], b[i]], 12)
    for (x, y) in [(96, 140), (124, 128), (132, 170), (150, 118), (112, 176)]:
        dot(d, (0, 103, 192, 255), 13, x, y)

# --- RGB --------------------------------------------------------------
@icon("r")
def _(d): dot(d, (229, 72, 77, 255))
@icon("g")
def _(d): dot(d, (48, 164, 108, 255))
@icon("b")
def _(d): dot(d, (62, 99, 221, 255))
@icon("rgb")
def _(d):
    for a, c in [(90, (229, 72, 77, 220)), (210, (48, 164, 108, 220)), (330, (62, 99, 221, 220))]:
        cx, cy = 128 + 38 * math.cos(math.radians(a)), 128 - 38 * math.sin(math.radians(a))
        layer = Image.new("RGBA", (N, N), (0, 0, 0, 0)); ImageDraw.Draw(layer).ellipse([cx - 62, cy - 62, cx + 62, cy + 62], fill=c)
        d._image.alpha_composite(layer)

# --- 表示・解析ツール ---------------------------------------------------
@icon("mark")
def _(d):
    for p in ([(128, 36), (128, 96)], [(128, 160), (128, 220)], [(36, 128), (96, 128)], [(160, 128), (220, 128)]):
        line(d, p)
    d.ellipse([112, 112, 144, 144], fill=INK)
@icon("workarea")
def _(d):
    x0, y0, x1, y1 = 48, 64, 208, 192
    segs = []
    for (a, b) in [((x0, y0), (x1, y0)), ((x1, y0), (x1, y1)), ((x1, y1), (x0, y1)), ((x0, y1), (x0, y0))]:
        n = 3
        for k in range(n):
            t0, t1 = k / n + 0.06, k / n + 0.24
            segs.append([(a[0] + (b[0] - a[0]) * t0, a[1] + (b[1] - a[1]) * t0), (a[0] + (b[0] - a[0]) * t1, a[1] + (b[1] - a[1]) * t1)])
    for s in segs: line(d, s, 16)
@icon("mask")
def _(d):
    # 画像の枠の外側を塗りつぶし、自由な形の部分だけをくりぬいて中の山の絵を見せる（画像の一部の領域を指定する）
    d.rounded_rectangle([28, 40, 228, 216], 16, fill=INK)
    blob = []
    for k in range(72):
        t = 2 * math.pi * k / 72
        r = 1 + 0.10 * math.sin(3 * t + 0.6) + 0.05 * math.cos(2 * t)
        blob.append((130 + 66 * r * math.cos(t), 126 + 58 * r * math.sin(t)))
    d.polygon(blob, fill=(0, 0, 0, 0))
    d.rounded_rectangle([28, 40, 228, 216], 16, outline=INK, width=SW)
    line(d, [(86, 160), (112, 122), (130, 142), (146, 126), (172, 160)], 16)
@icon("histogram")
def _(d):
    line(d, [(40, 212), (216, 212)], 14)
    for x, h in [(64, 60), (100, 120), (136, 168), (172, 100), (204, 44)]:
        d.rounded_rectangle([x - 14, 200 - h, x + 14, 200], 6, fill=INK)
@icon("ruler")
def _(d):
    d.polygon([(40, 168), (168, 40), (216, 88), (88, 216)], outline=INK, width=16)
    for k in range(1, 5):
        x, y = 40 + k * 26, 168 - k * 26
        line(d, [(x + 6, y + 6), (x + 26, y + 26)], 12)
@icon("profile")
def _(d):
    line(d, [(36, 212), (220, 212)], 14)
    line(d, [(40, 176), (84, 104), (124, 150), (168, 60), (216, 120)], SW)
@icon("pen")
def _(d):
    d.polygon([(64, 216), (40, 216), (40, 192), (168, 64), (192, 88)], outline=INK, width=16)
    line(d, [(152, 80), (176, 104)], 16); line(d, [(176, 56), (200, 80)], 16)
@icon("fx")
def _(d): text(d, "f(x)", 104, font=r"C:\Windows\Fonts\segoeuiz.ttf" if os.path.exists(r"C:\Windows\Fonts\segoeuiz.ttf") else FONT_B)
# --- 表示倍率・ライブ ---------------------------------------------------
@icon("fit")
def _(d):
    for p in ([(48, 100), (48, 48), (100, 48)], [(156, 48), (208, 48), (208, 100)], [(48, 156), (48, 208), (100, 208)], [(208, 156), (208, 208), (156, 208)]):
        line(d, p)
@icon("one")
def _(d): text(d, "1:1", 118)
def loupe(d):
    """拡大・縮小のルーペ（レンズと柄）"""
    d.ellipse([36, 36, 180, 180], outline=INK, width=22)
    line(d, [(164, 164), (222, 222)], 30)
@icon("zoomin")
def _(d):
    loupe(d); line(d, [(72, 108), (144, 108)], 22); line(d, [(108, 72), (108, 144)], 22)
@icon("zoomout")
def _(d):
    loupe(d); line(d, [(72, 108), (144, 108)], 22)
@icon("camera")
def _(d):
    d.rounded_rectangle([28, 76, 172, 188], 18, outline=INK, width=SW)
    d.polygon([(184, 112), (228, 84), (228, 180), (184, 152)], fill=INK)
@icon("camera_run")
def _(d):
    ICONS["camera"](d)
    d.polygon([(84, 104), (132, 132), (84, 160)], fill=(0, 103, 192, 255))

# --- ツールバー（段階2） -------------------------------------------------
@icon("open")
def _(d):
    d.polygon([(28, 72), (100, 72), (120, 92), (228, 92), (228, 204), (28, 204)], outline=INK, width=SW)
@icon("save")
def _(d):
    d.polygon([(44, 36), (176, 36), (212, 72), (212, 220), (44, 220)], outline=INK, width=SW)
    d.rectangle([84, 36, 164, 92], outline=INK, width=14)
    d.rectangle([80, 140, 176, 220], outline=INK, width=14)
@icon("undo")
def _(d):
    d.arc([64, 76, 216, 220], 270, 90, fill=INK, width=SW)
    line(d, [(140, 76), (76, 76)])
    line(d, [(110, 40), (70, 76), (110, 112)])
@icon("filter")
def _(d):
    # 画像（山の絵）の右下に 3x3 の升目（カーネル）を重ねる。升目の下は透明に抜いて画像の線と分ける
    d.rounded_rectangle([24, 36, 192, 172], 14, outline=INK, width=SW)
    line(d, [(42, 154), (88, 102), (120, 132), (144, 112), (174, 150)], SW)
    d.rounded_rectangle([118, 118, 242, 242], 14, fill=(0, 0, 0, 0))
    d.rounded_rectangle([128, 128, 232, 232], 10, outline=INK, width=SW)
    for k in (1, 2):
        v = 128 + 104 * k // 3
        d.line([(v, 132), (v, 228)], fill=INK, width=12)
        d.line([(132, v), (228, v)], fill=INK, width=12)
@icon("binarize")
def _(d):
    # 画像（山の絵）の左半分は線画のまま、右半分は山を塗りつぶして白黒にした様子
    d.polygon([(128, 216), (128, 140), (152, 112), (180, 148), (196, 132), (226, 170), (226, 216)], fill=INK)
    line(d, [(40, 196), (76, 132), (104, 164), (128, 140)], SW)
    d.rounded_rectangle([28, 40, 228, 216], 16, outline=INK, width=SW)
@icon("transform")
def _(d):
    # 画像（山の絵）の枠を斜めから見た四角形にゆがめる（射影変換）
    corners = [(64, 44), (200, 28), (232, 224), (28, 200)]
    line(d, corners + corners[:2], SW)
    line(d, [(52, 180), (96, 116), (128, 150), (152, 124), (196, 184)], SW)

for name, f in ICONS.items():
    im, d = canvas()
    d._image = im
    f(d)
    save(name, im)
print(len(ICONS), "icons ->", OUT)
