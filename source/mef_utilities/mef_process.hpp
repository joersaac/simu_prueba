float calculate_local_volume(
    float x1, float y1, float z1, 
    float x2, float y2, float z2, 
    float x3, float y3, float z3, 
    float x4, float y4, float z4
) {
    return abs(
        ((x2 - x1) * (y3 - y1) * (z4 - z1) + (y2 - y1) * (z3 - z1) * (x4 - x1) + (z2 - z1) * (x3 - x1) * (y4 - y1)) - 
        ((z2 - z1) * (y3 - y1) * (x4 - x1) + (y2 - y1) * (x3 - x1) * (z4 - z1) + (x2 - x1) * (z3 - z1) * (y4 - y1))) 
        / 6;
}

float calculate_local_jacobian(
    float x1, float y1, float z1, 
    float x2, float y2, float z2, 
    float x3, float y3, float z3, 
    float x4, float y4, float z4) {
    return 
        (x2 - x1) * (y3 - y1) * (z4 - z1) - 
        (x2 - x1) * (y4 - y1) * (z3 - z1) - 
        (x3 - x1) * (y2 - y1) * (z4 - z1) + 
        (x3 - x1) * (y4 - y1) * (z2 - z1) + 
        (x4 - x1) * (y2 - y1) * (z3 - z1) - 
        (x4 - x1) * (y3 - y1) * (z2 - z1);
}

void calculate_B(Matrix* B) {
    B->set_value_on_matrix(-1, 0, 0);
    B->set_value_on_matrix(1, 0, 1);
    B->set_value_on_matrix(0, 0, 2);
    B->set_value_on_matrix(0, 0, 3);
    B->set_value_on_matrix(-1, 1, 0);
    B->set_value_on_matrix(0, 1, 1);
    B->set_value_on_matrix(1, 1, 2);
    B->set_value_on_matrix(0, 1, 3);
    B->set_value_on_matrix(-1, 2, 0);
    B->set_value_on_matrix(0, 2, 1);
    B->set_value_on_matrix(0, 2, 2);
    B->set_value_on_matrix(1, 2, 3);
}

void calculate_local_A (
    Matrix* A, 
    float x1, float y1, float z1, 
    float x2, float y2, float z2, 
    float x3, float y3, float z3,
    float x4, float y4, float z4)
    {
        A->set_value_on_matrix( (y3 - y1)*(z4 - z1) - (y4 - y1)*(z3 - z1), 0, 0);  
        A->set_value_on_matrix(-(x3 - x1)*(z4 - z1) + (x4 - x1)*(z3 - z1), 0, 1);  
        A->set_value_on_matrix( (x3 - x1)*(y4 - y1) - (x4 - x1)*(y3 - y1), 0, 2);
        A->set_value_on_matrix(-(y2 - y1)*(z4 - z1) + (y4 - y1)*(z2 - z1), 1, 0);  
        A->set_value_on_matrix( (x2 - x1)*(z4 - z1) - (x4 - x1)*(z2 - z1), 1, 1);  
        A->set_value_on_matrix(-(x2 - x1)*(y4 - y1) + (x4 - x1)*(y2 - y1), 1, 2);
        A->set_value_on_matrix( (y2 - y1)*(z3 - z1) - (y3 - y1)*(z2 - z1), 2, 0);  
        A->set_value_on_matrix(-(x2 - x1)*(z3 - z1) + (x3 - x1)*(z2 - z1), 2, 1);  
        A->set_value_on_matrix( (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1), 2, 2);
}

