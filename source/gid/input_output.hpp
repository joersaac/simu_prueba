#include <fstream>

// Función para leer el archivo de entrada y cargar los datos en una estructura de malla (Mesh)
void read_input(std::string filename, Mesh* M) {
    std::string line;

    float k, Q, T_bar, T_hat;
    int num_nodes, num_elements, num_dirichlet, num_neumann;

    std::ifstream dat_file(filename + ".dat");

    // Lectura de los datos del problema desde el archivo
    dat_file >> k >> Q >> T_bar >> T_hat >> num_nodes >> num_elements >> num_dirichlet >> num_neumann;

    // Configuración de los datos del problema en la estructura de malla (Mesh)
    M->set_problem_data(k, Q);
    M->set_quantities(num_nodes, num_elements, num_dirichlet, num_neumann);

    // Inicialización de las estructuras de datos en la malla (Mesh)
    M->init_arrays();

    dat_file >> line;

    // Lectura de los nodos desde el archivo y su inserción en la malla (Mesh)
    for (int i = 0; i < num_nodes; i++) {
        int id;
        float coord_x, coord_y, coord_z;
        dat_file >> id >> coord_x >> coord_y >> coord_z;
        M->insert_node(new Node(id, coord_x, coord_y, coord_z), i);
    }

    dat_file >> line >> line;

    // Lectura de los elementos desde el archivo y su inserción en la malla (Mesh)
    for (int i = 0; i < num_elements; i++) {
        int id, node_1_id, node_2_id, node_3_id, node_4_id;
        dat_file >> id >> node_1_id >> node_2_id >> node_3_id >> node_4_id;
        M->insert_element(
            new Element(
                id, M->get_node(node_1_id - 1), 
                M->get_node(node_2_id - 1), 
                M->get_node(node_3_id - 1), 
                M->get_node(node_4_id - 1)), 
                i
            );
    }

    dat_file >> line >> line;

    // Lectura de las condiciones de Dirichlet desde el archivo y su inserción en la malla (Mesh)
    for (int i = 0; i < num_dirichlet; i++) {
        int id;
        dat_file >> id;
        M->insert_dirichlet_condition(
            new Condition(
                M->get_node(id - 1), T_bar
            ), i);
    }

    dat_file >> line >> line;

    // Lectura de las condiciones de Neumann desde el archivo y su inserción en la malla (Mesh)
    for (int i = 0; i < num_neumann; i++) {
        int id;
        dat_file >> id;
        M->insert_neumann_condition(
            new Condition(
                M->get_node(id - 1), T_hat
            ), 1);
    }

    dat_file.close();

    //M->report();
}

// Función para escribir los resultados en un archivo de salida
void write_output(std::string filename, Vector* T) {
    std::ofstream res_file(filename + ".post.res");

    // Escritura de la cabecera del archivo de resultados
    res_file << "GiD Post Results File 1.0\n";

    int n = T->get_size();

    // Escritura de los resultados de temperatura en el archivo de resultados
    res_file << "Result \"Temperature\" \"Load Case 1\" " << 1 << " Scalar OnNodes\n";
    res_file << "ComponentNames \"T\"\n";
    res_file << "Values\n";

    for (int i = 0; i < n; i++)
        res_file << i + 1 << "     " << T->get(i) << "\n";

    res_file << "End values\n";

    res_file.close();
}
