#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <vector> 


using namespace std;

struct point {
    float x;
    float y;
    float z;
    bool regdol = false;
};

using DynamicCube = vector<vector<vector<point>>>;

DynamicCube create_cube(int size) {
    return DynamicCube(size, vector<vector<point>>(size, vector<point>(size)));
}


void insert(DynamicCube& cube, int size) {
    int half = size / 2;

    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            for (int z = -half; z <= half; z++) {

                point p;

                p.x = x;
                p.y = y;
                p.z = z;

                int borders =
                    (abs(x) == half) +
                    (abs(y) == half) +
                    (abs(z) == half);

                p.regdol = borders >= 2;

                cube[y + half][x + half][z + half] = p;
            }
        }
    }
}

void rotation_y(float angle, DynamicCube& cube, int size) {
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                float y = cube[i][j][k].y;
                float z = cube[i][j][k].z;

                cube[i][j][k].y = y * c - z * s;
                cube[i][j][k].z = y * s + z * c;
            }
        }
    }
}

void rotation_x(float angle, DynamicCube& cube, int size) {
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                float x = cube[i][j][k].x;
                float z = cube[i][j][k].z;

                cube[i][j][k].x = x * c - z * s;
                cube[i][j][k].z = x * s + z * c;
            }
        }
    }
}

void print(DynamicCube& a, int size) {
    string frame;
    int half = size / 2;

    for (int y = -15; y < 15; y++) {
        for (int x = -15; x < 15; x++) {
            bool found = false;
            float max_z = -999;
            bool regd = false;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    for (int k = 0; k < size; k++) {
                        int px = round(a[i][j][k].x);
                        int py = round(a[i][j][k].y);

                        if (px == x && py == y) {
                            if (!found || a[i][j][k].z > max_z) {found = true;max_z = a[i][j][k].z;regd = a[i][j][k].regdol;}
                        }
                    }
                }
            }

            if (found) {
                float t = (max_z + (float)half) / (float)size;
                int color = 231 + round(t * (254 - 231));

                if (color > 254) color = 254;
                if (color < 231) color = 231;
                if (regd) color = 30;

                frame += "\033[38;5;" + to_string(color) + "m# \033[0m";
            } else {
                frame += "  ";
            }
        }
        frame += '\n';
    }
    cout << frame;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "\033[2J";
    float angle = 0;

    int size = 15;
    while (true) {
        DynamicCube cube = create_cube(size);

        insert(cube,size);

        rotation_x(angle, cube, size);
        rotation_y(angle, cube, size);

        cout << "\033[H";

        print(cube,size);

        angle += 3;

        if (angle >= 360)
            angle = 0;
    }
    return 0;
}
