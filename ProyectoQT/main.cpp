#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

#define MAX_DEPTH 8
#define THRESHOLD 15

struct Pixel {
    uint8_t r, g, b;
};

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

class QuadTree {
private:
    QuadTreeNode* root;
    vector<vector<Pixel>> image;

    Pixel computeAverageColor(int x, int y, int size) {
        int sumR = 0, sumG = 0, sumB = 0, count = 0;
        for (int i = y; i < y + size && i < image.size(); i++) {
            for (int j = x; j < x + size && j < image[0].size(); j++) {
                sumR += image[i][j].r;
                sumG += image[i][j].g;
                sumB += image[i][j].b;
                count++;
            }
        }
        return { (uint8_t)(sumR / count), (uint8_t)(sumG / count), (uint8_t)(sumB / count) };
    }

    double computeDetail(int x, int y, int size, Pixel avg) {
        double sumDiff = 0;
        int count = 0;
        for (int i = y; i < y + size && i < image.size(); i++) {
            for (int j = x; j < x + size && j < image[0].size(); j++) {
                sumDiff += pow(image[i][j].r - avg.r, 2);
                sumDiff += pow(image[i][j].g - avg.g, 2);
                sumDiff += pow(image[i][j].b - avg.b, 2);
                count++;
            }
        }
        return sqrt(sumDiff / (count * 3));
    }

    void insert(QuadTreeNode*& node, int x, int y, int size, int depth) {
        if (size <= 1 || x >= image[0].size() || y >= image.size()) return;

        Pixel avg = computeAverageColor(x, y, size);
        node = new QuadTreeNode(x, y, size, avg);

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

    void drawSegmentation(QuadTreeNode* node, vector<vector<Pixel>>& output) {
        if (!node) return;
        if (node->isLeaf) {
            for (int i = node->y; i < node->y + node->size && i < output.size(); i++) {
                for (int j = node->x; j < node->x + node->size && j < output[0].size(); j++) {
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
    QuadTree(vector<vector<Pixel>>& img) : image(img) {
        root = nullptr;
        insert(root, 0, 0, image[0].size(), 0);
    }

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

int main() {
    int width, height;
    vector<vector<Pixel>> img = loadImage("images/goku.jpg", width, height);

    QuadTree qt(img);
    qt.saveCompressedImage("goku-segmentation.jpg");

    cout << "Imagen segmentada exitosamente" << endl;
    return 0;
}
