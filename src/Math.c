#include "Cello.h"
#include "Math.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Mathematical constants
var PI;
var E;
var TAU;
var GOLDEN_RATIO;

// Mathematical types
var Complex;
var Vector2;
var Vector3;
var Matrix;

// Static initialization flag
static bool math_initialized = false;

// Complex type implementation
static void Complex_New(var self, var args) {
    struct Complex* c = cast(self, Complex);
    c->real = 0.0;
    c->imag = 0.0;
    
    if (len(args) >= 1) c->real = c_float(get(args, $I(0)));
    if (len(args) >= 2) c->imag = c_float(get(args, $I(1)));
}

static void Complex_Del(var self) {
    // No dynamic allocation to clean up
}

static int Complex_Show(var self, var output, int pos) {
    struct Complex* c = cast(self, Complex);
    if (c->imag >= 0) {
        return print_to(output, pos, "%.6f+%.6fi", $F(c->real), $F(c->imag));
    } else {
        return print_to(output, pos, "%.6f%.6fi", $F(c->real), $F(c->imag));
    }
}

// Vector2 type implementation
static void Vector2_New(var self, var args) {
    struct Vector2* v = cast(self, Vector2);
    v->x = 0.0;
    v->y = 0.0;
    
    if (len(args) >= 1) v->x = c_float(get(args, $I(0)));
    if (len(args) >= 2) v->y = c_float(get(args, $I(1)));
}

static void Vector2_Del(var self) {
    // No dynamic allocation to clean up
}

static int Vector2_Show(var self, var output, int pos) {
    struct Vector2* v = cast(self, Vector2);
    return print_to(output, pos, "(%.6f, %.6f)", $F(v->x), $F(v->y));
}

// Vector3 type implementation
static void Vector3_New(var self, var args) {
    struct Vector3* v = cast(self, Vector3);
    v->x = 0.0;
    v->y = 0.0;
    v->z = 0.0;
    
    if (len(args) >= 1) v->x = c_float(get(args, $I(0)));
    if (len(args) >= 2) v->y = c_float(get(args, $I(1)));
    if (len(args) >= 3) v->z = c_float(get(args, $I(2)));
}

static void Vector3_Del(var self) {
    // No dynamic allocation to clean up
}

static int Vector3_Show(var self, var output, int pos) {
    struct Vector3* v = cast(self, Vector3);
    return print_to(output, pos, "(%.6f, %.6f, %.6f)", $F(v->x), $F(v->y), $F(v->z));
}

// Matrix type implementation
static void Matrix_New(var self, var args) {
    struct Matrix* m = cast(self, Matrix);
    m->rows = 0;
    m->cols = 0;
    m->data = NULL;
    
    if (len(args) >= 2) {
        m->rows = c_int(get(args, $I(0)));
        m->cols = c_int(get(args, $I(1)));
        m->data = calloc(m->rows * m->cols, sizeof(double));
    }
}

static void Matrix_Del(var self) {
    struct Matrix* m = cast(self, Matrix);
    if (m->data) {
        free(m->data);
        m->data = NULL;
    }
}

static int Matrix_Show(var self, var output, int pos) {
    struct Matrix* m = cast(self, Matrix);
    int written = 0;
    written += print_to(output, pos + written, "Matrix(%lux%lu)[\n", $I(m->rows), $I(m->cols));
    
    for (size_t i = 0; i < m->rows; i++) {
        written += print_to(output, pos + written, "  [");
        for (size_t j = 0; j < m->cols; j++) {
            written += print_to(output, pos + written, "%.6f", $F(m->data[i * m->cols + j]));
            if (j < m->cols - 1) written += print_to(output, pos + written, ", ");
        }
        written += print_to(output, pos + written, "]\n");
    }
    written += print_to(output, pos + written, "]");
    return written;
}

// Type definitions
var Complex = Cello(Complex,
    Instance(New, Complex_New, Complex_Del),
    Instance(Show, Complex_Show));

