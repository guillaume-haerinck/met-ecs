#pragma once

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
        View(entity matchingEntitiesCount, entity* matchingEntities, ComponentCollection<Comps>*... compCollections)
            : m_matchingEntitiesCount(matchingEntitiesCount), m_matchingEntities(matchingEntities), m_matchingComponentCollections(compCollections...) {}

        /**
         * @brief
         */
        template<typename Func>
        void each(Func&& consumer) {
            for (size_t i = 0; i < m_matchingEntitiesCount; i++) {
                apply(m_matchingEntities[i], consumer, m_matchingComponentCollections, std::index_sequence_for<Comps...> {});
            }
        }

        /**
         * @brief Number of entities iterated by the view
         */
        unsigned int size() {
            return m_matchingEntitiesCount;
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
        void apply(entity id, Func&& consumer, const Tuple& compCollection, std::index_sequence<I...>) {
            consumer(id, std::get<I>(compCollection)->at(id)...);
        }

    private:
        entity* m_matchingEntities;
        entity m_matchingEntitiesCount;
        std::tuple<ComponentCollection<Comps> *...> m_matchingComponentCollections;
    };
}
