#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <typeinfo>

#ifndef MAX_ENTITIES
    #define MAX_ENTITIES 32
#endif

namespace met {
    using entity = uint32_t;

    /**
     * @brief Abstract class used to store an array of ComponentCollections
     */
    class IComponentCollection {
        public:
            IComponentCollection() {};
            virtual ~IComponentCollection() {};
    };

    /**
     * @brief Store an array of component of the same type
     * 
     * @note The index of the array corresponds to entity id. It works like a LUT
     */
    template<class T>
    class ComponentCollection : public IComponentCollection {
    public:
        ComponentCollection(T firstComponent) {
            components.at(1) = firstComponent;
        };
        virtual ~ComponentCollection() {};

    public:
        std::array<T, MAX_ENTITIES> components;
    };

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
        // TODO check that it works
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
