#include <catch2/catch.hpp>
#include <met/met.hpp>

#include <iostream>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float t;
};

SCENARIO( "Views are supposed to ...", "[view]" ) {
    Position pos = {3, 2};
    Velocity vel = {5};

    met::View<Position, Velocity> myView(pos, vel);

    myView.each([](){
        std::cout << "It works" << std::endl;
    });
}
