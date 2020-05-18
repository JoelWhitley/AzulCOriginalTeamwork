class Node {
    public:
        Node(char value, Node * next);
        char value;
        Node* next;
};

class LinkedList {
    public:
        LinkedList();
        LinkedList(const LinkedList& other);
        ~LinkedList();

        int size();
        
        char get(int index);
        bool get(int index, char& returnValue);

        void addBack(char value);
        void addFront(char value);

        void removeBack();
        void removeFront();

        void clear();
        void removeNodeAtIndex(int index);

        bool contains(char input);

    private:
        Node* head;

};