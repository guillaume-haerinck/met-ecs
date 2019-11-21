#include <catch2/catch.hpp>
#include <met/met.hpp>

#include <iostream>

SCENARIO( "Views are supposed to ...", "[view]" ) {
    met::View myView;

    myView.each([](){ 
        std::cout << "It works !" << std::endl;
    });
}
