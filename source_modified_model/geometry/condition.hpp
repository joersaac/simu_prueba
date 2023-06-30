class Condition {
    private:
        Node* node;
        float value;

    public:
        // Constructor de la clase
        Condition(Node* node_to_assign, float value_to_assign){
            node = node_to_assign;
            value = value_to_assign;
        }

        // Establece el nodo de la condición
        void set_node(Node* node_to_assign){
            node = node_to_assign;
        }

        // Obtiene el nodo de la condición
        Node* get_node(){
            return node;
        }

        // Establece el valor de la condición
        void set_value(float value_to_assign){
            value = value_to_assign;
        }

        // Obtiene el valor de la condición
        float get_value(){
            return value;
        }
};
