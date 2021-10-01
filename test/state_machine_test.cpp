#include "state_machine.h"
#include "gtest/gtest.h"

namespace {
    enum class TestTriggers {
        TRIGGER1,
    };

    struct TestContext {

    };

    class AbstractTestState : public stm::VirtualAbstractState<TestContext> {
    public:
        virtual std::shared_ptr<AbstractTestState> on_trigger_1() = 0;
    };

    class StartingState : public AbstractTestState {
    public:
        std::shared_ptr<AbstractTestState> on_trigger_1() override {
            return nullptr;
        }

        std::string name() override {
            return "StartingState";
        }
    };

    class SecondState : public AbstractTestState {
    public:
        std::shared_ptr<AbstractTestState> on_trigger_1() override {
            return nullptr;
        }

        std::string name() override {
            return "StartingState";
        }
    };

    TEST(StateMachineTest, ExecutesExpectedCallback) {
        // Given
        auto starting_state = std::make_shared<StartingState>();
        auto context = std::make_shared<TestContext>();

        auto boolean_v = false;

        auto callback = [&boolean_v](AbstractTestState *abstract_test_state,
                                     const std::shared_ptr<TestContext> &incoming_context) {
            boolean_v = true;

            return std::make_shared<StartingState>();
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();

        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<StartingState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        EXPECT_TRUE(boolean_v);
    }

    TEST(StateMachineTest, TransitionsToCorrectState) {
        // Given
        auto starting_state = std::make_shared<StartingState>();
        auto second_state = std::make_shared<SecondState>();

        auto context = std::make_shared<TestContext>();

        auto callback = [second_state](AbstractTestState *abstract_test_state,
                                       const std::shared_ptr<TestContext> &incoming_context) {

            return second_state;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();

        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<StartingState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        auto transitioned_state = sut->get_current_state();
        EXPECT_EQ(transitioned_state, second_state);
    }

    TEST(StateMachineTest, CurrentStateReturnsNullOnTrigerStateMachineDoesNotTransition) {
        // Given
        auto starting_state = std::make_shared<StartingState>();
        auto context = std::make_shared<TestContext>();

        auto callback = [](AbstractTestState *abstract_test_state,
                           const std::shared_ptr<TestContext> &incoming_context) {

            return nullptr;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();

        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<StartingState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        auto transitioned_state = sut->get_current_state();
        EXPECT_EQ(transitioned_state, starting_state);
    }

    struct MockState : public AbstractTestState, std::enable_shared_from_this<AbstractTestState> {
        bool was_i_called = false;
        bool was_i_called_twice = false;

        std::shared_ptr<AbstractTestState> on_trigger_1() override {
            return std::enable_shared_from_this<AbstractTestState>::shared_from_this();
        }

        void on_enter(std::shared_ptr<TestContext> context) override {
            if (was_i_called) {
                was_i_called_twice = true;
            }

            was_i_called = true;
        }

        std::string name() override {
            return "MockState";
        }
    };

    TEST(StateMachineTest, CallsOnEnterOnStartingState) {
        // Given
        auto starting_state = std::make_shared<MockState>();
        auto context = std::make_shared<TestContext>();

        auto callback = [](AbstractTestState *abstract_test_state,
                           const std::shared_ptr<TestContext> &incoming_context) {

            return nullptr;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();
        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        // When
        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<MockState>(
                starting_state, context, trigger_mappings);

        // Then
        EXPECT_TRUE(starting_state->was_i_called);
    }

    TEST(StateMachineTest, CallsOnEnterOnTransition) {
        // Given
        auto starting_state = std::make_shared<StartingState>();
        auto second_state = std::make_shared<MockState>();
        auto context = std::make_shared<TestContext>();

        auto callback = [second_state](AbstractTestState *abstract_test_state,
                                       const std::shared_ptr<TestContext> &incoming_context) {
            return second_state;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();
        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<StartingState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        EXPECT_TRUE(second_state->was_i_called);
    }


    TEST(StateMachineTest, CurrentStateReturnsCurrentState_DoesNotCallOnEnterAgain) {
        // Given
        auto starting_state = std::make_shared<MockState>();
        auto context = std::make_shared<TestContext>();

        auto callback = [starting_state](AbstractTestState *abstract_test_state,
                                         const std::shared_ptr<TestContext> &incoming_context) {

            return starting_state;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();
        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<MockState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        EXPECT_FALSE(starting_state->was_i_called_twice);
    }

    TEST(StateMachineTest, ContextPassedToStateCallbackIsCorrect) {
        // Given
        auto starting_state = std::make_shared<MockState>();
        auto context = std::make_shared<TestContext>();

        auto boolean_v = false;

        auto callback = [context, &boolean_v](AbstractTestState *abstract_test_state,
                                              const std::shared_ptr<TestContext> &incoming_context) {
            boolean_v = true;
            EXPECT_EQ(incoming_context, context);
            return nullptr;
        };

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();
        trigger_mappings[TestTriggers::TRIGGER1] = callback;

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<MockState>(
                starting_state, context, trigger_mappings);

        // When && Then
        sut->on_trigger(TestTriggers::TRIGGER1);
        EXPECT_TRUE(boolean_v);
    }

    TEST(StateMachineTest, UnmappedTriggerPassedToStateMachine_DoesNotTransitionState) {
        // Given
        auto starting_state = std::make_shared<MockState>();
        auto context = std::make_shared<TestContext>();

        auto trigger_mappings = std::map<TestTriggers, stm::FUNC_TYPE<AbstractTestState, TestContext>>();

        auto sut = stm::StateMachine<AbstractTestState, TestContext, TestTriggers>::builder<MockState>(
                starting_state, context, trigger_mappings);

        // When
        sut->on_trigger(TestTriggers::TRIGGER1);

        // Then
        EXPECT_EQ(sut->get_current_state(), starting_state);
    }
}