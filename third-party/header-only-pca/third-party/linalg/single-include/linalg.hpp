#pragma once
namespace hola {


struct linalg_obj {
    bool owns_memory;
    struct linalg_obj* memory_owner;
    int ref_count;
    double* data;
};

#ifndef OWNS_MEMORY
#define OWNS_MEMORY(object) (((struct hola::linalg_obj*) object)->owns_memory)
#endif

#ifndef MEMORY_OWNER
#define MEMORY_OWNER(object) (((struct hola::linalg_obj*) object)->memory_owner)
#endif

#ifndef REF_COUNT
#define REF_COUNT(object) (((struct hola::linalg_obj*) object)->ref_count)
#endif

#ifndef DATA
#define DATA(object) (((struct hola::linalg_obj*) object)->data)
#endif

#include <stdbool.h>
#include <stdarg.h>


#ifndef _VECTOR_MACROS
#define _VECTOR_MACROS
#define VECTOR_IDX_INTO(v, i) (DATA(v)[i])
#endif


struct vector {
    struct linalg_obj la_obj;
    int length;
};

struct vector* vector_new(int length);
struct vector* vector_new_view(struct linalg_obj* parent, double* view, int length);
struct vector* vector_from_array(double* data, int length);
void           vector_free(struct vector*);
void           vector_free_many(int n_to_free, ...);

struct vector* vector_zeros(int length);
struct vector* vector_constant(int length, double x);
struct vector* vector_linspace(int length, double min, double max);
struct vector* vector_slice(struct vector* v, int begin_idx, int end_idx);
struct vector* vector_copy(struct vector* v);
void           vector_copy_into(struct vector* reciever, struct vector* v);


struct vector* vector_subtract(struct vector* v1, struct vector* v2);
void           vector_subtract_into(struct vector* reciever,
                                    struct vector* v1, struct vector* v2);
struct vector* vector_add(struct vector* v1, struct vector* v2);
void           vector_add_into(struct vector* reciever,
                               struct vector* v1, struct vector* v2);
struct vector* vector_normalize(struct vector* v);
void           vector_normalize_into(struct vector* reciever, struct vector* v);
struct vector* vector_scalar_multiply(struct vector* v, double s);
void           vector_scalar_multiply_into(struct vector* reciever,
                                           struct vector* v, double s);

bool           vector_equal(struct vector* v1, struct vector* v2, double tol);

double         vector_dot_product(struct vector* v1, struct vector* v2);
double         vector_norm(struct vector* v);

void           vector_print(struct vector*);

bool           vector_lengths_equal(struct vector* v1, struct vector* v2);

#include <stdarg.h>



struct matrix {
    struct linalg_obj la_obj;
    int n_row;
    int n_col;
};


#ifndef _MATRIX_MACROS
#define _MATRIX_MACROS
#define MATRIX_ROW(M, i) ((i) / (M->n_row))
#define MATRIX_COL(M, i) ((i) % (M->n_row))
#define MATRIX_IDX(M, r, c) (((r) * (M->n_col)) + (c))
#define MATRIX_IDX_INTO(M, r, c) (DATA(M)[MATRIX_IDX(M, r, c)])
#endif


struct matrix* matrix_new(int n_row, int n_col);
struct matrix* matrix_from_array(double* data, int n_row, int n_col);
void           matrix_free(struct matrix* M);
void           matrix_free_many(int n_to_free, ...);

struct matrix* matrix_copy(struct matrix* M);

struct vector* matrix_row_view(struct matrix* M, int row);
struct vector* matrix_row_copy(struct matrix* M, int row);
/* No column view, columns are not contiguous memory */
struct vector* matrix_column_copy(struct matrix* M, int col);

struct vector* matrix_diagonal(struct matrix* M);

void           matrix_copy_vector_into_row(struct matrix* M, struct vector* v, int row);
void           matrix_copy_vector_into_column(struct matrix* M, struct vector* v, int col);

struct matrix* matrix_zeros(int n_row, int n_col);
struct matrix* matrix_identity(int size);

struct matrix* matrix_transpose(struct matrix* M);
struct matrix* matrix_multiply_cache(struct matrix* Mleft, struct matrix* Mright, int cache);
struct matrix* matrix_multiply(struct matrix* Mleft, struct matrix* Mright);
void           matrix_multiply_into(struct matrix* reciever,
                                    struct matrix* Mleft, struct matrix* Mright);
struct matrix* matrix_multiply_MtN(struct matrix* Mleft, struct matrix* Mright);
struct vector* matrix_vector_multiply(struct matrix* M, struct vector* v);
struct vector* matrix_vector_multiply_Mtv(struct matrix* M, struct vector* v);

struct matrix* matrix_M_minus_lambda_I(struct matrix* M, double lambda);

bool           matrix_equal(struct matrix* M1, struct matrix* M2, double tol);
bool           matrix_is_upper_triangular(struct matrix* M, double tol);

void           matrix_print(struct matrix*);


/* Matrix decomposition: QR decomposition. */
struct qr_decomp {
    struct matrix* q;
    struct matrix* r;
};

struct qr_decomp* qr_decomp_new(struct matrix* M);
void              qr_decomp_free(struct qr_decomp* qr);

struct qr_decomp* matrix_qr_decomposition(struct matrix* M);



struct linreg {
    int n;
    int p;
    struct vector* beta;
    struct vector* y_hat;
    double sigma_resid;
};

struct linreg* linreg_new(void);
void           linreg_free(struct linreg* lr);

struct linreg* linreg_fit(struct matrix* X, struct vector* y);
struct vector* linreg_predict(struct linreg* lr, struct matrix* X);


void check_memory(void* mem);




struct vector* linsolve_qr(struct matrix* M, struct vector* v);
struct vector* linsolve_from_qr(struct qr_decomp* qr, struct vector* v);
struct vector* linsolve_upper_triangular(struct matrix* M, struct vector* v);




struct eigen {
    int n;
    struct vector* eigenvalues;
    struct matrix* eigenvectors;
};

struct eigen* eigen_new();
void eigen_free(struct eigen* e);

struct eigen* eigen_solve(struct matrix* M, double tol, int max_iter);
struct vector* eigen_solve_eigenvalues(struct matrix* M, double tol, int max_iter);
struct matrix* eigen_solve_eigenvectors(
    struct matrix* M, struct vector* eigenvectors, double tol, int max_iter);
struct vector* eigen_backsolve(
    struct matrix* M, double eigenvalue, double tol, int max_iter);


void raise_memory_allocation_error();
void raise_non_commensurate_vector_error();
void raise_non_zero_reference_free_error();





void init_random();
struct vector* vector_random_uniform(int length, double low, double high);
struct matrix* matrix_random_uniform(int n_row, int n_col, double low, double high);

struct vector* vector_random_gaussian(int length, double mu, double sigma);
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>





struct vector* vector_new(int length) {
    assert(length >= 0);

