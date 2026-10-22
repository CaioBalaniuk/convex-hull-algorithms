import sys
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

    points = []
    hull = []
    section = None

    with open("hull_data.txt", "r", encoding="utf-8") as file:
        for line in file:
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
        raise ValueError("No input points found in hull_data.txt.")

    if not hull:
        raise ValueError("No hull vertices found in hull_data.txt.")

    return algorithm, points, hull


def plot_hull(algorithm, points, hull):
    name = ALGORITHM_NAMES[algorithm]

    # Clean, readable figure
    fig, ax = plt.subplots(figsize=(10, 7), dpi=120)
    fig.patch.set_facecolor("white")
    ax.set_facecolor("white")

    xs, ys = zip(*points)

    # Input points: small and transparent
    ax.scatter(
        xs, ys,
        s=9,
        color="steelblue",
        alpha=0.25,
        edgecolors="none",
        label=f"Input points (n={len(points)})",
        rasterized=True,
        zorder=1
    )

    # Draw the convex hull without filling its interior
    if len(hull) >= 2:
        closed_hull = hull + [hull[0]]
        hx, hy = zip(*closed_hull)

        ax.plot(
            hx, hy,
            color="crimson",
            linewidth=2,
            label=f"{name} hull (h={len(hull)})",
            zorder=3
        )

        # Highlight the actual hull vertices
        hull_x, hull_y = zip(*hull)

        ax.scatter(
            hull_x, hull_y,
            s=30,
            color="crimson",
            edgecolors="white",
            linewidths=0.7,
            zorder=4
        )

    ax.set_title(
        f"Convex Hull — {name}",
        fontsize=15,
        fontweight="bold",
        pad=14
    )
    ax.set_xlabel("x", fontsize=11)
    ax.set_ylabel("y", fontsize=11)

    # Subtle grid and clean axes
    ax.grid(
        True,
        linestyle="--",
        linewidth=0.6,
        alpha=0.25
    )
    ax.set_axisbelow(True)
    ax.set_aspect("equal", adjustable="box")
    ax.margins(0.04)

    # Remove unnecessary visual clutter
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    ax.legend(
        loc="best",
        frameon=True,
        framealpha=0.95,
        edgecolor="lightgray",
        fontsize=9
    )

    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    algorithm, points, hull = get_data_from_c()

    print(f"Algorithm: {ALGORITHM_NAMES[algorithm]}")
    print(f"Input points: {len(points)}")
    print(f"Hull vertices: {len(hull)}")

    plot_hull(algorithm, points, hull)
