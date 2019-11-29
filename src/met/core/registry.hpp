#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <typeinfo>
#include <cassert>

#include "../config/config.hpp"
#include "component-collection.hpp"
#include "view.hpp"

namespace met {
    /**
     * @brief The global data handler and entry point of this library
     */
    class Registry {
    public:
        Registry() : m_lastMaxEntityId(0), m_tempMatchCount(0) {
            m_componentCollections.reserve(MIN_COMPONENT_TYPES);
        }

        ~Registry() {
            for (IComponentCollection* componentCollection : m_componentCollections) {
                delete componentCollection;
            }
        }

        /**
         * @brief Create a new entity
         */
        entity create() {
            if (m_unusedEntityIndices.size() > 0) {
                unsigned int id = m_unusedEntityIndices.front();
                m_unusedEntityIndices.pop_front();
                return id;
            } else {
                assert(m_lastMaxEntityId < MAX_ENTITIES && "You reached MAX_ENTITIES");
                return ++m_lastMaxEntityId;
            }
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
                ComponentCollection<T>* collection = static_cast<ComponentCollection<T>*>(m_componentCollections.at(index));
                collection->insert(id, component);
            } else {
                ComponentCollection<T>* collection = new ComponentCollection<T>(id, component);
                m_componentCollections.push_back(collection);
                m_componentCollectionIndices[type.name()] = static_cast<unsigned int>(m_componentCollections.size() - 1);
            }
        }

        /**
         * @brief Says wether the entity has the given component or not
         */
        template<typename Comp>
        bool has(entity id) const {
            const ComponentCollection<Comp>* collection = getCollection<Comp>();
            return collection->has(id);
        }

        /**
         * @brief Says if the entity exist
         */
        bool valid(entity id) const {
            if (id > m_lastMaxEntityId) {
                return false;
            }

            for (entity unusedId : m_unusedEntityIndices) {
                if (id == unusedId) {
                    return false;
                }
            }

            return true;
        }

        /**
         * @brief Remove the given components from the given entity
         */
        template<typename Comp>
        void remove(entity id) {
            ComponentCollection<Comp>* collection = getCollection<Comp>();
            collection->remove(id);
        }

        /**
         * @brief Destroy an entity and all of its components
         */
        void destroy(entity id) {
            m_unusedEntityIndices.push_back(id);
            for (IComponentCollection* collection : m_componentCollections) {
                if (collection->has(id)) {
                    collection->removeWithGap(id);
                }
            }
        }

        /**
         * @brief Removes all of the components from the given entity
         */
        void reset(entity id) {
            for (IComponentCollection* collection : m_componentCollections) {
                if (collection->has(id)) {
                    collection->removeWithGap(id);
                }
            }
        }

        /**
         * @brief Destroy all of the components and all of the entities
         */
        void reset() {
            m_unusedEntityIndices.clear();
            m_componentCollectionIndices.clear();
            for (IComponentCollection* componentCollection : m_componentCollections) {
                delete componentCollection;
            }
            m_lastMaxEntityId = 0;
        }

        /**
         * @brief Get the entities which holds at least each one of the asked components
         */
        template<typename Comp, typename... Comps>
        View<Comp, Comps...> view() {
            fillMatchingEntities<Comp>();
            (removeUnmatchingEntities<Comps>(), ...);
            View<Comp, Comps...> view(m_tempMatchCount, m_tempMatchingEntities.data(), getCollection<Comp>(), getCollection<Comps>()...);
            return view;
        }

        /**
         * @brief Get the asked component for the given entity
         */
        template<typename Comp>
        Comp& get(const entity id) {
            ComponentCollection<Comp>* collection = getCollection<Comp>();
            return collection->at(id);
        }

    private:
        /**
         * @brief fill the m_tempMatchingEntities array with the entities which have the given component
         */
        template<typename Comp>
        void fillMatchingEntities() {
            const ComponentCollection<Comp>* collection = getCollection<Comp>();
            m_tempMatchCount = 0;

            for (entity id = 1; id <= collection->size(); ++id) {
                if (collection->has(id)) {
                    m_tempMatchingEntities.at(m_tempMatchCount) = id;
                    m_tempMatchCount++;
                }
            }
        }

        /**
         * @brief Remove the entities which does not have the given component from m_tempMatchingEntities array
         */
        template<typename Comp>
        void removeUnmatchingEntities() {
            const ComponentCollection<Comp>* collection = getCollection<Comp>();

            for (size_t i = 0; i < m_tempMatchCount; ++i) {
                const entity id = m_tempMatchingEntities.at(i);

                if (!collection->has(id)) {
                    m_tempMatchingEntities.at(i) = m_tempMatchingEntities.at(i + 1);
                    m_tempMatchCount--;
                    i--;
                }
            }
        }

        /**
         * @brief Get the collection pointer of the asked component
         */
        template<typename Comp>
        ComponentCollection<Comp>* getCollection() {
            return static_cast<ComponentCollection<Comp>*>(m_componentCollections.at(getCollectionIndex<Comp>()));
        }

        /**
         * @brief Get the index of the component collection inside of the vector of IComponentCollection*
         */
        template<typename Comp>
        unsigned int getCollectionIndex() {
            const std::type_info& type = typeid(Comp);
            if (m_componentCollectionIndices.find(type.name()) != m_componentCollectionIndices.end()) {
                return m_componentCollectionIndices[type.name()];
            }
            assert(false && "The component type does not exist in the registry");
            return 0;
        }

    private:
        entity m_lastMaxEntityId;
        std::deque<entity> m_unusedEntityIndices;
        std::vector<IComponentCollection*> m_componentCollections;
        std::unordered_map<std::string, unsigned int> m_componentCollectionIndices;

        // TODO Allow for multi-threading with std::async when matching entities with components to create views
        // Have multiple independant compile-time arrays. 6 might be enough because 6 processor cores to work with is already a lot
        std::array<entity, MAX_ENTITIES> m_tempMatchingEntities;
        unsigned int m_tempMatchCount;
    };
}