    struct vector* new_vector = static_cast<struct vector*>(malloc(sizeof(struct vector)));;
    check_memory((void*) new_vector);

    DATA(new_vector) = static_cast<double*>(malloc((sizeof(double)) * length));;
    check_memory((void*) DATA(new_vector));

    new_vector->length = length;
    OWNS_MEMORY(new_vector)= true;
    MEMORY_OWNER(new_vector) = NULL;
    REF_COUNT(new_vector) = 0;

    return new_vector;
}

/* Create a new vector which is a *view* into an already existing vector.

   The new and parent vectors share the same data, and modifying the data in
   either will modify both vectors.  One the other hand, we do not have to copy
   any data to create a view.
*/
struct vector* vector_new_view(struct linalg_obj* parent, double* view, int length) {
    assert(length >= 0);
    // TODO: Make this view check work.
    /* Check that pointers to the beginning and end of view vector live
       within the data segment of the parent object.

       This doesn't work because matricies have no length.  This could be
       a property of linalg_obj, but then a macro would be needed to
       make the lookup type generic.

    assert(DATA(parent) <= view && view < DATA(parent) + parent->length);
    assert(view + length <= DATA(parent) + parent->length);
    */

    struct vector* new_vector = static_cast<struct vector*>(malloc(sizeof(struct vector)));;
    check_memory((void*)new_vector);

    DATA(new_vector) = view;
    new_vector->length = length;
    OWNS_MEMORY(new_vector) = false;
    MEMORY_OWNER(new_vector) = parent;
    REF_COUNT(new_vector) = 0;
    REF_COUNT(parent) += 1;

    return new_vector;
}

struct vector* vector_from_array(double* data, int length) {
    assert(length > 0);
    struct vector* v = vector_new(length);
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(v, i) = data[i];
    }
    return v;
}

void vector_free(struct vector* v) {
    struct linalg_obj* mem_owner;
    if(OWNS_MEMORY(v)) {
        if(REF_COUNT(v) == 0) {
            free(DATA(v));
            free(v);
        } else {
            raise_non_zero_reference_free_error();
        }
    } else {
        if(REF_COUNT(v) == 0) {
            mem_owner = MEMORY_OWNER(v);
            REF_COUNT(mem_owner) -= 1;
            free(v);
        } else {
            raise_non_zero_reference_free_error();
        }
    }
}

void vector_free_many(int n_to_free, ...) {
    struct vector* v;
    va_list argp;
    va_start(argp, n_to_free);
    for(int i = 0; i < n_to_free; i++) {
        v = va_arg(argp, struct vector*);
        vector_free(v);
    }
}

/* Construct a vector of a given length filled with a given constant. */
struct vector* vector_constant(int length, double x) {
    assert(length > 0);
    struct vector* v = vector_new(length);
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(v, i) = x;
    }
    return v;
}

/* Construct a vector of a given length filled with zeros. */
struct vector* vector_zeros(int length) {
    assert(length > 0);
    return vector_constant(length, 0);
}

/* Construct a vector of equally spaced points within the closed
   interval [min, max].
*/
struct vector* vector_linspace(int length, double min, double max) {
    assert(min <= max);
    assert(length > 1);
    struct vector* v = vector_new(length);
    double step = (max - min) / (length - 1);
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(v, i) = min + step*i;
    }
    return v;
}

/* Construct a view into a segement of a vector.  The returned vector is
   a reference to a segment of data contained in the vector v, with left
   index begin_idx and right index (excluded) end_idx.
*/
struct vector* vector_slice(struct vector* v, int begin_idx, int end_idx) {
    assert(begin_idx <= end_idx);
    assert(end_idx <= v->length - 1);
    int new_vector_length = end_idx - begin_idx;
    double* begin_ptr = DATA(v) + begin_idx;
    struct vector* w = vector_new_view((struct linalg_obj*) v, begin_ptr, new_vector_length);
    return w;
}

/* Copy all the data in a given vector into a new vector. */
struct vector* vector_copy(struct vector* v) {
    struct vector* w = vector_new(v->length);
    vector_copy_into(w, v);
    return w;
}

void vector_copy_into(struct vector* reciever, struct vector* v) {
    assert(v->length == reciever->length);
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(reciever, i) = VECTOR_IDX_INTO(v, i);
    }
}

