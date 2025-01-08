#include <iostream>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
};

template <typename T>
class List {
private:
    Node<T>* head;

    Node<T>* merge(Node<T>* first, Node<T>* second) {
        if (!first)
            return second;
        if (!second)
            return first;
        if (first->data < second->data) {
            first->next = merge(first->next,second);
            return first;
        } else {
            second->next = merge(first,second->next);
            return second;
        }
    }
    Node<T>* mergeSort(Node<T>* head) {
        if (!head || !head->next)
            return head;
        Node<T>* slow = head;
        Node<T>* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node<T>* temp = slow->next;
        slow->next = nullptr;
        return merge(mergeSort(head), mergeSort(temp));
    }

public:
    List(){
        head = nullptr;
    }
    ~List() {
        clear();
    }
    T front(){
        if(head != nullptr){
            return head->data;
        }
        return T();
    }
    T back(){
        if(head != nullptr){
            Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            return temp->data;
        }
        return T();
    }
    void push_front(T data){
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = head;
        head = newNode;
    }
    void push_back(T data){
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->next = nullptr;
        if(!head){
            head = newNode;
        }else{
            Node<T>* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    T pop_front(){
        if(head){
            Node<T>* temp = head;
            head = head->next;
            T data = head->data;
            delete temp;
            return data;
        }
        return T();
    }
    T pop_back(){
        if(head){
            Node<T>* temp = head;
            Node<T>* prev = nullptr;
            while(temp->next != nullptr){
                prev = temp;
                temp = temp->next;
            }
            if(prev){
                prev->next = nullptr;
            }else{
                head = nullptr;
            }
            T data = temp->data;
            delete temp;
            return data;
        }
        return T();
    }
    T operator[](int index){
        if(head != nullptr){
            Node<T>* temp = head;
            for(int i = 0; i < index; i++){
                if(temp->next == nullptr){
                    return T();
                }
                temp = temp->next;
            }
            return temp->data;
        }
        return T();
    }
    bool isEmpty(){
        return head == nullptr;
    }
    void insert(int index, T data){
        if(index == 0){
            push_front(data);
        }else{
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
            temp->next = newNode;
        }
    }
    void remove(int index){
        if(head != nullptr){
            if(index == 0){
                pop_front();
            }else{
                Node<T>* temp = head;
                for(int i = 0; i < index - 1; i++){
                    if(temp->next == nullptr){
                        return;
                    }
                    temp = temp->next;
                }
                if(temp->next != nullptr){
                    Node<T>* toDelete = temp->next;
                    temp->next = toDelete->next;
                    delete toDelete;
                }
            }
        }
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
    void clear(){
        while(head != nullptr){
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void sort() {
        head = mergeSort(head);
    }
    void reverse(){
        if(head != nullptr){
            Node<T>* prev = nullptr;
            Node<T>* current = head;
            Node<T>* next = nullptr;
            while(current != nullptr){
                next = current->next;
                current->next = prev;
                prev = current;
                current = next;
            }
            head = prev;
        }
    }
    Node<T> *getHead(){
        return head;
    }
    friend ostream& operator<<(ostream& os, List<T>& list){
        Node<T>* temp = list.head;
        os << "[";
        while(temp != nullptr){
            os << temp->data;
            if (temp->next != nullptr){
                os << ", ";
            }
            temp = temp->next;
        }
        os << "]";
        return os;
    }
};






int main(){
    // test lista vacia
    cout << "Test lista vacia" << endl;
    List<double> list1;
    cout << list1.front() << endl;
    cout << list1.back() << endl;
    cout << list1.pop_front() << endl;
    cout << list1.pop_back() << endl;
    cout << list1[3] << endl;
    cout << list1.size() << endl;
    // test push
    cout << "Test push" << endl;
    List<string> list2;
    list2.push_front("front1");
    list2.push_front("front2");
    list2.push_back("back1");
    list2.push_back("back2");
    cout << list2.front() << endl;
    cout << list2.back() << endl;
    // test []
    cout << "Test []" << endl;
    cout << "Imprimiendo " <<list2.size() << " elementos:"<< endl;
    for (int i = 0; i < list2.size(); i++){
        cout <<"- "<< list2[i] << endl;
    }

    // test pop
    cout << "Test Pop" << endl;
    cout << list2.pop_front() << endl;
    cout << list2.pop_back() << endl;
    cout << list2.front() << endl;

    // test clear
    cout << "Test clear" << endl;
    list2.clear();
    cout << list2.size() << endl;

    // test reverse, insert, remove, sort
    cout << "Test reverse. insert, remove, sort" << endl;
    List<int> list3;
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
    cout << list3 << endl;

    list3.sort();
    cout << "sorted: ";
    cout << list3 << endl;

    cout << "\n";
    list3.reverse();
    cout << "reversed: ";
    cout << list3 << endl;

    return 0;
}