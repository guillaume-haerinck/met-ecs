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
    registry.assign<Position>(0);
    registry.view<Position>();
    auto test = registry.get<Position>();

    std::cout << test.x << " " << test.y << std::endl;

    REQUIRE( true );
}
