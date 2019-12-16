#include <catch2/catch.hpp>
#include <met/met.hpp>


#include "common.hpp"

SCENARIO("registry is supposed to handle entities and components", "[registry]") {
    GIVEN("An empty registry") {
        met::Registry registry;

        WHEN("we ask for new entities") {
            met::entity entity1 = registry.create();
            met::entity entity2 = registry.create();
            met::entity entity3 = registry.create();

            THEN("it should give us unique ids") {
                REQUIRE(entity1 != entity2);
                REQUIRE(entity1 != entity3);
                REQUIRE(entity2 != entity3);
            }

            THEN("they should be destructible") {
                registry.destroy(entity1);

                REQUIRE(!registry.valid(entity1));
            }
        }
        WHEN("we assign a component") {
            met::entity entity = registry.create();
            const Position pos = { 5, 6 };
            registry.assign<Position>(entity, pos);

            THEN("we should be able to get it back") {
                const Position storedPos = registry.get<Position>(entity);

                REQUIRE(storedPos.x == pos.x);
                REQUIRE(storedPos.y == pos.y);
            }

            THEN("we should be able to modify it") {
                Position& storedPos = registry.get<Position>(entity);
                storedPos.x = 8;
                storedPos.y = 15;

                const Position storedPosCheck = registry.get<Position>(entity);

                REQUIRE(storedPos.x == storedPosCheck.x);
                REQUIRE(storedPos.y == storedPosCheck.y);
            }
        }
    }
}
