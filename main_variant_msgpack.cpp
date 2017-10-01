#include <iostream>
#include <string>
#include "msgpack-alt.hpp"
#include "variant.hpp"


Variant getVariant() {
    Variant v;
    v["key1"] = "value1";
    v["group1"]["subgroup1"] = "value1-1";

    return v;
}


int main() {
    Variant v = getVariant();
    MsgPack msgpack(v);

    msgpack.save("test.mpac");
    //std::cout << v.str() << std::endl;

    return 0;
}
