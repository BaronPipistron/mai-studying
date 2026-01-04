import sys
import numpy as np
import matplotlib.pyplot as plt

def is_float(s: str) -> bool:
    try:
        float(s)
        return True
    except:
        return False

def read_input(path: str):
    tokens = open(path, "r", encoding="utf-8").read().split()
    it = 0

    def next_token():
        nonlocal it
        if it >= len(tokens):
            return None
        t = tokens[it]
        it += 1
        return t

    def next_int():
        t = next_token()
        if t is None: raise ValueError("unexpected EOF")
        return int(float(t))

    def next_float():
        t = next_token()
        if t is None: raise ValueError("unexpected EOF")
        return float(t)

    frames = next_int()
    out_pattern = next_token()
    w = next_int()
    h = next_int()
    fov = float(next_token())

    cam = [next_float() for _ in range(20)]
    (r0c, z0c, phi0c, arc, azc, wrc, wzc, wphic, prc, pzc,
     r0n, z0n, phi0n, arn, azn, wrn, wzn, wphin, prn, pzn) = cam

    figs = []
    for _ in range(3):
        cx, cy, cz = next_float(), next_float(), next_float()
        cr, cg, cb = next_float(), next_float(), next_float()
        radius = next_float()

        # у тебя после radius идут ещё числа (0.0 0.0 0) — просто проглотим до 3 чисел
        extra = []
        for __ in range(3):
            if it < len(tokens) and is_float(tokens[it]):
                extra.append(next_float())
            else:
                break

        figs.append({
            "c": np.array([cx, cy, cz], dtype=float),
            "rgb": np.array([cr, cg, cb], dtype=float),
            "r": radius
        })

    # floor: 12 floats + textureName + rgb(3) + (1 float)
    p = [next_float() for _ in range(12)]
    floor_pts = np.array(p, dtype=float).reshape(4, 3)
    floor_tex = next_token()  # "none"
    fr, fg, fb = next_float(), next_float(), next_float()
    _ = next_float()  # reflect/other

    # lights: N then N*(pos+rgb)
    nlights = next_int()
    lights = []
    for _ in range(nlights):
        lx, ly, lz = next_float(), next_float(), next_float()
        lr, lg, lb = next_float(), next_float(), next_float()
        lights.append({"p": np.array([lx, ly, lz], dtype=float), "rgb": np.array([lr, lg, lb], dtype=float)})

    # tail: maybe "depth sqrt_rpp"
    tail = []
    while it < len(tokens) and is_float(tokens[it]):
        tail.append(float(next_token()))

    return {
        "frames": frames, "w": w, "h": h, "fov": fov,
        "cam": (r0c, z0c, phi0c, arc, azc, wrc, wzc, wphic, prc, pzc,
                r0n, z0n, phi0n, arn, azn, wrn, wzn, wphin, prn, pzn),
        "figs": figs,
        "floor": floor_pts,
        "lights": lights,
        "tail": tail,
        "out_pattern": out_pattern,
        "floor_tex": floor_tex,
        "floor_rgb": (fr, fg, fb)
    }

def circ_scale(V, target_radius):
    # V: (n,3) centered at origin
    r = np.max(np.linalg.norm(V, axis=1))
    if r < 1e-12:
        return V
    return V * (target_radius / r)

def edges_from_vertices(V, tol=1e-2):
    n = V.shape[0]
    dmin = None
    D = np.zeros((n, n), dtype=float)
    for i in range(n):
        for j in range(i+1, n):
            d = np.linalg.norm(V[i] - V[j])
            D[i, j] = D[j, i] = d
            if d > 1e-9:
                if dmin is None or d < dmin:
                    dmin = d
    if dmin is None:
        return []
    edges = []
    thr = dmin * (1.0 + tol)
    for i in range(n):
        for j in range(i+1, n):
            if D[i, j] <= thr:
                edges.append((i, j))
    return edges

def verts_tetra():
    # базовый тетраэдр около origin
    # (ориентация любая, главное — правильная форма)
    V = np.array([
        [ 1,  1,  1],
        [ 1, -1, -1],
        [-1,  1, -1],
        [-1, -1,  1],
    ], dtype=float)
    return V

def verts_icosa():
    phi = (1.0 + np.sqrt(5.0)) / 2.0
    V = []
    for a in (-1, 1):
        for b in (-1, 1):
            V.append([0, a, b*phi])
            V.append([a, b*phi, 0])
            V.append([a*phi, 0, b])
    return np.array(V, dtype=float)

def verts_dodeca():
    phi = (1.0 + np.sqrt(5.0)) / 2.0
    inv = 1.0 / phi
    V = []
    # (±1, ±1, ±1)
    for x in (-1, 1):
        for y in (-1, 1):
            for z in (-1, 1):
                V.append([x, y, z])
    # (0, ±1/φ, ±φ)
    for y in (-inv, inv):
        for z in (-phi, phi):
            V.append([0, y, z])
            V.append([y, z, 0])
            V.append([z, 0, y])
    return np.array(V, dtype=float)

