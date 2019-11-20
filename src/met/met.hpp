#pragma once

#include <array>
#include <vector>
#include <unordered_map>

namespace met {
    using entity = uint32_t;

    /**
     * @brief 
     */
    class IComponentHandle {
        public:
            IComponentHandle() {};
            virtual ~IComponentHandle() {};
    };

    /**
     * @brief 
     */
    template <class T>
    class ComponentHandle : public IComponentHandle {
    public:
        ComponentHandle() {};
        virtual ~ComponentHandle() {};

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
            for (auto handle : m_componentHandles) {
                delete handle;
            }
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
        void assign(entity id) {
            auto test = new ComponentHandle<T>();
            m_componentHandles.push_back(test);
        }

        /**
         * @brief Get the entities which holds at least each of the given components
         */
        template<typename T>
        void view() {
            
        }

        // TODO how to get raw data of each component handle from here ?
        // Use iterator pattern ? But how to determine return type ?

    private:
        uint32_t m_entityCount;
        std::vector<IComponentHandle*> m_componentHandles;
    };
}
