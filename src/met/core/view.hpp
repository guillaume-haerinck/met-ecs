#pragma once

#include <vector>
#include <tuple>
#include <utility>

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
        : m_matchingEntities(matchingEntities), m_matchingComponentArrays(compArrays...) {}

        // Temp
        View(const std::vector<entity>& matchingEntities) : m_matchingEntities(matchingEntities) {}

        /**
         * @brief 
         */
        template<typename Func>
        void each(Func&& consumer) {
            for (const entity id: m_matchingEntities) {
                apply(id, consumer, m_matchingComponentArrays, std::index_sequence_for<Comps...> {});
            }
        }

        // TODO implement iterator to allow for (entity id : myView) {} iterations

    private:
        /**
         * @brief Unpack a tuple as the arguments of a variadic lambda function.
         * @note Alteration to std::apply to get entity id and change accessed indices from the arrays stored by the tuple.
         * 
         * @link https://github.com/hokein/Wiki/wiki/How-to-unpack-a-std::tuple-to-a-function-with-multiple-arguments%3F
         * @link https://cpppatterns.com/patterns/apply-tuple-to-function.html
         */
        template<typename Func, typename Tuple, std::size_t... I>
        void apply(entity id, Func&& consumer, const Tuple& compArrays, std::index_sequence<I...>) {
            consumer(id, std::get<I>(compArrays)[id]...);
        }

    private:
        std::vector<entity> m_matchingEntities;
        std::tuple<Comps*...> m_matchingComponentArrays;
    };
}
