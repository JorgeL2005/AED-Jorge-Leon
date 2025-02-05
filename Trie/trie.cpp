#include <iostream>
using namespace std;

#define ALPHABET_SIZE 26

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (node->children[index] == nullptr) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    bool search(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (node->children[index] == nullptr) {
                return false;
            }
            node = node->children[index];
        }
        return node->isEndOfWord;
    }

    bool startsWith(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (node->children[index] == nullptr) {
                return false;
            }
            node = node->children[index];
        }
        return true;
    }
};

int main() {
    Trie trie;

    // Insertar palabras en el Trie
    trie.insert("cat");
    trie.insert("car");
    trie.insert("dog");
    trie.insert("dot");

    // Búsquedas de palabras
    cout << "Buscar 'cat': " << trie.search("cat") << endl;  // 1 (true)
    cout << "Buscar 'car': " << trie.search("car") << endl;  // 1 (true)
    cout << "Buscar 'cow': " << trie.search("cow") << endl;  // 0 (false)

    // Buscar prefijos
    cout << "Prefijo 'ca': " << trie.startsWith("ca") << endl;  // 1 (true)
    cout << "Prefijo 'do': " << trie.startsWith("do") << endl;  // 1 (true)
    cout << "Prefijo 'da': " << trie.startsWith("da") << endl;  // 0 (false)

    return 0;
}
