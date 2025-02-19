#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// 🔹 Implementación de Pair
template <typename edgeValueType>
struct Pair {
    int key;
    edgeValueType value;

    Pair() : key(int()), value(edgeValueType()) {}
    Pair(const int& key, const edgeValueType& value) : key(key), value(value) {}

    void setValue(const edgeValueType& newValue) {
        value = newValue;
    }
};

// 🔹 Implementación de Queue
template <typename T>
class Queue {
private:
    T* arr;
    int MAX_SIZE;
    int front;
    int rear;
    int count;
public:
    Queue(int size) : MAX_SIZE(size), front(0), rear(0), count(0) {
        arr = new T[MAX_SIZE];
    }

    ~Queue() {
        delete[] arr;
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == MAX_SIZE;
    }

    void enqueue(T value) {
        if (isFull()) {
            return;
        }
        arr[rear] = value;
        rear = (rear + 1) % MAX_SIZE;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            return T();
        }
        T value = arr[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return value;
    }

    int size() const {
        return count;
    }
};

// 🔹 Implementación de Grafo
template <typename edgeValueType>
class Grafo {
private:
    bool isDirected;
    int numVertices;
    vector<vector<Pair<edgeValueType>>> listaAdyacencia;

public:
    Grafo(int vertices, bool dirigido = false) {
        numVertices = vertices;
        isDirected = dirigido;
        listaAdyacencia.resize(numVertices);
    }

    void agregarArista(int origen, int destino, edgeValueType value) {
        listaAdyacencia[origen].push_back(Pair<edgeValueType>(destino, value));
        if (!isDirected) {
            listaAdyacencia[destino].push_back(Pair<edgeValueType>(origen, value));
        }
    }

    // 🔹 Implementación de Dijkstra
    vector<edgeValueType> dijkstra(int inicio) {
        vector<edgeValueType> dist(numVertices, numeric_limits<edgeValueType>::max());
        vector<bool> visitado(numVertices, false);
        Queue<int> q(numVertices * numVertices); // Simulación de una cola de prioridad

        dist[inicio] = 0;
        q.enqueue(inicio);

        while (!q.isEmpty()) {
            // 🟢 Encontrar el nodo con menor distancia en la cola
            int u = -1;
            edgeValueType minDist = numeric_limits<edgeValueType>::max();
            int qSize = q.size();

            for (int i = 0; i < qSize; i++) {
                int v = q.dequeue();
                if (!visitado[v] && dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
                q.enqueue(v); // Volver a meterlo en la cola
            }

            // Si no encontramos un nodo válido, terminamos
            if (u == -1) break;
            visitado[u] = true;

            // 🔹 Relajación de distancias
            for (const Pair<edgeValueType>& vecino : listaAdyacencia[u]) {
                int v = vecino.key;
                edgeValueType peso = vecino.value;
                edgeValueType nuevaDistancia = dist[u] + peso;

                if (!visitado[v] && nuevaDistancia < dist[v]) {
                    dist[v] = nuevaDistancia;
                    q.enqueue(v); // Añadir a la cola para procesar
                }
            }
        }

        return dist;
    }
};

// 🔹 Función principal para probar Dijkstra sin `priority_queue`
int main() {
    Grafo<int> g(5, false);

    g.agregarArista(0, 1, 2);
    g.agregarArista(0, 3, 6);
    g.agregarArista(1, 2, 3);
    g.agregarArista(1, 3, 8);
    g.agregarArista(1, 4, 5);
    g.agregarArista(2, 4, 7);
    g.agregarArista(3, 4, 9);

    vector<int> distancias = g.dijkstra(0);

    cout << "Distancias mínimas desde el nodo 0:" << endl;
    for (int i = 0; i < distancias.size(); i++) {
        cout << "Nodo " << i << ": " << distancias[i] << endl;
    }

    return 0;
}
