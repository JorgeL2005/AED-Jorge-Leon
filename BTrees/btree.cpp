#include <iostream>
using namespace std;

// Clase que representa un nodo del árbol B+.
// Se utiliza un arreglo dinámico para almacenar las claves y (para nodos internos) un arreglo dinámico para los punteros a hijos.
class NodoBPlus {
public:
    bool isLeaf;      // Indica si es un nodo hoja (almacena datos) o interno.
    int numKeys;      // Número actual de claves en el nodo.
    int* keys;        // Arreglo dinámico de claves.
    NodoBPlus** children; // Arreglo dinámico de punteros a hijos (se usa en nodos internos).
    NodoBPlus* next;  // Puntero al siguiente nodo hoja (útil para recorridos secuenciales).

    // Constructor. Recibe un parámetro 'leaf' que indica si el nodo es hoja, y 'order' que es el número máximo de claves.
    NodoBPlus(bool leaf, int order) {
        isLeaf = leaf;
        numKeys = 0;
        keys = new int[order]; // Se reserva espacio para "order" claves.
        if (!leaf) {
            // En un nodo interno se requieren (order+1) punteros a hijos.
            children = new NodoBPlus*[order + 1];
            // Inicializamos los punteros a nullptr.
            for (int i = 0; i < order + 1; i++) {
                children[i] = nullptr;
            }
        } else {
            // En los nodos hoja no usamos el arreglo de hijos.
            children = nullptr;
        }
        next = nullptr;
    }

    // Destructor. Libera la memoria dinámica asignada para las claves y el arreglo de punteros.
    ~NodoBPlus() {
        delete[] keys;
        if (children != nullptr) {
            delete[] children;
        }
    }
};

// Clase que representa el árbol B+.
// Incluye funciones públicas para buscar, insertar y eliminar claves.
class BPlusTree {
private:
    NodoBPlus* root;  // Puntero a la raíz del árbol.
    int order;        // Orden del árbol: número máximo de claves en cada nodo.

public:
    // Constructor: crea un árbol B+ con la raíz siendo inicialmente un nodo hoja.
    BPlusTree(int order) {
        this->order = order;
        root = new NodoBPlus(true, order);
    }

    // Destructor: libera la memoria de todos los nodos del árbol.
    ~BPlusTree() {
        destroyTree(root);
    }

    // Función para buscar una clave en el árbol.
    // Recorre desde la raíz hasta la hoja correspondiente y realiza búsqueda lineal en el nodo hoja.
    bool search(int key) {
        NodoBPlus* current = root;
        // Mientras el nodo actual no sea hoja, se determina en qué hijo continuar la búsqueda.
        while (!current->isLeaf) {
            int i = 0;
            while (i < current->numKeys && key >= current->keys[i])
                i++;
            current = current->children[i];
        }
        // Se busca la clave en el nodo hoja.
        int i = 0;
        while (i < current->numKeys && current->keys[i] < key)
            i++;
        if (i < current->numKeys && current->keys[i] == key)
            return true;
        return false;
    }

    // Función pública para insertar una clave en el árbol.
    // Se encarga de llamar a la función interna recursiva que maneja la inserción.
    void insert(int key) {
        int newKey;              // Clave a promover si se produce división.
        NodoBPlus* newChild = nullptr; // Nuevo nodo que se crea en caso de división.
        bool needNewRoot = insertInternal(root, key, newKey, newChild);
        if (needNewRoot) {
            // Si la raíz se dividió, se crea una nueva raíz que contiene la clave promovida.
            NodoBPlus* newRoot = new NodoBPlus(false, order);
            newRoot->keys[0] = newKey;
            newRoot->numKeys = 1;
            newRoot->children[0] = root;
            newRoot->children[1] = newChild;
            root = newRoot;
        }
    }

