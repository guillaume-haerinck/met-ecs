#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>

#include "../config/config.hpp"
#include "component-collection.hpp"
#include "view.hpp"

namespace met {
    /**
     * @brief The global data handler and entry point of this library
     */
    class Registry {
    public:
        Registry() : m_entityCount(0) {
            m_componentCollections.reserve(MIN_COMPONENT_TYPES);
        }

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
         * @brief Destroy an entity and all of its components
         */
        void destroy(entity id) {

        }

        /**
         * @brief Assign the given components to the given entity
         */
        template<typename T>
        void assign(entity id, T component) {
            const std::type_info& type = typeid(T);
            const bool collectionExist = m_componentCollectionIndices.find(type.name()) != m_componentCollectionIndices.end();

            if (collectionExist) {
                unsigned int index = m_componentCollectionIndices[type.name()];
                ComponentCollection<T>* collection = reinterpret_cast<ComponentCollection<T>*>(m_componentCollections.at(index));
                collection->components.at(id) = component;
                collection->hasComponent.at(id) = true;
            } else {
                ComponentCollection<T>* collection = new ComponentCollection<T>();
                collection->components.at(id) = component;
                collection->hasComponent.at(id) = true;

                m_componentCollections.push_back(collection);
                m_componentCollectionIndices[type.name()] = static_cast<unsigned int>(m_componentCollections.size() - 1);
            }
        }

        /**
         * @brief Says wether the entity has the given component or not
         */
        template<typename T>
        bool has(entity id) {
            return true;
        }

        /**
         * @brief Says if the entity exist
         */
        bool valid(entity id) {

        }

        /**
         * @brief Remove the given components from the given entity
         */
        template<typename T>
        void remove(entity id) {

        }

        /**
         * @brief Removes all of the components from the given entity
         */
        template<typename T>
        void reset(entity id) {

        }

        /**
         * @brief Destroy all of the components and all of the entities
         */
        void reset() {

        }

        /**
         * @brief Get the entities which holds at least each one of the asked components
         */
        template<typename T>
        View view(entity id) {
            View view;
            return view;
        }

        /**
         * @brief Get the asked component for the given entity
         */
        template<typename T>
        T& get(entity id) {
            const std::type_info& type = typeid(T);
            if (m_componentCollectionIndices.find(type.name()) != m_componentCollectionIndices.end()) {
                unsigned int index = m_componentCollectionIndices[type.name()];
                return reinterpret_cast<ComponentCollection<T>*>(m_componentCollections.at(index))->components.at(id);
            }
            assert(false && "The entity does not have the required component(s)");
        }

    private:
        entity m_entityCount;
        std::vector<entity> m_unusedIndices;
        std::vector<IComponentCollection*> m_componentCollections;
        std::unordered_map<std::string, unsigned int> m_componentCollectionIndices; 
    };
}
