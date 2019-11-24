#pragma once

#include <array>
#include <vector>
#include <cassert>

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
            m_components.reserve(10);
            m_componentToIndices.reserve(10);

            // Unused, index 0 is for false
            m_components.push_back(component);
            m_componentToIndices.push_back(0);

            // Add first entity
            insert(id, component);
        };

        virtual ~ComponentCollection() {};

        /**
         * @brief Insert a component to an entity
         */
        void insert(entity id, T& component) {
            assert(id != null_entity && "Null entity cannot have components");
            m_components.push_back(component);
            m_componentToIndices.push_back(id);
            m_componentIndices.at(id) = static_cast<unsigned int>(m_components.size()) - 1;
        }

        /**
         * @brief Removes the component from the given entity
         */
        void remove(entity id) {
            // Fill deleted entity data position with last data
            const auto lastIndex = m_components.size() - 1;
            at(id) = at(lastIndex);

            // Gives the new index to the entity which had the last data
            m_componentIndices.at(m_componentToIndices.at(lastIndex)) = m_componentIndices.at(id);

            // Removes unsused data
            m_componentIndices.at(id) = 0;
            m_components.pop_back();
            m_componentToIndices.pop_back();
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

        // TODO handle sorting

    private:
        std::vector<unsigned int> m_componentToIndices; // Gives the entity which has the component at the given index of m_components
        std::vector<T> m_components;
    };
}
