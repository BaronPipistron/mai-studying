#!/usr/bin/env python3
"""
convert_img.py — конвертация между .data (w,h + RGBA) и .png

Формат .data:
- Первые 8 байт: два 32-битных целых (little-endian) — ширина (w), высота (h).
- Далее w*h*4 байта RGBA по пикселям в порядке строк (j от 0..h-1, i от 0..w-1).

Режимы:
  --to-png  : .data → .png
  --to-bin  : .png  → .data (альфа устанавливается в 255)

Примеры:
  1) PNG → DATA с явным выходом:
     python convert_img.py --to-bin image.png -o image.data

  2) PNG → DATA с автогенерацией имени (image.data):
     python convert_img.py --to-bin image.png

  3) DATA → PNG с явным выходом:
     python convert_img.py --to-png frame.data -o frame.png

  4) DATA → PNG с автогенерацией имени (frame.png):
     python convert_img.py --to-png frame.data
"""

import argparse
import numpy as np
import os
import struct
import ctypes
from PIL import Image


def data_to_png(in_path: str, out_path: str | None, palette=None):
    # Палитра по умолчанию для 0..7 классов (добавь свои при желании)
    if palette is None:
        palette = [
            (255,   0,   0),  # 0 red
            (  0, 255,   0),  # 1 green
            (  0,   0, 255),  # 2 blue
            (255, 255,   0),  # 3 yellow
            (255,   0, 255),  # 4 magenta
            (  0, 255, 255),  # 5 cyan
            (255, 128,   0),  # 6 orange
            (128,   0, 255),  # 7 violet
        ]

    with open(in_path, 'rb') as f:
        header = f.read(8)
        if len(header) != 8:
            raise ValueError("Нет заголовка 8 байт (w,h).")
        # Явно little-endian, чтобы не было сюрпризов
        w, h = struct.unpack('<ii', header)
        expected = 4 * w * h
        buf = f.read(expected)
        if len(buf) != expected:
            raise ValueError(f"Ожидалось {expected} байт пикселей, прочитано {len(buf)}.")

    arr = np.frombuffer(buf, dtype=np.uint8).reshape((h, w, 4))
    classes = arr[..., 3]  # A = метка класса
    out = np.zeros((h, w, 4), dtype=np.uint8)

    # Раскрасим по палитре
    for cls_id, color in enumerate(palette):
        mask = (classes == cls_id)
        out[mask, 0] = color[0]
        out[mask, 1] = color[1]
        out[mask, 2] = color[2]
        out[mask, 3] = 255

    # На всякий случай: всё, что вне известных классов → белый
    mask_unknown = np.ones((h, w), dtype=bool)
    for cls_id in range(len(palette)):
        mask_unknown &= (classes != cls_id)
    out[mask_unknown] = (255, 255, 255, 255)

    if not out_path:
        base, _ = os.path.splitext(in_path)
        out_path = base + '_classes.png'

    Image.fromarray(out, 'RGBA').save(out_path)
    print(f"OK: {in_path} → {out_path} ({w}x{h})")



def png_to_data(in_path: str, out_path: str | None):
    img = Image.open(in_path).convert('RGBA')
    w, h = img.size
    pix = img.load()

    buff = ctypes.create_string_buffer(4 * w * h)
    offset = 0
    for j in range(h):
        for i in range(w):
            r, g, b, a = pix[i, j]
            struct.pack_into('BBBB', buff, offset, r, g, b, a)
            offset += 4

    if not out_path:
        base, _ = os.path.splitext(in_path)
        out_path = base + '.data'
    with open(out_path, 'wb') as out:
        out.write(struct.pack('ii', w, h)) 
        out.write(buff.raw)

    print(f"OK: {in_path} → {out_path} ({w}x{h}, {4*w*h} байт пикселей)")


def main():
    parser = argparse.ArgumentParser(
        description="Конвертация между .data (w,h + RGBA) и .png."
    )
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--to-png', action='store_true', help='Перевод .data → .png')
    mode.add_argument('--to-bin', action='store_true', help='Перевод .png → .data')
    parser.add_argument('input', help='Путь к входному файлу')
    parser.add_argument('-o', '--output', help='Путь к выходному файлу (необязательно)')

    args = parser.parse_args()

    if args.to_png:
        data_to_png(args.input, args.output)
    elif args.to_bin:
        png_to_data(args.input, args.output)


if __name__ == '__main__':
    main()
