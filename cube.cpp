#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

struct point {
    float x;
    float y;
    float z;
    bool regdol = false;
};

void insert(point cube[15][15][15]) {
    for (int i = -7; i < 9; i++) {
        for (int f = -7; f < 9; f++) {
            for (int z = -7; z < 9; z++) {
                point p;

                p.x = f;
                p.y = i;
                p.z = z;

                if ((i == -7 || i == 7 || f  == -7 || f == 7) && ((z == -7 || z == 7)|| ((i == -7 || i == 7) && (f == -7 || f == 7)))) p.regdol = true;
                cube[i + 7][f + 7][z + 7] = p;
            }
        }
    }
}

void rotation_y(float angle, point cube[15][15][15]) {
    float rad = angle * M_PI / 180.0f;

    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            for (int k = 0; k < 15; k++) {
                float y = cube[i][j][k].y;
                float z = cube[i][j][k].z;

                cube[i][j][k].y = y * c - z * s;
                cube[i][j][k].z = y * s + z * c;
            }
        }
    }
}

void rotation_x(float angle, point cube[15][15][15]) {
    float rad = angle * M_PI / 180.0f;

    float c = cos(rad);
    float s = sin(rad);

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            for (int k = 0; k < 15; k++) {
                float x = cube[i][j][k].x;
                float z = cube[i][j][k].z;

                cube[i][j][k].x = x * c - z * s;
                cube[i][j][k].z = x * s + z * c;
            }
        }
    }
}

void print(point a[15][15][15]) {
    string frame;

    for (int y = -15; y < 15; y++) {
        for (int x = -15; x < 15; x++) {
            bool found = false;
            float max_z = -999;
            bool regd = false;

            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    for (int k = 0; k < 15; k++) {
                        int px = round(a[i][j][k].x);
                        int py = round(a[i][j][k].y);

                        if (px == x && py == y) {
                            if (!found || a[i][j][k].z > max_z) {
                                found = true;
                                max_z = a[i][j][k].z;
                                regd = a[i][j][k].regdol;
                            }
                        }
                    }
                }
            }

            if (found) {
                float t = (max_z + 7.0f) / 15.0f;
                int color = 231 + round(t * (254 - 231));

                if (color > 254)color = 254;

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

    while (true) {
        point cube[15][15][15];

        insert(cube);

        rotation_x(angle, cube);
        rotation_y(angle, cube);

        cout << "\033[H";

        print(cube);

        angle += 3;

        if (angle >= 360)
            angle = 0;
    }
    return 0;
}
