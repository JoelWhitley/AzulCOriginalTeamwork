class Node {
    public:
        Node(int value, Node * next);
        int value;
        Node* next;
};

class LinkedList {
    public:
    LinkedList();
    LinkedList(const LinkedList& other);
    ~LinkedList();

    int size();
    
    int get(int index);
    bool get(int index, int& returnValue);

    void addBack(int value);
    void addFront(int value);

    void removeBack();
    void removeFront();

    void clear();

    private:
        Node* head;

};