void create_local_K(Matrix* K, int element_id, Mesh* M) {
    K->set_matrix_size(4, 4);

    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);

    float 
        x1 = M->get_element(element_id)->get_node_1()->get_x_coordinate(), 
        y1 = M->get_element(element_id)->get_node_1()->get_y_coordinate(), 
        z1 = M->get_element(element_id)->get_node_1()->get_z_coordinate(),
        
        x2 = M->get_element(element_id)->get_node_2()->get_x_coordinate(), 
        y2 = M->get_element(element_id)->get_node_2()->get_y_coordinate(), 
        z2 = M->get_element(element_id)->get_node_2()->get_z_coordinate(),
        
        x3 = M->get_element(element_id)->get_node_3()->get_x_coordinate(), 
        y3 = M->get_element(element_id)->get_node_3()->get_y_coordinate(), 
        z3 = M->get_element(element_id)->get_node_3()->get_z_coordinate(),
        
        x4 = M->get_element(element_id)->get_node_4()->get_x_coordinate(), 
        y4 = M->get_element(element_id)->get_node_4()->get_y_coordinate(), 
        z4 = M->get_element(element_id)->get_node_4()->get_z_coordinate();

    float J = calculate_local_jacobian(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
    float Volume = calculate_local_volume(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);

    Matrix B(3, 4), A(3, 3);

    calculate_B(&B);
    calculate_local_A(&A, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);

    // B.show_matrix(); 
    // A.show_matrix();

    Matrix Bt(4, 3), At(3, 3);

    transpose(&B, 3, 4, &Bt);
    transpose(&A, 3, 3, &At);

    // Bt.show_matrix(); 
    // At.show_matrix();

    Matrix res1, res2, res3;

    product_matrix_by_matrix(&A, &B, &res1);
    product_matrix_by_matrix(&At, &res1, &res2);
    product_matrix_by_matrix(&Bt, &res2, &res3);
    product_scalar_by_matrix(k * Volume/(J * J), &res3, 4, 4, K);

    // std::cout << "\t\tLocal matrix created for Element " << element_id + 1 << ": "; K->show_matrix(); std::cout << "\n";
}

void create_local_b(Vector* b, int element_id, Mesh* M){
    b->set_vector_size(4);

    float Q = M->get_problem_data(HEAT_SOURCE);

    float 
        x1 = M->get_element(element_id)->get_node_1()->get_x_coordinate(), 
        y1 = M->get_element(element_id)->get_node_1()->get_y_coordinate(),
        z1 = M->get_element(element_id)->get_node_1()->get_z_coordinate(),

        x2 = M->get_element(element_id)->get_node_2()->get_x_coordinate(), 
        y2 = M->get_element(element_id)->get_node_2()->get_y_coordinate(), 
        z2 = M->get_element(element_id)->get_node_2()->get_z_coordinate(),

        x3 = M->get_element(element_id)->get_node_3()->get_x_coordinate(), 
        y3 = M->get_element(element_id)->get_node_3()->get_y_coordinate(), 
        z3 = M->get_element(element_id)->get_node_3()->get_z_coordinate(),

        x4 = M->get_element(element_id)->get_node_4()->get_x_coordinate(), 
        y4 = M->get_element(element_id)->get_node_4()->get_y_coordinate(), 
        z4 = M->get_element(element_id)->get_node_4()->get_z_coordinate();

    float J = calculate_local_jacobian(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);

    b->set_value_on_pos(Q * J/24, 0);
    b->set_value_on_pos(Q * J/24, 1);
    b->set_value_on_pos(Q * J/24, 2);
    b->set_value_on_pos(Q * J/24, 3);

    // std::cout << "\t\tLocal vector created for Element " << element_id + 1 << ": "; b->show_vector(); std::cout << "\n";
}

void create_local_systems(Matrix* Ks, Vector* bs, int num_elements, Mesh* M) {
    for(int element = 0; element < num_elements; element++){
        std::cout << "\tCreating local system for Element " << element + 1 << "...\n\n";
        create_local_K(&Ks[element], element, M);
        create_local_b(&bs[element], element, M);
    }
}

void assembly_K(Matrix* K, Matrix* local_K, int index1, int index2, int index3, int index4) {
    K->add_value_on_pos(local_K->get_pos_value(0, 0),index1,index1);  
    K->add_value_on_pos(local_K->get_pos_value(0, 1),index1,index2);  
    K->add_value_on_pos(local_K->get_pos_value(0, 2),index1,index3);  
    K->add_value_on_pos(local_K->get_pos_value(0, 3),index1,index4); 

    K->add_value_on_pos(local_K->get_pos_value(1, 0),index2,index1);  
    K->add_value_on_pos(local_K->get_pos_value(1, 1),index2,index2);  
    K->add_value_on_pos(local_K->get_pos_value(1, 2),index2,index3);  
    K->add_value_on_pos(local_K->get_pos_value(1, 3),index2,index4);

    K->add_value_on_pos(local_K->get_pos_value(2, 0),index3,index1);  
    K->add_value_on_pos(local_K->get_pos_value(2, 1),index3,index2);  
    K->add_value_on_pos(local_K->get_pos_value(2, 2),index3,index3);  
    K->add_value_on_pos(local_K->get_pos_value(2, 3),index3,index4);

    K->add_value_on_pos(local_K->get_pos_value(3, 0),index4,index1);  
    K->add_value_on_pos(local_K->get_pos_value(3, 1),index4,index2);  
    K->add_value_on_pos(local_K->get_pos_value(3, 2),index4,index3);  
    K->add_value_on_pos(local_K->get_pos_value(3, 3),index4,index4);
}

