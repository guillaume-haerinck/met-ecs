#include <catch2/catch.hpp>
#include <met/met.hpp>

#include "common.hpp"

SCENARIO("Component collections are supposed to handle component storage, deletion and sorting", "[component-collection]") {
    GIVEN("A collection with 3 components") {
        Position pos = { 57, 50 };
        met::ComponentCollection<Position> collection(1, pos);
        pos = { 84, 52 };
        collection.insert(2, pos);
        pos = { 48, 21 };
        collection.insert(3, pos);

        WHEN("we remove a component from an entity") {
            collection.remove(2);

            THEN("it shouldn't store it anymore") {
                REQUIRE(collection.has(2) == false);
            }
            THEN("it shouldn't affect other entities") {
                Position storedPos = collection.at(3);
                REQUIRE(storedPos.x == pos.x);
                REQUIRE(storedPos.y == pos.y);
            }
            THEN("the size should match the number of components in use") {
                REQUIRE(collection.size() == 2);
            }
        }
    }
}