    // Función pública para eliminar una clave del árbol.
    void remove(int key) {
        removeInternal(root, key);
        // Si la raíz resulta interna y queda sin claves, se promueve su único hijo como nueva raíz.
        if (!root->isLeaf && root->numKeys == 0) {
            NodoBPlus* tmp = root;
            root = root->children[0];
            delete tmp;
        }
    }

private:
    // Función recursiva para liberar la memoria de los nodos del árbol.
    void destroyTree(NodoBPlus* node) {
        if (node != nullptr) {
            if (!node->isLeaf) {
                // Para nodos internos se libera recursivamente cada hijo.
                for (int i = 0; i <= node->numKeys; i++) {
                    destroyTree(node->children[i]);
                }
            }
            delete node;
        }
    }

    // Función recursiva para insertar una clave.
    // Parámetros:
    // - node: nodo actual donde se inserta la clave.
    // - key: clave a insertar.
    // - upKey (por referencia): clave que se promoverá al padre si se produce división.
    // - newChild (por referencia): nuevo nodo (resultado de la división) que se enlazará al padre.
    //
    // Retorna true si el nodo se dividió y es necesario insertar la clave promovida en el padre.
    bool insertInternal(NodoBPlus* node, int key, int &upKey, NodoBPlus* &newChild) {
        // Caso base: nodo hoja.
        if (node->isLeaf) {
            // Se busca la posición en la que se insertará la nueva clave de forma ordenada.
            int pos = 0;
            while (pos < node->numKeys && node->keys[pos] < key) {
                pos++;
            }
            // Se desplazan las claves a la derecha para dejar espacio.
            for (int i = node->numKeys; i > pos; i--) {
                node->keys[i] = node->keys[i - 1];
            }
            node->keys[pos] = key;
            node->numKeys++;

            // Si el nodo hoja aún no excede el límite, no se produce división.
            if (node->numKeys < order) {
                return false;
            } else {
                // **División de nodo hoja:**
                // Se crea un nuevo nodo hoja y se trasladan la segunda mitad de las claves.
                NodoBPlus* newLeaf = new NodoBPlus(true, order);
                // Se determina el índice de división. Con (order+1)/2 se asegura que ambos nodos tengan al menos
                // ceil(order/2) claves.
                int split = (order + 1) / 2;
                newLeaf->numKeys = node->numKeys - split;
                for (int i = 0; i < newLeaf->numKeys; i++) {
                    newLeaf->keys[i] = node->keys[i + split];
                }
                node->numKeys = split;
                // Se actualiza el puntero de hojas (para recorridos secuenciales).
                newLeaf->next = node->next;
                node->next = newLeaf;
                // La clave promovida es la primera clave del nuevo nodo hoja.
                upKey = newLeaf->keys[0];
                newChild = newLeaf;
                return true;
            }
        } else {
            // Caso: nodo interno.
            // Se determina en qué hijo se debe insertar la clave.
            int pos = 0;
            while (pos < node->numKeys && key >= node->keys[pos])
                pos++;
            int newKeyChild;
            NodoBPlus* childNew = nullptr;
            // Inserción recursiva en el hijo correspondiente.
            bool splitChild = insertInternal(node->children[pos], key, newKeyChild, childNew);
            if (!splitChild)
                return false;
            // Si el hijo se dividió, se debe insertar la clave promovida y el nuevo puntero en el nodo actual.
            int insertPos = pos;
            // Se desplazan las claves y punteros a la derecha para dejar espacio.
            for (int i = node->numKeys; i > insertPos; i--) {
                node->keys[i] = node->keys[i - 1];
            }
            for (int i = node->numKeys + 1; i > insertPos + 1; i--) {
                node->children[i] = node->children[i - 1];
            }
            // Se inserta la clave promovida y se enlaza el nuevo hijo.
            node->keys[insertPos] = newKeyChild;
            node->numKeys++;
            node->children[insertPos + 1] = childNew;

            // Si el nodo interno no se desborda, finaliza.
            if (node->numKeys < order) {
                return false;
            } else {
                // **División de nodo interno:**
                // Se crea un nuevo nodo interno y se transfiere la segunda mitad de las claves y punteros.
                NodoBPlus* newInternal = new NodoBPlus(false, order);
                // Se define el índice de división (en este ejemplo, usamos order/2).
                int split = (order) / 2;
                // En la división de nodos internos en un árbol B⁺, se promueve la clave central.
                upKey = node->keys[split];
                newInternal->numKeys = node->numKeys - split - 1;
                // Copia de las claves que van al nuevo nodo (se descarta la clave promovida).
                for (int i = 0; i < newInternal->numKeys; i++) {
                    newInternal->keys[i] = node->keys[i + split + 1];
                }
                // Copia de los punteros a hijos.
                for (int i = 0; i < newInternal->numKeys + 1; i++) {
                    newInternal->children[i] = node->children[i + split + 1];
                }
                // Se actualiza el número de claves en el nodo actual.
                node->numKeys = split;
                newChild = newInternal;
                return true;
            }
        }
    }

