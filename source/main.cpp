#include <iostream>
#include "geometry/mesh.hpp"
#include "math_utilities/matrix_operations.hpp"
#include "mef_utilities/mef_process.hpp"
#include "gid/input_output.hpp"

int main(int argc, char** argv) {
    // Verificar el número de argumentos de línea de comandos
    if (argc != 2) {
        std::cout << "Incorrect use of the program, it must be: mef filename\n";
        exit(EXIT_FAILURE);
    }

    Mesh M;

    std::cout << "Reading geometry and mesh data...\n\n";

    std::string filename(argv[1]);
    read_input(filename, &M);

    // Print report
    M.report();

    int num_nodes = M.get_quantity(NUM_NODES);
    int num_elements = M.get_quantity(NUM_ELEMENTS);

    Matrix K(num_nodes, num_nodes), local_Ks[num_elements];
    Vector b(num_nodes), local_bs[num_elements];

    // M.report();

    std::cout << "Creating local systems...\n\n";
    create_local_systems(local_Ks, local_bs, num_elements, &M);

    std::cout << "Performing Assembly...\n\n";
    assembly(&K, &b, local_Ks, local_bs, num_elements, &M);

    // K.show_matrix();
    // b.show_vector();

    std::cout << "Applying Neumann Boundary Conditions...\n\n";
    apply_neumann_boundary_conditions(&b, &M);

    // b.show_vector();

    std::cout << "Applying Dirichlet Boundary Conditions...\n\n";
    apply_dirichlet_boundary_conditions(&K, &b, &M);

    // K.show_matrix();
    // b.show_vector();

    std::cout << "Solving global system...\n\n";

    Vector T(b.get_vector_size()), T_full(num_nodes);
    solve_system(&K, &b, &T);

    // T.show_vector();

    std::cout << "Preparing results...\n\n";
    merge_results_with_dirichlet(&T, &T_full, num_nodes, &M);

    // T_full.show_vector();

    std::cout << "Writing output file...\n\n";
    write_output(filename, &T_full);

    return 0;
}
