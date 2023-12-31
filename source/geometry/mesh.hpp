#include "node.hpp"
#include "element.hpp"
#include "condition.hpp"

enum parameter {THERMAL_CONDUCTIVITY, HEAT_SOURCE};
enum quantity {NUM_NODES, NUM_ELEMENTS, NUM_DIRICHLET, NUM_NEUMANN};

class Mesh {
    private:
        float problem_data[2];                  // Datos del problema: conductividad térmica, fuente de calor
        int quantities[4];                      // Cantidad de nodos, elementos, condiciones de Dirichlet y condiciones de Neumann
        Node** nodes;                           // Arreglo de nodos
        Element** elements;                     // Arreglo de elementos
        Condition** dirichlet_conditions;       // Arreglo de condiciones de Dirichlet
        Condition** neumann_conditions;         // Arreglo de condiciones de Neumann

    public:
        Mesh() {}

        ~Mesh() {
            free(nodes);
            free(elements);
            free(dirichlet_conditions);
            free(neumann_conditions);
        }

        void set_problem_data(float k, float Q) {
            problem_data[THERMAL_CONDUCTIVITY] = k;
            problem_data[HEAT_SOURCE] = Q;
        }

        float get_problem_data(parameter position) {
            return problem_data[position];
        }

        void set_quantities(int num_nodes, int num_elements, int num_dirichlet, int num_neumann) {
            quantities[NUM_NODES] = num_nodes;
            quantities[NUM_ELEMENTS] = num_elements;
            quantities[NUM_DIRICHLET] = num_dirichlet;
            quantities[NUM_NEUMANN] = num_neumann;
        }

        int get_quantity(quantity position) {
            return quantities[position];
        }

        void init_arrays() {
            nodes = (Node**) malloc(sizeof(Node*) * quantities[NUM_NODES]);
            elements = (Element**) malloc(sizeof(Element*) * quantities[NUM_ELEMENTS]);
            dirichlet_conditions = (Condition**) malloc(sizeof(Condition*) * quantities[NUM_DIRICHLET]);
            neumann_conditions = (Condition**) malloc(sizeof(Condition*) * quantities[NUM_NEUMANN]);
        }

        void insert_node(Node* node, int position) {
            nodes[position] = node;
        }

        Node* get_node(int position) {
            return nodes[position];
        }

        void insert_element(Element* element, int position) {
            elements[position] = element;
        }

        Element* get_element(int position) {
            return elements[position];
        }

        void insert_dirichlet_condition(Condition* dirichlet_condition, int position) {
            dirichlet_conditions[position] = dirichlet_condition;
        }

        Condition* get_dirichlet_condition(int position) {
            return dirichlet_conditions[position];
        }

        bool does_node_have_dirichlet_condition(int id) {
            bool ans = false;
            for (int i = 0; i < quantities[NUM_DIRICHLET]; i++) {
                if (dirichlet_conditions[i]->get_node()->get_ID() == id) {
                    ans = true;
                    break;
                }
            }
            return ans;
        }

        void insert_neumann_condition(Condition* neumann_condition, int position) {
            neumann_conditions[position] = neumann_condition;
        }

        Condition* get_neumann_condition(int position) {
            return neumann_conditions[position];
        }

        void print_nodes_list() {
            for (int i = 0; i < quantities[NUM_NODES]; i++)
                std::cout << 
                    "Node: " << nodes[i]->get_ID() << 
                    ", x= " << nodes[i]->get_x_coordinate() << 
                    ", y= " << nodes[i]->get_y_coordinate() << "\n\n";
        }

        void print_elements_list() {
            for (int i = 0; i < quantities[NUM_ELEMENTS]; i++) {
                std::cout << 
                    "Element: " << elements[i]->get_ID() << 
                    ", Node 1= " << elements[i]->get_node_1()->get_ID() <<
                    ", Node 2= " << elements[i]->get_node_2()->get_ID() << 
                    ", Node 3= " << elements[i]->get_node_3()->get_ID() << 
                    ", Node 4= " << elements[i]->get_node_4()->get_ID() << "\n";
            }
        }

        void print_dirichlet_conditions() {
            for (int i = 0; i < quantities[NUM_DIRICHLET]; i++) {
                std::cout << 
                    "Condition: " << i + 1 << ": " <<
                    dirichlet_conditions[i]->get_node()->get_ID() << 
                    ", Value= " << dirichlet_conditions[i]->get_value() << "\n";
            }
        }

        void print_neumann_conditions() {
            for (int i = 0; i < quantities[NUM_NEUMANN]; i++)
                std::cout << 
                    "Condition: " << i + 1 << ": " << 
                    neumann_conditions[i]->get_node()->get_ID() << 
                    ", Value: " << neumann_conditions[i]->get_value() << "\n";
        }

        void report() {
            std::cout << "\n\t ***** PROBLEM DATA *****\t\n\n";
            std::cout << "Thermal Conductivity: " << problem_data[THERMAL_CONDUCTIVITY] << "\n";
            std::cout << "Heat Source: " << problem_data[HEAT_SOURCE] << "\n\n";
            std::cout << "\n\t ***** QUANTITIES *****\t\n\n";
            std::cout << "Number of nodes: " << quantities[NUM_NODES] << "\n";
            std::cout << "Number of elements: " << quantities[NUM_ELEMENTS] << "\n";
            std::cout << "Number of dirichlet boundary conditions: " << quantities[NUM_DIRICHLET] << "\n";
            std::cout << "Number of neumann boundary conditions: " << quantities[NUM_NEUMANN] << "\n\n";
            std::cout << "\n\t ***** LIST OF NODES *****\t\n\n";
            print_nodes_list();
            std::cout << "\n\t ***** LIST OF ELEMENTS *****\t\n\n";
            print_elements_list();
            std::cout << "\n\t***** LIST OF DIRICHLET BOUNDARY CONDITIONS *****\t\n\n";
            print_dirichlet_conditions();
            std::cout << "\n\t***** LIST OF NEUMANN BOUNDARY CONDITIONS *****\t\n\n";
            print_neumann_conditions();
            std::cout << "\n\n";
        }
};