/* Arithmatic methods.

    Each of the following methods implements an arithmetic operation on vectors.
  Each comes in two flavors, anaoagous to the + and += operators:

    - vector_operation(v1, v2) applies the given operation elementwise to pairs
        taken from v1, v2, and stores the results in a *new* vector, which is
        then returned.
    - vector_operation_into(v1, v2) applies the given operation elementwise to
        pairs taken from v1, v2, and stores the results in v1.  Note that this
        operation destroys the data in v1, and if v1 is a reference, will
        mutate the referenced object.
*/
struct vector* vector_subtract(struct vector* v1, struct vector* v2) {
    assert(vector_lengths_equal(v1, v2));
    struct vector* v = vector_new(v1->length);
    vector_subtract_into(v, v1, v2);
    return v;
}

void vector_subtract_into(struct vector* reciever, struct vector* v1, struct vector* v2) {
    assert(vector_lengths_equal(v1, v2));
    for(int i = 0; i < v1->length; i++) {
        VECTOR_IDX_INTO(reciever, i) = VECTOR_IDX_INTO(v1, i) - VECTOR_IDX_INTO(v2, i);
    }
}

struct vector* vector_add(struct vector* v1, struct vector* v2) {
    assert(vector_lengths_equal(v1, v2));
    struct vector* v = vector_new(v1->length);
    vector_add_into(v, v1, v2);
    return v;
}

void vector_add_into(struct vector* reciever, struct vector* v1, struct vector* v2) {
    assert(vector_lengths_equal(v1, v2));
    for(int i = 0; i < v1->length; i++) {
        VECTOR_IDX_INTO(reciever, i) = VECTOR_IDX_INTO(v1, i) + VECTOR_IDX_INTO(v2, i);
    }
}

struct vector* vector_normalize(struct vector* v) {
    struct vector* vnorm = vector_new(v->length);
    [[maybe_unused]] double norm = vector_norm(v);
    assert(norm != 0);
    vector_normalize_into(vnorm, v);
    return vnorm;
}

void vector_normalize_into(struct vector* reciever, struct vector* v) {
    [[maybe_unused]] double norm = vector_norm(v);
    assert(norm != 0);
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(reciever, i) = VECTOR_IDX_INTO(v, i) / norm;
    }
}

struct vector* vector_scalar_multiply(struct vector* v, double s) {
    struct vector* w = vector_new(v->length);
    vector_scalar_multiply_into(w, v, s);
    return w;
}

void vector_scalar_multiply_into(struct vector* reciever, struct vector* v, double s) {
    for(int i = 0; i < v->length; i++) {
        VECTOR_IDX_INTO(reciever, i) = VECTOR_IDX_INTO(v, i) * s;
    }
}

/* Check that two vectors are equal to within some additive tolerance. */
bool vector_equal(struct vector* v1, struct vector* v2, double tol) {
    if(!vector_lengths_equal(v1, v2)) {
        return false;
    }
    for(int i = 0; i < v1->length; i++) {
        if(fabs(VECTOR_IDX_INTO(v1, i) - VECTOR_IDX_INTO(v2, i)) > tol) {
            return false;
        }
    }
    return true;
}

bool vector_lengths_equal(struct vector* v1, struct vector* v2) {
    return (v1->length == v2->length);
}

double vector_dot_product(struct vector* v1, struct vector* v2) {
    assert(vector_lengths_equal(v1, v2));
    double dp = 0;
    for(int i = 0; i < v1->length; i++) {
        dp += VECTOR_IDX_INTO(v1, i) * VECTOR_IDX_INTO(v2, i);
    }
    return dp;
}

double vector_norm(struct vector* v) {
    double norm_squared = vector_dot_product(v, v);
    return sqrt(norm_squared);
}

/* Print a vector to the console like:
    [1, 2, 3, 4]
*/
// TODO: Maybe this should return a string, we are computing the representation and
// displaying it in the same method.
void vector_print(struct vector* v) {
    if(v->length == 0) {
        printf("[]\n");
    } else if (v->length == 1) {
        printf("[%.2f]\n", VECTOR_IDX_INTO(v, 0));
    } else {
        printf("[%.2f", VECTOR_IDX_INTO(v, 0));
        for(int i = 1; i < v->length - 1; i++) {
            printf(", ");
            printf("%.2f", VECTOR_IDX_INTO(v, i));
        }
        printf(", %.2f]\n", VECTOR_IDX_INTO(v, v->length - 1));
    }
}
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>




void init_random() {
    srand(time(NULL));
}


/***********************
 * Uniform Distribution
 ***********************/

double _random_uniform(double low, double high) {
    assert(low < high);
    return (high - low) * ((double) rand() / (double) RAND_MAX) - low;
}

struct vector* vector_random_uniform(int length, double low, double high) {
    assert(length > 0);
    struct vector* v = vector_new(length);
    for(int i = 0; i < length; i++) {
        VECTOR_IDX_INTO(v, i) = _random_uniform(low, high);
    }
    return v;
}

struct matrix* matrix_random_uniform(int n_row, int n_col, double low, double high) {
    assert(n_row > 0);
    assert(n_col >0);
    struct matrix* M = matrix_new(n_row, n_col);
    for(int i = 0; i < n_row; i++) {
        for(int j = 0; j < n_col; j++) {
            MATRIX_IDX_INTO(M, i, j) = _random_uniform(low, high);
        }
    }
    return M;
}


/*************************
 * Gaussian Dristribution
 *************************/