var Vector2 = Cello(Vector2,
    Instance(New, Vector2_New, Vector2_Del),
    Instance(Show, Vector2_Show));

var Vector3 = Cello(Vector3,
    Instance(New, Vector3_New, Vector3_Del),
    Instance(Show, Vector3_Show));

var Matrix = Cello(Matrix,
    Instance(New, Matrix_New, Matrix_Del),
    Instance(Show, Matrix_Show));

// Basic mathematical functions
double std_sqrt(double x) { return sqrt(x); }
double std_pow(double base, double exp) { return pow(base, exp); }
double std_abs(double x) { return fabs(x); }
double std_floor(double x) { return floor(x); }
double std_ceil(double x) { return ceil(x); }
double std_round(double x) { return round(x); }

// Trigonometric functions
double std_sin(double x) { return sin(x); }
double std_cos(double x) { return cos(x); }
double std_tan(double x) { return tan(x); }
double std_asin(double x) { return asin(x); }
double std_acos(double x) { return acos(x); }
double std_atan(double x) { return atan(x); }
double std_atan2(double y, double x) { return atan2(y, x); }

// Hyperbolic functions
double std_sinh(double x) { return sinh(x); }
double std_cosh(double x) { return cosh(x); }
double std_tanh(double x) { return tanh(x); }

// Logarithmic functions
double std_log(double x) { return log(x); }
double std_log10(double x) { return log10(x); }
double std_log2(double x) { return log2(x); }
double std_exp(double x) { return exp(x); }

// Statistical functions
double std_min(double a, double b) { return (a < b) ? a : b; }
double std_max(double a, double b) { return (a > b) ? a : b; }
double std_clamp(double x, double min, double max) {
    return std_min(std_max(x, min), max);
}
double std_lerp(double a, double b, double t) {
    return a + t * (b - a);
}

// Random number functions
void std_seed_random(unsigned int seed) {
    srand(seed);
}

int std_random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

double std_random_double(void) {
    return (double)rand() / RAND_MAX;
}

double std_random_range(double min, double max) {
    return min + std_random_double() * (max - min);
}

// Complex number functions
var complex_new(double real, double imag) {
    return new(Complex, $F(real), $F(imag));
}

var complex_add(var a, var b) {
    struct Complex* ca = cast(a, Complex);
    struct Complex* cb = cast(b, Complex);
    return complex_new(ca->real + cb->real, ca->imag + cb->imag);
}

var complex_sub(var a, var b) {
    struct Complex* ca = cast(a, Complex);
    struct Complex* cb = cast(b, Complex);
    return complex_new(ca->real - cb->real, ca->imag - cb->imag);
}

var complex_mul(var a, var b) {
    struct Complex* ca = cast(a, Complex);
    struct Complex* cb = cast(b, Complex);
    double real = ca->real * cb->real - ca->imag * cb->imag;
    double imag = ca->real * cb->imag + ca->imag * cb->real;
    return complex_new(real, imag);
}

var complex_div(var a, var b) {
    struct Complex* ca = cast(a, Complex);
    struct Complex* cb = cast(b, Complex);
    double denom = cb->real * cb->real + cb->imag * cb->imag;
    double real = (ca->real * cb->real + ca->imag * cb->imag) / denom;
    double imag = (ca->imag * cb->real - ca->real * cb->imag) / denom;
    return complex_new(real, imag);
}

double complex_magnitude(var c) {
    struct Complex* comp = cast(c, Complex);
    return std_sqrt(comp->real * comp->real + comp->imag * comp->imag);
}

double complex_phase(var c) {
    struct Complex* comp = cast(c, Complex);
    return std_atan2(comp->imag, comp->real);
}

// Vector2 functions
var vector2_new(double x, double y) {
    return new(Vector2, $F(x), $F(y));
}

var vector2_add(var a, var b) {
    struct Vector2* va = cast(a, Vector2);
    struct Vector2* vb = cast(b, Vector2);
    return vector2_new(va->x + vb->x, va->y + vb->y);
}

