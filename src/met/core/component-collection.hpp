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
            m_sparse.resize(MIN_ENTITIES);
        };
        virtual ~IComponentCollection() {};

        /**
         * @brief Check if an entity has the component
         */
        bool has(entity id) const {
            assert(id != null && "Null entity cannot have components");

            if (id > m_sparse.size() - 1) {
                return false;
            } else if (m_sparse.at(id) != null) {
                return true;
            } else {
                return false;
            }
        }

        size_t capacity() const {
            return m_sparse.size();
        }

        virtual void remove(entity id) = 0;
        virtual size_t size() const = 0;

    protected:
        std::vector<unsigned int> m_sparse; // Entity to component index
    };

    /**
     * @brief Store an array of component of the same type
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(entity id, T& component) {
            m_dense.reserve(MIN_ENTITIES);
            m_components.reserve(MIN_ENTITIES);

            // Unused, allow entity id to match array id
            m_components.push_back(component);
            m_dense.push_back(null);
            m_sparse.at(0) = null;

            // Add first entity
            insert(id, component);
        };

        virtual ~ComponentCollection() {};

        /**
         * @brief Insert a component to an entity
         */
        void insert(entity id, T& component) {
            assert(id != null && "Null entity cannot have components");

            if (m_sparse.size() <= id) {
                m_sparse.resize(id + 10);
                std::fill(m_sparse.begin() + id, m_sparse.end(), null);
            }

            if (m_sparse.at(id) != null) {
                // Overwrite existing component
                at(id) = component;
            } else {
                // Add a new component at the end of the packed array
                m_components.push_back(component);
                m_dense.push_back(id);
                m_sparse.at(id) = static_cast<unsigned int>(m_dense.size()) - 1;
            } 
        }

        /**
         * @brief Removes the component from the given entity
         */
        void remove(entity id) override {
            assert(has(id) && "The entity does not have this component");
      
            // Find owner of last component
            const unsigned int lastComponentOwner = m_dense.at(m_components.size() - 1);

            // Move last component to removed component
            m_components.at(m_sparse.at(id)) = m_components.at(m_components.size() - 1);
            m_dense.at(m_sparse.at(id)) = lastComponentOwner;
            m_sparse.at(lastComponentOwner) = m_sparse.at(id);
            
            // Remove unsused data
            m_sparse.at(id) = null;
            m_dense.pop_back();
            m_components.pop_back();
        }

        /**
         * @brief Get the component of an entity
         */
        T& at(entity id) {
            assert(has(id) && "The entity does not have this component");
            return m_components.at(m_sparse.at(id));
        }

        /**
         * @brief Get the number of entities which uses this component type
         */
        size_t size() const override {
            return m_components.size() - 1;
        }

        // TODO handle sorting

    private:
        std::vector<unsigned int> m_dense; // Component index to entity
        std::vector<T> m_components;
    };
}
