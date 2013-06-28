#ifndef TYPES_H
#define TYPES_H

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
