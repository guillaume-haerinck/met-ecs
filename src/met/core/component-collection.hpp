#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "../config/config.hpp"

namespace met {
    /**
     * @brief Abstract class used to store an array of ComponentCollections
     * @note The data structure is a kind of sparse set.
     *       m_componentIndices being sparse array, and m_components being dense array
     */
    class IComponentCollection {
    public:
        IComponentCollection() {
            m_componentIndices.resize(MIN_ENTITIES);
        };
        virtual ~IComponentCollection() {};

        /**
         * @brief Check if an entity has the component
         */
        bool has(entity id) const {
            assert(id != null_entity && "Null entity cannot have components");

            if (id > m_componentIndices.size()) {
                return false;
            } else if (m_componentIndices.at(id) != null_component) {
                return true;
            } else {
                return false;
            }
        }

        virtual void remove(entity id) = 0;
        virtual size_t size() const = 0;

    protected:
        std::vector<unsigned int> m_componentIndices;
    };

    /**
     * @brief Store an array of component of the same type
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(entity id, T& component) {
            m_components.reserve(MIN_ENTITIES);
            m_componentToIndices.reserve(MIN_ENTITIES);

            // Unused, allow entity id to match array id
            m_components.push_back(component);
            m_componentToIndices.push_back(null_component);

            // Add first entity
            insert(id, component);
        };

        virtual ~ComponentCollection() {};

        /**
         * @brief Insert a component to an entity
         */
        void insert(entity id, T& component) {
            assert(id != null_entity && "Null entity cannot have components");

            if (m_componentIndices.size() <= id) {
                m_componentIndices.resize(id + 10);
                std::fill(m_componentIndices.begin() + id, m_componentIndices.end(), null_component);
            }

            // Add a new component at the end of the packed array
            m_components.push_back(component);
            m_componentToIndices.push_back(id);
            m_componentIndices.at(id) = static_cast<unsigned int>(m_components.size()) - 1;
        }

        /**
         * @brief Removes the component from the given entity
         * @note The packed array of components stays packed, no holes in it
         */
        void remove(entity id) override {
            assert(has(id) && "The entity does not have this component");

            // Fill deleted entity data position with last data
            const unsigned int lastIndex = static_cast<unsigned int>(m_components.size() - 1);
            at(id) = at(lastIndex);

            // Gives the new index to the entity which had the last data
            m_componentIndices.at(m_componentToIndices.at(lastIndex)) = m_componentIndices.at(id);

            // Removes unsused data
            m_componentIndices.at(id) = null_component;
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
         * @brief Get the number of entities which uses this component type
         */
        size_t size() const override {
            return m_components.size() - 1;
        }

        // TODO handle sorting

    private:
        std::vector<unsigned int> m_componentToIndices; // Gives the entity which has the component at the given index of m_components
        std::vector<T> m_components;
    };
}
