#pragma once

#include <array>

#include "../met-config.hpp"

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
            std::array<bool, MAX_ENTITIES> hasComponent; // Does the entity at this index use this component ?
    };

    /**
     * @brief Store an array of component of the same type
     * 
     * @note The index of the array corresponds to entity id. It works like a LUT
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(T firstComponent) {
            components.at(1) = firstComponent;
        };
        virtual ~ComponentCollection() {};

    public:
        std::array<T, MAX_ENTITIES> components;
    };
}
