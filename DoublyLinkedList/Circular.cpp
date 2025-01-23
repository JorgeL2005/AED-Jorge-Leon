#include <iostream>

template <typename T>
class CircularDoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
    };

    Node* sentinel;
    int list_size;

public:
    CircularDoublyLinkedList() {
        sentinel = new Node();
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        list_size = 0;
    }

    ~CircularDoublyLinkedList() {
        clear();
        delete sentinel;
    }

    T front() {
        if (!empty()) {
            return sentinel->next->data;
        }
    }

    T back() {
        if (!empty()) {
            return sentinel->prev->data;
        }
    }

    void push_front(T value) {
        Node* new_node = new Node();
        new_node->data = value;
        Node* first = sentinel->next;
        new_node->next = first;
        new_node->prev = sentinel;
        sentinel->next = new_node;
        first->prev = new_node;
        list_size++;
    }

    void push_back(T value) {
        Node* new_node = new Node();
        new_node->data = value;
        Node* last = sentinel->prev;
        new_node->next = sentinel;
        new_node->prev = last;
        last->next = new_node;
        sentinel->prev = new_node;
        list_size++;
    }

    T pop_front() {
        if (!empty()) {
            Node* first = sentinel->next;
            T value = first->data;
            sentinel->next = first->next;
            first->next->prev = sentinel;
            delete first;
            list_size--;
            return value;
        }
        return T();
    }

    T pop_back() {
        if (!empty()) {
            Node* last = sentinel->prev;
            T value = last->data;
            sentinel->prev = last->prev;
            last->prev->next = sentinel;
            delete last;
            list_size--;
            return value;
        }
        return T();
    }

    void insert(T value, int position) {
        if (position < 0 || position > list_size) {
            return;
        }
        Node* new_node = new Node();
        new_node->data = value;
        Node* current = sentinel;
        for (int i = 0; i <= position; i++) {
            current = current->next;
        }
        new_node->next = current;
        new_node->prev = current->prev;
        current->prev->next = new_node;
        current->prev = new_node;
        list_size++;
    }

    void remove(int position) {
        if (position >= list_size) {
            return;
        }
        Node* current = sentinel->next;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        list_size--;
    }

    int size() const {
        return list_size;
    }

    bool empty() const {
        return list_size == 0;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    void reverse() {
        if (empty()) return;
        Node* current = sentinel;

        Node* temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
        while (current != sentinel) {
            temp = current->next;
            current->next = current->prev;
            current->prev = temp;
            current = temp;
        }
    }

    void print() const {
        if (empty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        Node* current = sentinel->next;
        while (current != sentinel) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    CircularDoublyLinkedList<int> list;

    list.push_back(6);
    list.push_back(2);
    list.push_back(9);
    list.push_back(3);


    list.print();
    list.pop_front();
    list.print();
    list.remove(0);
    list.remove(0);
    list.remove(0);
    list.print();
    list.push_back(3);
    list.print();
    list.push_back(6);
    list.push_back(2);
    list.push_front(9);
    list.push_back(1);
    list.insert(5, 2);
    list.print();

    list.reverse();
    list.print();

    return 0;
}