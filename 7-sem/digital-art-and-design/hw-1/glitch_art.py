#!/usr/bin/env python3
"""
glitch_art.py — simple CLI to apply glitch-art style distortions to an image.

Usage examples:
  python glitch_art.py input.jpg out_rgb.png --mode rgb_shift --strength 0.6
  python glitch_art.py input.jpg out_wave.png --mode wave --strength 0.4 --seed 42
  python glitch_art.py input.jpg out_slice.png --mode slice --strength 0.8
  python glitch_art.py input.jpg out_jpeg.png --mode jpeg_corrupt --strength 0.5

Requirements: Pillow, numpy
  pip install pillow numpy
"""

import argparse
import io
import math
import os
import random
from typing import Tuple

import numpy as np
from PIL import Image, ImageOps, ImageFile


ImageFile.LOAD_TRUNCATED_IMAGES = True


def clamp01(x: float) -> float:
    return max(0.0, min(1.0, float(x)))


def roll_channel(img: np.ndarray, dx: int, dy: int) -> np.ndarray:
    return np.roll(np.roll(img, dy, axis=0), dx, axis=1)


def rgb_shift(im: Image.Image, strength: float, seed: int = None) -> Image.Image:
    rng = random.Random(seed)
    arr = np.array(im.convert("RGB"))
    h, w, c = arr.shape
    max_shift = max(1, int(2 + 28 * clamp01(strength)))

    shifts = []
    for _ in range(3):
        dx = rng.randint(-max_shift, max_shift)
        dy = rng.randint(-max_shift, max_shift)
        shifts.append((dx, dy))
    r = roll_channel(arr[:, :, 0], *shifts[0])
    g = roll_channel(arr[:, :, 1], *shifts[1])
    b = roll_channel(arr[:, :, 2], *shifts[2])
    out = np.stack([r, g, b], axis=2)
    return Image.fromarray(out)


def slice_displacement(im: Image.Image, strength: float, seed: int = None) -> Image.Image:
    rng = random.Random(seed)
    arr = np.array(im.convert("RGB"))
    h, w, _ = arr.shape
    # number of slices proportional to strength
    n_slices = 6 + int(40 * clamp01(strength))
    max_offset = max(2, int(4 + 60 * clamp01(strength)))
    horizontal_ratio = 0.7

    out = arr.copy()
    for i in range(n_slices):
        if rng.random() < horizontal_ratio:
            # horizontal band
            band_h = rng.randint(max(2, int(h * 0.01)), max(3, int(h * (0.03 + 0.12 * strength))))
            y = rng.randint(0, max(0, h - band_h))
            dx = rng.randint(-max_offset, max_offset)
            out[y:y+band_h, :, :] = np.roll(out[y:y+band_h, :, :], dx, axis=1)
        else:
            # vertical band
            band_w = rng.randint(max(2, int(w * 0.01)), max(3, int(w * (0.03 + 0.12 * strength))))
            x = rng.randint(0, max(0, w - band_w))
            dy = rng.randint(-max_offset, max_offset)
            out[:, x:x+band_w, :] = np.roll(out[:, x:x+band_w, :], dy, axis=0)
    return Image.fromarray(out)


def wave_distortion(im: Image.Image, strength: float, seed: int = None) -> Image.Image:
    rng = random.Random(seed)
    arr = np.array(im.convert("RGB"))
    h, w, _ = arr.shape
    y_coords, x_coords = np.meshgrid(np.arange(h), np.arange(w), indexing="ij")

    A = (0.01 + 0.11 * clamp01(strength)) * min(w, h)
    lam = (0.9 - 0.65 * clamp01(strength)) * min(w, h)
    k = 2 * math.pi / max(1.0, lam)

    phase_x = rng.uniform(0, 2 * math.pi)
    phase_y = rng.uniform(0, 2 * math.pi)

    # displacement fields
    dx = (A * np.sin(k * y_coords + phase_y)).astype(np.float32)
    dy = (A * np.sin(k * x_coords + phase_x)).astype(np.float32)

    # destination coords
    x_new = (x_coords + dx).clip(0, w - 1)
    y_new = (y_coords + dy).clip(0, h - 1)

    # bilinear sampling
    x0 = np.floor(x_new).astype(np.int32)
    x1 = np.clip(x0 + 1, 0, w - 1)
    y0 = np.floor(y_new).astype(np.int32)
    y1 = np.clip(y0 + 1, 0, h - 1)

    wx = (x_new - x0)[..., None]
    wy = (y_new - y0)[..., None]

    c00 = arr[y0, x0]
    c01 = arr[y0, x1]
    c10 = arr[y1, x0]
    c11 = arr[y1, x1]

    top = c00 * (1 - wx) + c01 * wx
    bottom = c10 * (1 - wx) + c11 * wx
    out = (top * (1 - wy) + bottom * wy).astype(np.uint8)

    return Image.fromarray(out)