/* Generate a number distributed as random gaussian noise using the Box-Muller
 * method.
   The trick of using the static keyword to remember pairs of values is taken
   from wikipedia.
*/
double _random_gaussian(double mu, double sigma) {
    const double two_pi = 2.0 * 3.14159265358979323846;

    static double z0, z1;
    double u0, u1;

    // Since the strategy employed generates pairs of random values, we can,
    // on each other call, simply remember the leftover value and return it.
    static bool generate_new_values = false;
    generate_new_values = !generate_new_values;
    if (!generate_new_values) {
        return z1 * sigma + mu;
    }

    u0 = _random_uniform(0, 1);
    u1 = _random_uniform(0, 1);
    z0 = sqrt(-2.0 * log(u0)) * cos(two_pi * u1);
    z1 = sqrt(-2.0 * log(u0)) * sin(two_pi * u1);

    return z0 * sigma + mu;
}

struct vector* vector_random_gaussian(int length, double mu, double sigma) {
    assert(length > 0);
    struct vector* v = vector_new(length);
    for(int i = 0; i < length; i++) {
        VECTOR_IDX_INTO(v, i) = _random_gaussian(mu, sigma);
    }
    return v;
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>





/* Linear Regression.

  Fit and score a linear regression to a design matrix X and a response vector y.
  The regression is fit by solving the following linear equation for b:

      Xt X b = Xt y
*/

struct linreg* linreg_new(void) {
    struct linreg* lr = static_cast<struct linreg*>(malloc(sizeof(struct linreg)));
    return lr;
}

void linreg_free(struct linreg* lr) {
    vector_free(lr->beta);
    vector_free(lr->y_hat);
    free(lr);
}

/* Solve a linear regression problem using the qr decomposition of the matrix X.

  The idea here is that if X = QR, then the linear regression equations reduce
  to R b = Q^t y.
*/
struct linreg* linreg_fit(struct matrix* X, struct vector* y) {
    assert(X->n_row == y->length);
    struct linreg* lr = linreg_new();
    lr->n = X->n_row;
    lr->p = X->n_col;

    // Solve linear equation for the regression coefficients.
    struct qr_decomp* qr = matrix_qr_decomposition(X);
    struct vector* qtv = matrix_vector_multiply_Mtv(qr->q, y);
    lr->beta = linsolve_upper_triangular(qr->r, qtv);

    // Calculate the residual standard deviation.
    struct vector* y_hat = linreg_predict(lr, X);
    double sigma_resid_sq = 0; double resid = 0;
    for(int i = 0; i < y->length; i++) {
        resid = VECTOR_IDX_INTO(y, i) - VECTOR_IDX_INTO(y_hat, i);
        sigma_resid_sq += resid * resid;
    }
    double norm_factor = 1 / (lr->n - lr->p - 1);
    double sigma_resid = sqrt(norm_factor * sigma_resid_sq);
    lr->y_hat = y_hat;
    lr->sigma_resid = sigma_resid;

    qr_decomp_free(qr);
    vector_free(qtv);

    return lr;
}

struct vector* linreg_predict(struct linreg* lr, struct matrix* X) {
    assert(lr->n == X->n_row);
    assert(lr->p == X->n_col);
    struct vector* preds = matrix_vector_multiply(X, lr->beta);
    return preds;
}
#include <assert.h>
#include <stdlib.h>





struct eigen* eigen_new() {
    struct eigen* e = static_cast<struct eigen*>(malloc(sizeof(struct eigen)));
    return e;
}

void eigen_free(struct eigen* e) {
    vector_free(e->eigenvalues);
    matrix_free(e->eigenvectors);
    free(e);
}

/* Compute the eigenvalues and eigenvectors of a matrix M.

  The eigenvalues are computed using the QR algorithm, then the eigenvectors
  are computed by inverse iteration.
*/
struct eigen* eigen_solve(struct matrix* M, double tol, int max_iter) {
    assert(M->n_row == M->n_col);

    struct vector* eigenvalues = eigen_solve_eigenvalues(M, tol, max_iter);
    struct matrix* eigenvectors = eigen_solve_eigenvectors(
                                      M, eigenvalues, tol, max_iter);

    struct eigen* e = eigen_new();
    e->n = M->n_row;
    e->eigenvalues = eigenvalues;
    e->eigenvectors = eigenvectors;

    return e;
}

/* Compute the eigenvalues of a matrix using the QR algorithm.

  This is a renormalized version of power iteration that converges to a full
  set of eigenvalues.  Starting with the matrix M = M0, we iterate:

    M0 = Q0 R0, M1 = R0 Q0;
    M1 = Q1 R1, M2 = R1 Q1;
    M2 = Q2 R2, M3 = R2 Q2;
    ...

  For a general matrix with a full set of eigenvalues, this sequence will
  converge to an upper diagonal matrix:

    Mi -> upper diagonal matrix

  The diagonal entries of this matrix are the eigenvalues of M.
*/
struct vector* eigen_solve_eigenvalues(struct matrix* M,
                                       double tol,
                                       int max_iter) {

    assert(M->n_row == M->n_col);

    struct matrix* X = matrix_copy(M);
    int i = 0;
    // QR algorithm iterations.
    do {
        struct qr_decomp* qr = matrix_qr_decomposition(X);
        matrix_multiply_into(X, qr->r, qr->q);
        qr_decomp_free(qr);
        i++;
    } while(!matrix_is_upper_triangular(X, tol) && (i < max_iter));

    struct vector* res = matrix_diagonal(X);

    matrix_free( X );

    return res;

}

/* Solve for the eigenvectors of a matrix M once the eigenvalues are known
   using inverse iteration.
*/
struct matrix* eigen_solve_eigenvectors(struct matrix* M,
                                        struct vector* eigenvalues,
                                        double tol,
                                        int max_iter) {

    assert(eigenvalues->length = M->n_row);
    assert(eigenvalues->length = M->n_col);

    double eigenvalue;
    int n_eigenvalues = M->n_col;
    struct matrix* eigenvectors = matrix_new(n_eigenvalues, n_eigenvalues);

    for(int i = 0; i < n_eigenvalues; i++) {
        eigenvalue = VECTOR_IDX_INTO(eigenvalues, i);
        struct vector* eigenvector = eigen_backsolve(M, eigenvalue, tol, max_iter);
        matrix_copy_vector_into_column(eigenvectors, eigenvector, i);
        vector_free(eigenvector);
    }

    return eigenvectors;
}

/* Solve for the eigenvector associated with an eigenvalue using the inverse
   iteration algorithm.

  Given an approximate eigenvalue lambda, the inverse iteration algorithm
  computes the matrix:

    M' = M - lambda I

  And then solves the following sequence of linear equations:

    v0 = solve(M', random_vector), v0' = normalize(v0);
    v1 = solve(M', v0'), v1' = normalize(v1);
    v2 = solve(<', v1'), v2' = normalize(v2);
    ...

  This algorithm will converge to the eigenvector associated with the eigenvalue
  closest to lambda.
*/
struct vector* eigen_backsolve(
                   struct matrix* M, double eigenvalue, double tol, int max_iter) {

