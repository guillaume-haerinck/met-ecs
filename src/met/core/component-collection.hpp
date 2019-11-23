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
			m_componentIndices.fill(0);
        };
        virtual ~IComponentCollection() {};

    protected:
        std::array<unsigned int, MAX_ENTITIES> m_componentIndices; // 0 if the entity at this index does not have this component
    };

    /**
     * @brief Store an array of component of the same type
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(entity id, T& component) {
			assert(id != null_entity && "Null entity cannot have components");
			m_components.reserve(10);
			m_components.push_back(component); // Unused, index 0 is for false
			m_components.push_back(component);
			m_componentIndices.at(id) = m_components.size() - 1;
		};

        virtual ~ComponentCollection() {};

		/**
		 * @brief Insert a component to an entity
		 */
		void insert(entity id, T& component) {
			assert(id != null_entity && "Null entity cannot have components");
			m_components.push_back(component);
			m_componentIndices.at(id) = m_components.size() - 1;
		}

		/**
		 * @brief Get the component of an entity
		 */
		T& at(entity id) {
			assert(id != null_entity && "Null entity cannot have components");
			return m_components.at(m_componentIndices.at(id));
		}

		/**
		 * @brief Check if an entity has the component
		 */
		bool has(entity id) const {
			assert(id != null_entity && "Null entity cannot have components");
			if (m_componentIndices.at(id) != 0) {
				return true;
			} else {
				return false;
			}
		}

		/**
		 * @brief Get the number of entities which uses this component type
		 */
		size_t size() const {
			return m_components.size() - 1;
		}

    private:
        std::vector<T> m_components;
    };
}
