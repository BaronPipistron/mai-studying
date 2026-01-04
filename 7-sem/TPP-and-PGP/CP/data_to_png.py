import os
import glob
import struct
from PIL import Image

def convert_one(in_path: str, out_path: str) -> None:
    with open(in_path, "rb") as f:
        hdr = f.read(8)
        if len(hdr) != 8:
            raise RuntimeError("file too small (no header)")
        w, h = struct.unpack("<ii", hdr)
        if w <= 0 or h <= 0 or w > 20000 or h > 20000:
            raise RuntimeError(f"bad dimensions: {w}x{h}")

        expected = w * h * 4
        data = f.read()

    if len(data) != expected:
        raise RuntimeError(f"bad pixel data size: got {len(data)}, expected {expected}")

    img = Image.frombytes("RGBA", (w, h), data)
    img.save(out_path, "PNG")

def main():
    in_dir = "out_test"
    out_dir = "frames"   # можешь поменять на "out" если хочешь рядом

    os.makedirs(out_dir, exist_ok=True)

    files = sorted(glob.glob(os.path.join(in_dir, "*.data")))
    if not files:
        print(f"No .data files found in '{in_dir}'")
        return

    ok = 0
    bad = 0
    for p in files:
        base = os.path.splitext(os.path.basename(p))[0]
        out_path = os.path.join(out_dir, base + ".png")
        try:
            convert_one(p, out_path)
            ok += 1
        except Exception as e:
            bad += 1
            print(f"FAIL: {p} -> {e}")

    print(f"Done. OK={ok}, FAIL={bad}. Output: {out_dir}")

if __name__ == "__main__":
    main()
