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
				componentIndices.fill(0);
            };
            virtual ~IComponentCollection() {};

        public:
            std::array<entity, MAX_ENTITIES> componentIndices; // 0 if the entity at this index does not have this component
    };

    /**
     * @brief Store an array of component of the same type
     * 
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection() {
			components.reserve(10);
		};
        virtual ~ComponentCollection() {};

    public:
        std::vector<T> components;
    };
}
