#pragma once

#include <array>

#include "../config/config.hpp"

namespace met {
    /**
     * @brief Abstract class used to store an array of ComponentCollections
	 * @note The data structure is a kind of sparse set. 
	 *       ComponentIndicies being sparse array, and components of concrete classes being dense array
     */
    class IComponentCollection {
    public:
        IComponentCollection() {
			componentIndices.fill(0);
        };
        virtual ~IComponentCollection() {};

    public:
        std::array<unsigned int, MAX_ENTITIES> componentIndices; // 0 if the entity at this index does not have this component
    };

    /**
     * @brief Store an array of component of the same type
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(entity id, T& component) {
			components.reserve(10);
			components.push_back(component); // Unused, index 0 is for false
			components.push_back(component);
			componentIndices.at(id) = components.size() - 1;
		};

        virtual ~ComponentCollection() {};

		void insert(entity id, T& component) {
			components.push_back(component);
			componentIndices.at(id) = components.size() - 1;
		}

		T& at(entity id) {
			return components.at(componentIndices.at(id));
		}

    public:
        std::vector<T> components;
    };
}