    struct vector* current = vector_constant(M->n_row, 1);
    struct vector* previous;
    // Preturb the eigenvalue a litle to prevent our right hand side matrix
    // from becoming singular.
    double lambda = eigenvalue + ((double) rand() / (double) RAND_MAX) * 0.000001;

    struct matrix* M_minus_lambda_I = matrix_M_minus_lambda_I(M, lambda);

    double i = 0;
    do {
        if(i > 0) {
            vector_free(previous);
        }
        previous = current;
        current = linsolve_qr(M_minus_lambda_I, previous);
        // We reverse the sign of the vector if the first entry is not positive.
        // Often the algorithm will oscilate between a vector and its negative
        // after convergence.
        if(VECTOR_IDX_INTO(current, 0) < 0) {
            for(int j = 0; j < current->length; j++) {
                VECTOR_IDX_INTO(current, j) = -VECTOR_IDX_INTO(current, j);
            }
        }
        vector_normalize_into(current, current);
        // vector_print(current);
        i++;
    } while(!vector_equal(current, previous, tol) && (i < max_iter));

    vector_free(previous);
    matrix_free(M_minus_lambda_I);
    return current;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>







struct matrix* matrix_new(int n_row, int n_col) {
    assert(n_row >= 1 && n_col >= 1);
    struct matrix* new_matrix = static_cast<struct matrix*>(malloc(sizeof(struct matrix)));
    check_memory((void*) new_matrix);

    DATA(new_matrix) = static_cast<double*>(malloc((sizeof(double)) * n_row * n_col));
    check_memory((void*) DATA(new_matrix));

    new_matrix->n_row = n_row;
    new_matrix->n_col = n_col;
    OWNS_MEMORY(new_matrix) = true;
    MEMORY_OWNER(new_matrix) = NULL;
    REF_COUNT(new_matrix) = 0;

