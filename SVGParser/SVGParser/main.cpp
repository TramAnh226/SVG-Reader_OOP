#include <iostream>
#include "SVGParser.h"
using namespace std;

int main() {
	SVGParser parser;
	SVGNode* root = parser.readXML("sample.svg");
	if (root) {
		cout << "Successfully read!";
	}
	else {
		cout << "Cannot read svg file!" << endl;
	}
	delete root;
	return 0;
}