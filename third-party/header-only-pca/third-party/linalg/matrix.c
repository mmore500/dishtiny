#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include "matrix.h"
#include "vector.h"
#include "errors.h"
#include "util.h"
#include "linalg_obj.h"


struct matrix* matrix_new(int n_row, int n_col) {
    assert(n_row >= 1 && n_col >= 1);
    struct matrix* new_matrix = static_cast<struct matrix*>(malloc(sizeof(struct matrix)));;
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
