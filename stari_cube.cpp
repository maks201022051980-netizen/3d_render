#include <iostream>
using namespace std;
struct point{
	int x;
	int y;
	int z;
};


void insert(struct point a[4][4]){
	for (int i = 0;i < 4;i++){
		for (int f = 0;f < 4;f++){
			point b;
			b.x = f;
			b.y = i;
			b.z = 255;
			a[i][f] = b;
		}
	}	
}

void print(struct point a[4][4]){
	for (int y = -5;y < 5;y++){
		for (int x = -5;x< 5;x++){
			bool flag = false;
			int z = 0;
			for (int i = 0;i < 4;i++){
				for (int f = 0;f < 4;f++){
					if (a[i][f].y == y && a[i][f].x == x){
						flag = true;
						z = a[i][f].z;
						break;
					}		
				}
				if (flag){break;}
			}
			if (flag){
				std::cout << "\033[38;5;" << z << "m# ";
				std::cout << "\033[0m";

			}
			else{
				std::cout << " " ;
			}
		}
		std::cout<< "\n";
	}
}
int main(){
	struct point a[4][4];
	insert(a);
	print(a);
}


