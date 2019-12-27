#include <catch2/catch.hpp>
#include <met/met.hpp>


#include "common.hpp"

SCENARIO("registry is supposed to handle entities and components", "[registry]") {
    GIVEN("An empty registry") {
        met::registry registry;

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
        WHEN("we create 3 entities with a component") {
            for (size_t i = 1; i < 4; i++) {
                met::entity id = registry.create();
                Position pos = { (float) i, (float) i };
                registry.assign<Position>(id, pos);
            }
            
            THEN("we should be able to get them back") {
                unsigned int i = 1;
                registry.view<Position>().each([&i](met::entity id, Position pos) {
                    Position checkPos = { (float) i, (float) i };
                    i++;

                    REQUIRE(pos.x == checkPos.x);
                    REQUIRE(pos.y == checkPos.y);
                });
            }

            AND_WHEN("we delete the 2nd entity") {
                registry.destroy(2);

                THEN("it shouldn't affect other entities") {
                    unsigned int i = 1;
                    registry.view<Position>().each([&i](met::entity id, Position pos) {
                        Position checkPos = { (float) i, (float) i };
                        i += 2;

                        REQUIRE(pos.x == checkPos.x);
                        REQUIRE(pos.y == checkPos.y);
                    });
                }
            }
        }
        WHEN("we create 3 entities with 2 components each") {
            for (size_t i = 1; i < 4; i++) {
                met::entity id = registry.create();
                Position pos = { (float) i, (float) i };
                Velocity vel = { (float) i, (float) i };
                registry.assign<Position>(id, pos);
                registry.assign<Velocity>(id, vel);
            }
            
            THEN("we should be able to get them back") {
                unsigned int i = 1;
                registry.view<Position, Velocity>().each([&i](met::entity id, Position pos, Velocity vel) {
                    Position checkPos = { (float) i, (float) i };
                    Velocity checkVel = { (float) i, (float) i };
                    i++;

                    REQUIRE(pos.x == checkPos.x);
                    REQUIRE(pos.y == checkPos.y);

                    REQUIRE(vel.dx == checkVel.dx);
                    REQUIRE(vel.dy == checkVel.dy);
                });
            }

            AND_WHEN("we delete the 2nd entity") {
                registry.destroy(2);

                THEN("it shouldn't affect other entities") {
                    unsigned int i = 1;
                    registry.view<Position, Velocity>().each([&i](met::entity id, Position pos, Velocity vel) {
                        Position checkPos = { (float) i, (float) i };
                        Velocity checkVel = { (float) i, (float) i };
                        i += 2;

                        REQUIRE(pos.x == checkPos.x);
                        REQUIRE(pos.y == checkPos.y);

                        REQUIRE(vel.dx == checkVel.dx);
                        REQUIRE(vel.dy == checkVel.dy);
                    });
                }
            }
        }
    }
}
