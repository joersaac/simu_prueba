class Element {
    private:
        int ID;
        Node* node_1;
        Node* node_2;
        Node* node_3;
        Node* node_4;

    public:
        Element (int identifier, Node* first_node, Node* second_node, Node* third_node, Node* fourth_node) {
            ID = identifier;
            node_1 = first_node;
            node_2 = second_node;
            node_3 = third_node;
            node_4 = fourth_node;
        }

        void set_ID(int identifier) {
            ID = identifier;
        }

        int get_ID() {
            return ID;
        }

        void set_node_1(Node* node) {
            node_1 = node;
        }

        Node* get_node_1() {
            return node_1;
        }

        void set_node_2(Node* node) {
            node_2 = node;
        }

        Node* get_node_2() {
            return node_2;
        }

        void set_node_3(Node* node) {
            node_3 = node;
        }

        Node* get_node_3() {
            return node_3;
        }

        void set_node_4(Node* node) {
            node_4 = node;
        }
        
        Node* get_node_4() {
            return node_4;
        }
};