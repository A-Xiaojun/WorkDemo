#include<iostream>
using namespace std;

class MyClass1 {
public:
	MyClass1(int value) {

	}

	int sum(int a, int b) {
		return a + b;
	}
};

class MyClass2 {
public:
	explicit MyClass2(int value) {

	}

	int dec(int a, int b) {
		return a - b;
	}
};

int main() {
	MyClass1 obj = 10;
	MyClass2 obj1(11);

	obj.sum(10, 10);
}
