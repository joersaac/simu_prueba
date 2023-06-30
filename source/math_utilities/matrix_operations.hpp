#include <cmath>

#include "vector.hpp"
#include "matrix.hpp"

void product_scalar_by_matrix(float scalar, Matrix* M, int n, int m, Matrix* R) {
    for(int rows = 0; rows < n; rows++)
        for(int cols = 0; cols < m; cols++)
            R->set_value_on_matrix(scalar*M->get_pos_value(rows,cols), rows, cols);
}

void product_matrix_by_vector(Matrix* M, Vector* V, int n, int m, Vector* R) {
    for(int rows = 0; rows < n; rows++) {
        float acc = 0;

        for(int cols = 0; cols < n; cols++)
            acc += M->get_pos_value(rows, cols)*V->get_value_on_pos(cols);
        
        R->set_value_on_pos(acc, rows);
    }
}

void product_matrix_by_matrix(Matrix* A, Matrix* B, Matrix* R) {
    int n = A->get_num_rows(), m = A->get_num_cols(), p = B->get_num_rows(), q = B->get_num_cols();

    if (m == p) {
        R->set_matrix_size(n, q);
        R->init_matrix();
        
        for(int rows = 0; rows < n; rows++)
            for(int cols = 0; cols < q; cols++)
                for(int i = 0; i < m; i++)
                    R->add_value_on_pos(
                        A->get_pos_value(rows, i)*B->get_pos_value(i, cols), rows, cols
                    );
    } else {
        std::cout << "Incompatibilidad de dimensiones al multiplicar matrices.\n\nAbortando...\n";
        exit(EXIT_FAILURE);
    }
}

float determinant(Matrix* M);

float determinant_auxiliar(Matrix* M) {
    int n = M->get_num_cols();
    float acc = 0;

    for(int cols = 0; cols < n; cols++) {
        Matrix clon(n, n);

        M->clone_matrix(&clon);

        clon.remove_row(0);
        clon.remove_column(cols);

        acc += pow(-1, cols)*M->get_pos_value(0, cols)*determinant(&clon);
    }

    return acc;
}

float determinant(Matrix* M) {
    float ans;

    switch(M->get_num_cols()) {
        case 1: ans = M->get_pos_value(0, 0); break;
        case 2: ans = 
            M->get_pos_value(0, 0) * 
            M->get_pos_value(1, 1) - 
            M->get_pos_value(0, 1) * 
            M->get_pos_value(1, 0); 
            break;
        case 3: ans = 
            M->get_pos_value(0,0) * 
            M->get_pos_value(1,1) * 
            M->get_pos_value(2,2) - 
            M->get_pos_value(0,0) * 
            M->get_pos_value(1,2) * 
            M->get_pos_value(2,1) - 
            M->get_pos_value(0,1) *
            M->get_pos_value(1,0) *
            M->get_pos_value(2,2) +
            M->get_pos_value(0,1) *
            M->get_pos_value(1,2) *
            M->get_pos_value(2,0) +
            M->get_pos_value(0,2) *
            M->get_pos_value(1,0) * 
            M->get_pos_value(2,1) -
            M->get_pos_value(0,2) *
            M->get_pos_value(1,1) *
            M->get_pos_value(2,0);
            break;
        default: ans = determinant_auxiliar(M);
    }

    return ans;
}

void transpose(Matrix* M, int n, int m, Matrix* T) {
    for(int rows = 0; rows < n; rows++)
        for(int cols = 0; cols < m; cols++)
            T->set_value_on_matrix(M->get_pos_value(rows, cols), cols, rows);
}

void calculate_L_matrix(Matrix* A, int n, Matrix* L) {
    float acum;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (i == j) {
                acum = 0;
                for(int k = 0; k < j; k++) {
                    // std::cout << L->get_pos_value(j, k) << " ";
                    acum += pow(L->get_pos_value(k, j), 2);
                }

                // std::cout << A->get_pos_value(j, j) << " " << acum << "\n";

                if (A->get_pos_value(j, j) - acum >= 0)
                    L->set_value_on_matrix(sqrt(A->get_pos_value(j, j) - acum), j, j);
                else
                    L->set_value_on_matrix(0.000001, j, j);
            }
            else {
                if (i > j) {
                    if (L->get_pos_value(j, j) != 0) {
                        acum = 0;

                        for(int k = 0; k < j; k++) {
                            acum += L->get_pos_value(i, k)*L->get_pos_value(j, k);
                        }

                        // std::cout << L->get_pos_value(j,j) << "\n";

                        L->set_value_on_matrix(
                            (1/L->get_pos_value(j, j)) * (A->get_pos_value(i, j) - acum),
                        i, j);
                    }
                    else {
                        L->set_value_on_matrix(0.000001, i, j);
                    }
                } 
                else {
                    L->set_value_on_matrix(0, i, j);
                }
            }
        }
    }
}

void calculate_Y_matrix(Matrix* L, int n, Matrix* Y) {
    float acum;

    for(int i= 0; i < n; i++) {
        for(int j= 0; j < n; j++) {
            if (i == j) {
                if (L->get_pos_value(j, j) != 0)
                    Y->set_value_on_matrix(1/L->get_pos_value(i, i),i, i);
                else
                    L->set_value_on_matrix(0.000001, i, j);
            }
            else {
                if (i > j) {
                    if (L->get_pos_value(i, i) != 0) {
                        acum = 0;

                        for(int k = j; k < i; k++) {
                            acum += L->get_pos_value(i, k)*Y->get_pos_value(k, j);
                        }

                        Y->set_value_on_matrix(-(1/L->get_pos_value(i, i)) * acum,i, j);
                    }
                    else
                        L->set_value_on_matrix(0.000001, i, j);
                }
                else {
                    Y->set_value_on_matrix(0, i ,j);
                }
            }
        }
    }
}

void calculate_X_matrix(Matrix* Y,Matrix* L, int n, Matrix* X) {
    float acum;

    for(int i= n-1; i >= 0; i--) {
        for(int j= 0; j < n; j++) {
                if ( L->get_pos_value(i, i) != 0) {
                acum = 0;

                for(int k = i+1; k < n; k++) {
                    acum += L->get_pos_value(k, i)*X->get_pos_value(k, j);
                }

                X->set_value_on_matrix((1/L->get_pos_value(i, i))*( Y->get_pos_value(i, j) - acum ), i, j);
            }
            else
                L->set_value_on_matrix(0.000001, i, j);
        }
    }
}

void calculate_inverse(Matrix* A, int n, Matrix* X) {
    Matrix L(n, n), Y(n, n);

    L.init_matrix();
    Y.init_matrix();

    
    calculate_L_matrix(A, n, &L);

    calculate_Y_matrix(&L, n, &Y);

    calculate_X_matrix(&Y, &L, n, X);

}
