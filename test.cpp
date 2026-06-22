#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

struct Point {
    float x, y, z;
    bool edge = false;
};

using DynamicCube = vector<vector<vector<Point>>>;

DynamicCube create_cube(int size) {
    return DynamicCube(size,
        vector<vector<Point>>(size,
        vector<Point>(size)));
}

void insert(DynamicCube& cube, int size) {
    int half = size / 2;

    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            for (int z = -half; z <= half; z++) {

                Point p;

                p.x = x;
                p.y = y;
                p.z = z;

                int borders =
                    (abs(x) == half) +
                    (abs(y) == half) +
                    (abs(z) == half);

                p.edge = borders >= 2;

                cube[y + half][x + half][z + half] = p;
            }
        }
    }
}

void rotation_x(float angle, DynamicCube& cube, int size) {
    float rad = angle * M_PI / 180.f;

    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++) {

                float y = cube[i][j][k].y;
                float z = cube[i][j][k].z;

                cube[i][j][k].y = y * c - z * s;
                cube[i][j][k].z = y * s + z * c;
            }
}

void rotation_y(float angle, DynamicCube& cube, int size) {
    float rad = angle * M_PI / 180.f;

    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++) {

                float x = cube[i][j][k].x;
                float z = cube[i][j][k].z;

                cube[i][j][k].x = x * c + z * s;
                cube[i][j][k].z = -x * s + z * c;
            }
}

void print(DynamicCube& cube, int size) {

    string frame;
    int half = size / 2;

    for (int y = -15; y < 15; y++) {
        for (int x = -15; x < 15; x++) {

            bool found = false;
            float bestZ = -999;
            bool edge = false;

            for (auto& a : cube)
                for (auto& b : a)
                    for (auto& p : b) {

                        if (round(p.x) == x &&
                            round(p.y) == y &&
                            p.z > bestZ) {

                            found = true;
                            bestZ = p.z;
                            edge = p.edge;
                        }
                    }

            if (found) {

                int color =
                    edge
                    ? 30
                    : 231 + (bestZ + half) * 21 / size;

                frame +=
                    "\033[38;5;" +
                    to_string(color) +
                    "m# \033[0m";
            }
            else {
                frame += "  ";
            }
        }

        frame += '\n';
    }

    cout << frame;
}

int main() {

    cout << "\033[2J";

    int size = 15;

    DynamicCube original = create_cube(size);

    insert(original, size);

    float angle = 0;

    while (true) {

        DynamicCube cube = original;

        rotation_x(angle, cube, size);
        rotation_y(angle * 0.7f, cube, size);

        cout << "\033[H";

        print(cube, size);

        angle += 3;

        this_thread::sleep_for(
            chrono::milliseconds(40));

        if (angle >= 360)
            angle = 0;
    }

    return 0;
}