def jpeg_corrupt(im: Image.Image, strength: float, seed: int = None) -> Image.Image:
    rng = random.Random(seed)

    buf = io.BytesIO()
    im_rgb = im.convert("RGB")
    im_rgb.save(buf, format="JPEG", quality=94, optimize=True, progressive=False, subsampling=0)
    data = bytearray(buf.getvalue())

    def find_marker(seq: bytes, marker: bytes, start=0):
        idx = seq.find(marker, start)
        return idx if idx != -1 else None

    sos = find_marker(data, b"\xFF\xDA")
    eoi = find_marker(data, b"\xFF\xD9", start=(sos or 0) + 2)

    if sos is None or eoi is None or eoi - sos < 100:
        header_guard = 800
        tail_guard = 50
        sos = max(sos or header_guard, header_guard)
        eoi = min(eoi or (len(data) - tail_guard), len(data) - tail_guard)

    start = sos + 2
    end = eoi

    span = max(1, end - start)
    base_ratio = 0.0004 + 0.0048 * clamp01(strength)
    flips_target = max(1, int(span * base_ratio))

    # если декодер падает — уменьшаем интенсивность
    attempts = [1.0, 0.6, 0.35]
    for scale in attempts:
        trial = bytearray(data)  # копия
        flips = max(1, int(flips_target * scale))

        for _ in range(flips):
            idx = rng.randint(start, end - 1)

            if rng.random() < 0.6:
                bit = 1 << rng.randint(0, 7)
                trial[idx] ^= bit
                if trial[idx] == 0xFF:
                    trial[idx] = 0xFE
            else:
                val = rng.randint(0, 255)
                trial[idx] = 0xFE if val == 0xFF else val

        try:
            out = Image.open(io.BytesIO(bytes(trial)))
            out.load()  
            return out.convert("RGB")
        except Exception:
            continue 

    return im_rgb


def apply_mode(im: Image.Image, mode: str, strength: float, seed: int = None) -> Image.Image:
    if mode == "rgb_shift":
        return rgb_shift(im, strength, seed)
    if mode == "slice":
        return slice_displacement(im, strength, seed)
    if mode == "wave":
        return wave_distortion(im, strength, seed)
    if mode == "jpeg_corrupt":
        return jpeg_corrupt(im, strength, seed)
    if mode == "combo":
        # layered look: rgb_shift -> slice -> wave
        im1 = rgb_shift(im, strength * 0.8, seed)
        im2 = slice_displacement(im1, strength * 0.9, seed)
        im3 = wave_distortion(im2, strength, seed)
        return im3
    raise ValueError(f"Unknown mode: {mode}")


def main():
    parser = argparse.ArgumentParser(description="Glitch-art distortions with adjustable strength.")
    parser.add_argument("input", help="Input image path")
    parser.add_argument("output", help="Output image path")
    parser.add_argument("--mode", choices=["rgb_shift", "slice", "wave", "jpeg_corrupt", "combo"], default="combo",
                        help="Type of glitch to apply")
    parser.add_argument("--strength", type=float, default=0.5, help="Glitch strength in [0..1]")
    parser.add_argument("--seed", type=int, default=None, help="Random seed for reproducibility")
    args = parser.parse_args()

    strength = clamp01(args.strength)

    im = Image.open(args.input)
    im = ImageOps.exif_transpose(im)

    out = apply_mode(im, args.mode, strength, args.seed)
    out.save(args.output)
    print(f"[OK] Saved {args.output} ({args.mode}, strength={strength})")


if __name__ == "__main__":
    main()
