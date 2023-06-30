#include <stdio.h>

class Matrix {
    private:
        int nrows, ncols;
        float** data;

        void create(){
            data = (float**) malloc(sizeof(float*) * nrows);
            for(int r = 0; r < nrows; r++)
                data[r] = (float*) malloc(sizeof(float) * ncols);
        }

    public:
        Matrix(){}
        Matrix(int rows, int cols){
            nrows = rows;
            ncols = cols;
            create();
        }
        ~Matrix(){
            for(int r = 0; r < nrows; r++)
                free(data[r]);
            free(data);
        }

        void init_matrix(){
            for(int r = 0; r < nrows; r++)
                for(int c = 0; c < ncols; c++)
                    data[r][c] = 0;
        }

        void set_matrix_size(int rows, int cols){
            nrows = rows;
            ncols = cols;
            create();
        }
        int get_nrows(){
            return nrows;
        }
        int get_ncols(){
            return ncols;
        }

        void set_value_on_matrix(float value, int row, int col){
            data[row][col] = value;
        }
        void add_value_on_pos(float value, int row, int col){
            data[row][col] += value;
        }
        float get_pos_value(int row, int col){
            return data[row][col];
        }

        void remove_row(int row){
            int neo_index = 0;
            float** neo_data = (float**) malloc(sizeof(float*) * (nrows-1));
            for(int i = 0; i < nrows; i++)
                if(i != row){
                    neo_data[neo_index] = data[i];
                    neo_index++;
                }
            //printf("remove_row: %p - %p\n",data[row],data);
            free(data[row]);
            free(data);
            data = neo_data;
            nrows--;
        }

        void remove_column(int col){
            int neo_index = 0;
            float** neo_data = (float**) malloc(sizeof(float*) * nrows);
            for(int r = 0; r < nrows; r++)
                neo_data[r] = (float*) malloc(sizeof(float) * (ncols-1));

            for(int r = 0; r < nrows; r++){
                for(int c = 0; c < ncols; c++)
                    if(c != col){
                        neo_data[r][neo_index] = data[r][c];
                        neo_index++;
                    }
                neo_index = 0;
            }
            //printf("remove_column: %p - %p\n",data[0],data);
            for(int r = 0; r < nrows; r++)
                free(data[r]);
            free(data);
            data = neo_data;
            ncols--;
        }

        void clone_matrix(Matrix* other){
            for(int r = 0; r < nrows; r++)
                for(int c = 0; c < ncols; c++)
                    other->set_value_on_matrix(data[r][c],r,c);
        }

        void show_matrix(){
            cout << "[ ";
            for(int r = 0; r < nrows; r++){
                cout << "[ " << data[r][0];
                for(int c = 1; c < ncols; c++){
                    cout << ", " << data[r][c];
                }
                cout << " ] ";
            }
            cout << " ]\n\n";
        }
};