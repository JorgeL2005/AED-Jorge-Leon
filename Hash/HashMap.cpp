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
        slow->next = NULL;
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
        if(head == nullptr){
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
        if(head != nullptr){
            Node<T>* temp = head;
            head = head->next;
            T data = temp->data;
            delete temp;
            return data;
        }
        return T();
    }
    T pop_back(){
        if(head != nullptr){
            if(head->next == nullptr){
                T data = head->data;
                delete head;
                head = nullptr;
                return data;
            }else{
                Node<T>* temp = head;
                while(temp->next->next != nullptr){
                    temp = temp->next;
                }
                T data = temp->next->data;
                delete temp->next;
                temp->next = nullptr;
                return data;
            }
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
    bool empty(){
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
};

template <typename key_type, typename value_type>
class Hashmap {
private:
    struct Pair {
        key_type key;
        value_type value;

        Pair() : key(key_type()), value(value_type()) {}
        Pair(const key_type& key, const value_type& value) : key(key), value(value) {}

        void setValue(const value_type& newValue) {
            value = newValue;
        }

        bool operator==(const Pair& other) const {
            return (key == other.key) && (value == other.value);
        }
        bool operator!=(const Pair& other) const {
            return !(*this == other);
        }
    };

    int MAX_SIZE;
    float MAX_FACTOR;
    int MAX_LIST;
    List<Pair>* arr;

    // Funcion hash para string
    int hash_function(const string& key) {
        int hash = 0;
        int a = 31;
        for (char c : key) {
            hash = (hash*a + static_cast<int>(c)) % MAX_SIZE;
        }
        return hash;
    }
    // Sobrecarga de funcion hash para int
    int hash_function(int key) {
        return key % MAX_SIZE;
    }
public:
    Hashmap(int size, float factor, int list){
        MAX_SIZE = size;
        arr = new List<Pair>[MAX_SIZE];
        MAX_FACTOR = factor;
        MAX_LIST = list;
    }
    ~Hashmap(){
        delete[] arr;
    }
    void insert(key_type key, value_type value){
        int index = hash_function(key);
        Pair p(key, value);
        // si la llave ya existe se actualiza el valor
        Node<Pair>* temp = arr[index].getHead();
        int nodeCount = 0;
        while (temp != nullptr) {
            if (temp->data.key == key) {
                temp->data.setValue(value);
                return;
            }
            temp = temp->next;
            nodeCount++;
            // si la lista es mayor a MAX_LIST se hace rehash
            if (nodeCount > MAX_LIST) {
                rehash(MAX_SIZE * 2);
                insert(key, value);
                return;
            }
        }
        // si no existe se inserta
        arr[index].push_back(p);
    }
    value_type search(key_type key){
        int index = hash_function(key);
        Node<Pair>* temp = arr[index].getHead();
        while (temp != nullptr) {
            if (temp->data.key == key) {
                return temp->data.value;
            }
            temp = temp->next;
        }
        return value_type();
    }
    void remove(key_type key){
        int index = hash_function(key);
        Node<Pair>* temp = arr[index].getHead();
        int i = 0;
        while (temp != nullptr) {
            if (temp->data.key == key) {
                arr[index].remove(i);
                return;
            }
            temp = temp->next;
            i++;
        }
    }
    void clear(){
        for (int i = 0; i < MAX_SIZE; i++) {
            arr[i].clear();
        }
    }
    void rehash(int newSize){
        List<Pair>* temp = arr;
        int oldSize = MAX_SIZE;
        MAX_SIZE = newSize;
        arr = new List<Pair>[MAX_SIZE];
        for (int i = 0; i < oldSize; i++) {
            Node<Pair>* tempNode = temp[i].getHead();
            while (tempNode != nullptr) {
                insert(tempNode->data.key, tempNode->data.value);
                tempNode = tempNode->next;
            }
        }
        delete[] temp;
        cout << "Se hizo un rehash. Nuevo tamanio: " << newSize << endl;
    }

    void print(){
        for (int i = 0; i < MAX_SIZE; i++) {
            Node<Pair>* temp = arr[i].getHead();
            cout << i << ": ";
            while (temp != nullptr) {
                cout << "(" << temp->data.key << ", " << temp->data.value << ") ";
                temp = temp->next;
            }
            cout << endl;
        }

    }
};


string generarPalabra(int longitud) {
    const string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string palabra;

    for (int i = 0; i < longitud; ++i) {
        int indice = rand() % caracteres.length();
        palabra += caracteres[indice];
    }

    return palabra;
}

int main() {
    Hashmap<string, int> h(2, 0.75, 4);

    for(int i = 0; i < 100; i++){
        string word = generarPalabra(rand()%8+1);
        h.insert(word, rand()%1000);
    }

    h.print();

    cout << h.search("cs");

    return 0;
}