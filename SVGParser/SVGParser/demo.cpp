#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

int main() {
    XMLDocument doc;
    if (doc.LoadFile("sample.svg") == XML_SUCCESS) {
        cout << "Doc file SVG thanh cong!\n";
    }
    else {
        cout << "Doc file that bai!\n";
    }
    return 0;
}