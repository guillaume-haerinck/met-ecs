#pragma once

#include <vector>

#include "../config/config.hpp"
#include "component-collection.hpp"

namespace met {
    /**
     * @brief Collection of components of multiple types associated to multiple entities
     */
    template<typename... Comps>
    class View {
    public:
        View(const Comps&... comps) : m_matchingComponentsCollection(comps...) {};

        /**
         * @brief 
         */
        template<typename Func>
        void each(Func&& consumer) {
            // TODO check for std::move and std::get from entt view.hpp line 411
            // TODO check in ECS.h by redxdev the line 1065

            consumer();

            /*
            for (const auto components : m_matchingComponentsCollection) {
                // TODO pass each matching components at the current entity index as function parameters
                // How to handle variable number of function parameters ? Variadic function
                // Does not work need to know the concrete data
                lambda();
            }
            */
        }

    private:
        // Only use std::tuple, with it we can have a perfect AOS (pvs.pvs.pvs) of pointers instead of SOA as stored (ppp.vvv.sss)
        // const std::tuple<pool_type<Component> *...> pools;
        // using pool_type = std::conditional_t<std::is_const_v<Comp>, const storage<Entity, std::remove_const_t<Comp>>, storage<Entity, Comp>>;

        std::tuple<Comps...> m_matchingComponentsCollection;
    };
}
