#include <stddef.h>
#include <stdlib.h>
#include "../include/graham.h"

static Point pivot;

static double cross(Point o, Point a, Point b)
{
    return (a.x - o.x) * (b.y - o.y)
         - (a.y - o.y) * (b.x - o.x);
}

static double distance_squared(Point a, Point b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

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

static int compare_polar_angle(const void *a, const void *b)
{
    const Point *p1 = (const Point *)a;
    const Point *p2 = (const Point *)b;

    double turn = cross(pivot, *p1, *p2);

    if (turn > 0.0) return -1;
    if (turn < 0.0) return 1;

    double d1 = distance_squared(pivot, *p1);
    double d2 = distance_squared(pivot, *p2);

    if (d1 < d2) return -1;
    if (d1 > d2) return 1;

    return 0;
}

int graham_scan(const Point points[], int n, Point hull[])
{
    if (points == NULL || hull == NULL || n <= 0)
        return 0;

    Point *sorted = malloc((size_t)n * sizeof(Point));
    if (sorted == NULL)
        return 0;

    for (int i = 0; i < n; i++)
        sorted[i] = points[i];

    //Remove duplicate points
    qsort(sorted, (size_t)n, sizeof(Point), compare_points);

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

    //Choose the lowest point; break ties by the smallest x
    int pivot_index = 0;

    for (int i = 1; i < unique_count; i++) {
        if (sorted[i].y < sorted[pivot_index].y ||
            (sorted[i].y == sorted[pivot_index].y &&
             sorted[i].x < sorted[pivot_index].x)) {
            pivot_index = i;
        }
    }

    Point temp = sorted[0];
    sorted[0] = sorted[pivot_index];
    sorted[pivot_index] = temp;

    pivot = sorted[0];

    //Sort the remaining points by polar angle around the pivot
    qsort(sorted + 1, (size_t)(unique_count - 1),
          sizeof(Point), compare_polar_angle);

    Point *stack = malloc((size_t)unique_count * sizeof(Point));

    if (stack == NULL) {
        free(sorted);
        return 0;
    }

    int top = 0;
    stack[top++] = sorted[0];
    stack[top++] = sorted[1];

    for (int i = 2; i < unique_count; i++) {
        while (top >= 2 &&
               cross(stack[top - 2], stack[top - 1], sorted[i]) <= 0.0) {
            top--;
        }

        stack[top++] = sorted[i];
    }

    for (int i = 0; i < top; i++)
        hull[i] = stack[i];

    free(stack);
    free(sorted);

    return top;
}
