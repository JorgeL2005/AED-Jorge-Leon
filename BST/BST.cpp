#include <iostream>

using namespace std;

template<class T>
struct NodeBT {
    T data;
    NodeBT *left;
    NodeBT *right;
    NodeBT(T value) : data(value), left(nullptr), right(nullptr) {}
};

template<typename T>
class BST{
private:
    NodeBT<T>* raiz;

    // Función auxiliar para insertar un valor en el árbol
    NodeBT<T>* insert(NodeBT<T>* node, T value){
        if(node == nullptr)
            return new NodeBT<T>(value);

        if(value < node->data)
            node->left = insert(node->left, value);
        else if(value > node->data)
            node->right = insert(node->right, value);

        return node;
    }

    // Función auxiliar para buscar un valor en el árbol
    NodeBT<T>* find(NodeBT<T>* node, T value){
        if(node == nullptr || node->data == value)
            return node;

        if(value < node->data)
            return find(node->left, value);
        else
            return find(node->right, value);
    }

    // Función auxiliar para eliminar un nodo del árbol
    NodeBT<T>* remove(NodeBT<T>* node, T value){
        if(node == nullptr)
            return node;

        if(value < node->data)
            node->left = remove(node->left, value);
        else if(value > node->data)
            node->right = remove(node->right, value);
        else {
            // Caso 1: El nodo es una hoja (sin hijos)
            if(node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
                // Caso 2: El nodo tiene un solo hijo
            else if(node->left == nullptr) {
                NodeBT<T>* temp = node->right;
                delete node;
                return temp;
            }
            else if(node->right == nullptr) {
                NodeBT<T>* temp = node->left;
                delete node;
                return temp;
            }
                // Caso 3: El nodo tiene dos hijos
            else {
                // Buscar el nodo más pequeño en el subárbol derecho
                NodeBT<T>* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }
        return node;
    }

    // Función auxiliar para encontrar el nodo con el valor mínimo
    NodeBT<T>* findMin(NodeBT<T>* node) {
        while(node && node->left != nullptr)
            node = node->left;
        return node;
    }

public:
    BST() : raiz(nullptr) {}

    // Función pública para insertar un valor
    void insert(T value) {
        raiz = insert(raiz, value);
    }

    // Función pública para buscar un valor y devolver el nodo si existe
    NodeBT<T>* find(T value) {
        return find(raiz, value);
    }

    // Función pública para eliminar un nodo
    void remove(T value) {
        raiz = remove(raiz, value);
    }

    // Función para mostrar el árbol (in-order traversal)
    void inOrder(NodeBT<T>* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->data << " ";
            inOrder(node->right);
        }
    }

    // Función pública para mostrar el árbol
    void display() {
        inOrder(raiz);
        cout << endl;
    }
};

int main() {
    BST<int> tree;

    tree.insert(1);
    tree.insert(5);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    tree.insert(11);
    tree.insert(12);
    tree.insert(13);
    tree.insert(15);
    tree.insert(18);
    tree.insert(20);
    tree.insert(22);
    tree.insert(25);
    tree.insert(26);





    cout << "Arbol en in-order despues de inserciones: ";
    tree.display();

    cout << "Buscando valor 40: ";
    if(tree.find(40))
        cout << "Encontrado!" << endl;
    else
        cout << "No encontrado!" << endl;

    tree.remove(50);
    cout << "Arbol en in-order despues de eliminar 50: ";
    tree.display();

    return 0;
}
