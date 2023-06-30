#include <stdio.h>

class Matrix {
private:
    int num_rows, num_cols;
    float** data;

    // Función auxiliar para crear la matriz en memoria dinámica
    void create() {
        data = (float**)malloc(sizeof(float*) * num_rows);
        for (int row = 0; row < num_rows; row++) {
            data[row] = (float*)malloc(sizeof(float) * num_cols);
        }
    }

public:
    // Constructor por defecto
    Matrix() {}

    // Constructor que acepta el número de filas y columnas
    Matrix(int rows, int cols) {
        num_rows = rows;
        num_cols = cols;
        create();
    }

    // Destructor de la clase
    ~Matrix() {
        for (int rows = 0; rows < num_rows; rows++) {
            free(data[rows]);
        }

        free(data);
    }

    // Inicializa todos los elementos de la matriz a cero
    void init_matrix() {
        for (int rows = 0; rows < num_rows; rows++) {
            for (int cols = 0; cols < num_cols; cols++) {
                data[rows][cols] = 0;
            }
        }
    }

    // Establece el tamaño de la matriz
    void set_matrix_size(int rows, int cols) {
        num_rows = rows;
        num_cols = cols;
        create();
    }

    // Devuelve el número de filas de la matriz
    int get_num_rows() {
        return num_rows;
    }

    // Devuelve el número de columnas de la matriz
    int get_num_cols() {
        return num_cols;
    }

    // Establece el valor en la posición (row, col) de la matriz
    void set_value_on_matrix(float value, int row, int col) {
        data[row][col] = value;
    }

    // Añade el valor a la posición (row, col) de la matriz
    void add_value_on_pos(float value, int row, int col) {
        data[row][col] += value;
    }

    // Devuelve el valor en la posición (row, col) de la matriz
    float get_pos_value(int row, int col) {
        return data[row][col];
    }

    // Elimina una fila de la matriz
    void remove_row(int row) {
        int temp_index = 0;
        float** temp_data = (float**)malloc(sizeof(float*) * (num_rows - 1));

        for (int i = 0; i < num_rows; i++) {
            if (i != row) {
                temp_data[temp_index] = data[i];
                temp_index++;
            }
        }

        free(data[row]);
        free(data);

        data = temp_data;
        num_rows--;
    }

    // Elimina una columna de la matriz
    void remove_column(int col) {
        int temp_index = 0;
        float** temp_data = (float**)malloc(sizeof(float*) * num_rows);

        for (int rows = 0; rows < num_rows; rows++) {
            temp_data[rows] = (float*)malloc(sizeof(float) * (num_cols - 1));
        }

        for (int rows = 0; rows < num_rows; rows++) {
            for (int cols = 0; cols < num_cols; cols++) {
                if (cols != col) {
                    temp_data[rows][temp_index] = data[rows][cols];
                    temp_index++;
                }
            }
            temp_index = 0;
        }

        for (int rows = 0; rows < num_rows; rows++) {
            free(data[rows]);
        }

        free(data);
        data = temp_data;
        num_cols--;
    }

    // Clona los valores de la matriz a otra matriz proporcionada
    void clone_matrix(Matrix* other) {
        for (int rows = 0; rows < num_rows; rows++) {
            for (int cols = 0; cols < num_cols; cols++) {
                other->set_value_on_matrix(data[rows][cols], rows, cols);
            }
        }
    }

    // Muestra los elementos de la matriz en la consola
    void show_matrix() {
        std::cout << "[ ";
            for (int rows = 0; rows < num_rows; rows++) {
                std::cout << "[ " << data[rows][0];
                for (int cols = 1; cols < num_cols; cols++) {
                    std::cout << ", " << data[rows][cols];
                }
                std::cout << " ] ";
            }
        std::cout << " ]\n\n";
    }
};
