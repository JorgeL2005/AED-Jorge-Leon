#include <iostream>

using namespace std;

template <typename T>
class Deque {
private:
    T* arr;
    int MAX_SIZE;
    int front;
    int rear;
    int count;
public:
    Deque(int size) : MAX_SIZE(size), front(0), rear(0), count(0) {
        arr = new T[MAX_SIZE];
    }

    ~Deque() {
        delete[] arr;
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == MAX_SIZE;
    }

    void push_front(T value) {
        if (isFull()) {
            return;
        }
        arr[front] = value;
        front = (front - 1 + MAX_SIZE) % MAX_SIZE;
        count++;
    }

    void push_back(T value) {
        if (isFull()) {
            return;
        }
        arr[rear] = value;
        rear = (rear + 1) % MAX_SIZE;
        count++;
    }

    T pop_front() {
        if (isEmpty()) {
            return T();
        }
        front = (front + 1) % MAX_SIZE;
        count--;
        return arr[front];
    }

    T pop_back() {
        if (isEmpty()) {
            return T();
        }
        rear = (rear - 1) % MAX_SIZE;
        count--;
        return arr[rear];
    }

    int size() const {
        return count;
    }

    void print() {
        if (isEmpty()) {
            return;
        }
        int i = (front + 1) % MAX_SIZE;
        while (i != rear) {
            cout << arr[i] << " ";
            i = (i + 1) % MAX_SIZE;
        }
        cout << endl;

    }
};

// Ejemplo de uso
int main() {
    Deque<int> dq(6);
    dq.push_front(1);
    dq.print();
//    dq.push_back("push_back");
//    dq.print();
//    dq.push_front("push_front");
//    dq.print();
//    dq.push_back("push_back");
//    dq.print();

    return 0;
}
