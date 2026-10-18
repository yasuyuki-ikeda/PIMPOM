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
    d.ellipse([52, 52, 204, 204], outline=INK, width=SW)
    d.pieslice([52, 52, 204, 204], 90, 270, fill=INK)
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
@icon("zoomin")
def _(d): line(d, [(128, 40), (128, 216)], 30); line(d, [(40, 128), (216, 128)], 30)
@icon("zoomout")
def _(d): line(d, [(40, 128), (216, 128)], 30)
@icon("camera")
def _(d):
    d.rounded_rectangle([28, 76, 172, 188], 18, outline=INK, width=SW)
    d.polygon([(184, 112), (228, 84), (228, 180), (184, 152)], fill=INK)
@icon("camera_run")
def _(d):
    ICONS["camera"](d)
    d.polygon([(84, 104), (132, 132), (84, 160)], fill=(0, 103, 192, 255))

for name, f in ICONS.items():
    im, d = canvas()
    d._image = im
    f(d)
    save(name, im)
print(len(ICONS), "icons ->", OUT)
