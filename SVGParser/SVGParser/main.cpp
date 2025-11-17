#include <iostream>
#include "SVGParser.h"
using namespace std;

int main() {
	SVGParser parser;
	SVGGroup* root = parser.readXML("D:\\Downloads\\sample.svg");
	if (root) {
		cout << "Successfully read!";
	}
	else {
		cout << "Cannot read svg file!" << endl;
	}
	delete root;
	return 0;
}