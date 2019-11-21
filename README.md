[![Actions Status](https://github.com/guillaume-haerinck/met-ecs/workflows/unit%20test/badge.svg)](https://github.com/guillaume-haerinck/met-ecs/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# MET ECS

A simple ECS library made for learning purposes. The library is Header-Only, so just have to copy the `src` folder and include `met/met.hpp` to use it. 
The API is heavly based on [ENTT](https://github.com/skypjack/entt) by skypjack. Made by students, the project purposes are to be easy to understand and thoroughly documented.

---

> Violets are blue, roses are red, and when I MET ecs, my world rebooted

## Table of Contents

* [Introduction](#introduction)
    * [Code Example](#code-example)
* [Inspirations](#inspirations)
    * [Libraries](#libraries)

## Introduction

### Code Example

```C++
#include <met/met.hpp>

struct Position {
    float x;
    float y;
};

int main() {
    met::registry registry;

    auto entity = registry.create();
    Position pos = { 2, 3 };
    registry.assign<position>(entity, pos);

    // ...
}
```

## Inspirations

### Libraries

- [ENTT](https://github.com/skypjack/entt)
- [ECS by redxdev](https://github.com/redxdev/ECS)

