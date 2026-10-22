#include <stdlib.h>
#include "../include/andrews.h"

static int compare_points(const void *a, const void *b)
{
    const Point *p1 = (const Point *)a;
    const Point *p2 = (const Point *)b;

    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;

    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;

    return 0;
}

static double cross(Point o, Point a, Point b)
{
    return (a.x - o.x) * (b.y - o.y)
         - (a.y - o.y) * (b.x - o.x);
}

int andrews_monotone_chain(const Point points[], int n, Point hull[])
{
    if (points == NULL || hull == NULL || n <= 0)
        return 0;

    Point *sorted = malloc((size_t)n * sizeof(Point));
    if (sorted == NULL)
        return 0;

    for (int i = 0; i < n; i++)
        sorted[i] = points[i];

    qsort(sorted, (size_t)n, sizeof(Point), compare_points);

    // Remove duplicate points 
    int unique_count = 0;

    for (int i = 0; i < n; i++) {
        if (unique_count == 0 ||
            sorted[i].x != sorted[unique_count - 1].x ||
            sorted[i].y != sorted[unique_count - 1].y) {
            sorted[unique_count++] = sorted[i];
        }
    }

    if (unique_count <= 2) {
        for (int i = 0; i < unique_count; i++)
            hull[i] = sorted[i];

        free(sorted);
        return unique_count;
    }

    Point *result = malloc(
        (size_t)(2 * unique_count) * sizeof(Point)
    );

    if (result == NULL) {
        free(sorted);
        return 0;
    }

    int k = 0;

    //Build the lower hull
    for (int i = 0; i < unique_count; i++) {
        while (k >= 2 &&
               cross(result[k - 2], result[k - 1], sorted[i]) <= 0.0) {
            k--;
        }

        result[k++] = sorted[i];
    }

    // Build the upper hull
    int lower_size = k;

    for (int i = unique_count - 2; i >= 0; i--) {
        while (k > lower_size &&
               cross(result[k - 2], result[k - 1], sorted[i]) <= 0.0) {
            k--;
        }

        result[k++] = sorted[i];
    }

    // The first point is repeated at the end; remove that repetition
    if (k > 1)
        k--;

    for (int i = 0; i < k; i++)
        hull[i] = result[i];

    free(result);
    free(sorted);

    return k;
}
