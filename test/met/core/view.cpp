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

SCENARIO( "Views2 are supposed to ...", "[view]" ) {
    met::Registry registry;
    auto entity = registry.create();
    Position pos = { 5, 2 };
    Velocity vel = { 3 };
    registry.assign<Position>(entity, pos);
    registry.assign<Velocity>(entity, vel);

	auto entity2 = registry.create();
	pos = { 8, 4 };
	vel = { 9 };
	registry.assign<Position>(entity2, pos);
	// registry.assign<Velocity>(entity2, vel);

    registry.view<Position, Velocity>().each([](met::entity id, Position& pos, Velocity& vel) {
		std::cout << "It works for entity " << id << std::endl;
		std::cout << "pos " << pos.x << " " << pos.y << std::endl;
		std::cout << "vel " << vel.t << std::endl;
	});

	registry.view<Position>().each([](met::entity id, Position& pos) {
		std::cout << "It works for entity " << id << std::endl;
		std::cout << "pos " << pos.x << " " << pos.y << std::endl;
	});
}
