#ifndef CELLO_STD_MATH_H
#define CELLO_STD_MATH_H

#include "Cello.h"

// Mathematical constants
extern var PI;
extern var E;
extern var TAU;
extern var GOLDEN_RATIO;

// Mathematical types
extern var Complex;
extern var Vector2;
extern var Vector3;
extern var Matrix;

// Complex number structure
struct Complex {
    double real;
    double imag;
};

// 2D Vector structure
struct Vector2 {
    double x;
    double y;
};

// 3D Vector structure  
struct Vector3 {
    double x;
    double y;
    double z;
};

// Matrix structure
struct Matrix {
    double* data;
    size_t rows;
    size_t cols;
};

// Mathematical functions
double std_sqrt(double x);
double std_pow(double base, double exp);
double std_abs(double x);
double std_floor(double x);
double std_ceil(double x);
double std_round(double x);

// Trigonometric functions
double std_sin(double x);
double std_cos(double x);
double std_tan(double x);
double std_asin(double x);
double std_acos(double x);
double std_atan(double x);
double std_atan2(double y, double x);

// Hyperbolic functions
double std_sinh(double x);
double std_cosh(double x);
double std_tanh(double x);

// Logarithmic functions
double std_log(double x);
double std_log10(double x);
double std_log2(double x);
double std_exp(double x);

// Statistical functions
double std_min(double a, double b);
double std_max(double a, double b);
double std_clamp(double x, double min, double max);
double std_lerp(double a, double b, double t);

// Random number functions
void std_seed_random(unsigned int seed);
int std_random_int(int min, int max);
double std_random_double(void);
double std_random_range(double min, double max);

// Complex number functions
var complex_new(double real, double imag);
var complex_add(var a, var b);
var complex_sub(var a, var b);
var complex_mul(var a, var b);
var complex_div(var a, var b);
double complex_magnitude(var c);
double complex_phase(var c);

// Vector functions
var vector2_new(double x, double y);
var vector2_add(var a, var b);
var vector2_sub(var a, var b);
var vector2_scale(var v, double scalar);
double vector2_dot(var a, var b);
double vector2_magnitude(var v);
var vector2_normalize(var v);

var vector3_new(double x, double y, double z);
var vector3_add(var a, var b);
var vector3_sub(var a, var b);
var vector3_scale(var v, double scalar);
double vector3_dot(var a, var b);
var vector3_cross(var a, var b);
double vector3_magnitude(var v);
var vector3_normalize(var v);

// Matrix functions
var matrix_new(size_t rows, size_t cols);
var matrix_identity(size_t size);
var matrix_add(var a, var b);
var matrix_sub(var a, var b);
var matrix_mul(var a, var b);
var matrix_transpose(var m);
double matrix_determinant(var m);
var matrix_inverse(var m);

// Initialization function
void __cello_std_math_init(void);

#endif