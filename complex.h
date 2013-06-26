#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
	double R;
	double I;
} complex;

typedef struct {
	complex center;
	complex radius;
} region;

typedef struct {
	int X;
	int Y;
} pixel;

#endif
