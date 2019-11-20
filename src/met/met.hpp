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
    };

    /**
     * @brief 
     */
    template <class T>
    class ComponentHandle : public IComponentHandle {
    public:
        ComponentHandle() {};

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

        entity create() {
            return 0;
        }

        template<typename T>
        void assign(entity id) {
            auto test = new ComponentHandle<T>();
            m_componentHandles.push_back(test);
        }

        template<typename T>
        void view() {
            // TODO get component handle corresponding to T
        }

    private:
        uint32_t m_entityCount;
        std::vector<IComponentHandle*> m_componentHandles;
    };
}
