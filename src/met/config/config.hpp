#pragma once

#ifndef MAX_ENTITIES
    #define MAX_ENTITIES 32
#endif

#ifndef MIN_COMPONENT_TYPES
    #define MIN_COMPONENT_TYPES 10
#endif

namespace met {
    using entity = unsigned int;
    const static entity null_entity = 0;
}
