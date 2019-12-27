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

    GIVEN("A collection with 10 components") {
        Position pos = { 0, 0 };
        met::ComponentCollection<Position> collection(1, pos);
        for (unsigned int i = 2; i < 10; i++) {
            pos = { (float) i, (float) i };
            collection.insert(i, pos);
        }
        
        WHEN("we remove half of the components") {
            for (unsigned int i = 2; i < 10; i++) {
                if (i % 2 == 0)
                    collection.remove(i);
            }

            THEN("the size should match 5") {
                REQUIRE(collection.size() == 5);
            }

            THEN("the removed indices should be null") {
                for (unsigned int i = 2; i < 10; i++) {
                    if (i % 2 == 0)
                        REQUIRE(collection.has(i) == false);
                }
            }

            THEN("the stored data should match what was given") {
                for (unsigned int i = 2; i < 10; i++) {
                    if (i % 2 != 0) {
                        pos = { (float) i, (float) i };
                        REQUIRE(collection.has(i) == true);
                        REQUIRE(pos.x == collection.at(i).x);
                        REQUIRE(pos.y == collection.at(i).y);
                    }
                }
            }
        }

        WHEN("we overwrite existing components") {
            for (unsigned int i = 1; i < 10; i++) {
                pos = { (float) i + 1, (float) i + 1};
                collection.insert(i, pos);
            }

            THEN("the stored data should match the overwrite") {
                for (unsigned int i = 1; i < 10; i++) {
                    pos = { (float) i + 1, (float) i + 1 };
                    REQUIRE(collection.has(i) == true);
                    REQUIRE(pos.x == collection.at(i).x);
                    REQUIRE(pos.y == collection.at(i).y);
                }
            }
        }
    }
}
