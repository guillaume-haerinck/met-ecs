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
        View(const std::vector<entity>& matchingEntities, Comps*... compArrays) 
        : m_matchingEntities(matchingEntities), m_matchingComponentsArrays(compArrays...) {};

        /**
         * @brief 
         */
        template<typename Func>
        void each(Func&& consumer) {
            // TODO check for std::move and std::get from entt view.hpp line 411
            // TODO check in ECS.h by redxdev the line 1065

            for (const entity id: m_matchingEntities) {
                // TODO pass matching component from tuple at entity index
                consumer(id, std::get<0>(m_matchingComponentsArrays)[id], std::get<1>(m_matchingComponentsArrays)[id]);
            }
        }

    private:
        // Only use std::tuple, with it we can have a perfect AOS (pvs.pvs.pvs) of pointers instead of SOA as stored (ppp.vvv.sss)
        // const std::tuple<pool_type<Component> *...> pools;
        // using pool_type = std::conditional_t<std::is_const_v<Comp>, const storage<Entity, std::remove_const_t<Comp>>, storage<Entity, Comp>>;
        std::vector<entity> m_matchingEntities;
        std::tuple<Comps*...> m_matchingComponentsArrays;
    };
}