    return new_matrix;
}

struct matrix* matrix_from_array(double* data, int n_row, int n_col) {
    assert(n_row >= 1 && n_col >= 1);
    struct matrix* M = matrix_new(n_row, n_col);
    for(int i = 0; i < n_row * n_col; i++) {
        DATA(M)[i] = data[i];
    }
    return M;
}

void matrix_free(struct matrix* M) {
    struct linalg_obj* mem_owner;
    if(OWNS_MEMORY(M)) {
        if(REF_COUNT(M) == 0) {
            free(DATA(M));
            free(M);
        } else {
            raise_non_zero_reference_free_error();
        }
    } else {
        if(REF_COUNT(M) == 0) {
            mem_owner = MEMORY_OWNER(M);
            REF_COUNT(mem_owner) -= 1;
            free(M);
        } else {
            raise_non_zero_reference_free_error();
        }
    }
}

void matrix_free_many(int n_to_free, ...) {
    struct matrix* v;
    va_list argp;
    va_start(argp, n_to_free);
    for(int i = 0; i < n_to_free; i++) {
        v = va_arg(argp, struct matrix*);
        matrix_free(v);
    }
}

struct matrix* matrix_copy(struct matrix* M) {
    struct matrix* copy = matrix_new(M->n_row, M->n_col);
    for(int i = 0; i < M->n_row; i++) {
        for(int j = 0; j < M->n_col; j++) {
            MATRIX_IDX_INTO(copy, i, j) = MATRIX_IDX_INTO(M, i, j);
        }
    }
    return copy;
}

/* Create a new vector which is a *view* into a row of data in a matrix.

   The new and parent objects share the same data, and modifying the data in
   either will modify both vectors.  One the other hand, we do not have to copy
   any data to create a view.

   Note that this works because a *row* in a matrix is a contiguous chunk of
   data, a column is not, so there is no column_view method.
*/
struct vector* matrix_row_view(struct matrix* M, int row) {
    assert(0 <= row && row <= M->n_row - 1);
    double* row_p = DATA(M) + (row * M->n_col);
    struct vector* r = vector_new_view((struct linalg_obj*) M, row_p, M->n_col);
    return r;
}

/* Create a new vector which contains a copy of a row of a matrix. */
struct vector* matrix_row_copy(struct matrix* M, int row) {
    assert(0 <= row && row <= M->n_row - 1);
    struct vector* r = vector_new(M->n_col);
    for(int i = 0; i < M->n_col; i++) {
        VECTOR_IDX_INTO(r, i) = MATRIX_IDX_INTO(M, row, i);
    }
    return r;
}

/* Create a new vector which contains a copy of a row of a matrix. */
struct vector* matrix_column_copy(struct matrix* M, int col) {
    assert(0 <= col && col <= M->n_col - 1);
    struct vector* c = vector_new(M->n_row);
    for(int i = 0; i < M->n_row; i++) {
        VECTOR_IDX_INTO(c, i) = MATRIX_IDX_INTO(M, i, col);
    }
    return c;
}

/* Copy the diagonal of a matrix into a vector. */
struct vector* matrix_diagonal(struct matrix* M) {
    // The minimum of the number of rows and columns.
    int n = (M->n_row <= M->n_col) ? M->n_row : M->n_col;
    struct vector* diagonal = vector_zeros(n);
    for(int i = 0; i < n; i++) {
        VECTOR_IDX_INTO(diagonal, i) = MATRIX_IDX_INTO(M, i, i);
    }
    return diagonal;
}

/* Copy the data in a vector into a row of a matrix.

   Note that this method modifies the matrix in place, it does not create a
   new matrix.
*/
void matrix_copy_vector_into_row(struct matrix* M, struct vector* v, int row) {
    assert(M->n_col == v->length);
    for(int i = 0; i < v->length; i++) {
        MATRIX_IDX_INTO(M, row, i) = VECTOR_IDX_INTO(v, i);
    }
}

/* Copy the data in a vector into a column of a matrix.

   Note that this method modifies the matrix in place, it does not create a
   new matrix.
*/
void matrix_copy_vector_into_column(struct matrix* M, struct vector* v, int col) {
    assert(M->n_row == v->length);
    for(int i = 0; i < v->length; i++) {
        MATRIX_IDX_INTO(M, i, col) = VECTOR_IDX_INTO(v, i);
    }
}

/* Create a new matrix of a given dimension filled with zeros. */
struct matrix* matrix_zeros(int n_row, int n_col) {
    assert(n_row >= 1 && n_col >= 1);
    struct matrix* M = matrix_new(n_row, n_col);
    for(int i = 0; i < n_row * n_col; i++) {
        DATA(M)[i] = 0;
    }
    return M;
}

/* Create a (square) identity matrix of a given size. */
struct matrix* matrix_identity(int size) {
    assert(size >= 1);
    struct matrix* M = matrix_new(size, size);
    for(int i = 0; i < size * size; i++) {
        if(MATRIX_ROW(M, i) == MATRIX_COL(M, i)) {
            DATA(M)[i] = 1;
        } else {
            DATA(M)[i] = 0;
        }
    }
    return M;
}

/* Transpose a matrix.

   Note that this creates a new matrix, and copies the data into the new matrix.
*/
struct matrix* matrix_transpose(struct matrix* M) {
    struct matrix* Mt = matrix_new(M->n_col, M->n_row);
    for(int i = 0; i < M->n_row; i++) {
        for(int j = 0; j < M->n_col; j++) {
            MATRIX_IDX_INTO(Mt, j, i) = MATRIX_IDX_INTO(M, i, j);
        }
    }
    return Mt;
}

// TODO: Transpose a square matrix in place.

/* An attempt at a cache blocking algorithm for matrix multiplication

   I couldn't get this to run any faster than the naive algorithm with the j-k
   loop intercahnge.  Im not sure what I'm doing wrong at this point, it may turn
   into a stack overflow question at some point.

struct matrix* matrix_multiply_cache(struct matrix* Mleft, struct matrix* Mright, int cache) {
    assert(Mleft->n_col == Mright->n_row);
    struct matrix* Mprod = matrix_zeros(Mleft->n_row, Mright->n_col);
    // Cache blocking loops.
    for(int ii = 0; ii < Mprod->n_row; ii += cache) {
        for(int kk = 0; kk < Mleft->n_col; kk += cache) {
            for(int jj = 0; jj < Mprod->n_col; jj += cache) {
                // Matrix multiplication loops.
                for(int i = ii; i <  ii + cache && i < Mprod->n_row; i++) {
                    for(int k = kk; k < kk + cache && k < Mleft->n_col; k++) {
                        for(int j = jj; j < jj + cache && j < Mprod->n_col; j++) {
                            MATRIX_IDX_INTO(Mprod, i, j) +=
                                MATRIX_IDX_INTO(Mleft, i, k) * MATRIX_IDX_INTO(Mright, k, j);
                        }
                    }
                }
            }
        }
    }
    return Mprod;
}
*/


/* Compute the matrix product of two aligned matricies.

   Note that the order of the loops, i-k-j, is chosen to optimize the memory
   access patterns.  In this order the innermost loop is accessing memory
   contiguously.
*/
struct matrix* matrix_multiply(struct matrix* Mleft, struct matrix* Mright) {
    assert(Mleft->n_col == Mright->n_row);
    struct matrix* Mprod = matrix_zeros(Mleft->n_row, Mright->n_col);
    for(int i = 0; i < Mprod->n_row; i++) {
        for(int k = 0; k < Mleft->n_col; k++) {
            for(int j = 0; j < Mprod->n_col; j++) {
                MATRIX_IDX_INTO(Mprod, i, j) +=
                    MATRIX_IDX_INTO(Mleft, i, k) * MATRIX_IDX_INTO(Mright, k, j);
            }
        }
    }
    return Mprod;
}

void matrix_multiply_into(struct matrix* reciever,
                          struct matrix* Mleft, struct matrix* Mright) {
    assert(Mleft->n_col == Mright->n_row);
    // Zero out the reciever matrix.
    for(int i = 0; i < reciever->n_row; i++) {
        for(int j = 0; j < reciever->n_col; j++) {
            MATRIX_IDX_INTO(reciever, i, j) = 0;
        }
    }
    // Now multiply.
    for(int i = 0; i < Mleft->n_row; i++) {
        for(int k = 0; k < Mleft->n_col; k++) {
            for(int j = 0; j < Mright->n_col; j++) {
                MATRIX_IDX_INTO(reciever, i, j) +=
                    MATRIX_IDX_INTO(Mleft, i, k) * MATRIX_IDX_INTO(Mright, k, j);
            }
        }
    }
}

/* Compute the matrix product transpose(M) * N.

   This method is more efficient than an expicit transpose followed by probuct
   of the matrix M, which would necessitate a copy of all data in M.  The order
   of the loops, k-i-j, is chosen to optimize memory access patterns.  The
   innter two loops index contiguous memory in the factor matricies.
*/
struct matrix* matrix_multiply_MtN(struct matrix* Mleft, struct matrix* Mright) {
    assert(Mleft->n_row == Mright->n_row);
    struct matrix* Mprod = matrix_zeros(Mleft->n_col, Mright->n_col);
    for(int k = 0; k < Mleft->n_row; k++) {
        for(int i = 0; i < Mprod->n_row; i++) {
            for(int j = 0; j < Mprod->n_col; j++) {
                MATRIX_IDX_INTO(Mprod, i, j) +=
                    MATRIX_IDX_INTO(Mleft, k, i) * MATRIX_IDX_INTO(Mright, k, j);
            }
        }
    }
    return Mprod;
}

/* Compute the product of an aligned matrix vector pair. */
struct vector* matrix_vector_multiply(struct matrix* M, struct vector* v) {
    assert(M->n_col == v->length);
    struct vector* w = vector_new(M->n_row);
    double sum;
    for(int i = 0; i < M->n_row; i++) {
        sum = 0;
        for(int j = 0; j < M->n_col; j++) {
            sum += MATRIX_IDX_INTO(M, i, j) * VECTOR_IDX_INTO(v, j);
        }
        VECTOR_IDX_INTO(w, i) = sum;
    }
    return w;
}

/* Compute the product of the transpose of a matrix M with a vector v.

   This method is more efficient than an expicit transpose of the matrix M,
   which would necessitate a copy of all data in M.  The order of the loops,
   j-i, is chosen to optimize memory access patterns.  In this order the
   data within the matrix is accesses contigously in the innermost loop.
*/
struct vector* matrix_vector_multiply_Mtv(struct matrix* M, struct vector* v) {
    assert(M->n_row == v->length);
    struct vector* w = vector_zeros(M->n_col);
    for(int j = 0; j < M->n_row; j++) {
        for(int i = 0; i < M->n_col; i++) {
            VECTOR_IDX_INTO(w, i) += MATRIX_IDX_INTO(M, j, i) * VECTOR_IDX_INTO(v, j);
        }
    }
    return w;
}

/* Compute the expression M - lambda I, where:

  - M is a square matrix.
  - lambda is a number (generally an eigenvalue).
  - I is an identity matrix.

This is useful when backsolving for an eigenvector with known eigenvalue.
*/
struct matrix* matrix_M_minus_lambda_I(struct matrix* M, double lambda) {
    assert(M->n_row == M->n_col);
    struct matrix* M_minus_lambda_I = matrix_copy(M);