def set_axes_equal(ax):
    # equal aspect for 3D
    xlim = ax.get_xlim3d()
    ylim = ax.get_ylim3d()
    zlim = ax.get_zlim3d()
    xr = abs(xlim[1] - xlim[0])
    yr = abs(ylim[1] - ylim[0])
    zr = abs(zlim[1] - zlim[0])
    r = max([xr, yr, zr])
    cx = (xlim[0] + xlim[1]) / 2
    cy = (ylim[0] + ylim[1]) / 2
    cz = (zlim[0] + zlim[1]) / 2
    ax.set_xlim3d(cx - r/2, cx + r/2)
    ax.set_ylim3d(cy - r/2, cy + r/2)
    ax.set_zlim3d(cz - r/2, cz + r/2)

def poly_wire(ax, V, center, radius, color=None, lw=1.2):
    V = circ_scale(V, radius) + center.reshape(1, 3)
    edges = edges_from_vertices(V)
    for (i, j) in edges:
        xs = [V[i,0], V[j,0]]
        ys = [V[i,1], V[j,1]]
        zs = [V[i,2], V[j,2]]
        ax.plot(xs, ys, zs, linewidth=lw, color=color)

def build_paths(cam_params, frames):
    (r0c, z0c, phi0c, arc, azc, wrc, wzc, wphic, prc, pzc,
     r0n, z0n, phi0n, arn, azn, wrn, wzn, wphin, prn, pzn) = cam_params

    N = max(frames, 120)
    t = np.linspace(0, 2*np.pi, N, endpoint=False)

    rc = (r0c + arc*np.sin(wrc*t + prc))
    xc = rc*np.cos(phi0c + wphic*t)
    yc = rc*np.sin(phi0c + wphic*t)
    zc = z0c + azc*np.sin(wzc*t + pzc)

    rn = (r0n + arn*np.sin(wrn*t + prn))
    xv = rn*np.cos(phi0n + wphin*t)
    yv = rn*np.sin(phi0n + wphin*t)
    zv = z0n + azn*np.sin(wzn*t + pzn)

    C = np.vstack([xc, yc, zc]).T
    V = np.vstack([xv, yv, zv]).T
    return C, V

def main():
    inp = sys.argv[1] if len(sys.argv) > 1 else "input.txt"
    data = read_input(inp)

    C, V = build_paths(data["cam"], data["frames"])

    # фигуры как в popov: (обычно) слева - додека, центр - икоса, справа - тетра
    # если у тебя порядок другой — просто поменяй mapping здесь:
    mapping = [
        ("tetra",  verts_tetra()),
        ("dodeca", verts_dodeca()),
        ("icosa",  verts_icosa()),
    ]

    # ---------- 3D view ----------
    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection="3d")

    ax.plot(C[:,0], C[:,1], C[:,2], linewidth=2.5, color="red", label="camera")
    ax.plot(V[:,0], V[:,1], V[:,2], linewidth=2.5, color="blue", label="view")

    step = max(1, len(C)//80)
    for i in range(0, len(C), step):
        ax.plot([C[i,0], V[i,0]], [C[i,1], V[i,1]], [C[i,2], V[i,2]], linewidth=1.0, color="green", alpha=0.7)

    # floor
    floor = data["floor"]
    floor_loop = np.vstack([floor, floor[0]])
    ax.plot(floor_loop[:,0], floor_loop[:,1], floor_loop[:,2], linewidth=2.0, color="gray")

    # light(s)
    for L in data["lights"]:
        ax.scatter([L["p"][0]], [L["p"][1]], [L["p"][2]], s=80, color="gold")

    # bodies wireframes
    for i, figinfo in enumerate(data["figs"]):
        name, baseV = mapping[i]
        # цвет — можно любой; оставим читаемо
        col = ["#ff0e0e", "#2ca02c", "#1f77b4"][i]  # blue/green/orange
        poly_wire(ax, baseV, figinfo["c"], figinfo["r"], color=col, lw=1.4)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.set_title("Camera/View paths + wireframe bodies")
    set_axes_equal(ax)
    plt.tight_layout()
    plt.savefig("scene_3d.png", dpi=200)

    # ---------- top view ----------
    fig2 = plt.figure(figsize=(12, 8))
    ax2 = fig2.add_subplot(111, projection="3d")
    ax2.view_init(elev=90, azim=0)

    ax2.plot(C[:,0], C[:,1], C[:,2], linewidth=2.5, color="red")
    ax2.plot(V[:,0], V[:,1], V[:,2], linewidth=2.5, color="blue")
    for i in range(0, len(C), step):
        ax2.plot([C[i,0], V[i,0]], [C[i,1], V[i,1]], [C[i,2], V[i,2]], linewidth=1.0, color="green", alpha=0.7)

    ax2.plot(floor_loop[:,0], floor_loop[:,1], floor_loop[:,2], linewidth=2.0, color="gray")
    for L in data["lights"]:
        ax2.scatter([L["p"][0]], [L["p"][1]], [L["p"][2]], s=80, color="gold")
    for i, figinfo in enumerate(data["figs"]):
        name, baseV = mapping[i]
        col = ["#ff0e0e", "#2ca02c", "#1f77b4"][i]
        poly_wire(ax2, baseV, figinfo["c"], figinfo["r"], color=col, lw=1.4)

    ax2.set_xlabel("X")
    ax2.set_ylabel("Y")
    ax2.set_zlabel("Z")
    ax2.set_title("Top view")
    set_axes_equal(ax2)
    plt.tight_layout()
    plt.savefig("scene_top.png", dpi=200)

    print("Saved: scene_3d.png, scene_top.png")

if __name__ == "__main__":
    main()
