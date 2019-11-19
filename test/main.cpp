#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <met/met.hpp>

struct Position {
    float x;
    float y;
};

TEST_CASE( "Name of the case", "[category]" ) {
    met::Registry registry;
    met::ComponentHandle<Position> positionHandle;

    REQUIRE( true );
}
