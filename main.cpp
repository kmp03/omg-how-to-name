#include <iostream>
#include <vector>

#include "header.h"

void info(vint const& arr) {
    using namespace std;
    cout << "size: " << arr.size() << '\n';
    cout << "capacity: " << arr.capacity() << '\n';
    cout << arr;
}

int main() {
    using namespace std;

    vint vec;
    for (size_t i = 0; i < 25; i++) {
        vec.push_back(i + 1);
        info(vec);
        cout << "##############\n";
    }

    return 0;
}
