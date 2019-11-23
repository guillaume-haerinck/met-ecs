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
        Registry() : m_entityCount(0), m_tempMatchCount(0) {
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
			assert(m_entityCount < MAX_ENTITIES && "You reached MAX_ENTITIES");
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
		 * @brief Get the raw array for the asked component
		 */
		template<typename Comp>
		Comp* getRawArray() {
			return getCollection<Comp>()->components.data();
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
		}

    private:
        entity m_entityCount;
        std::vector<entity> m_unusedIndices;
        std::vector<IComponentCollection*> m_componentCollections;
        std::unordered_map<std::string, unsigned int> m_componentCollectionIndices;

		// TODO Find a way to be multi-thread friendly while matching components and entities
		// Is there a better solution than creating an array each time and copying it to the view ?
		std::array<entity, MAX_ENTITIES> m_tempMatchingEntities;
		unsigned int m_tempMatchCount;
    };
}
