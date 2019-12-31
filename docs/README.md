# Met ECS post-mortem

## Introdution

When I started to work on **Met ECS**, I had [one project](https://github.com/guillaume-haerinck/imac-tower-defense) dealing with Entity Component Systems on my back. At the time I used the [ENTT](https://github.com/skypjack/entt) library by Skypjack to structure my application. As the most popular Open-Source library currently available, it seemed like a great starting point to structure my library and test its performance afterwards.

Through these lines, I will do my best to explain what is ECS, what it stands for and how I implemented this paradigm following the ENTT library API. The code is not the most performant there is, but it is short, and tries to be easy to understand.

If you spot a mistake, or have any comment, you can open an [issue](https://github.com/guillaume-haerinck/met-ecs/issues) on this repository.

## I - Why ECS ?
> An explanation of data-oriented practices, their history, advantages and uses in the real world

There are multiple good reason to use an ECS architecture if you are building a game, or even an application. As any other paradigm, it has it own drawbacks and is **not suited for every need**. To know if it's made for you, you need to *"know your data"*. Where, how, by whom and at which frequency it will be modified. 

Many aspects will be detailled below, and you will find a summary table at the end of this part.

### Performance

#### Data locality

Cache misses. You might have never heard of this term but it is nowadays one of the barebones of your application execution speed. While the calcuation speed of our processors kept getting faster and faster - following [Moore's law](https://en.wikipedia.org/wiki/Moore%27s_law) - the data access time didn't keep up.

> Sure, we can _process_ data faster than ever, but we can’t _get_ that data faster. Bob Nystrom

<figure align="center">
  <img width="700" src="https://github.com/guillaume-haerinck/met-ecs/blob/master/docs/post-mortem-img/data-locality-chart.png" alt="Data locality chart" />
  <figcaption>Data locality chart from <a href="https://gameprogrammingpatterns.com/data-locality.html">Game Programming Patterns</a></figcaption>
</figure>

When the CPU has to read data from the RAM, it takes a lot of time. To prevent this from happening a lot, CPU vendors created **small local memory caches** inside of CPUs. Given their sizes and positions, they are way faster than RAM access. We name these caches by their levels, L1, L2, L3, etc. The smaller is the number, the smaller is the size but faster is the read access.

#### Data oriented design

> When there is one, there is many. XXX

- Performances gain with less cache miss and think about the bulk
- History of data oriented design and the emergence of ecs

### Maintenability
- Maintenability with separation of data and logic
- Inheritence problems with oop, or performance

### Portability
- Reuse systems accross projets
- Greets the fact that from one app to another easier to understand the structure with ECS

### Adoption in the industry
- Usage in the industry and the future
- Warn about getting used to it and problems to reason about static data and unity

## II - The theory
> Discover what are entities, component and systems. Understand how they interact together 

- Explain what are components entities and systems
- Give tangible examples of interaction for a specific behavior
- Talk about maintenability and easy to add features
- Difficult to start with and sometimes to prototype as difficult to reason for one entity

## III - Interacting with ECS
> What are the possible APIs available to use ECS and what are their advantages

- Registering components vs declaring them in advance
- Registrering systems or not
- Using a central registry
- Examples in the industry
- The API of met ecs

## IV - Data structure
> How the data of ECS is stored and accessed

- Pure array
- Sparse set
- Archetypes
- The sparse set of met ecs
- The view creation of met ecs
- The templates tips and tricks

## V - The case of static data
> What are the ways to handle the global states of your app like inputs or graphic objects

- Singleton components (outside or inside ECS)
- Event system
- Parse files
- Handle things outside of ECS is common

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
| [ECS - Where are my entities ?](https://skypjack.github.io/2019-03-07-ecs-baf-part-2/) | An article by Skypjack on Sparse set vs Archetypes for component storage | 2019 |
| [Programming lessons learned from making my first game](https://github.com/adnzzzzZ/blog/issues/31) | Post mortem by adnzzzzZ about why he doesn't recommand ECS for small projects | 2018 |
| [Simplest ECS](https://blog.therocode.net/2018/08/simplest-entity-component-system) | A very simple and easy to understand ECS implementation by Therocode | 2018 |
| [Nomad game engine](https://medium.com/@savas/nomad-game-engine-part-2-ecs-9132829188e5) | Articles about a game engine written in ECS | 2016 |
| [Sane usage of component and entity systems](https://www.randygaul.net/2014/06/10/sane-usage-of-components-and-entity-systems/) | A blog post by Randy Gaul | 2014 |
| [Data oriented design](http://gamesfromwithin.com/data-oriented-design) | Explanation of Data oriented design vs OOP by Noel Llopis | 2009 |

 
### Videos

| Link | Description | Date |
| --- | --- | --- |
| [ECS Overview](https://www.youtube.com/watch?v=2rW7ALyHaas) | An explanation by the Board To Bits Games channel of ECS in Unity Game Engine | 2019 |
| [Is there more to Game Architecture than ECS ?](https://www.youtube.com/watch?v=JxI3Eu5DPwE) | A talk by Bob Nystrom at Roguelike Celebration | 2018 |
| [CppCon - OOP Is Dead, Long Live Data-oriented Design](https://www.youtube.com/watch?v=yy8jQgmhbAU) | A talk by Stoyan Nikolov on its work at Coherent Labs about re-coding WebKit in data-oriented manner | 2018 |
| [Intro to ECS](https://www.youtube.com/watch?v=WLfhUKp2gag) | An official Unity video about their ECS implementation | 2018 |
| [GDC - Overwatch Gameplay Architecture and Netcode](https://www.youtube.com/watch?v=W3aieHjyNvw) | A talk by Timothy Ford on ECS and their concept of Singleton Components | 2017 |
| [GDC - Destiny's Multithreaded Rendering Architecture](https://www.youtube.com/watch?v=0nTDFLMLX9k) | A talk by Natalya Tatarchuk about data driven-architecture for rendering in their game Destiny | 2015 |
| [CppCon - Data oriented design and C++](https://www.youtube.com/watch?v=rX0ItVEVjHc) | A talk by Mike Aton (previously at Insomniac Games and now Working at Unity), about Data oriented design | 2014 |

### Slides

| Link | Description | Date |
| --- | --- | --- |
| [GDC - Content fueled gameplay programming in Frostpunk](https://www.gdcvault.com/play/1026275/Content-Fueled-Gameplay-Programming-in) | Slides of the talk by Aleksander Kauch on ECS | 2019 |
| [Unity Academy - Data oriented design](http://aras-p.info/texts/files/2018Academy%20-%20ECS-DoD.pdf) | Slides of the talk bp Aras Pranckevicius | 2018 |
| [DICE Coders Day - Intro to Data Oriented Design](https://www.dice.se/news/introduction-data-oriented-design/) | Slides of the talk of Dice | 2010 |
| [BitSquid - Practical examples in Data Oriented Design](http://bitsquid.blogspot.com/2010/05/practical-examples-in-data-oriented.html) | Slides of the talk by Niklas Frykholm at the Game Developer Forum in Stockholm | 2010 |

### Papers

| Link | Description | Date |
| --- | --- | --- |
| [Analysis of ECS](https://www.researchgate.net/publication/305730566_Analysis_of_entity_encoding_techniques_design_and_implementation_of_a_multithreaded_compile-time_Entity-Component-System_C14_library) | A thesis by Vittorio Romeo on ECS implementation| 2016 |
