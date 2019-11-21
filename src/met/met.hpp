#pragma once

#include <array>
#include <vector>
#include <unordered_map>

#define MAX_ENTITIES 32

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
     * @brief 
     */
    class Registry {
    public:
        Registry() : m_entityCount(0) {}
        ~Registry() {
            for (const auto& componentCollection : m_ComponentCollections) {
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
        void assign(entity id, T data) {
            ComponentCollection<T>* collection = new ComponentCollection<T>(data);
            m_ComponentCollections.push_back(collection);
        }

        /**
         * @brief Get the entities which holds at least each of the given components
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
            return reinterpret_cast<ComponentCollection<T>*>(m_ComponentCollections.at(0))->components.at(id);
        }

    private:
        uint32_t m_entityCount;
        std::vector<IComponentCollection*> m_ComponentCollections;
    };
}
