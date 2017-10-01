#include <iostream>
#include <string>
#include "variant.hpp"

int main() {
    variant::Variant d;
    d["key1"] = "value1";
    d["group1"]["subgroup1"] = "value1-1";

    variant::Variant v;
    v.resize(3);
    v[0] = 1.0;
    v[1] = 3;
    v[2] = "This is a test";




    std::cout << d.str() << std::endl;
    std::cout << v.str() << std::endl;

    return 0;
}
