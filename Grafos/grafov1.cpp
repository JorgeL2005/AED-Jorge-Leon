#include <iostream>
#include <vector>

using namespace std;

template <typename edgeValueType>
class Grafo {
private:
    struct Pair {
        int key;
        edgeValueType value;

        Pair() : key(int()), value(edgeValueType()) {}
        Pair(const int& key, const edgeValueType& value) : key(key), value(value) {}
    };

    bool isDirected;
    int numVertices;
    vector<Pair>* listaAdyacencia;

    void dfs(int v, bool* visited) {
        visited[v] = true;

        for (int i = 0; i < listaAdyacencia[v].size(); i++) {
            int vecino = listaAdyacencia[v][i].key;
            if (!visited[vecino]) {
                dfs(vecino, visited);
            }
        }
    }

public:
    Grafo(int vertices, bool dirigido = false) {
        numVertices = vertices;
        isDirected = dirigido;
        listaAdyacencia = new vector<Pair>[numVertices];
    }

    ~Grafo() {
        delete[] listaAdyacencia;
    }

    void agregarArista(int origen, int destino, edgeValueType value = edgeValueType()) {
        if (origen < 0 || origen >= numVertices || destino < 0 || destino >= numVertices) {
            return;
        }
        listaAdyacencia[origen].push_back(Pair(destino, value));
        if (!isDirected) {
            listaAdyacencia[destino].push_back(Pair(origen, value));
        }
    }

    int contarComponentesConectadas() {
        bool* visited = new bool[numVertices]();
        int count = 0;

        for (int i = 0; i < numVertices; i++) {
            if (!visited[i]) {
                dfs(i, visited);
                count++;
            }
        }
        delete[] visited;
        return count;
    }
};
int main() {
    return 0;
}