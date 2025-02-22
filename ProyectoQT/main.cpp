
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"




using namespace std;

// Parámetros de compresión
#define MAX_DEPTH 8
#define THRESHOLD 13  // Nivel de detalle permitido en un bloque

// Estructura para un píxel RGB
struct Pixel {
    uint8_t r, g, b;
};

// Nodo del QuadTree
struct QuadTreeNode {
    int x, y, size;
    Pixel avgColor;
    bool isLeaf;
    QuadTreeNode* children[4];

    QuadTreeNode(int x_, int y_, int size_, Pixel color)
            : x(x_), y(y_), size(size_), avgColor(color), isLeaf(true) {
        for (int i = 0; i < 4; i++) children[i] = nullptr;
    }
};

// Clase QuadTree
class QuadTree {
private:
    QuadTreeNode* root;
    vector<vector<Pixel>> image;

    // Calcula el color promedio de una región
    Pixel computeAverageColor(int x, int y, int size) {
        int sumR = 0, sumG = 0, sumB = 0, count = 0;
        for (int i = y; i < y + size; i++) {
            for (int j = x; j < x + size; j++) {
                sumR += image[i][j].r;
                sumG += image[i][j].g;
                sumB += image[i][j].b;
                count++;
            }
        }
        return { (uint8_t)(sumR / count), (uint8_t)(sumG / count), (uint8_t)(sumB / count) };
    }

    // Calcula la desviación estándar de una región
    double computeDetail(int x, int y, int size, Pixel avg) {
        double sumDiff = 0;
        for (int i = y; i < y + size; i++) {
            for (int j = x; j < x + size; j++) {
                sumDiff += pow(image[i][j].r - avg.r, 2);
                sumDiff += pow(image[i][j].g - avg.g, 2);
                sumDiff += pow(image[i][j].b - avg.b, 2);
            }
        }
        return sqrt(sumDiff / (size * size * 3));
    }

    // Inserta y segmenta la imagen
    void insert(QuadTreeNode*& node, int x, int y, int size, int depth) {
        if (!node) {
            Pixel avg = computeAverageColor(x, y, size);
            node = new QuadTreeNode(x, y, size, avg);
        }

        if (depth >= MAX_DEPTH || computeDetail(x, y, size, node->avgColor) <= THRESHOLD) {
            node->isLeaf = true;
            return;
        }

        node->isLeaf = false;
        int half = size / 2;
        insert(node->children[0], x, y, half, depth + 1);
        insert(node->children[1], x + half, y, half, depth + 1);
        insert(node->children[2], x, y + half, half, depth + 1);
        insert(node->children[3], x + half, y + half, half, depth + 1);
    }

    // Dibuja la imagen comprimida
    void drawSegmentation(QuadTreeNode* node, vector<vector<Pixel>>& output) {
        if (!node) return;

        if (node->isLeaf) {
            for (int i = node->y; i < node->y + node->size; i++) {
                for (int j = node->x; j < node->x + node->size; j++) {
                    output[i][j] = node->avgColor;
                }
            }
        } else {
            for (int i = 0; i < 4; i++) {
                drawSegmentation(node->children[i], output);
            }
        }
    }

public:
    // Constructor
    QuadTree(vector<vector<Pixel>>& img) : image(img) {
        root = nullptr;
        insert(root, 0, 0, img.size(), 0);
    }

    // Guarda la imagen comprimida
    void saveCompressedImage(const string& filename) {
        vector<vector<Pixel>> output = image;
        drawSegmentation(root, output);

        int width = output[0].size();
        int height = output.size();
        vector<uint8_t> imgData(width * height * 3);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int idx = (i * width + j) * 3;
                imgData[idx] = output[i][j].r;
                imgData[idx + 1] = output[i][j].g;
                imgData[idx + 2] = output[i][j].b;
            }
        }

        stbi_write_jpg(filename.c_str(), width, height, 3, imgData.data(), 100);
    }
};

// Cargar imagen con STB Image
vector<vector<Pixel>> loadImage(const string& filename, int& width, int& height) {
    int channels;
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, 3);

    if (!data) {
        cerr << "Error cargando la imagen!" << endl;
        exit(1);
    }

    vector<vector<Pixel>> img(height, vector<Pixel>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = (i * width + j) * 3;
            img[i][j] = { data[idx], data[idx + 1], data[idx + 2] };
        }
    }

    stbi_image_free(data);
    return img;
}

// Main
int main() {
    int width, height;
    vector<vector<Pixel>> img = loadImage("man.jpg", width, height);

    QuadTree qt(img);
    qt.saveCompressedImage("compressed.jpg");

    cout << "Imagen comprimida guardada como compressed.jpg" << endl;
    return 0;
}
