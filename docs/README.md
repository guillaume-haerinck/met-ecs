# Met ECS post-mortem

## Table of Content

[**Introduction**](#introduction)

[**I - Why ECS ?**](#i---why-ecs-)
*	[**Performance**](#a---performance)
*  [**Maintenability**](#b---maintenability)
* [**Portability**](#c---portability)
* [**Adoption in the industry**](#d---adoption-in-the-industry)

[**II - The theory**](#ii---the-theory)
*  [**Entities and Components**](#a---entities-and-components)
*  [**Systems**](#b---systems)
*  [**Inter-System communication**](#c---inter-system-communication)

[**III - Interacting with ECS**](#iii---interacting-with-ecs)
*  [**Shaping and filling Components**](#a---shaping-and-filling-components)
*  [**Getting and modifying Components**](#b---getting-and-modifying-components)
*  [**Handling singleton Components and Events**](#c---handling-singleton-components-and-events)

[**IV - Data structure**](#iv---data-structure)
*  [**Storing Components**](#a---storing-components)
*  [**Handling changes**](#b---handling-changes)
*  [**Getting Components in Systems**](#c---getting-components-in-systems)

[**Conclusion**](#conclusion)

## Introduction

When I started to work on **Met ECS**, I had [one project](https://github.com/guillaume-haerinck/imac-tower-defense) dealing with Entity Component Systems on my back. At the time I used the [ENTT](https://github.com/skypjack/entt) library by Skypjack to structure my application. As the most popular Open-Source library currently available, it seemed like a great starting point to structure my library and test its performance afterwards.

Through these lines, I will do my best to explain what is ECS, what it stands for and how I implemented this paradigm following the ENTT library API. The code is not the most performant there is, but it is short, and tries to be easy to understand.

If you spot a mistake, or have any comment, you can open an [issue](https://github.com/guillaume-haerinck/met-ecs/issues) on this repository.

## I - Why ECS ?
> An explanation of data-oriented practices, their advantages, flaws and uses in the real world

There are multiple good reason to use an ECS architecture if you are building a game, or even an application. As any other paradigm, it has it own drawbacks and is **not suited for every need**. To know if it's made for you, you need to *"know your data"*. Where, how, by whom and at which frequency it will be modified.

If you have no idea of what ECS is, please take a quick look at [the explanation on the readme](https://github.com/guillaume-haerinck/met-ecs#what-is-ecs-) as it will help you to undestrand this first part. Many aspects will be detailed below, and you will find a summary table at the end of this part.

___

### A - Performance

One of the first reason that comes in mind when we think about using ECS is getting a good execution time. 

#### Data locality

Cache misses. You might have never heard of this term but it is nowadays one of the barebones of your application execution speed. While the execution of our processors kept getting faster and faster - following [Moore's law](https://en.wikipedia.org/wiki/Moore%27s_law) - the data access time didn't keep up.

> Sure, we can *process* data faster than ever, but we can’t *get* that data faster. - Bob Nystrom

<p align="center">
  <img width="700" src="https://raw.githubusercontent.com/guillaume-haerinck/met-ecs/master/docs/post-mortem-img/data-locality-chart.png" alt="Data locality chart"/>
</p>

*Data speed chart from [Game Programming Patterns](https://gameprogrammingpatterns.com/data-locality.html)*

When the CPU has to read data from the RAM, it takes a lot of time. To prevent this from happening a lot, CPU vendors created **small local memory caches** inside of CPUs. Given their sizes and positions, they are way faster than RAM access. We name these caches by their levels, L1, L2, L3, etc. The smaller is the number, the smaller is the size but faster is the read access.

When processing, if the CPU find the data in the L1 cache, that's a **cache hit**, if it's not there, it's a **cache miss** and it will look on larger but slower memory L2, L3, and after a while the RAM. 

A CPU will fill its memory caches with the data most likely be needed next. This process is called [Data prefetching](https://en.wikipedia.org/wiki/Cache_prefetching) and it's all based on a prediction. Many things are involved to make the best prediction possible, it is a complicated process, but there are [ways](https://medium.com/software-design/why-software-developers-should-care-about-cpu-caches-8da04355bb8a) to improve the "predictability" of our program. 

1. **Sequential access** is the idea to read data in the same order as it is laid out in memory . For exemple if we take an array of size 5 :

```
SEQUENTIAL ACCESS :
[1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5]
 ^             ^             ^             ^             ^
RANDOM ACCESS :
[1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5] [1|2|3|4|5]
   ^                 ^   ^               ^               ^
```

2. **Contiguous data** is the idea that there is no hole or jumps to make when accessing the data. An array of pointers is not supposed to be contiguous for exemple.
```
CONTIGUOUS X :
[x|x|x|x|x|x|x|z|z|...]

NON-CONTIGUOUS X :
[x|z|x|z|z|x|x|y|a|...]
```
These ideas are shaping the concept of  [**data locality**](https://gameprogrammingpatterns.com/data-locality.html), which is a big part of data-oriented design and ECS by extension. When using ECS, the data is always contiguous, and the access tends to be sequential most of the time. Explanation of this fact is on the part IV.

#### Data oriented design

Data oriented design (DOD) is often opposed to Object-Oriented-Programming (OOP). If you are new to both, do not worry, we will take a look at how OPP typically works for a simple particle-generation app, and then we will see what is a DOD way to do the same thing.

It will be a summary of this [well written article](http://lucteo.ro/2018/01/04/Data-Oriented-Design-and-efficiency/) by Lucteo on 2018. Similar articles can be found [here](https://nikitablack.github.io/2017/02/02/Data-Oriented-Design-by-example.html), [here](http://aras-p.info/texts/files/2018Academy%20-%20ECS-DoD.pdf) and [here](https://www.gamedev.net/blogs/entry/2265481-oop-is-dead-long-live-oop/). They offers various viewpoints on the famous OOP vs DOD debate are worth looking into. The takeways from them can be found on conclusion of this part.

<p align="center">
  <img width="700" src="https://raw.githubusercontent.com/guillaume-haerinck/met-ecs/master/docs/post-mortem-img/particles.gif" alt="Data locality chart"/>
</p>

##### Object oriented

With OOP, we put both data and the logic to process it into the same class. The code can mimics a real life behavior, making the code easy to model and to understand for programmers. 

For our app, we are showing moving *dots* on the screen, so we will need a class for the dots, and a class for the *world* to put them in. We might add squares or circles to draw in the future, so we make dot inherits an abstract class *IGameObject* which will be the one stored by the world.

<p align="center">
  <img width="450" src="https://raw.githubusercontent.com/guillaume-haerinck/met-ecs/master/docs/post-mortem-img/particles-uml-oop.png" alt="Data locality chart"/>
</p>

##### Data oriented

With DOD, we split the data into different classes based on how frequently they are used. The logic is now handled by the World class.

<p align="center">
  <img width="550" src="https://raw.githubusercontent.com/guillaume-haerinck/met-ecs/master/docs/post-mortem-img/particles-uml-dod.png" alt="Data locality chart"/>
</p>

##### Which one performs best ?

The DOD approach wins by far in terms of performance ([code is available here](https://github.com/lucteo/DOD_samples/tree/master/gameobject)). Not because OOP is inherently wrong, but because our approach to OOP is flawed for this use-case. To understand why DOD is faster, we need to understand why OOP is so slow here and how we can improve it.

<p align="center">
  <img width="550" src="https://raw.githubusercontent.com/guillaume-haerinck/met-ecs/master/docs/post-mortem-img/particles-performance.png" alt="Data locality chart"/>
</p>

| Step | Problem description |  Fix description |
| --- | --- | --- |
| OOP flat | We store an array of *IGameObject**. They are pointers and they are not contiguous in memory, causing cache misses | Removing the *IGameObject* class allows you to store Dots without using pointers |
| Split struct | The Dot objects contains data that is not used as frequently as position and velocity which reduces the cache efficiency | Move the cold data used by Dot to another class |
| Split objects | Dots are all updated but some of them are not in sight | Sort the array of Dots by their position and stops to render when one Dot is out of sight |

By applying these steps, we reached the same layout presented by the DOD class diagram, and the exact same performances. The takeaway from this is that the right awnser depends on the situation, instead of looking at the most logical way to structure our OPP classes, we should have looked directly at how it is going to be used and have structured our app this way directly.

> Object oriented programming is not necessarily evil. Be careful not to design yourself into a corner - [Tony Albrecht](http://harmful.cat-v.org/software/OO_programming/_pdf/Pitfalls_of_Object_Oriented_Programming_GCAP_09.pdf), *Pitfalls of OOP*, 2009

A problem with OOP is that on larger projects, [classes tends to end up with unrelated data](https://youtu.be/yy8jQgmhbAU?t=3038), and though breaking a [SOLID](https://itnext.io/solid-principles-explanation-and-examples-715b975dcad4) structure, leading to less performance. We can call it bad engineering but it is just a fact of life.

> The purpose of all programs, and all parts of those programs, is to transform data from one form to another.  
– [Mike Acton](https://youtu.be/rX0ItVEVjHc?t=753), *CppCon*, 2014

Finally about DOD, be warned that there is more it than reducing cache misses. ECS might be a data-oriented way to program, but it's not **the** data-oriented way to program. DOD is a topic that is way to broad to explain here, so I recommend you to have a look at [this blog post](http://blog.s-schoener.com/2019-06-09-data-oriented-design/) by Sebastian Schoner and [this book](http://www.dataorienteddesign.com/dodbook/) by Richard Fabian if you want to know more about this subject. If you are aching for performance boosts, you might also have a look at [SIMD operations](http://sci.tuomastonteri.fi/programming/sse) and multithreading.

___
### B - Maintenability

In ECS, data and logic are separated. When you are used to OOP, it's a strange step to make, as grouping the two helped to find your way into the code. Doesn't putting related code in separate files and classes will make development comberstone ? No, and we will see why it has a lot of advantages.

#### Finding where data is changed

When you open up an existing codebase, 

- No hiden states

#### Thinking about the masses

> When there is one, there is many. - [Aras Pranckevičius](http://aras-p.info/texts/files/2018Academy%20-%20ECS-DoD.pdf), *Unity Training Academy*, 2018

This can be a problem when prototyping as it is difficult to reason about unity

#### Adding features

- Diamond inheritence problem

#### Testing

Unit tests
- Output your app data anytime you want

___
### C - Portability

You know your inputs and your outputs

#### Free functions

- Reuse systems accross projets

#### Similar architecture across projects

- Greets the fact that from one app to another easier to understand the structure with ECS
- But be warned that it might be more difficult to solve problem efficiently when you have an imposed architecture

___
### D - Adoption in the industry

Used by AAA games, why they picked ecs for their use cases

#### Overwatch

#### Destiny

#### Frostpunk

#### Unity engine

- Warn about getting used to it and problems to reason about static data and unity

## II - The theory
> Discover what are entities, component and systems. Understand how they interact together 

[Nomad game engine](https://medium.com/@savas/nomad-game-engine-part-2-ecs-9132829188e5)

### A - Entities and Components

- What they are and how they relate to each others

### B - Systems

- How many of them and what they do

### C - Inter-System communication

- Out of ecs question that is worth mentioning
- Event based with Observer pattern
- Singleton components check with ifs

## III - Interacting with ECS
> What are the possible APIs available to use ECS and what are their advantages

Pick different approaches
___
### A - Shaping and filling Components

- Registering components vs declaring them in advance
- Using external files

___
### B - Getting and modifying Components

- Registrering systems or not
- Using a central registry
- Examples in the industry
- The API of met ecs

___
### C - Handling singleton Components and Events

- How to handle static data inside of ecs
- Unity GetSingletonComponent API
- Overwatch singleton components
- ENTT event classes
- The beast voxel editor friend class approach
- Handle things outside of ECS is common (GUI, inputs, etc)

## IV - Data structure
> How the data of ECS is stored and accessed. Everything there is to know about Met ECS inner working.

___
### A - Storing Components

- Pure array
- Sparse set
- Archetypes
- The sparse set of met ecs
- Templates

___
### B - Handling changes

- Sorting
- Deletion

___
### C - Getting Components in Systems

- The view creation of met ecs
- The templates tips and tricks

## Conclusion

## References

### Libraries

| Link | Description |
| --- | --- |
| [ENTT](https://github.com/skypjack/entt) | The C++ API which MetECS mimics |
| [Flecs](https://github.com/SanderMertens/flecs) | A multi-threaded C library |
| [Unity ECS](https://docs.unity3d.com/Packages/com.unity.entities@0.4/manual/index.html) | The documentation of the ECS implementation of Unity Game Engine |

### Articles

| Link | Description | Date |
| --- | --- | --- |
| [Awesome list ECS](https://github.com/jslee02/awesome-entity-component-system) | A list of ressources about Entity Component System | Updated frequently |
| [Awesome list DOD](https://github.com/dbartolini/data-oriented-design) | A list of ressources about Data oriented Design| Updated frequently |
| [ECS faq](https://github.com/SanderMertens/ecs-faq) | Frequently asked questions about ECS by Sender Mertens | 2019 |
| [Task-Based Programming](https://software.intel.com/en-us/node/506100) | Intel article about parallel computing using load balancing (opposed to and more efficient than System-on-A-Thread) | 2019
| [An interpretation of DOD](http://blog.s-schoener.com/2019-06-09-data-oriented-design/) |  A post by Sebastian Schoner looking at DOD and ECS in more deepth than simply avoiding cache misses | 2019 |
| [ECS - Where are my entities ?](https://skypjack.github.io/2019-03-07-ecs-baf-part-2/) | An article by Skypjack on Sparse set vs Archetypes for component storage | 2019 |
| [Programming lessons learned from making my first game](https://github.com/adnzzzzZ/blog/issues/31) | Post mortem by adnzzzzZ about why he doesn't recommand ECS for small projects | 2018 |
| [Data Oriented Design and efficiency](http://lucteo.ro/2018/01/04/Data-Oriented-Design-and-efficiency/) | A post by Lucian Radu Teodorescu where he benchmarks OOP vs DOD for a particle generation program | 2018 |
| [Simplest ECS](https://blog.therocode.net/2018/08/simplest-entity-component-system) | A very simple and easy to understand ECS implementation by Therocode | 2018 |
| [Data oriented design book](http://www.dataorienteddesign.com/dodbook/) | A book by Richard Fabian aboute DOD practices | 2018 |
| [Nomad game engine](https://medium.com/@savas/nomad-game-engine-part-2-ecs-9132829188e5) | Articles about a game engine written in ECS | 2016 |
| [Sane usage of component and entity systems](https://www.randygaul.net/2014/06/10/sane-usage-of-components-and-entity-systems/) | A blog post by Randy Gaul | 2014 |
| [Load Balancing Between Threads](https://software.intel.com/en-us/articles/load-balancing-between-threads) | Intel article by Andrew Binstock about efficient parallel computing | 2011
| [Data oriented design](http://gamesfromwithin.com/data-oriented-design) | Explanation of Data oriented design vs OOP by Noel Llopis | 2009 |

 
### Videos

| Link | Description | Date |
| --- | --- | --- |
| [ECS Overview](https://www.youtube.com/watch?v=2rW7ALyHaas) | An explanation by the Board To Bits Games channel of ECS in Unity Game Engine | 2019 |
| [Is there more to Game Architecture than ECS ?](https://www.youtube.com/watch?v=JxI3Eu5DPwE) | A talk by Bob Nystrom at Roguelike Celebration | 2018 |
| [CppCon - OOP Is Dead, Long Live Data-oriented Design](https://www.youtube.com/watch?v=yy8jQgmhbAU) | A talk by Stoyan Nikolov on its work at Coherent Labs about re-coding WebKit in data-oriented manner | 2018 |
| [Intro to ECS](https://www.youtube.com/watch?v=WLfhUKp2gag) | An official Unity video about their ECS implementation | 2018 |
| [GDC - Overwatch Gameplay Architecture and Netcode](https://www.youtube.com/watch?v=W3aieHjyNvw) | A talk by Timothy Ford on ECS and their concept of Singleton Components | 2017 |
| [GDC - Destiny's Multithreaded Rendering Architecture](https://www.youtube.com/watch?v=0nTDFLMLX9k) | A talk by Natalya Tatarchuk about parrallel data driven-architecture for rendering in their game Destiny. Slides are available [here](http://advances.realtimerendering.com/destiny/gdc_2015/Tatarchuk_GDC_2015__Destiny_Renderer_web.pdf) | 2015 |
| [CppCon - Data oriented design and C++](https://www.youtube.com/watch?v=rX0ItVEVjHc) | A talk by Mike Aton (previously at Insomniac Games and now Working at Unity), about Data oriented design | 2014 |
| [Speeding up Game Logic in unreal Engine - Unreal Fest 2020](https://www.youtube.com/watch?v=QlKXPBFh5BM&list=PLZlv_N0_O1gZ4LHrX8orVNn3LFq_O3-Lm&index=13) | A talk by Daniel Rätzer on their implementation of ECS in Unreal engine for the game Conan Exile | 2020 |

### Slides

| Link | Description | Date |
| --- | --- | --- |
| [GDC - Content fueled gameplay programming in Frostpunk](https://www.gdcvault.com/play/1026275/Content-Fueled-Gameplay-Programming-in) | Slides of the talk by Aleksander Kauch on ECS | 2019 |
| [Unity Academy - Data oriented design](http://aras-p.info/texts/files/2018Academy%20-%20ECS-DoD.pdf) | Slides of the talk by Aras Pranckevicius | 2018 |
| [TGC - Pitfalls of OOP revisited](https://docs.google.com/presentation/d/1ST3mZgxmxqlpCFkdDhtgw116MQdCr2Fax2yjd8Az6zM/edit?usp=sharing) | A talk by Tony Albrecht, now at Riot Games and previously constultant at Sony, about how to measure performance of your game. It is a new take on [this talk](http://harmful.cat-v.org/software/OO_programming/_pdf/Pitfalls_of_Object_Oriented_Programming_GCAP_09.pdf). | 2017
| [DICE Coders Day - Intro to Data Oriented Design](https://www.dice.se/news/introduction-data-oriented-design/) | Slides of the talk of Dice | 2010 |
| [BitSquid - Practical examples in Data Oriented Design](http://bitsquid.blogspot.com/2010/05/practical-examples-in-data-oriented.html) | Slides of the talk by Niklas Frykholm at the Game Developer Forum in Stockholm | 2010 |

### Papers

| Link | Description | Date |
| --- | --- | --- |
| [Analysis of ECS](https://www.researchgate.net/publication/305730566_Analysis_of_entity_encoding_techniques_design_and_implementation_of_a_multithreaded_compile-time_Entity-Component-System_C14_library) | A thesis by Vittorio Romeo on ECS implementation| 2016 |