var vector2_sub(var a, var b) {
    struct Vector2* va = cast(a, Vector2);
    struct Vector2* vb = cast(b, Vector2);
    return vector2_new(va->x - vb->x, va->y - vb->y);
}

var vector2_scale(var v, double scalar) {
    struct Vector2* vec = cast(v, Vector2);
    return vector2_new(vec->x * scalar, vec->y * scalar);
}

double vector2_dot(var a, var b) {
    struct Vector2* va = cast(a, Vector2);
    struct Vector2* vb = cast(b, Vector2);
    return va->x * vb->x + va->y * vb->y;
}

double vector2_magnitude(var v) {
    struct Vector2* vec = cast(v, Vector2);
    return std_sqrt(vec->x * vec->x + vec->y * vec->y);
}

var vector2_normalize(var v) {
    double mag = vector2_magnitude(v);
    if (mag == 0.0) return copy(v);
    return vector2_scale(v, 1.0 / mag);
}

// Vector3 functions
var vector3_new(double x, double y, double z) {
    return new(Vector3, $F(x), $F(y), $F(z));
}

var vector3_add(var a, var b) {
    struct Vector3* va = cast(a, Vector3);
    struct Vector3* vb = cast(b, Vector3);
    return vector3_new(va->x + vb->x, va->y + vb->y, va->z + vb->z);
}

var vector3_sub(var a, var b) {
    struct Vector3* va = cast(a, Vector3);
    struct Vector3* vb = cast(b, Vector3);
    return vector3_new(va->x - vb->x, va->y - vb->y, va->z - vb->z);
}

var vector3_scale(var v, double scalar) {
    struct Vector3* vec = cast(v, Vector3);
    return vector3_new(vec->x * scalar, vec->y * scalar, vec->z * scalar);
}

double vector3_dot(var a, var b) {
    struct Vector3* va = cast(a, Vector3);
    struct Vector3* vb = cast(b, Vector3);
    return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}

var vector3_cross(var a, var b) {
    struct Vector3* va = cast(a, Vector3);
    struct Vector3* vb = cast(b, Vector3);
    return vector3_new(
        va->y * vb->z - va->z * vb->y,
        va->z * vb->x - va->x * vb->z,
        va->x * vb->y - va->y * vb->x
    );
}

double vector3_magnitude(var v) {
    struct Vector3* vec = cast(v, Vector3);
    return std_sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

var vector3_normalize(var v) {
    double mag = vector3_magnitude(v);
    if (mag == 0.0) return copy(v);
    return vector3_scale(v, 1.0 / mag);
}

// Matrix functions
var matrix_new(size_t rows, size_t cols) {
    return new(Matrix, $I(rows), $I(cols));
}

var matrix_identity(size_t size) {
    var m = matrix_new(size, size);
    struct Matrix* mat = cast(m, Matrix);
    for (size_t i = 0; i < size; i++) {
        mat->data[i * size + i] = 1.0;
    }
    return m;
}

var matrix_add(var a, var b) {
    struct Matrix* ma = cast(a, Matrix);
    struct Matrix* mb = cast(b, Matrix);
    if (ma->rows != mb->rows || ma->cols != mb->cols) return NULL;
    
    var result = matrix_new(ma->rows, ma->cols);
    struct Matrix* mr = cast(result, Matrix);
    
    for (size_t i = 0; i < ma->rows * ma->cols; i++) {
        mr->data[i] = ma->data[i] + mb->data[i];
    }
    return result;
}

var matrix_sub(var a, var b) {
    struct Matrix* ma = cast(a, Matrix);
    struct Matrix* mb = cast(b, Matrix);
    if (ma->rows != mb->rows || ma->cols != mb->cols) return NULL;
    
    var result = matrix_new(ma->rows, ma->cols);
    struct Matrix* mr = cast(result, Matrix);
    
    for (size_t i = 0; i < ma->rows * ma->cols; i++) {
        mr->data[i] = ma->data[i] - mb->data[i];
    }
    return result;
}

var matrix_mul(var a, var b) {
    struct Matrix* ma = cast(a, Matrix);
    struct Matrix* mb = cast(b, Matrix);
    if (ma->cols != mb->rows) return NULL;
    
    var result = matrix_new(ma->rows, mb->cols);
    struct Matrix* mr = cast(result, Matrix);
    
    for (size_t i = 0; i < ma->rows; i++) {
        for (size_t j = 0; j < mb->cols; j++) {
            for (size_t k = 0; k < ma->cols; k++) {
                mr->data[i * mb->cols + j] += 
                    ma->data[i * ma->cols + k] * mb->data[k * mb->cols + j];
            }
        }
    }
    return result;
}

var matrix_transpose(var m) {
    struct Matrix* mat = cast(m, Matrix);
    var result = matrix_new(mat->cols, mat->rows);
    struct Matrix* mr = cast(result, Matrix);
    
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            mr->data[j * mat->rows + i] = mat->data[i * mat->cols + j];
        }
    }
    return result;
}

