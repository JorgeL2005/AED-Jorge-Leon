#include <iostream>

using namespace std;

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

// Ejemplo de uso
int main() {
    Queue<int> q(6);

    q.enqueue(10);
    cout << "size: " << q.size() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "size: " << q.size() << endl;

    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);
    q.enqueue(6);
    q.enqueue(7);
    cout << "size: " << q.size() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;
    cout << "Dequeued: " << q.dequeue() << endl;

    return 0;
}