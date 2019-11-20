#pragma once

#include <array>
#include <vector>
#include <unordered_map>

namespace met {
    using entity = uint32_t;

    /**
     * @brief 
     */
    class IComponentContainer {
        public:
            IComponentContainer() {};
            virtual ~IComponentContainer() {};
    };

    /**
     * @brief 
     */
    template<class T>
    class ComponentContainer : public IComponentContainer {
    public:
        ComponentContainer(T data) {
            components.fill(data);
        };
        virtual ~ComponentContainer() {};

    public:
        std::array<T, 24> components;
    };

    /**
     * @brief 
     */
    class Registry {
    public:
        Registry() {}
        ~Registry() {
        }

        /**
         * @brief Create a new entity
         */
        entity create() {
            return 0;
        }

        /**
         * @brief Assign the given components to the given entity
         */
        template<typename T>
        void assign(entity id, T data) {
            auto test = new ComponentContainer<T>(data);
            m_componentContainers.push_back(test);
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
        // FIXME
        template<typename T>
        T get() {
            return reinterpret_cast<ComponentContainer<T>*>(m_componentContainers.at(0))->components.at(0);
        }

    private:
        uint32_t m_entityCount;
        std::vector<IComponentContainer*> m_componentContainers;
    };
}
