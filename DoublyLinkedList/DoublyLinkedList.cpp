#include <iostream>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

    Node<T>* merge(Node<T>* first, Node<T>* second) {
        if (!first)
            return second;
        if (!second)
            return first;
        if (first->data < second->data) {
            first->next = merge(first->next, second);
            if (first->next)
                first->next->prev = first;
            return first;
        } else {
            second->next = merge(first, second->next);
            if (second->next)
                second->next->prev = second;
            return second;
        }
    }

    Node<T>* mergeSort(Node<T>* node) {
        if (!node || !node->next)
            return node;
        Node<T>* secondHalf = split(node);
        return merge(mergeSort(node), mergeSort(secondHalf));
    }

    Node<T>* split(Node<T>* head) {
        Node<T>* fast = head->next;
        Node<T>* slow = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node<T>* temp = slow->next;
        slow->next = nullptr;
        if (temp)
            temp->prev = nullptr;
        return temp;
    }

public:
    DoublyLinkedList(){
        head = nullptr;
        tail = nullptr;
    }
    ~DoublyLinkedList() {
        clear();
    }
    T front(){
        if(head != nullptr){
            return head->data;
        }
        return T();
    }
    T back(){
        if(tail != nullptr){
            return tail->data;
        }
        return T();
    }
    void push_front(T data) {
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = head;
        newNode->prev = nullptr;
        if (head != nullptr) {
            head->prev = newNode;
        }
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
    }

    void push_back(T data) {
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = nullptr;
        newNode->prev = tail;
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = tail;
        }
    }

    T pop_front() {
        if (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            T data = temp->data;
            delete temp;
            return data;
        }
        return T();
    }

    T pop_back() {
        if (tail != nullptr) {
            Node<T>* temp = tail;
            tail = tail->prev;
            if (tail) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            T data = temp->data;
            delete temp;
            return data;
        }
        return T();
    }

    T operator[](int index) {
        Node<T>* temp = head;
        for (int i = 0; i < index && temp != nullptr; i++) {
            temp = temp->next;
        }
        if (temp != nullptr) {
            return temp->data;
        }
        return T();
    }

    void insert(T data, int index) {
        if (index == 0) {
            push_front(data);
        } else {
            Node<T>* temp = head;
            for(int i = 0; i < index - 1; i++){
                if(temp->next == nullptr){
                    return;
                }
                temp = temp->next;
            }
            Node<T>* newNode = new Node<T>();
            newNode->data = data;
            newNode->next = temp->next;
            newNode->prev = temp;
            if (temp->next != nullptr) {
                temp->next->prev = newNode;
            }
            temp->next = newNode;
            if (tail == temp) {
                tail = newNode;
            }
        }
    }

    void remove(int index){
        if (index == 0) {
            pop_front();
        } else {
            Node<T>* temp = head;
            for(int i = 0; i < index - 1; i++){
                if(temp->next == nullptr){
                    return;
                }
                temp = temp->next;
            }
            Node<T>* toDelete = temp->next;
            temp->next = toDelete->next;
            if (toDelete->next != nullptr) {
                toDelete->next->prev = temp;
            }
            if (tail == toDelete) {
                tail = temp;
            }
            delete toDelete;
        }
    }

    bool empty(){
        return head == nullptr;
    }

    int size(){
        int count = 0;
        Node<T>* temp = head;
        while(temp != nullptr){
            temp = temp->next;
            count++;
        }
        return count;
    }

    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    void sort() {
        // ordenar
        head = mergeSort(head);
        // actualizar tail
        Node<T>* temp = head;
        while (temp && temp->next) {
            temp = temp->next;
        }
        tail = temp;
    }

    void reverse() {
        if (head != nullptr) {
            Node<T>* current = head;
            Node<T>* temp = nullptr;
            while (current != nullptr) {
                temp = current->prev;
                current->prev = current->next;
                current->next = temp;
                current = current->prev;
            }
            if (temp != nullptr) {
                head = temp->prev;
            }
        }
    }
};






int main(){
    // test lista vacia
    DoublyLinkedList<double> list1;
    cout << list1.front() << endl;
    cout << list1.back() << endl;
    cout << list1.pop_front() << endl;
    cout << list1.pop_back() << endl;
    cout << list1[3] << endl;
    cout << list1.size() << endl;
    // test push
    DoublyLinkedList<string> list2;
    list2.push_front("front1");
    list2.push_front("front2");
    list2.push_back("back1");
    list2.push_back("back2");
    cout << list2.front() << endl;
    cout << list2.back() << endl;
    // test []
    cout << "Imprimiendo " <<list2.size() << " elementos:"<< endl;
    for (int i = 0; i < list2.size(); i++){
        cout <<"- "<< list2[i] << endl;
    }

    // test pop
    cout << list2.pop_front() << endl;
    cout << list2.pop_back() << endl;
    cout << list2.front() << endl;

    // test clear
    list2.clear();
    cout << list2.size() << endl;

    // test reverse, insert, remove, sort
    DoublyLinkedList<int> list3;
    list3.push_front(5);
    list3.push_front(2);
    list3.push_front(4);
    list3.push_front(8);
    list3.insert(99,7);
    list3.remove(7);
    list3.push_front(1);
    list3.insert(5,3);
    list3.push_front(9);
    list3.push_front(6);
    list3.insert(6, 7);
    cout << "original: ";
    for (int i = 0; i < list3.size(); i++){
        cout << list3[i] << " ";
    }
    cout << "\n";
    list3.sort();
    cout << "sorted: ";
    for (int i = 0; i < list3.size(); i++){
        cout << list3[i] << " ";
    }
    cout << "\n";
    list3.reverse();
    cout << "reversed: ";
    for (int i = 0; i < list3.size(); i++){
        cout << list3[i] << " ";
    }

    return 0;
}