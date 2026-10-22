#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

typedef struct {
    double x;
    double y;
} Point;

int jarvis_march(const Point points[], int n, Point hull[]);

#endif
