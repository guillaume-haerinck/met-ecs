#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "../config/config.hpp"

namespace met {
    /**
     * @brief Abstract class used to store an array of ComponentCollections
     * @note The data structure is a modified sparse set.
     */
    class IComponentCollection {
    public:
        IComponentCollection() {
            m_entityToComponentIndex.resize(MIN_ENTITIES);
        };
        virtual ~IComponentCollection() {};

        /**
         * @brief Check if an entity has the component
         */
        bool has(entity id) const {
            assert(id != null_entity && "Null entity cannot have components");

            if (id > m_entityToComponentIndex.size()) {
                return false;
            } else if (m_entityToComponentIndex.at(id) != null_component) {
                return true;
            } else {
                return false;
            }
        }

        virtual void remove(entity id) = 0;
        virtual size_t size() const = 0;

    protected:
        std::vector<unsigned int> m_entityToComponentIndex;
    };

    /**
     * @brief Store an array of component of the same type
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(entity id, T& component) {
            m_components.reserve(MIN_ENTITIES);
            m_componentIndexToEntity.reserve(MIN_ENTITIES);

            // Unused, allow entity id to match array id
            m_components.push_back(component);
            m_componentIndexToEntity.push_back(null_component);

            // Add first entity
            insert(id, component);
        };

        virtual ~ComponentCollection() {};

        /**
         * @brief Insert a component to an entity
         */
        void insert(entity id, T& component) {
            assert(id != null_entity && "Null entity cannot have components");

            if (m_entityToComponentIndex.size() <= id) {
                m_entityToComponentIndex.resize(id + 10);
                std::fill(m_entityToComponentIndex.begin() + id, m_entityToComponentIndex.end(), null_component);
            }

            // Add a new component at the end of the packed array
            m_components.push_back(component);
            m_componentIndexToEntity.push_back(id);
            m_entityToComponentIndex.at(id) = static_cast<unsigned int>(m_components.size()) - 1;
        }

        /**
         * @brief Removes the component from the given entity
         */
        void remove(entity id) override {
            assert(has(id) && "The entity does not have this component");

            // Fill deleted entity data position with last valid data
            const unsigned int lastComponentIndex = static_cast<unsigned int>(m_components.size() - 1);
            at(id) = at(lastComponentIndex);

            // Gives the new index to the entity which had the last data
            // FIXME
            // m_entityToComponentIndex.at(m_componentIndexToEntity.at(lastComponentIndex)) = m_entityToComponentIndex.at(id);

            // Removes unsused data
            m_entityToComponentIndex.at(id) = null_component;
            m_components.pop_back();
            m_componentIndexToEntity.pop_back();
        }

        /**
         * @brief Get the component of an entity
         */
        T& at(entity id) {
            assert(id != null_entity && "Null entity cannot have components");
            return m_components.at(m_entityToComponentIndex.at(id));
        }

        /**
         * @brief Get the number of entities which uses this component type
         */
        size_t size() const override {
            return m_components.size() - 1;
        }

        // TODO handle sorting

    private:
        std::vector<unsigned int> m_componentIndexToEntity;
        std::vector<T> m_components;
    };
}
