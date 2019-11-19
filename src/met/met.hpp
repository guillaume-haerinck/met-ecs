#pragma once

#include <array>

namespace met {
    /**
     * @brief 
     */
    class Registry {
    public:
        Registry() {};

    private:
        uint32_t m_entityCount;
    };

    /**
     * @brief 
     */
    template <class T>
    class ComponentHandle {
    public:
        std::array<T, 24> components;
    };
}
