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

        void setValue(const edgeValueType& newValue) {
            value = newValue;
        }
    };

    bool isDirected;
    int numVertices;
    vector<vector<Pair>> listaAdyacencia;

    void auxDFS(int vertice, vector<bool>& visitado) {
        visitado[vertice] = true;
        cout << vertice << " ";

        for (Pair vecino : listaAdyacencia[vertice]) {
            if (!visitado[vecino.key]) {
                auxDFS(vecino.key, visitado);
            }
        }
    }
    // Función para inicializar distancias y predecesores
    void initializeSingleSource(vector<edgeValueType>& dist, vector<int>& predecesor, int inicio) {
        for (int i = 0; i < numVertices; i++) {
            dist[i] = numeric_limits<edgeValueType>::max();
            predecesor[i] = -1; // -1 indica que no tiene predecesor
        }
        dist[inicio] = 0;
    }

    // Función de relajación
    void relax(int u, int v, edgeValueType peso, vector<edgeValueType>& dist, vector<int>& predecesor) {
        if (dist[u] != numeric_limits<edgeValueType>::max() && dist[u] + peso < dist[v]) {
            dist[v] = dist[u] + peso;
            predecesor[v] = u;
        }
    }


public:
    Grafo(int vertices, bool dirigido = false) {
        numVertices = vertices;
        isDirected = dirigido;
        listaAdyacencia.resize(numVertices);
    }

    void agregarArista(int origen, int destino, edgeValueType value) {
        if (origen < 0 || origen >= numVertices || destino < 0 || destino >= numVertices) {
            cout << "Error: Vertices fuera de rango" << endl;
            return;
        }
        if (isDirected) { // Dirigido
            listaAdyacencia[origen].push_back(Pair(destino, value));
        } else { // No dirigido
            listaAdyacencia[origen].push_back(Pair(destino, value));
            listaAdyacencia[destino].push_back(Pair(origen, value));
        }
    }

    void DFS(int inicio) {
        vector<bool> visitado(numVertices, false);
        auxDFS(inicio, visitado);
    }

    void imprimirGrafo() {
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertice " << i << ":";
            for (Pair arista : listaAdyacencia[i]) {
                cout << " -> " << arista.key << ":(" << arista.value << ")";
            }
            cout << endl;
        }
    }

    double bellmanFord(int inicio, int end_node) {
        vector<double> dist(numVertices, numeric_limits<double>::max());
        vector<int> predecesor(numVertices, -1);

        // Inicializamos las distancias y los predecesores
        initializeSingleSource(dist, predecesor, inicio);
        // Relajamos las aristas numVertices - 1 veces
        for (int i = 0; i < numVertices - 1; i++) {
            for (int u = 0; u < numVertices; u++) {
                for (const Pair& vecino : listaAdyacencia[u]) {
                    relax(u, vecino.key, vecino.value, dist, predecesor);
                }
            }
        }
        // Comprobamos ciclos negativos
        for (int u = 0; u < numVertices; u++) {
            for (const Pair& vecino : listaAdyacencia[u]) {
                if (dist[u] != numeric_limits<double>::max() &&
                    dist[u] + vecino.value < dist[vecino.key]) {
                    cout << "Error: El grafo contiene un ciclo negativo." << endl;
                    return numeric_limits<double>::max();
                }
            }
        }
        // Si la distancia al nodo final es infinita, no hay camino
        if (dist[end_node] == numeric_limits<double>::max()) {
            cout << "No hay camino desde " << inicio << " a " << end_node << endl;
            return dist[end_node];
        }
        // Retornamos la distancia al nodo final
        return dist[end_node];
    }

};

int main() {
    Grafo<double> grafo(5, true);
    grafo.agregarArista(0, 1, 6);
    grafo.agregarArista(0, 2, 7);
    grafo.agregarArista(1, 2, 8);
    grafo.agregarArista(1, 3, 5);
    grafo.agregarArista(1, 4, -4);
    grafo.agregarArista(2, 3, -3);
    grafo.agregarArista(2, 4, 9);
    grafo.agregarArista(3, 1, -2);
    grafo.agregarArista(4, 0, 2);
    grafo.agregarArista(4, 3, 7);

    grafo.DFS(0);
    cout << "Distancia más corta de 0 a 3: " << grafo.bellmanFord(0, 3) << endl;

    return 0;
}