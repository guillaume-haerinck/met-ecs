#include <catch2/catch.hpp>
#include <met/met.hpp>

#include <iostream>

struct Velocity {
    float t;
};

SCENARIO( "Views are supposed to ...", "[view]" ) {
    auto velocities = new met::ComponentCollection<Velocity>();
    Velocity vel = { 5 };
    velocities->components.at(0) = vel;

    std::vector<met::IComponentCollection*> components = {
        velocities
    };
    std::vector<met::entity> entities = {
        0, 1, 2
    };

    met::View myView(components, entities);

    myView.each([]() { 
        std::cout << "It works !" << std::endl;
    });
}