void assembly_b(Vector* b, Vector* local_b, int index1, int index2, int index3, int index4){
    b->sum_value_on_pos(local_b->get_value_on_pos(0), index1);
    b->sum_value_on_pos(local_b->get_value_on_pos(1), index2);
    b->sum_value_on_pos(local_b->get_value_on_pos(2), index3);
    b->sum_value_on_pos(local_b->get_value_on_pos(3), index4);
}

void assembly(Matrix* K, Vector* b, Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    K->init_matrix();
    b->init_vector();

    // K->show_matrix(); 
    // b->show_vector();

    for(int element = 0; element < num_elements; element++){
        std::cout << "\tAssembling for Element " << element + 1 << "...\n\n";

        int index1 = M->get_element(element)->get_node_1()->get_ID() - 1;
        int index2 = M->get_element(element)->get_node_2()->get_ID() - 1;
        int index3 = M->get_element(element)->get_node_3()->get_ID() - 1;
        int index4 = M->get_element(element)->get_node_4()->get_ID() - 1;

        assembly_K(K, &Ks[element], index1, index2, index3, index4);
        assembly_b(b, &bs[element], index1, index2, index3, index4);

        // std::cout << "\t\t"; K->show_matrix(); std::cout << "\t\t"; b->show_vector(); std::cout << "\n";
    }
}

void apply_neumann_boundary_conditions(Vector* b, Mesh* M) {
    int num_conditions = M->get_quantity(NUM_NEUMANN);

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_neumann_condition(c);
        
        int index = cond->get_node()->get_ID() - 1;
        b->sum_value_on_pos(cond->get_value(), index);
    }

    // std::cout << "\t\t"; b->show_vector(); std::cout << "\n";
}

void add_column_to_RHS(Matrix* K, Vector* b, int col, float T_bar) {
    for(int rows = 0; rows < K->get_num_rows(); rows++)
        b->sum_value_on_pos(-T_bar*K->get_pos_value(rows, col), rows);
}

void apply_dirichlet_boundary_conditions(Matrix* K, Vector* b, Mesh* M){
    int num_conditions = M->get_quantity(NUM_DIRICHLET);
    int previous_removed = 0;

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_dirichlet_condition(c);
        
        int index = cond->get_node()->get_ID() - 1 - previous_removed;
        float cond_value = cond->get_value();

        // K->show_matrix();

        K->remove_row(index);

        // K->show_matrix();
        // b->show_vector();

        b->remove_row(index);

        // b->show_vector();

        add_column_to_RHS(K, b, index, cond_value);

        // b->show_vector();

        K->remove_column(index);

        // K->show_matrix();

        previous_removed++;
    }
}

void solve_system(Matrix* K, Vector* b, Vector* T) {
    int num = K->get_num_rows();
    
    Matrix Kinv(num, num);

    std::cout << "\tCalculating inverse of global matrix K...\n\n";
    calculate_inverse(K, num, &Kinv);

    std::cout << "\tPerforming final calculation...\n\n";
    product_matrix_by_vector(&Kinv, b, num, num, T);
}

void merge_results_with_dirichlet(Vector* T, Vector* Tf, int n, Mesh* M) {
    int num_dirichlet = M->get_quantity(NUM_DIRICHLET);

    int cont_dirichlet = 0;
    int cont_T = 0;

    for (int i = 0; i < n; i++) {
        if (M->does_node_have_dirichlet_condition(i + 1)) {
            Condition* cond = M->get_dirichlet_condition(cont_dirichlet);
            cont_dirichlet++;
        
            float cond_value = cond->get_value();

            Tf->set_value_on_pos(cond_value, i);
        } else {
            Tf->set_value_on_pos(T->get_value_on_pos(cont_T), i);
            cont_T++;
        }
    }
}
