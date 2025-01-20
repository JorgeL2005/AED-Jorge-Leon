#include <iostream>

using namespace std;

template <typename T>
class Stack {
private:
    T* arr;
    int MAX_SIZE;
    int top;

public:
    Stack(int size) : MAX_SIZE(size), top(-1) {
        arr = new T[MAX_SIZE];
    }

    ~Stack() {
        delete[] arr;
    }

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == MAX_SIZE - 1;
    }

    void push(T value) {
        if (isFull()) {
            cout << "Stack Overflow\n";
            return;
        }
        arr[++top] = value;
    }

    T pop() {
        if (isEmpty()) {
            return T();
        }
        return arr[top--];
    }

    int size() const {
        return top + 1;
    }
};

int main() {
    Stack<int> stack(3);

    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.push(40);
    cout << stack.size() << endl;
    cout << "Elemento superior: " << stack.pop() << endl;
    cout << "Elemento superior: " << stack.pop() << endl;
    cout << "Elemento superior: " << stack.pop() << endl;
    cout << stack.size() << endl;

    return 0;
}