import os
import glob
import math
from PIL import Image, ImageDraw

BASE_DIR = r"E:\Saint Seiya\GITHUB\DATA"
LOADING_PNG = os.path.join(BASE_DIR, "bgs", "loading.png")
OUTPUT_GIF = os.path.join(BASE_DIR, "bgs", "loading.gif")

# Ordre d'affichage de gauche à droite : Ikki, Shiryu, Shun, Seiya, Hyoga
CHARS_CONFIG = [
    ("Ikki",   "ikki",   125, 392),
    ("Shiryu", "shiryu", 270, 392),
    ("Shun",   "shun",   425, 392),
    ("Seiya",  "seiya",  575, 392),
    ("Hyoga",  "hyoga",  720, 392),
]

SCALE = 1.0        # Taille d'origine 1:1 (ne touche plus l'horloge)
TOTAL_FRAMES = 12  # Nombre d'images dans le GIF final
FRAME_DELAY = 80   # Vitesse en ms par image

def find_char_txt(folder_name):
    folder = os.path.join(BASE_DIR, "chars", folder_name)
    if not os.path.isdir(folder):
        return None
    txts = sorted(glob.glob(os.path.join(folder, "*.txt")))
    for t in txts:
        if os.path.basename(t).startswith("00_"):
            return t
    return txts[0] if txts else None

def parse_anim(txt_path, target_anim="run"):
    frames = []
    in_anim = False
    current_offset = (0, 0)
    with open(txt_path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            clean = line.split("#")[0].strip()
            if not clean:
                continue
            low = clean.lower()
            if low.startswith("anim "):
                if low == f"anim {target_anim}" or low.startswith(f"anim {target_anim} "):
                    in_anim = True
                    current_offset = (0, 0)
                elif in_anim:
                    break
            elif in_anim:
                parts = clean.split()
                if parts[0].lower() == "offset" and len(parts) >= 3:
                    current_offset = (int(parts[1]), int(parts[2]))
                elif parts[0].lower() == "frame" and len(parts) >= 2:
                    rel_path = parts[1].replace("/", os.sep).replace("\\", os.sep)
                    if rel_path.lower().startswith("data" + os.sep):
                        rel_path = rel_path[5:]
                    img_path = os.path.join(BASE_DIR, rel_path)
                    frames.append((img_path, current_offset))
    return frames

def load_sprite_rgba(img_path):
    raw = Image.open(img_path)
    im = raw.convert("RGBA")
    # Détecte la couleur de fond transparente en haut à gauche (0, 0)
    bg_color = im.getpixel((0, 0))[:3]
    datas = im.getdata()
    new_data = []
    for item in datas:
        if item[:3] == bg_color:
            new_data.append((0, 0, 0, 0))
        else:
            new_data.append((item[0], item[1], item[2], 255))
    im.putdata(new_data)
    return im

def animate_clock_flames(base_img, f_idx, total_frames):
    """Fait vaciller les 12 flammes bleues autour de l'horloge."""
    img = base_img.copy()
    pixels = img.load()
    # Zone de l'horloge du Sanctuaire (x: 315..550, y: 105..285)
    for y in range(105, 285):
        for x in range(315, 550):
            r, g, b, a = pixels[x, y]
            # Repère les pixels des flammes bleues (bleu fort, rouge faible)
            if b > 140 and b > r + 50:
                # Variation ondulante selon la position et la frame
                wave = math.sin((f_idx / total_frames) * 2 * math.pi + (x + y) * 0.12)
                factor = 1.0 + 0.35 * wave
                nr = min(255, max(0, int(r * factor + (40 if wave > 0.4 else 0))))
                ng = min(255, max(0, int(g * factor + (30 if wave > 0.4 else 0))))
                nb = min(255, max(80, int(b * factor)))
                pixels[x, y] = (nr, ng, nb, a)
    return img

def main():
    if not os.path.exists(LOADING_PNG):
        print(f"[ERREUR] Image source introuvable : {LOADING_PNG}")
        return

    base = Image.open(LOADING_PNG).convert("RGBA")
    # Efface la bande des anciens chevaliers statiques (y=295 à y=415)
    draw = ImageDraw.Draw(base)
    draw.rectangle([0, 295, base.width, 415], fill=(0, 0, 0, 255))

    char_anims = []
    for name, folder, pos_x, pos_y in CHARS_CONFIG:
        txt_path = find_char_txt(folder)
        if not txt_path:
            print(f"[ATTENTION] Dossier introuvable : chars\\{folder}")
            continue
        anim = parse_anim(txt_path, "run") or parse_anim(txt_path, "walk")
        if not anim:
            print(f"[ATTENTION] Aucune 'anim run' dans {txt_path}")
            continue
        loaded = [(load_sprite_rgba(p), off) for p, off in anim if os.path.exists(p)]
        char_anims.append((name, loaded, pos_x, pos_y))
        print(f"[OK] {name} : {len(loaded)} frames chargées.")

    gif_frames = []
    for f_idx in range(TOTAL_FRAMES):
        canvas = animate_clock_flames(base, f_idx, TOTAL_FRAMES)
        for idx, (name, loaded, pos_x, pos_y) in enumerate(char_anims):
            sprite, (ox, oy) = loaded[(f_idx + idx * 2) % len(loaded)]
            if SCALE != 1.0:
                new_w = int(sprite.width * SCALE)
                new_h = int(sprite.height * SCALE)
                sprite = sprite.resize((new_w, new_h), Image.NEAREST)
            paste_x = pos_x - int(ox * SCALE)
            paste_y = pos_y - int(oy * SCALE)
            canvas.alpha_composite(sprite, (paste_x, paste_y))

        pal_frame = canvas.convert("RGB").quantize(colors=256, method=Image.Quantize.MEDIANCUT)
        gif_frames.append(pal_frame)

    gif_frames[0].save(
        OUTPUT_GIF,
        save_all=True,
        append_images=gif_frames[1:],
        duration=FRAME_DELAY,
        loop=0,
        optimize=False
    )
    print(f"\nSuccès ! Fichier généré : {OUTPUT_GIF}")

if __name__ == "__main__":
    main()
