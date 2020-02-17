#include <iostream>

struct class1 {
	class1() { std::cout << "1\n"; }
	class1(const class1 &) { std::cout << "3\n"; }
	~class1() { std::cout << "2\n"; }
	
	void f() { std::cout << "4\n"; }
} object;

class C {
public:
	explicit C(int) {
		std::cout << "i";
	} 
	C(double ) {
		std::cout << "d";	
	}
};

int main(int argc,char *argv[]) {
	C c1(7);
	C c2 = 7;
    return 0;
}
