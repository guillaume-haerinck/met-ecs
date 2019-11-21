#pragma once

#include "../config/config.hpp"

namespace met {
    template<typename T>
    void Func1(T&& Arg2){
        Arg2();
    }

    /**
     * @brief Collection of components of multiple types associated to multiple entities
     */
    class View {
    public:
        View() {};
        ~View() {};

        /**
         * @brief 
         */
        template<typename Func>
        void each(Func&& lambda) {
            for (size_t i = 0; i < 5; i++) {
                lambda();
            }
        }
    };
}
