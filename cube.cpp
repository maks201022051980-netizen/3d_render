#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <vector> 
#include <atomic>




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


void move_to_x(int &qu_x,atomic<char>& b,bool flag){
    if(flag)qu_x -= 2;
    else if(!flag)qu_x += 2;
    b = ' ';
}

void move_to_y(int &qu_x,atomic<char>& b,bool flag){
    if(!flag)qu_x -= 2;
    else if(flag)qu_x += 2;
    b = ' ';
}


void insert(DynamicCube& cube, int size) {
    int half = size / 2;    
    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            for (int z = -half; z <= half; z++) {

                point p;

                p.x = x;
                p.y = y;
                p.z = z ;

                int borders = (abs(x) == half) + (abs(y) == half) +(abs(z) == half);

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

void print(DynamicCube& a, int size,const int & qu_z,const int & qu_x,const int & qu_y) {
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
                        int px = round(a[i][j][k].x - qu_x);
                        int py = round(a[i][j][k].y - qu_y);

                        if (px == x && py == y) {
                            if (!found || a[i][j][k].z > max_z) {found = true;max_z = a[i][j][k].z;regd = a[i][j][k].regdol;}
                        }
                    }
                }
            }

            if (found) {
                float t = (max_z + (float)half) / (float)size;
                int color = 231 + round(t * (254 - 231)) - qu_z;

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

void input(atomic<char>& b) {
    while (true) {
        char c;
        cin >> c;
        b = c;
    }
}

void back(int& size, atomic<char>& b,int &qu_z) {

    qu_z += 2;

    if (size > 3)
        size -= 2;

    b = ' ';
}
void up(int& size, atomic<char>& b,int &qu_z) {
    qu_z -= 2;
    if (size < 30)
        size += 2;

    b = ' ';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    atomic<char> b{' '};

    float angle = 0;
    int size = 15;
    int quality_z = 0;
    int quality_x = 0;
    int quality_y = 0;

    thread t(input, ref(b));
    t.detach();

    while (true) {

        DynamicCube cube = create_cube(size);

        insert(cube, size);

        rotation_x(angle, cube, size);
        rotation_y(angle, cube, size);

        cout << "\033[H";

        print(cube, size,quality_z,quality_x,quality_y);

        angle += 3;

        if (angle >= 360)
            angle = 0;

        if (b == 's')
            back(size, b,quality_z);
        if (b == 'i')
            move_to_y(quality_y,b,true);
        if (b == 'k')
            move_to_y(quality_y,b,false);
        if (b == 'w')
            up(size, b,quality_z);
        if (b == 'd')
            move_to_x(quality_x,b,true);
        if (b == 'a')
            move_to_x(quality_x,b,false);
        
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}