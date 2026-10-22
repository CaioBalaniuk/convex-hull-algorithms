#include <stddef.h>
#include "../include/jarvis.h"

static double orientation(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - a.y)
         - (b.y - a.y) * (c.x - a.x);
}

static double distance_squared(Point a, Point b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;

    return dx * dx + dy * dy;
}

static int same_point(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

int jarvis_march(const Point points[], int n, Point hull[])
{
    if (points == NULL || hull == NULL || n <= 0)
        return 0;

    // Find the leftmost point. Break ties by choosing the lowest y
    int start = 0;

    for (int i = 1; i < n; i++) {
        if (points[i].x < points[start].x || (points[i].x == points[start].x && points[i].y < points[start].y)) {
            start = i;
        }
    }

    int current = start;
    int hull_size = 0;

    do {
        hull[hull_size++] = points[current];

        // Find an initial candidate different from current
        int candidate = -1;

        for (int i = 0; i < n; i++) {
            if (!same_point(points[i], points[current])) {
                candidate = i;
                break;
            }
        }

        // All points have the same coordinates
        if (candidate == -1)
            return hull_size;

        // Find the next vertex of the convex hull
        for (int i = 0; i < n; i++) {
            if (i == current || i == candidate) continue;

            double cross = orientation(points[current], points[candidate], points[i]);

            if (cross < 0.0 || (cross == 0.0 &&
                distance_squared(points[current], points[i]) >
                distance_squared(points[current], points[candidate]))) {

                candidate = i;
            }
        }

        current = candidate;

    } while (current != start && hull_size < n);

    return hull_size;
}