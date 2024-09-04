#include <iostream>

// Listas enlazadas (Forward List)


using namespace std;
template <typename T>
struct Node{
    T data;
    Node<T>* next;
};

template<typename T>
class List{
private:
    Node<T>* head;
public:
    List(){
        head = nullptr;
    };
    ~List(){
        clear();
    }

    T front(){
        if(head == nullptr){
            return T();
        }
        else{
            return head->data;
        }
    }; // Retorna el elemento al comienzo

    T back(){
        if (head == nullptr){
            return T();
        }
        Node<T>* temp = head;
        while(temp->next != nullptr){
            temp = temp->next;
        }
        return temp->data;

    }; // Retorna el elemento al final

    void push_front(T dato){
        Node<T>* nuevo = new Node<T>;
        nuevo->data = dato;
        nuevo->next = head;
        head = nuevo;
    }; // Agrega un elemento al comienzo

    void push_back(T dato){
        Node<T>* nuevo = new Node<T>;
        nuevo->data = dato;
        Node<T>* temp = head;
        while (temp->next != nullptr){
            temp = temp -> next;
        }
        temp->next = nuevo;
        nuevo->next = nullptr;
    }; // Agrega un elemento al final

    void pop_front(){
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }; // Remueve el elemento al comienzo

    void pop_back(){
        if (head->next == nullptr){
            delete head;
            head = nullptr;
        }
        else{
            Node<T>* temp = head;
            while(temp->next->next != nullptr){
                temp = temp -> next;
            }
            delete temp -> next;
            temp->next = nullptr;
        }
    }; // Remueve el elemento al final

    T operator[ ](int pos){
        if (head == nullptr){
            return T();
        }
        Node<T>* temp = head;
        for(int i = 0; i < pos; i++){
            if(temp->next == nullptr){
                return T();
            }
            temp = temp->next;
        }
        return temp->data;
    }; // Retorna el elemento en la posición indicada

    bool empty(){
        if (head == nullptr){
            return true;
        }else
            return false;
    }; // Retorna si la lista está vacía o no

    int size(){
        Node<T>* temp = head;
        int contador = 0;
        if (head == nullptr){
            return 0;
        }
        if (head->next == nullptr){
            return 1;
        }
        while (temp != nullptr){
            temp = temp->next;
            contador += 1;
        }
        return contador;
    }; // Retorna el tamaño de la lista

    void clear(){
        if (head == nullptr){
            cout << "La lista esta vacia" << endl;
        }
        while (head != nullptr){
            Node<T>* temp = head;
            head = head -> next;
            delete temp;
        }
    }; // Elimina todos los elementos de la lista

    void sort(); // Implemente un algoritmo de ordenacion con listas enlazadas)

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

    }; // Revierte la lista


int main(){
// Pruebas de los códigos

    // Test de front y back
    List<double> lista;

    cout << lista.empty() << endl;
    cout << lista.back() << endl;
    cout << lista.front() << endl;

    lista.push_front(6);
    lista.push_front(7);
    lista.push_front(2);
    lista.push_back(4);
    lista.push_back(9);

    cout << lista.empty() << endl;
    cout << lista.front() << endl;
    cout << lista.back() << endl;

    lista.pop_front();
    cout << lista.front() << endl;
    lista.pop_back();
    cout << lista.back() << endl;


    // test reverse
    List<int> list3;
    list3.push_front(5);
    list3.push_front(2);
    list3.push_front(4);
    list3.push_front(8);
    list3.push_front(7);
    list3.push_front(1);
    list3.push_front(3);
    list3.push_front(9);
    list3.push_front(6);
    cout << "original: ";
    for (int i = 0; i < list3.size(); i++){
        cout << list3[i] << " ";
    }

    list3.reverse();
    cout << endl;
    cout << "reverso: ";
    for (int i = 0; i < list3.size(); i++){
        cout << list3[i] << " ";
    }

}
