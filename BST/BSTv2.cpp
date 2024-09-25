#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(T value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void auxInsert(Node* node, T value) {
        if (value < node->data) {
            if (node->left == nullptr) {
                node->left = new Node(value);
            } else {
                auxInsert(node->left, value);
            }
        } else if (value > node->data) {
            if (node->right == nullptr) {
                node->right = new Node(value);
            } else {
                auxInsert(node->right, value);
            }
        }
    }

    Node* auxRemove(Node* current, T value) {
        if (current == nullptr) return current;
        // search
        if (value < current->data) {
            current->left = auxRemove(current->left, value);
        } else if (value > current->data) {
            current->right = auxRemove(current->right, value);
        }
            // current->data == value
        else {
            // no children
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
                // one child
            else if (current->left == nullptr) {
                Node* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Node* temp = current->left;
                delete current;
                return temp;
            }
                // two children
            else {
                Node* temp_parent = current;
                Node* temp = current->left;
                while (temp->right != nullptr) {
                    temp_parent = temp;
                    temp = temp->right;
                }
                if (temp_parent == current) {
                    temp->right = current->right;
                    delete current;
                    return temp;
                } else {
                    temp_parent->right = temp->left;
                    temp->left = current->left;
                    temp->right = current->right;
                    delete current;
                    return temp;
                }
            }
        }
        return current;
    }

    void auxClear(Node* node) {
        if (node != nullptr) {
            auxClear(node->left);
            auxClear(node->right);
            delete node;
        }
    }

    int auxHeight(Node* node) {
        if (node == nullptr) return -1;
        int left = auxHeight(node->left);
        int right = auxHeight(node->right);
        return 1 + max(left, right);
    }

    void preOrder(Node* node) const {
        if (node == nullptr) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void postOrder(Node* node) const {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }

    void drawCurrentDepth(Node* node, int depth, int spaces) {
        if (node == nullptr) {
            if (depth == 0) {
                for (int i = 0; i < spaces; i++) cout << " ";
                cout << "X";
                for (int i = 0; i < spaces + 1; i++) cout << " ";
                return;
            }
            drawCurrentDepth(nullptr, depth - 1, spaces);
            drawCurrentDepth(nullptr, depth - 1, spaces);
            return;
        }
        if (depth == 0) {
            for (int i = 0; i < spaces; i++) cout << " ";
            cout << node->data;
            for (int i = 0; i < spaces + 1; i++) cout << " ";
            return;
        }
        drawCurrentDepth(node->left, depth - 1, spaces);
        drawCurrentDepth(node->right, depth - 1, spaces);
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { auxClear(root); }

    void insert(T value) {
        if (root == nullptr) {
            root = new Node(value);
            return;
        }
        auxInsert(root, value);
    }

    Node* search(T value) {
        Node* current = root;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            } else if (value < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    void remove(T value) {
        root = auxRemove(root, value);
    }

    void clear() {
        auxClear(root);
        root = nullptr;
    }

    int height() {
        return auxHeight(root);
    }

    void preOrderTraversal() const {
        preOrder(root);
    }
    void inOrderTraversal() const {
        inOrder(root);
    }
    void postOrderTraversal() const {
        postOrder(root);
    }

    void draw() {
        for(int i = 0; i <= height(); i++) {
            int prod = 1<<(height() - i + 1);
            drawCurrentDepth(root, i, prod-1);
            cout << "\n";
        }
    }
};

int main() {
    BinaryTree<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    cout << "Pre-order Traversal: ";
    tree.preOrderTraversal();
    cout << "\n";
    cout << "In-order Traversal: ";
    tree.inOrderTraversal();
    cout << "\n";
    cout << "Post-order Traversal: ";
    tree.postOrderTraversal();
    cout << "\n";

    tree.draw();

    cout << "insert (i)\tremove (r)\tsearch (s)\theight (h)\ttraverse (t)\t\texit (e)\n";
    char option;
    while (cin >> option) {
        if (option == 'i') {
            int value;
            cin >> value;
            tree.insert(value);
            tree.draw();
        } else if (option == 'r') {
            int value;
            cin >> value;
            tree.remove(value);
            tree.draw();
        } else if (option == 's') {
            int value;
            cin >> value;
            if (tree.search(value) != nullptr) {
                cout << "Found\n";
            } else {
                cout << "Not found\n";
            }
        } else if (option == 'h') {
            cout << "Height: " << tree.height() << "\n";
        } else if (option == 't') {
            cout << "Pre-order Traversal: ";
            tree.preOrderTraversal();
            cout << "\n";
            cout << "In-order Traversal: ";
            tree.inOrderTraversal();
            cout << "\n";
            cout << "Post-order Traversal: ";
            tree.postOrderTraversal();
            cout << "\n";
        } else if (option == 'e') {
            break;
        }
        cout << "insert (i)\t\tremove (r)\t\tsearch (s)\t\theight (h)\t\texit (e)\n";
    }
    return 0;
}



/*
 *
 * // Modificar la función para aceptar un vector por referencia
    void inOrder(TreeNode* node, vector<int>& rpta) const {
        if (node == nullptr) return;
        inOrder(node->left, rpta);   // Recorrer el subárbol izquierdo
        rpta.push_back(node->val);   // Almacenar el valor del nodo en el vector
        inOrder(node->right, rpta);  // Recorrer el subárbol derecho
    }
 *
 *
 * Invertir un arbol:
 *
 * TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr){
            return nullptr;
        }
        TreeNode* temp = root->left;
        root->left = root->right;
        root->right = temp;
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
 */