#include <cmath>

#include "vector.hpp"
#include "matrix.hpp"

void product_scalar_by_matrix(float scalar, Matrix* M, int n, int m, Matrix* R){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < m; c++)
            R->set_value_on_matrix(scalar*M->get_pos_value(r,c),r,c);
}

void product_matrix_by_vector(Matrix* M, Vector* V, int n, int m, Vector* R){
    for(int r = 0; r < n; r++){
        float acc = 0;
        for(int c = 0; c < n; c++)
            acc += M->get_pos_value(r,c)*V->get_value_on_pos(c);
        R->set_value_on_pos(acc,r);
    }
}

void product_matrix_by_matrix(Matrix* A, Matrix* B, Matrix* R){
    int n = A->get_nrows(), m = A->get_ncols(), p = B->get_nrows(), q = B->get_ncols();
    if(m == p){
        R->set_matrix_size(n,q);
        R->init_matrix();
        
        for(int r = 0; r < n; r++)
            for(int c = 0; c < q; c++)
                for(int i = 0; i < m; i++)
                    R->add_value_on_pos(A->get_pos_value(r,i)*B->get_pos_value(i,c),r,c);
    }else{
        cout << "Incompatibilidad de dimensiones al multiplicar matrices.\n\nAbortando...\n";
        exit(EXIT_FAILURE);
    }
}

float determinant(Matrix* M);

float determinant_auxiliar(Matrix* M){
    int n = M->get_ncols();
    float acc = 0;

    for(int c = 0; c < n; c++){
        Matrix clon(n,n);
        M->clone_matrix(&clon);
        clon.remove_row(0);
        clon.remove_column(c);
        acc += pow(-1,c)*M->get_pos_value(0,c)*determinant(&clon);
    }

    return acc;
}

float determinant(Matrix* M){
    float ans;
    switch(M->get_ncols()){
        case 1: ans = M->get_pos_value(0,0); break;
        case 2: ans = M->get_pos_value(0,0)*M->get_pos_value(1,1) - M->get_pos_value(0,1)*M->get_pos_value(1,0); break;
        case 3: ans = M->get_pos_value(0,0)*M->get_pos_value(1,1)*M->get_pos_value(2,2)-M->get_pos_value(0,0)*M->get_pos_value(1,2)*M->get_pos_value(2,1)-M->get_pos_value(0,1)*M->get_pos_value(1,0)*M->get_pos_value(2,2)+M->get_pos_value(0,1)*M->get_pos_value(1,2)*M->get_pos_value(2,0)+M->get_pos_value(0,2)*M->get_pos_value(1,0)*M->get_pos_value(2,1)-M->get_pos_value(0,2)*M->get_pos_value(1,1)*M->get_pos_value(2,0); break;
        default: ans = determinant_auxiliar(M);
    }
    return ans;
}

float get_minor(Matrix* M, int n, int r, int c){
    Matrix clon(n,n);
    M->clone_matrix(&clon);

    //M.show(); clon.show();

    clon.remove_row(r);
    clon.remove_column(c);

    //clon.show();

    return determinant(&clon);
}

void conjugate_matrix(Matrix* M, int n, Matrix* C){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < n; c++)
            C->set_value_on_matrix(pow(-1,r+c)*get_minor(M,n,r,c),r,c);
}

void transpose(Matrix* M, int n, int m, Matrix* T){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < m; c++)
            T->set_value_on_matrix(M->get_pos_value(r,c),c,r);
}

void calculate_inverse(Matrix* M, int n, Matrix* R){
    cout << "\t\tCalculating determinant...\n\n";
    float detM = determinant(M);
    //cout << "n: " << M.get_ncols() << "\n";
    //cout << "detM: " << detM << "\n";
    
    cout << "\t\tCalculating Conjugate Matrix...\n\n";
    Matrix Conj(n,n);
    conjugate_matrix(M,n,&Conj);
    //Conj.show();
    
    cout << "\t\tCalculating Adjunct Matrix...\n\n";
    Matrix Adj(n,n);
    transpose(&Conj, n, n, &Adj);
    //Adj.show();

    cout << "\t\tMultiplying the Adjunct by the determinant...\n\n";
    product_scalar_by_matrix(1/detM, &Adj, n, n, R);
    //R->show();
}