    for(int i = 0; i < M->n_row; i++) {
        MATRIX_IDX_INTO(M_minus_lambda_I, i, i) -= lambda;
    }

    return M_minus_lambda_I;
}

/* Are two matricies equal to within a given tolerance? */
bool matrix_equal(struct matrix* M1, struct matrix* M2, double tol) {
    if(M1->n_row != M2->n_row) {
        return false;
    }
    if(M1->n_col != M2->n_col) {
        return false;
    }
    for(int i = 0; i < M1->n_row; i++) {
        for(int j = 0; j < M1->n_col; j++) {
            if(fabs(MATRIX_IDX_INTO(M1, i, j) - MATRIX_IDX_INTO(M2, i, j)) > tol) {
                return false;
            }
        }
    }
    return true;
}

/* Is a matrix upper triangular to within a given tolerance? */
bool matrix_is_upper_triangular(struct matrix* M, double tol) {
    // For now, only treat square matricies. */
    assert(M->n_row == M->n_col);
    for (int i = 0; i < M->n_row; i++) {
        for(int j = 0; j < i; j++) {
            if(fabs(MATRIX_IDX_INTO(M, i, j)) > tol) {
                return false;
            }
        }
    }
    return true;
}

/* Print a matrix to the console like:
    [
      [1, 2],
      [3, 4]
    ]
*/
// TODO: Maybe this should return a string, we are computing the representation and
// displaying it in the same method.
void matrix_print(struct matrix* M) {
    struct vector* current_row;
    printf("[\n");
    for(int i = 0; i < M->n_row; i++) {
        printf("  ");
        current_row = matrix_row_view(M, i);
        vector_print(current_row);
        vector_free(current_row);
    }
    printf("]\n");
}


/* The QR decomposition of a matrix M.

   This decomposition factors a general matrix into the product of an
   orthogonal matrix Q with an upper triangular matrix R.

   The decomposition will fail if M is rank deficcient.  Currently this will
   simply cause the algorithm to fail with a divide by zero error.  I plan
   to implement some error tracking system into linalg_obj in the future.
*/
struct qr_decomp* qr_decomp_new(struct matrix* M) {
    struct qr_decomp*  qr = static_cast<struct qr_decomp*>(malloc(sizeof(struct qr_decomp)));;
    return qr;
}

void qr_decomp_free(struct qr_decomp* qr) {
    matrix_free(qr->q);
    matrix_free(qr->r);
    free(qr);
}

/* Compute the QR decomposition of a matrix M.

   The current algorithm here is grahm-schmidt.  The columns of M are converted
   into an orthogonal basis by projection onto the previous vectors in the
   basis and subtracting out the projections from the current vector, resulting
   in a matrix Q.  These projections are accumulated into the matrix R.

   The resulting matricies Q and R satisfy:
     - M = Q * R
     - transpose(Q) * Q = Identity
     - R is upper triangular

   This decomposition gives a convienient way to solve general linear equations.
*/
// TODO: Split matrix decompositions out into their own module (matrix_decomp.c).
struct qr_decomp* matrix_qr_decomposition(struct matrix* M) {
    struct qr_decomp* qr = qr_decomp_new(M);
    struct matrix* q = matrix_new(M->n_row, M->n_col);
    struct matrix* r = matrix_zeros(M->n_col, M->n_col);
    /* current_column:
       Initialized to the columns in M, in an outer loop.
       Transformed by subtracting out the projections of current_column onto
       the currently existing columns of Q.  After all projections are removed,
       what results is a vector orthogonal to all previous columns in Q.
    */
    struct vector* current_column;
    /* current_unit_vector:
       Used to hold previously computed columns in Q, and the projections of
       columns of M onto them.
    */
    struct vector* current_unit_vector;
    double current_dot_product;
    double norm;

