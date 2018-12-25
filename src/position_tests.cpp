#include <iostream>
#include "position.h"
#include "checkerboard.h"
#include <cassert>

int main()
{
    Position p1("c4");
    Position p2(2, 6);

    assert(p1.toString() == "c4");
    assert(p2.toString() == "g3");
    assert(!(p1 == p2));

    p2 = p1;
    assert((p1 == p2));

    p1 = "c1";
    assert(p1 == "c1");

    Checkerboard c;
    std::cout << c;
    return 0;
}