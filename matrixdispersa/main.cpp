#include <iostream>
using namespace std;


struct Cell {
    int row, col, value;
    Cell* nextRow;
    Cell* nextCol;

    Cell(int r, int c, int v) : row(r), col(c), value(v), nextRow(nullptr), nextCol(nullptr) {}
};


struct Header {
    int index;
    Cell* next; // puntero al primer nodo
    Header* nextHeader;

    Header(int i) : index(i), next(nullptr), nextHeader(nullptr) {}
};


class SparseMatrix {
    friend SparseMatrix addSparseMatrices(const SparseMatrix& sm1, const SparseMatrix& sm2);
private:
    Header* rowHead;
    Header* colHead;
    int rows, cols;

    // Buscar o crear cabecera para filas
    Header* getOrCreateRowHeader(int row) {
        Header* prev = nullptr;
        Header* curr = rowHead;

        while (curr && curr->index < row) {
            prev = curr;
            curr = curr->nextHeader;
        }

        if (!curr || curr->index != row) {
            Header* newHeader = new Header(row);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                rowHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }

    // Buscar o crear cabecera para columnas
    Header* getOrCreateColHeader(int col) {
        Header* prev = nullptr;
        Header* curr = colHead;

        while (curr && curr->index < col) {
            prev = curr;
            curr = curr->nextHeader;
        }

        if (!curr || curr->index != col) {
            Header* newHeader = new Header(col);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                colHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }

public:
    SparseMatrix(int r, int c) : rowHead(nullptr), colHead(nullptr), rows(r), cols(c) {}

    void insert(int row, int col, int value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || value == 0) return;

        Header* rowHeader = getOrCreateRowHeader(row);
        Header* colHeader = getOrCreateColHeader(col);

        Cell* newCell = new Cell(row, col, value);

        // Insertar en la fila
        Cell* rowPrev = nullptr;
        Cell* rowCurr = rowHeader->next;
        while (rowCurr && rowCurr->col < col) {
            rowPrev = rowCurr;
            rowCurr = rowCurr->nextRow;
        }
        if (rowPrev) {
            rowPrev->nextRow = newCell;
        } else {
            rowHeader->next = newCell;
        }
        newCell->nextRow = rowCurr;

        // Insertar en la columna
        Cell* colPrev = nullptr;
        Cell* colCurr = colHeader->next;
        while (colCurr && colCurr->row < row) {
            colPrev = colCurr;
            colCurr = colCurr->nextCol;
        }
        if (colPrev) {
            colPrev->nextCol = newCell;
        } else {
            colHeader->next = newCell;
        }
        newCell->nextCol = colCurr;
    }

    // Mostrar la matriz
    void display() {
        for (int i = 0; i < rows; ++i) {
            Header* rowHeader = getOrCreateRowHeader(i);
            Cell* cell = rowHeader->next;
            for (int j = 0; j < cols; ++j) {
                if (cell && cell->col == j) {
                    cout << cell->value << " ";
                    cell = cell->nextRow;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }

    ~SparseMatrix() {
        Header* rowCurr = rowHead;
        while (rowCurr) {
            Cell* cellCurr = rowCurr->next;
            while (cellCurr) {
                Cell* toDelete = cellCurr;
                cellCurr = cellCurr->nextRow;
                delete toDelete;
            }
            Header* toDelete = rowCurr;
            rowCurr = rowCurr->nextHeader;
            delete toDelete;
        }

        Header* colCurr = colHead;
        while (colCurr) {
            Header* toDelete = colCurr;
            colCurr = colCurr->nextHeader;
            delete toDelete;
        }
    }
};

SparseMatrix addSparseMatrices(const SparseMatrix& sm1, const SparseMatrix& sm2) {
    SparseMatrix result(sm1.rows, sm1.cols);

    // Recorrer la primera matriz y agregar sus valores a la matriz resultante
    Header* rowCurr = sm1.rowHead;
    while (rowCurr) {
        Cell* cellCurr = rowCurr->next;
        while (cellCurr) {
            result.insert(cellCurr->row, cellCurr->col, cellCurr->value);
            cellCurr = cellCurr->nextRow;
        }
        rowCurr = rowCurr->nextHeader;
    }

    // Recorrer la segunda matriz y sumar sus valores a la matriz resultante
    rowCurr = sm2.rowHead;
    while (rowCurr) {
        Cell* cellCurr = rowCurr->next;
        while (cellCurr) {
            // Obtener el valor actual en la matriz resultante
            int currentVal = 0;
            Header* resultRowHeader = result.getOrCreateRowHeader(cellCurr->row);
            Cell* resultCell = resultRowHeader->next;

            while (resultCell && resultCell->col < cellCurr->col) {
                resultCell = resultCell->nextRow;
            }

            if (resultCell && resultCell->col == cellCurr->col) {
                currentVal = resultCell->value;
            }

            // Insertar la suma en la matriz resultante
            result.insert(cellCurr->row, cellCurr->col, currentVal + cellCurr->value);

            cellCurr = cellCurr->nextRow;
        }
        rowCurr = rowCurr->nextHeader;
    }

    return result;
}


int main() {
    // Ejercicio 1
    cout << "Problema 1:" << endl;
    SparseMatrix sm(9, 9);

    sm.insert(2, 1, 3);
    sm.insert(2, 2, 5);
    sm.insert(2, 7, 9);
    sm.insert(3, 7, 5);
    sm.insert(4, 5, 5);
    sm.insert(4, 9, 22);
    sm.insert(6, 5, 5);
    sm.insert(7, 2, 5);
    sm.insert(7, 8, 5);
    sm.insert(9, 9, 5);

    sm.display();

    // Ejercicio 2
    cout << "Problema 2:" << endl;

    cout << "Matriz 1:" << endl;
    SparseMatrix sm1(9,7);

    sm1.insert(2, 3, 3);
    sm1.insert(5, 1, 3);
    sm1.insert(5, 4, 1);
    sm1.insert(7, 4, 5);
    sm1.insert(8, 1, 1);
    sm1.insert(8, 6, 4);
    sm1.insert(9, 3, 2);

    sm1.display();

    cout << "Matriz 2:" << endl;
    SparseMatrix sm2(9,7);
    sm2.insert(2, 3, 4);
    sm2.insert(5, 1, 2);
    sm2.insert(5, 4, 1);
    sm2.insert(7, 4, 4);
    sm2.insert(8, 1, 3);
    sm2.insert(8, 6, 2);
    sm2.insert(9, 3, 1);

    sm2.display();

    cout << "Matriz resultante:" << endl;
    SparseMatrix result = addSparseMatrices(sm1, sm2);
    result.display();





    return 0;
}
