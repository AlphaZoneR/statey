#ifndef STATEY_STATE_MACHINE_H
#define STATEY_STATE_MACHINE_H

#include <map>
#include <iostream>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <functional>

#include "boost/type_traits/is_abstract.hpp"
#include "boost/type_traits.hpp"

namespace stm {
    template<typename CONTEXT>
    class VirtualAbstractState {
    public:
        virtual ~VirtualAbstractState() = default;

        virtual std::string name() = 0;

        virtual void on_enter(std::shared_ptr<CONTEXT> context) {
            std::cout << "Default on_enter for " << this->name() << '\n';
        }
    };

    template<typename ABSTRACT_STATE, typename CONTEXT, typename ... ARGS>
    using FUNC_TYPE = std::function<std::shared_ptr<ABSTRACT_STATE>(ABSTRACT_STATE *, std::shared_ptr<CONTEXT>,
                                                                    ARGS...)>;

    template<
            typename ABSTRACT_STATE,
            typename CONTEXT,
            typename TRIGGER,
            typename = std::enable_if_t<(boost::is_base_and_derived<VirtualAbstractState<CONTEXT>, ABSTRACT_STATE>())>
    >
    class StateMachine {
    private:
        using CALLBACK_TYPE = FUNC_TYPE<ABSTRACT_STATE, CONTEXT>;

        std::shared_ptr<ABSTRACT_STATE> current_state;
        std::shared_ptr<CONTEXT> context;
        std::map<TRIGGER, CALLBACK_TYPE> trigger_map;

    public:
        StateMachine(std::shared_ptr<ABSTRACT_STATE> starting_state, std::shared_ptr<CONTEXT> context,
                     std::map<TRIGGER, CALLBACK_TYPE> trigger_map) : current_state(starting_state), context(context),
                                                                     trigger_map(trigger_map) {
            starting_state->on_enter(context);
        }

        template<typename ... ARGS>
        void on_trigger(TRIGGER trigger, ARGS ... args) {
            auto trigger_executor_iterator = trigger_map.find(trigger);

            if (trigger_executor_iterator != trigger_map.end()) {
                auto new_state = trigger_executor_iterator->second(current_state.get(), context, args...);

                if (new_state == nullptr) {
                    std::cerr << "Invalid nullptr return from " << current_state->name() << ". Trigger: "
                              << std::type_index(typeid(trigger)).name() << "::" << int(trigger) << '\n';
                } else if (new_state != current_state) {
                    std::cout << "Transitioning from " << current_state->name() << " to "
                              << new_state->name()
                              << " with " << std::type_index(typeid(trigger)).name() << "::" << int(trigger) << '\n';
                    new_state->on_enter(context);
                    current_state = new_state;
                } else {
                    std::cout << "Remaining in state " << current_state->name() << ". Trigger: "
                              << std::type_index(typeid(trigger)).name() << "::" << int(trigger) << '\n';
                }
            } else {
                std::cerr << "Cannot transition with " << std::type_index(typeid(trigger)).name() << "::"
                          << int(trigger)
                          << '\n';
            }
        }

        std::shared_ptr<ABSTRACT_STATE> get_current_state() const {
            return this->current_state;
        }

    public:
        template<typename BASE_STATE>
        static std::shared_ptr<StateMachine<ABSTRACT_STATE, CONTEXT, TRIGGER>>
        builder(
                std::shared_ptr<BASE_STATE> baseState,
                std::shared_ptr<CONTEXT> context,
                std::map<TRIGGER, CALLBACK_TYPE> trigger_mappings) {
            return std::make_shared<StateMachine>(baseState, context, trigger_mappings);
        }
    };
}
#endif //STATEY_STATE_MACHINE_H
