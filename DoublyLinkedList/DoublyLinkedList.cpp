#include <iostream>

template<typename T>
struct Nodo{
    T data;
    Nodo<T>* prev;
    Nodo<T>* next;
};

template<typename T>
class Lista{
private:
    Nodo<T>* head;
    Nodo<T>* prev;
public:
    Lista(){
        head = nullptr;
        prev = nullptr;
    }
    T front(); // Retorna el elemento al comienzo
    T back(); // Retorna el elemento al final
    void push_front(T dato); // Agrega un elemento al comienzo
    void push_back(T dato); // Agrega un elemento al final
    T pop_front(); // Remueve el elemento al comienzo
    T pop_back(); // Remueve el elemento al final
    void insert(T dato, int pos); // Inserta en cualquier posición
    void remove(int pos); // Remueve en cualquier posicion
    T operator[ ](int pos); // Retorna el elemento en la posición indicada
    bool empty(); // Retorna si la lista está vacía o no
    int size(); // Retorna el tamaño de la lista
    void clear(); // Elimina todos los elementos de la lista
    void reverse(); // Revierte la lista

};



int main() {

}
