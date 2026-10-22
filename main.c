#include <stdio.h>
#include <string.h>

#include "include/jarvis.h"
#include "include/andrews.h"
#include "include/graham.h"

int main(int argc, char *argv[])
{
    Point points[] = {
        {0, 3},
        {1, 1},
        {2, 5},
        {3, 2},
        {4, 0},
        {5, 3},
        {6, 1},
        {7, 4},
        {6, 7},
        {4, 6},
        {2, 7},
        {3, 4},
        {4, 3},
        {5, 5},
        {1, 5}
    };

    int n = (int)(sizeof(points) / sizeof(points[0]));
    Point hull[2 * n];

    if (argc < 2) {
        fprintf(stderr,
                "Usage: hull.exe [jarvis|andrews|graham]\n");
        return 1;
    }

    int hull_size;

    if (strcmp(argv[1], "jarvis") == 0) {
        hull_size = jarvis_march(points, n, hull);
    } else if (strcmp(argv[1], "andrews") == 0) {
        hull_size = andrews_monotone_chain(points, n, hull);
    } else if (strcmp(argv[1], "graham") == 0) {
        hull_size = graham_scan(points, n, hull);
    } else {
        fprintf(stderr, "Unknown algorithm: %s\n", argv[1]);
        return 1;
    }

    printf("POINTS\n");
    for (int i = 0; i < n; i++)
        printf("%.10g %.10g\n", points[i].x, points[i].y);

    printf("HULL\n");
    for (int i = 0; i < hull_size; i++)
        printf("%.10g %.10g\n", hull[i].x, hull[i].y);

    return 0;
}
