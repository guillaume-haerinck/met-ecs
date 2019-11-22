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
    Position positions[] = {
        {3, 2},
        {8, 5},
        {9, 15}
    };
    Velocity velocities[] = {
        {5},
        {6},
        {8}
    };
    std::vector<met::entity> entities = {
        0, 1, 2
    };

    met::View<Position, Velocity> myView(entities, positions, velocities);

    myView.each([](met::entity id) {
        std::cout << "It works for entity " << id << std::endl;
    });
}
