# Met ECS post-mortem

## Introdution

## I - Why ECS ?
> An explanation of data-oriented practices, their history, advantages and uses in the real world

## II - The theory
> Discover what are entities, component and systems. Understand how they interact together 

## III - Interacting with ECS
> What are the possible APIs available to use ECS and what are their advantages

## IV - Data structure
> How the data of ECS is stored and accessed

## V - The case of static data
> What are the ways to handle the global states of your app like inputs or graphic objects

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

