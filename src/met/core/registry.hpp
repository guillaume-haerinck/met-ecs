#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>

#include "../met-config.hpp"
#include "component-collection.hpp"

namespace met {
    /**
     * @brief The global data handler and entry point of this library
     */
    class Registry {
    public:
        Registry() : m_entityCount(0) {}
        ~Registry() {
            for (const auto& componentCollection : m_componentCollections) {
                delete componentCollection;
            }
        }

        /**
         * @brief Create a new entity
         */
        entity create() {
            return ++m_entityCount;
        }

        /**
         * @brief Assign the given components to the given entity
         */
        template<typename T>
        void assign(entity id, T component) {
            const std::type_info& type = typeid(T);

            if (m_componentCollectionIndices.find(type.name()) != m_componentCollectionIndices.end()) {
                unsigned int index = m_componentCollectionIndices[type.name()];
                ComponentCollection<T>* collection = reinterpret_cast<ComponentCollection<T>*>(m_componentCollections.at(index));
                collection->components.at(id) = component;
            } else {
                ComponentCollection<T>* collection = new ComponentCollection<T>(component);
                m_componentCollections.push_back(collection);
                m_componentCollectionIndices[type.name()] = static_cast<unsigned int>(m_componentCollections.size() - 1);
            }
        }

        /**
         * @brief Get the entities which holds at least each one of the given components
         */
        template<typename T>
        void view() {
            
        }

        /**
         * @brief Get the given components for the given entity
         */
        // TODO return reference to allow modification
        template<typename T>
        T get(entity id) {
            const std::type_info& type = typeid(T);
            if (m_componentCollectionIndices.find(type.name()) != m_componentCollectionIndices.end()) {
                unsigned int index = m_componentCollectionIndices[type.name()];
                return reinterpret_cast<ComponentCollection<T>*>(m_componentCollections.at(index))->components.at(id);
            }
            // TODO throw an exception ?
            assert(false && "The entity does not have the asked component");
        }

    private:
        uint32_t m_entityCount;
        std::vector<IComponentCollection*> m_componentCollections;
        std::vector<entity> m_deletedEntites;
        std::unordered_map<std::string, unsigned int> m_componentCollectionIndices;
    };
}
