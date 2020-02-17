#include <iostream>

struct class1 {
	class1() { std::cout << "1\n"; }
	class1(const class1 &) { std::cout << "3\n"; }
	~class1() { std::cout << "2\n"; }
	
	void f() { std::cout << "4\n"; }
} object;


int main(int argc,char *argv[]) {
	//class1(object);
	class1 object;
    return 0;
}