double matrix_determinant(var m) {
    struct Matrix* mat = cast(m, Matrix);
    if (mat->rows != mat->cols) return 0.0;
    if (mat->rows == 1) return mat->data[0];
    if (mat->rows == 2) {
        return mat->data[0] * mat->data[3] - mat->data[1] * mat->data[2];
    }
    // For larger matrices, would need more complex implementation
    return 0.0;
}

var matrix_inverse(var m) {
    // Simplified inverse for 2x2 matrices only
    struct Matrix* mat = cast(m, Matrix);
    if (mat->rows != 2 || mat->cols != 2) return NULL;
    
    double det = matrix_determinant(m);
    if (det == 0.0) return NULL;
    
    var result = matrix_new(2, 2);
    struct Matrix* mr = cast(result, Matrix);
    
    mr->data[0] = mat->data[3] / det;
    mr->data[1] = -mat->data[1] / det;
    mr->data[2] = -mat->data[2] / det;
    mr->data[3] = mat->data[0] / det;
    
    return result;
}

// Initialization function - TinyCC compatible
void __cello_std_math_init(void) {
    if (math_initialized) return;
    
    // Initialize constants with raw allocation for TinyCC compatibility
    #ifdef CELLO_NGC
        // Use simple static allocation when GC is disabled
        static struct Float pi_obj = {3.14159265358979323846};
        static struct Float e_obj = {2.71828182845904523536};
        static struct Float tau_obj = {6.28318530717958647692};
        static struct Float golden_obj = {1.61803398874989484820};
        PI = &pi_obj;
        E = &e_obj;
        TAU = &tau_obj;
        GOLDEN_RATIO = &golden_obj;
    #else
        // Use simpler approach - just use static structs
        static struct Float pi_obj = {3.14159265358979323846};
        static struct Float e_obj = {2.71828182845904523536};
        static struct Float tau_obj = {6.28318530717958647692};
        static struct Float golden_obj = {1.61803398874989484820};
        PI = &pi_obj;
        E = &e_obj;
        TAU = &tau_obj;
        GOLDEN_RATIO = &golden_obj;
    #endif
    
    // Seed random number generator
    std_seed_random((unsigned int)time(NULL));
    
    math_initialized = true;
}

// TinyCC-safe initialization function
void __cello_std_math_init_safe(void) {
    if (math_initialized) return;
    
    // Initialize constants with static allocation for TinyCC compatibility
    static struct Float pi_obj = {3.14159265358979323846};
    static struct Float e_obj = {2.71828182845904523536};
    static struct Float tau_obj = {6.28318530717958647692};
    static struct Float golden_obj = {1.61803398874989484820};
    PI = &pi_obj;
    E = &e_obj;
    TAU = &tau_obj;
    GOLDEN_RATIO = &golden_obj;
    
    // Seed random number generator
    std_seed_random((unsigned int)time(NULL));
    
    printf("Math module imported (TinyCC safe mode) - constants available\n");
    math_initialized = true;
}