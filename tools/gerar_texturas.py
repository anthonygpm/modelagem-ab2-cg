"""
Gera as texturas procedurais usadas pela aplicação.

Uso:  python tools/gerar_texturas.py
Requer: pip install pillow numpy

As imagens vão para assets/textures/. Qualquer uma pode ser trocada por uma
foto real (ex.: foto do piso da sala) mantendo o mesmo nome de arquivo.
"""

import math
import os
import random

import numpy as np
from PIL import Image, ImageDraw, ImageFilter, ImageFont

PASTA = os.path.join(os.path.dirname(__file__), "..", "assets", "textures")
os.makedirs(PASTA, exist_ok=True)
rng = np.random.default_rng(42)
random.seed(42)


def fonte(tamanho, negrito=False, mono=False):
    nomes = (["DejaVuSansMono-Bold.ttf", "DejaVuSansMono.ttf"] if mono else
             ["DejaVuSans-Bold.ttf", "arialbd.ttf"] if negrito else
             ["DejaVuSans.ttf", "arial.ttf"])
    if mono:
        nomes = nomes[1:] + nomes[:1]
    for n in nomes:
        for base in ["", "/usr/share/fonts/truetype/dejavu/", "C:/Windows/Fonts/"]:
            try:
                return ImageFont.truetype(base + n, tamanho)
            except OSError:
                pass
    return ImageFont.load_default()


def salvar(img, nome):
    img.save(os.path.join(PASTA, nome + ".png"))
    print("  ", nome)


def ruido_suave(h, w, escala, oitavas=4):
    """Ruído de valor em várias oitavas (aparência orgânica)."""
    total = np.zeros((h, w))
    amp, peso = 1.0, 0.0
    for o in range(oitavas):
        gh, gw = max(2, int(h / escala * 2 ** o)), max(2, int(w / escala * 2 ** o))
        grade = rng.random((gh, gw))
        camada = np.array(Image.fromarray((grade * 255).astype(np.uint8)).resize((w, h), Image.BICUBIC)) / 255.0
        total += camada * amp
        peso += amp
        amp *= 0.5
    return total / peso