    for(int i = 0; i < M->n_col; i++) {
        current_column = matrix_column_copy(M, i);
        for(int j = 0; j < i; j++) {
            // TODO: Allocate current_unit_vector one time, and then copy *into* this
            // vector, saving a malloc and free.
            current_unit_vector = matrix_column_copy(q, j);
            current_dot_product = vector_dot_product(current_unit_vector, current_column);
            vector_scalar_multiply_into(current_unit_vector,
                                        current_unit_vector, current_dot_product);
            vector_subtract_into(current_column,
                                 current_column, current_unit_vector);
            vector_free(current_unit_vector);
            MATRIX_IDX_INTO(r, j, i) = current_dot_product;
        }
        norm = vector_norm(current_column);
        // TODO: Check for zero norm here, indicating the the matrix is not full rank.
        MATRIX_IDX_INTO(r, i, i) = norm;
        vector_normalize_into(current_column, current_column);
        matrix_copy_vector_into_column(q, current_column, i);
        vector_free(current_column);
    }

    qr->q = q;
    qr->r = r;
    return qr;
}
#include <stdlib.h>



void check_memory(void* mem) {
    if(mem == NULL) {
        raise_memory_allocation_error();
    }
}
#include <stdlib.h>
#include <stdio.h>


void raise_memory_allocation_error() {
    fprintf(stderr, "Memory allocation error.\n");
    exit(EXIT_FAILURE);
}

void raise_non_commensurate_vector_error() {
    fprintf(stderr, "Vectors are not of the same length.\n");
    exit(EXIT_FAILURE);
}
void raise_non_zero_reference_free_error() {
    fprintf(stderr, "Attempted to free memory with non-zero reference count.\n");
    exit(EXIT_FAILURE);
}
#include <assert.h>




/* Solve a general linear equation Mx = v using the QR decomposition of M.

   If QR is the decomposion of M, then using the fact that Q is orthogonal

     QRx = v => Rx = transpose(Q)v

   The matrix product transpose(Q)v is easy to compute, so the decomposition
   reduces the problem to solving a linear equation Rx = y for an upper
   triangular matrix R.

*/
struct vector* linsolve_qr(struct matrix* M, struct vector* v) {
    assert(M->n_row == v->length);
    struct qr_decomp* qr = matrix_qr_decomposition(M);
    struct vector* solution = linsolve_from_qr(qr, v);
    qr_decomp_free(qr);
    return solution;
}

/* Solve from the qr decomposition itself.  Useful if multiple equations
   with the same left hand side need to be solved.
*/
struct vector* linsolve_from_qr(struct qr_decomp* qr, struct vector* v) {
    struct vector* rhs = matrix_vector_multiply_Mtv(qr->q, v);
    struct vector* solution = linsolve_upper_triangular(qr->r, rhs);
    vector_free(rhs);
    return solution;
}

/* Solve a linear equation Rx = v, where R is an upper triangular matrix.

   This type of equation is easy to solve by back substitution.  We work *up*
   the rows of R solving for the components of x backwards.  For example, the
   final row in R gives the equation

     r_{l,l} x_l = v_l

   whose solution is simply x_l = v_l / r_{l,l}.  The second to final row gives
   the equation

     r_(l-1,l-1} x_{l-1} + r_{l-1,l} x_{l} = v_l

   which can be solved by substituting in the value of x_l already found, and
   then solving the resulting equation for x_{l-1}.  Continuing in this way
   solves the entire system.
*/
struct vector* linsolve_upper_triangular(struct matrix* R, struct vector* v) {
    assert(R->n_col == v->length);
    assert(R->n_row = R->n_col);
    // TODO: Check upper triangular.
    int n_eq = v->length;
    struct vector* solution = vector_new(n_eq);
    /* back_substitute:
       Tracks the part of the current equation (row) that reduces to a constant
       after substituting in the values for the already solved for varaiables.
    */
    float back_substitute;

    for(int i = n_eq - 1; i >= 0; i--) {
        back_substitute = 0;
        for(int j = i+1; j <= n_eq - 1; j++) {
            back_substitute += VECTOR_IDX_INTO(solution, j) * MATRIX_IDX_INTO(R, i, j);
        }
        VECTOR_IDX_INTO(solution, i) =
            (VECTOR_IDX_INTO(v, i) - back_substitute) / MATRIX_IDX_INTO(R, i, i);
    }
    return solution;
}
} // namespace hola
