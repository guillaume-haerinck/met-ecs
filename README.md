[![Actions Status](https://github.com/guillaume-haerinck/met-ecs/workflows/unit%20test/badge.svg)](https://github.com/guillaume-haerinck/met-ecs/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Met ECS

A simple [Entity Component System](#what-is-ecs) library made for learning purposes. It is **header-only**, so just have to copy the content of the `src` folder and include `<met/met.hpp>` to check it out. It does require a C++ 17 version of your compiler though.

Made by students, the project purposes are to be easy to understand and thoroughly documented. It is seen as an **entry point for users and developers**. We provide both a [post-mortem](docs/README.md) to explain the inner-working of the library, and many [exercices](learn/README.md) to get started with ECS.

The API is heavly based on [ENTT](https://github.com/skypjack/entt) by skypjack, you should check it out if you're looking for a highly performant and production-ready library.

---

> Violets are blue, roses are red, and when I met ecs, my world rebooted 🌐

## Table of Contents

* [Introduction](#introduction)
	* [What is ECS ?](#what-is-ecs)
    * [Code Example](#code-example)
* [Inspirations](#inspirations)
    * [Libraries](#libraries)

## Introduction

### What is ECS ?

ECS (Entity Component System) is way to organize your data and your logic which radicaly changes from traditional Object-Oriented Programming. It is seen as both a way to improve performance and to ease maintenance.

### Code Example

```C++
#include <met/met.hpp>

struct Position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

int main() {
    met::registry registry;
    auto entity = registry.create();
    
    Position pos = { 2, 3 };
    registry.assign<position>(entity, pos);
    
    Velocity vel = { 5, 8 };
    registry.assign<position>(entity, vel);
    
    registry.view<Position, Velocity>().each([](met::entity id, Position& pos, Velocity& vel) {
        pos.x += 2;
        std::cout << "pos " << pos.x << " " << pos.y << std::endl;
        std::cout << "vel " << vel.dx << " " << vel.dy << std::endl;
    });

    // ...
}
```

## Inspirations

### Libraries

- [ENTT](https://github.com/skypjack/entt)
- [ECS by redxdev](https://github.com/redxdev/ECS)