# ---------------------------------------------------------------------------
# Piso de granilite: 2x2 placas de 60 cm (a textura cobre 1,2 m)
# ---------------------------------------------------------------------------
def piso():
    n = 1024
    base = np.ones((n, n, 3)) * np.array([196, 192, 184])
    base += (ruido_suave(n, n, 256)[..., None] - 0.5) * 18
    # pedrinhas do granilite
    for _ in range(9000):
        x, y = rng.integers(0, n, 2)
        r = rng.integers(1, 4)
        c = rng.choice([[90, 88, 85], [240, 238, 232], [150, 130, 110], [60, 60, 62]])
        base[max(0, y - r):y + r, max(0, x - r):x + r] = c
    img = Image.fromarray(np.clip(base, 0, 255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(0.6))
    d = ImageDraw.Draw(img)
    for p in [0, n // 2, n - 1]:   # rejunte
        d.line([(p, 0), (p, n)], fill=(120, 118, 112), width=5)
        d.line([(0, p), (n, p)], fill=(120, 118, 112), width=5)
    salvar(img, "piso")


# ---------------------------------------------------------------------------
# Reboco pintado (parede) — textura neutra, a cor vem do material
# ---------------------------------------------------------------------------
def parede():
    n = 512
    v = 235 + (ruido_suave(n, n, 64) - 0.5) * 16 + rng.normal(0, 3, (n, n))
    img = np.clip(np.stack([v, v, v], -1), 0, 255).astype(np.uint8)
    salvar(Image.fromarray(img), "parede")


# ---------------------------------------------------------------------------
# Forro do teto: placas de 62,5 cm com furinhos (textura cobre 1,25 m)
# ---------------------------------------------------------------------------
def forro():
    n = 512
    v = 240 + rng.normal(0, 4, (n, n))
    img = Image.fromarray(np.clip(np.stack([v, v, v], -1), 0, 255).astype(np.uint8))
    d = ImageDraw.Draw(img)
    for _ in range(2500):
        x, y = rng.integers(0, n, 2)
        d.point((int(x), int(y)), fill=(190, 190, 190))
    for p in [0, n // 2, n - 1]:
        d.line([(p, 0), (p, n)], fill=(175, 175, 172), width=4)
        d.line([(0, p), (n, p)], fill=(175, 175, 172), width=4)
    salvar(img, "forro")


# ---------------------------------------------------------------------------
# Madeira (tampo das mesas) e porta
# ---------------------------------------------------------------------------
def veios(w, h, cor_clara, cor_escura, frequencia, vertical=False):
    y, x = np.mgrid[0:h, 0:w].astype(float)
    eixo, outro = (x, y) if vertical else (y, x)
    distorcao = ruido_suave(h, w, 128) * 40
    anel = np.sin((eixo + distorcao) / h * frequencia * math.pi * 2 + np.sin(outro / 90) * 2)
    t = (anel * 0.5 + 0.5) ** 2.2
    t = t * 0.7 + ruido_suave(h, w, 16, 2) * 0.3
    clara, escura = np.array(cor_clara), np.array(cor_escura)
    img = clara * (1 - t[..., None]) + escura * t[..., None]
    img += rng.normal(0, 4, img.shape)
    return Image.fromarray(np.clip(img, 0, 255).astype(np.uint8))


def madeira():
    salvar(veios(512, 512, [200, 160, 115], [150, 105, 65], 7), "madeira")


def porta():
    img = veios(512, 1024, [150, 98, 58], [100, 60, 32], 5, vertical=True)
    d = ImageDraw.Draw(img)
    # dois painéis rebaixados
    for (y0, y1) in [(80, 470), (550, 960)]:
        d.rectangle([60, y0, 452, y1], outline=(80, 48, 25), width=6)
        d.rectangle([70, y0 + 10, 442, y1 - 10], outline=(175, 125, 80), width=3)
    salvar(img, "porta")


# ---------------------------------------------------------------------------
# Quadro branco com anotações da disciplina
# ---------------------------------------------------------------------------
def quadro():
    w, h = 1800, 600
    img = Image.new("RGB", (w, h), (246, 247, 248))
    # manchas de apagador
    manchas = (ruido_suave(h, w, 200) - 0.5) * 12
    arr = np.array(img).astype(float) - np.clip(manchas, 0, None)[..., None]
    img = Image.fromarray(np.clip(arr, 0, 255).astype(np.uint8))
    d = ImageDraw.Draw(img)
    azul, verm, preto = (25, 55, 170), (190, 30, 30), (30, 30, 35)

    d.text((60, 40), "Computação Gráfica — UFAL", font=fonte(64, True), fill=azul)
    d.line([(60, 120), (900, 120)], fill=azul, width=4)
    d.text((60, 160), "Transformações geométricas", font=fonte(44), fill=preto)
    d.text((90, 230), "M = T(x, y, z) · R(θ) · S(sx, sy, sz)", font=fonte(42), fill=verm)
    d.text((90, 300), "Pivô:  T(p) · R(θ) · T(−p)", font=fonte(40), fill=preto)
    d.text((60, 390), "Iluminação de Phong", font=fonte(44), fill=preto)
    d.text((90, 460), "I = ka·Ia + Σ [ kd(N·L) + ks(R·V)ⁿ ] · Il", font=fonte(40), fill=azul)

    # matriz de translação desenhada
    x0, y0 = 1180, 170
    d.text((x0 - 120, y0 + 120), "T =", font=fonte(48), fill=preto)
    linhas = ["1  0  0  tx", "0  1  0  ty", "0  0  1  tz", "0  0  0  1"]
    for i, l in enumerate(linhas):
        d.text((x0 + 30, y0 + i * 70), l, font=fonte(44), fill=preto)
    d.line([(x0 + 10, y0 - 5), (x0, y0 - 5), (x0, y0 + 280), (x0 + 10, y0 + 280)], fill=preto, width=4)
    d.line([(x0 + 330, y0 - 5), (x0 + 340, y0 - 5), (x0 + 340, y0 + 280), (x0 + 330, y0 + 280)], fill=preto, width=4)

    # eixos 3D
    ox, oy = 1600, 520
    d.line([(ox, oy), (ox + 150, oy)], fill=verm, width=5)
    d.line([(ox, oy), (ox, oy - 150)], fill=(20, 140, 40), width=5)
    d.line([(ox, oy), (ox - 90, oy + 60)], fill=azul, width=5)
    d.text((ox + 155, oy - 25), "x", font=fonte(36), fill=verm)
    d.text((ox - 10, oy - 200), "y", font=fonte(36), fill=(20, 140, 40))
    d.text((ox - 120, oy + 50), "z", font=fonte(36), fill=azul)
    salvar(img, "quadro")


# ---------------------------------------------------------------------------
# Paisagem vista pela janela (céu, nuvens, coqueiros e prédios do campus)
# ---------------------------------------------------------------------------
def paisagem():
    w, h = 2048, 820
    y = np.linspace(0, 1, h)[:, None]
    topo, horizonte = np.array([70, 130, 210]), np.array([185, 215, 240])
    ceu = topo * (1 - y[..., None]) + horizonte * y[..., None]
    ceu = np.repeat(ceu, w, axis=1)
    nuvens = np.clip((ruido_suave(h, w, 260, 5) - 0.55) * 4, 0, 1)
    nuvens *= np.clip(1.2 - y * 1.6, 0, 1)
    ceu = ceu * (1 - nuvens[..., None]) + 250 * nuvens[..., None]
    img = Image.fromarray(np.clip(ceu, 0, 255).astype(np.uint8))
    d = ImageDraw.Draw(img)

    chao = int(h * 0.62)
    # prédios ao fundo
    x = 0
    while x < w:
        lw = random.randint(140, 320)
        lh = random.randint(60, 170)
        tom = random.randint(195, 225)
        d.rectangle([x, chao - lh, x + lw, chao], fill=(tom, tom - 5, tom - 15))
        for jy in range(chao - lh + 15, chao - 10, 28):
            for jx in range(x + 12, x + lw - 20, 34):
                d.rectangle([jx, jy, jx + 18, jy + 12], fill=(110, 140, 165))
        x += lw + random.randint(30, 120)

    # árvores (copas)
    for _ in range(70):
        cx, cy = random.randint(0, w), chao - random.randint(0, 40)
        r = random.randint(35, 80)
        g = random.randint(80, 120)
        d.ellipse([cx - r, cy - r, cx + r, cy + r // 2], fill=(40, g, 45))

    # gramado
    d.rectangle([0, chao, w, h], fill=(85, 135, 60))
    grama = np.array(img).astype(float)
    grama[chao:] += rng.normal(0, 8, grama[chao:].shape)
    img = Image.fromarray(np.clip(grama, 0, 255).astype(np.uint8))
    d = ImageDraw.Draw(img)

    # coqueiros
    for cx in [260, 700, 1350, 1780]:
        base_y = chao + random.randint(10, 60)
        altura = random.randint(300, 380)
        topo_x = cx + random.randint(-40, 40)
        d.line([(cx, base_y), (topo_x, base_y - altura)], fill=(120, 95, 70), width=16)
        for ang in range(0, 360, 30):
            a = math.radians(ang)
            comp = random.randint(110, 160)
            ex = topo_x + math.cos(a) * comp
            ey = base_y - altura + abs(math.sin(a)) * 50 + math.sin(a) * 30
            mx = (topo_x + ex) / 2
            my = base_y - altura - 25
            d.line([(topo_x, base_y - altura), (mx, my), (ex, ey)], fill=(35, 100, 40), width=10, joint="curve")
    salvar(img.filter(ImageFilter.GaussianBlur(0.8)), "paisagem")


# ---------------------------------------------------------------------------
# Tela do notebook (editor de código) e slide da projeção
# ---------------------------------------------------------------------------
def tela_notebook():
    w, h = 620, 400
    img = Image.new("RGB", (w, h), (30, 32, 40))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, w, 26], fill=(45, 48, 60))
    d.text((12, 5), "main.cpp — sala-de-aula-3d", font=fonte(14), fill=(200, 200, 210))
    codigo = [
        ("#include", "<glad/gl.h>"),
        ("", ""),
        ("void", "Porta::desenhar(Renderer& r) {"),
        ("  auto", "F = base() * translacao(pivo)"),
        ("", "        * rotacao(angulo, EIXO_Y);"),
        ("  r.", "cubo(peca(F, centro, tam), madeira);"),
        ("}", ""),
        ("", ""),
        ("// Phong", ": ambiente + difusa + especular"),
        ("float", "nl = max(dot(N, L), 0.0);"),
        ("vec3", "R = reflect(-L, N);"),
        ("float", "es = pow(max(dot(V, R), 0.0), n);"),
    ]
    f = fonte(17, mono=True)
    for i, (k, resto) in enumerate(codigo):
        y = 40 + i * 28
        d.text((10, y), f"{i + 1:>2}", font=f, fill=(90, 95, 110))
        d.text((50, y), k, font=f, fill=(200, 120, 220))
        d.text((50 + len(k) * 10 + 8, y), resto, font=f, fill=(170, 210, 160) if k.startswith("//") else (220, 220, 225))
    salvar(img, "tela_notebook")


def slide():
    w, h = 1440, 1020
    img = Image.new("RGB", (w, h), (250, 250, 252))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, w, 180], fill=(18, 60, 130))
    d.text((70, 50), "Modelagem 3D Interativa com OpenGL", font=fonte(54, True), fill=(255, 255, 255))
    itens = ["Transformações: translação, rotação e escala",
             "Iluminação de Phong com materiais por objeto",
             "Mapeamento de textura",
             "Navegação em primeira pessoa e colisão",
             "Interações: porta, luz, persiana, projetor"]
    for i, t in enumerate(itens):
        y = 270 + i * 120
        d.ellipse([90, y + 18, 116, y + 44], fill=(18, 60, 130))
        d.text((150, y), t, font=fonte(48), fill=(35, 35, 45))
    d.rectangle([0, h - 70, w, h], fill=(230, 232, 238))
    d.text((70, h - 55), "Computação Gráfica — Instituto de Computação — UFAL", font=fonte(32), fill=(70, 75, 90))
    salvar(img, "slide")


if __name__ == "__main__":
    print("Gerando texturas em", os.path.abspath(PASTA))
    piso()
    parede()
    forro()
    madeira()
    porta()
    quadro()
    paisagem()
    tela_notebook()
    slide()
