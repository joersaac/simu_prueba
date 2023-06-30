class Vector {
    private:
        int size;     // Tamaño del vector
        float* data;  // Puntero al arreglo de datos

        // Crea el arreglo de datos
        void create() {
            data = (float*) malloc(sizeof(float) * size);
        }

    public:
        // Constructores
        Vector() {}
        Vector(int data_qty) {
            size = data_qty;
            create();
        }

        // Destructor
        ~Vector() {
            free(data);
        }

        // Inicializa todos los elementos del vector con cero
        void init_vector() {
            for (int i = 0; i < size; i++)
                data[i] = 0;
        }

        // Establece el tamaño del vector y crea el arreglo de datos correspondiente
        void set_vector_size(int num_values) {
            size = num_values;
            create();
        }

        // Obtiene el tamaño del vector
        int get_vector_size() {
            return size;
        }

        // Establece el valor de un elemento en una posición dada
        void set_value_on_pos(float value, int position) {
            data[position] = value;
        }

        // Suma un valor al elemento en una posición dada
        void sum_value_on_pos(float value, int position) {
            data[position] += value;
        }

        // Obtiene el valor del elemento en una posición dada
        float get_value_on_pos(int position) {
            return data[position];
        }

        // Elimina una fila del vector
        void remove_row(int row) {
            int temp_index = 0;
            float* temp_data = (float*) malloc(sizeof(float) * (size-1));
            for (int i = 0; i < size; i++)
                if (i != row) {
                    temp_data[temp_index] = data[i];
                    temp_index++;
                }
            free(data);
            data = temp_data;
            size--;
        }

        // Muestra los elementos del vector
        void show_vector() {
            std::cout << "[ " << data[0];
            for (int i = 1; i < size; i++)
                std::cout << "; " << data[i];
            std::cout << " ]\n\n";
        }
};