    // Función recursiva para eliminar una clave del árbol.
    // Esta función busca la clave y, tras eliminarla, verifica que se cumplan las condiciones de ocupación.
    // En caso de que el nodo resultante tenga menos de la cantidad mínima de claves, se intentará:
    //  - Tomar prestado (redistribución) de un hermano adyacente, o
    //  - Fusionar con un hermano.
    //
    // Para simplificar, en esta implementación se asume que el número mínimo de claves es ceil(order/2).
    bool removeInternal(NodoBPlus* node, int key) {
        // Caso: nodo hoja.
        if (node->isLeaf) {
            int pos = 0;
            // Se busca la posición de la clave a eliminar.
            while (pos < node->numKeys && node->keys[pos] < key) {
                pos++;
            }
            // Si la clave no se encuentra, se retorna false.
            if (pos == node->numKeys || node->keys[pos] != key) {
                return false;
            }
            // Se desplazan las claves a la izquierda para sobreescribir la clave eliminada.
            for (int i = pos; i < node->numKeys - 1; i++) {
                node->keys[i] = node->keys[i + 1];
            }
            node->numKeys--;
            return true;
        } else {
            // Caso: nodo interno.
            int pos = 0;
            // Se determina el hijo por el que se debe descender.
            while (pos < node->numKeys && key >= node->keys[pos])
                pos++;
            bool flag = removeInternal(node->children[pos], key);
            if (!flag)
                return false;
            // Se define el número mínimo de claves permitido (mínimo = ceil(order/2)).
            int minKeys = (order + 1) / 2;
            // Si el hijo descendente tiene menos claves de las requeridas, se intenta ajustar.
            if (node->children[pos]->numKeys < minKeys) {
                // **Caso 1: Redistribución (préstamo) desde el hermano izquierdo.**
                if (pos > 0 && node->children[pos - 1]->numKeys > minKeys) {
                    NodoBPlus* child = node->children[pos];
                    NodoBPlus* leftSibling = node->children[pos - 1];
                    // Se desplazan las claves del hijo a la derecha para hacer espacio.
                    for (int i = child->numKeys; i > 0; i--) {
                        child->keys[i] = child->keys[i - 1];
                    }
                    if (!child->isLeaf) {
                        // Para nodos internos, también se desplazan los punteros a hijos.
                        for (int i = child->numKeys + 1; i > 0; i--) {
                            child->children[i] = child->children[i - 1];
                        }
                    }
                    // Se mueve la clave proveniente del padre hacia el hijo.
                    child->keys[0] = node->keys[pos - 1];
                    if (!child->isLeaf) {
                        // El hijo recibe el puntero al último hijo del hermano izquierdo.
                        child->children[0] = leftSibling->children[leftSibling->numKeys];
                    }
                    child->numKeys++;
                    // Se actualiza la clave del padre con la última clave del hermano izquierdo.
                    node->keys[pos - 1] = leftSibling->keys[leftSibling->numKeys - 1];
                    leftSibling->numKeys--;
                }
                    // **Caso 2: Redistribución (préstamo) desde el hermano derecho.**
                else if (pos < node->numKeys && node->children[pos + 1]->numKeys > minKeys) {
                    NodoBPlus* child = node->children[pos];
                    NodoBPlus* rightSibling = node->children[pos + 1];
                    // La clave del padre se traslada al final del hijo.
                    child->keys[child->numKeys] = node->keys[pos];
                    if (!child->isLeaf) {
                        child->children[child->numKeys + 1] = rightSibling->children[0];
                    }
                    child->numKeys++;
                    // Se actualiza la clave del padre con la primera clave del hermano derecho.
                    node->keys[pos] = rightSibling->keys[0];
                    // Se desplazan las claves del hermano derecho a la izquierda.
                    for (int i = 0; i < rightSibling->numKeys - 1; i++) {
                        rightSibling->keys[i] = rightSibling->keys[i + 1];
                    }
                    if (!rightSibling->isLeaf) {
                        // Se desplazan también los punteros a hijos.
                        for (int i = 0; i < rightSibling->numKeys; i++) {
                            rightSibling->children[i] = rightSibling->children[i + 1];
                        }
                    }
                    rightSibling->numKeys--;
                }
                    // **Caso 3: Fusión (merge)**
                else {
                    // Si se puede fusionar con el hermano izquierdo.
                    if (pos > 0) {
                        NodoBPlus* child = node->children[pos];
                        NodoBPlus* leftSibling = node->children[pos - 1];
                        // Para nodos internos se baja la clave del padre entre los dos nodos.
                        if (!child->isLeaf) {
                            leftSibling->keys[leftSibling->numKeys] = node->keys[pos - 1];
                            leftSibling->numKeys++;
                        }
                        // Se copian todas las claves del hijo al hermano izquierdo.
                        for (int i = 0; i < child->numKeys; i++) {
                            leftSibling->keys[leftSibling->numKeys + i] = child->keys[i];
                        }
                        if (!child->isLeaf) {
                            // Se copian también los punteros a hijos.
                            for (int i = 0; i <= child->numKeys; i++) {
                                leftSibling->children[leftSibling->numKeys + i] = child->children[i];
                            }
                        } else {
                            // Para hojas, se mantiene el enlace (next) para la lista encadenada.
                            leftSibling->next = child->next;
                        }
                        leftSibling->numKeys += child->numKeys;
                        // Se elimina la entrada correspondiente en el nodo padre.
                        for (int i = pos - 1; i < node->numKeys - 1; i++) {
                            node->keys[i] = node->keys[i + 1];
                        }
                        for (int i = pos; i < node->numKeys; i++) {
                            node->children[i] = node->children[i + 1];
                        }
                        node->numKeys--;
                        delete child;
                    }
                        // Sino, se fusiona con el hermano derecho.
                    else {
                        NodoBPlus* child = node->children[pos];
                        NodoBPlus* rightSibling = node->children[pos + 1];
                        if (!child->isLeaf) {
                            child->keys[child->numKeys] = node->keys[pos];
                            child->numKeys++;
                        }
                        for (int i = 0; i < rightSibling->numKeys; i++) {
                            child->keys[child->numKeys + i] = rightSibling->keys[i];
                        }
                        if (!child->isLeaf) {
                            for (int i = 0; i <= rightSibling->numKeys; i++) {
                                child->children[child->numKeys + i] = rightSibling->children[i];
                            }
                        } else {
                            child->next = rightSibling->next;
                        }
                        child->numKeys += rightSibling->numKeys;
                        for (int i = pos; i < node->numKeys - 1; i++) {
                            node->keys[i] = node->keys[i + 1];
                        }
                        for (int i = pos + 1; i < node->numKeys; i++) {
                            node->children[i] = node->children[i + 1];
                        }
                        node->numKeys--;
                        delete rightSibling;
                    }
                }
            }
            return true;
        }
    }
};

/////////////////////
// Función main  //
// Se muestran algunos ejemplos de uso del árbol B+.
/////////////////////
int main() {
    // Se crea un árbol B+ de orden 4 (máximo 4 claves por nodo)
    BPlusTree tree(4);

    // Inserción de algunas claves.
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);
    tree.insert(7);
    tree.insert(17);

    cout << "Resultados de búsqueda en el árbol B+:" << endl;
    cout << "Buscar 6: " << (tree.search(6) ? "Encontrado" : "No encontrado") << endl;
    cout << "Buscar 15: " << (tree.search(15) ? "Encontrado" : "No encontrado") << endl;

    // Eliminación de una clave.
    tree.remove(6);
    cout << "Luego de eliminar 6, buscar 6: " << (tree.search(6) ? "Encontrado" : "No encontrado") << endl;

    return 0;
}
