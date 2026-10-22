#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jarvis.h"
#include "andrews.h"
#include "graham.h"

#define N 50000
#define REPETITIONS 100


static double measure_time(
    int (*algorithm)(const Point[], int, Point[]),
    const Point points[],
    int n,
    Point hull[],
    int *hull_size)
{
    clock_t start = clock();

    for (int i = 0; i < REPETITIONS; i++) {
        *hull_size = algorithm(points, n, hull);
    }

    clock_t end = clock();

    double total_time =
        (double)(end - start) / CLOCKS_PER_SEC;

    return total_time / REPETITIONS;
}


static int same_hull(
    const Point a[], int na,
    const Point b[], int nb)
{
    if (na != nb)
        return 0;

    for (int i = 0; i < na; i++) {
        int found = 0;

        for (int j = 0; j < nb; j++) {
            if (a[i].x == b[j].x && a[i].y == b[j].y) {
                found = 1;
                break;
            }
        }

        if (!found)
            return 0;
    }

    return 1;
}

int main(void)
{
    Point *points = malloc(N * sizeof(Point));
    Point *hull_jarvis = malloc(2 * N * sizeof(Point));
    Point *hull_andrews = malloc(2 * N * sizeof(Point));
    Point *hull_graham = malloc(2 * N * sizeof(Point));

    if (points == NULL ||
        hull_jarvis == NULL ||
        hull_andrews == NULL ||
        hull_graham == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");

        free(points);
        free(hull_jarvis);
        free(hull_andrews);
        free(hull_graham);

        return 1;
    }

    int size_jarvis, size_andrews, size_graham;

    /* Generate reproducible random points. */
    srand(42);

    for (int i = 0; i < N; i++) {
        points[i].x = rand() % 1000000;
        points[i].y = rand() % 1000000;
    }

    double time_jarvis = measure_time(
        jarvis_march, points, N,
        hull_jarvis, &size_jarvis);

    double time_andrews = measure_time(
        andrews_monotone_chain, points, N,
        hull_andrews, &size_andrews);

    double time_graham = measure_time(
        graham_scan, points, N,
        hull_graham, &size_graham);

    printf("Number of points: %d\n\n", N);

    printf("Jarvis March:            %.6f seconds\n", time_jarvis);
    printf("Andrew's Monotone Chain: %.6f seconds\n", time_andrews);
    printf("Graham Scan:             %.6f seconds\n\n", time_graham);

    printf("Hull vertices:\n");
    printf("Jarvis:  %d\n", size_jarvis);
    printf("Andrew:  %d\n", size_andrews);
    printf("Graham:  %d\n\n", size_graham);

    if (size_jarvis > 0 &&
        same_hull(hull_jarvis, size_jarvis,
                  hull_andrews, size_andrews) &&
        same_hull(hull_jarvis, size_jarvis,
                  hull_graham, size_graham)) {
        printf("Verification: All three algorithms agree.\n");
    } else {
        printf("Verification: Results differ or an algorithm failed.\n");
    }

    
    FILE *file = fopen("hull_data.txt", "w");

    if (file == NULL) {
        perror("Could not create hull_data.txt");
        return 1;
    }

    fprintf(file, "POINTS\n");
    for (int i = 0; i < N; i++)
        fprintf(file, "%.10g %.10g\n",
                points[i].x, points[i].y);

    fprintf(file, "HULL\n");
    for (int i = 0; i < size_andrews; i++)
        fprintf(file, "%.10g %.10g\n",
                hull_andrews[i].x, hull_andrews[i].y);

    fclose(file);

    free(points);
    free(hull_jarvis);
    free(hull_andrews);
    free(hull_graham);
    return 0;
}
