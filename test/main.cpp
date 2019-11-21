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
    pos.x = 2;
    pos.y = 3;

    auto entity = registry.create();
    registry.assign<Position>(entity, pos);

    auto storedPos = registry.get<Position>(entity);
    std::cout << storedPos.x << " " << storedPos.y << std::endl;

    REQUIRE( true );
}
