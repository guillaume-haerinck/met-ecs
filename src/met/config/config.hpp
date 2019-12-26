#pragma once

#ifndef MIN_ENTITIES
    #define MIN_ENTITIES 1
#endif

#ifndef MIN_COMPONENT_TYPES
    #define MIN_COMPONENT_TYPES 1
#endif

namespace met {
    using entity = unsigned int;
    const static entity null_entity = 0;
    const static unsigned int null_component = 0;
}
