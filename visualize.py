import sys
import subprocess
import matplotlib.pyplot as plt


ALGORITHM_NAMES = {
    "jarvis": "Jarvis March",
    "andrews": "Andrew's Monotone Chain",
    "graham": "Graham Scan",
}


def get_data_from_c():
    algorithm = (
        sys.argv[1].lower()
        if len(sys.argv) > 1
        else "andrews"
    )

    if algorithm not in ALGORITHM_NAMES:
        raise ValueError(
            f"Unknown algorithm '{algorithm}'. "
            f"Choose from: {', '.join(ALGORITHM_NAMES)}"
        )

    result = subprocess.run(
        [".\\hull.exe", algorithm],
        capture_output=True,
        text=True,
        check=True
    )

    points = []
    hull = []
    section = None

    for line in result.stdout.splitlines():
        line = line.strip()

        if line == "POINTS":
            section = "points"
            continue

        if line == "HULL":
            section = "hull"
            continue

        if section in ("points", "hull") and line:
            x, y = map(float, line.split())

            if section == "points":
                points.append((x, y))
            else:
                hull.append((x, y))

    if not points:
        raise ValueError("No input points found in C output.")

    if not hull:
        raise ValueError("No hull vertices found in C output.")

    return algorithm, points, hull


def plot_hull(algorithm, points, hull):
    name = ALGORITHM_NAMES[algorithm]

    plt.figure(figsize=(9, 7))

    xs, ys = zip(*points)

    plt.scatter(
        xs, ys,
        color="steelblue",
        s=65,
        label="Input points"
    )

    for x, y in points:
        plt.annotate(
            f"({x:g}, {y:g})",
            (x, y),
            xytext=(5, 5),
            textcoords="offset points",
            fontsize=8
        )

    if len(hull) >= 2:
        closed_hull = hull + [hull[0]]
        hx, hy = zip(*closed_hull)

        plt.plot(
            hx, hy,
            color="crimson",
            linewidth=2,
            marker="o",
            markersize=6,
            label=f"{name} hull"
        )

    plt.title(f"Convex Hull - {name}")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.axis("equal")
    plt.legend()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    algorithm, points, hull = get_data_from_c()

    print(f"Algorithm: {ALGORITHM_NAMES[algorithm]}")
    print(f"Input points: {len(points)}")
    print(f"Hull vertices: {len(hull)}")

    plot_hull(algorithm, points, hull)
