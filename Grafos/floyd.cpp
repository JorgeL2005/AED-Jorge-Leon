#include <iostream>
#include <vector>
#include <limits>

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

    // 🔹 Implementación de Floyd-Warshall
    vector<vector<edgeValueType>> floydWarshall() {
        const edgeValueType INF = numeric_limits<edgeValueType>::max();
        vector<vector<edgeValueType>> dist(numVertices, vector<edgeValueType>(numVertices, INF));

        // Inicializar la matriz de distancias
        for (int i = 0; i < numVertices; i++) {
            dist[i][i] = 0;  // Distancia de un nodo a sí mismo es 0
            for (const Pair& vecino : listaAdyacencia[i]) {
                dist[i][vecino.key] = vecino.value;  // Inicializar con pesos de las aristas
            }
        }

        // Aplicar el algoritmo de Floyd-Warshall
        for (int k = 0; k < numVertices; k++) {
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {  // Evitar sumas con infinito
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        return dist;  // Retornamos la matriz de distancias mínimas
    }
};

// 🔹 Función de prueba del algoritmo
int main() {
    Grafo<int> g(4, true);

    g.agregarArista(0, 1, 5);
    g.agregarArista(0, 3, 10);
    g.agregarArista(1, 2, 3);
    g.agregarArista(2, 3, 1);

    vector<vector<int>> resultado = g.floydWarshall();

    cout << "Matriz de distancias mínimas usando Floyd-Warshall:\n";
    for (int i = 0; i < resultado.size(); i++) {
        for (int j = 0; j < resultado[i].size(); j++) {
            if (resultado[i][j] == numeric_limits<int>::max()) {
                cout << "INF ";
            } else {
                cout << resultado[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
