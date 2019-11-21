#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <met/met.hpp>

#include <iostream>

struct Position {
    float x;
    float y;
};

TEST_CASE( "Name of the case", "[category]" ) {
    met::Registry registry;
    Position pos;

    // Entity 1
    pos.x = 2;
    pos.y = 3;
    auto entity = registry.create();
    registry.assign<Position>(entity, pos);

    // Entity 2
    pos.x = 6;
    pos.y = 8;
    auto entity2 = registry.create();
    registry.assign<Position>(entity2, pos);

    auto& storedPos1 = registry.get<Position>(entity);
    storedPos1.x = 200;
    std::cout << storedPos1.x << " " << storedPos1.y << std::endl;

    auto& storedPos2 = registry.get<Position>(entity2);
    std::cout << storedPos2.x << " " << storedPos2.y << std::endl;

    REQUIRE( true );
}
