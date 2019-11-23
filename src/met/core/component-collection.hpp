#pragma once

#include <array>

#include "../config/config.hpp"

namespace met {
    /**
     * @brief Abstract class used to store an array of ComponentCollections
     */
    class IComponentCollection {
        public:
            IComponentCollection() {
                hasComponent.fill(false);
            };
            virtual ~IComponentCollection() {};

        public:
            std::array<bool, MAX_ENTITIES> hasComponent; // Says wether the entity at this index use this component
    };

    /**
     * @brief Store an array of component of the same type
     * 
     * @note The index of the array corresponds to entity id. It works like a LUT
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection() {};
        virtual ~ComponentCollection() {};

    public:
		// TODO consider using sparse-set to prevent storing components not used by entities
		// It works with two array, one Dense with components, and the other, sparse, linking entity id to their component indices
		// The Dense can match the size of the actually used components, while the other can match the max number of entities. 
		// Value 0 in sparse being not using the component
        std::array<T, MAX_ENTITIES> components;
    };
}
