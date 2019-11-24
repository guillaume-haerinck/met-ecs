#include <catch2/catch.hpp>
#include <met/met.hpp>

#include "common.hpp"

SCENARIO("Views are supposed to allow us to iterate through entities with a given set of components", "[view]") {
    GIVEN("A registry with 2 entities and 2 components") {
        met::Registry registry;
        Position pos = { 5, 2 };
        Velocity vel = { 3 };

        auto entity1 = registry.create();
        registry.assign<Position>(entity1, pos);
        registry.assign<Velocity>(entity1, vel);

        auto entity2 = registry.create();
        pos = { 8, 4 };
        registry.assign<Position>(entity2, pos);

        WHEN("we ask for views") {
            auto view1 = registry.view<Position, Velocity>();
            auto view2 = registry.view<Position>();

            THEN("they should have the right number of matching entities") {
                REQUIRE(view1.size() == 1);
                REQUIRE(view2.size() == 2);
            }

            THEN("we should be able to iterate through them with a lambda function") {
                view1.each([vel](met::entity id, Position& storedPos, Velocity& storedVel) {
                    REQUIRE(vel.dx == storedVel.dx);
                    REQUIRE(vel.dy == storedVel.dy);
                    });
            }

            THEN("we should be able to modify components in a lambda function") {
                vel = { 25, 56 };

                view1.each([vel](met::entity id, Position& storedPos, Velocity& storedVel) {
                    storedVel = vel;
                    });

                view1.each([vel](met::entity id, Position& storedPos, Velocity& storedVel) {
                    REQUIRE(vel.dx == storedVel.dx);
                    REQUIRE(vel.dy == storedVel.dy);
                    });
            }
        }
    }
}
