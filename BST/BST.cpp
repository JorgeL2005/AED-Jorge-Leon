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
class BST {
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

    // Función auxiliar para obtener la altura de un nodo
    int height(NodeBT<T>* node) {
        if (node == nullptr)
            return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    // Función auxiliar para verificar si un nodo y sus subárboles están balanceados
    bool isBalanced(NodeBT<T>* node) {
        if (node == nullptr)
            return true;

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        // Verificar si la diferencia de alturas es mayor a 1
        if (abs(leftHeight - rightHeight) > 1)
            return false;

        // Recursivamente verificar si los subárboles están balanceados
        return isBalanced(node->left) && isBalanced(node->right);
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

    // Función pública para verificar si el árbol está balanceado
    bool isBalanced() {
        return isBalanced(raiz);
    }
};

int main() {
    BST<int> tree;

    tree.insert(3);
    tree.insert(2);
    tree.insert(20);
    tree.insert(15);
    tree.insert(30);


    cout << "Arbol en in-order traversal: ";
    tree.display();

    cout << "El arbol está balanceado? ";
    if (tree.isBalanced()) {
        cout << "Si, esta balanceado." << endl;
    } else {
        cout << "No, no esta balanceado." << endl;
    }

    tree.remove(30);
    cout << "Arbol en in-order despues de eliminar 30: ";
    tree.display();

    cout << "El arbol está balanceado? ";
    if (tree.isBalanced()) {
        cout << "Si, esta balanceado." << endl;
    } else {
        cout << "No, no esta balanceado." << endl;
    }

    return 0;
}
