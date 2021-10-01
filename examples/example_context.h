#ifndef STATEY_EXAMPLE_CONTEXT_H
#define STATEY_EXAMPLE_CONTEXT_H

#include <string>
#include <optional>
#include <memory>

#include "state_machine.h"

#include "abstract_example_state.h"
#include "starting_state.h"

class ExampleContext: public std::enable_shared_from_this<ExampleContext> {
private:
    stm_example::FailureReason failure_reason;
    std::optional<stm_example::FileContents> file_contents;
    std::shared_ptr<stm::StateMachine<AbstractExampleState, ExampleContext, stm_example::DownloadTrigger>> state_machine;

public:

    ExampleContext() {
        failure_reason = stm_example::FailureReason::NONE;
        file_contents = {};
        auto current_state = std::make_shared<StartingState>();
        state_machine = stm::StateMachine<AbstractExampleState, ExampleContext, stm_example::DownloadTrigger>::builder<StartingState>(current_state, std::enable_shared_from_this<ExampleContext>::shared_from_this(), AbstractExampleState::trigger_map());
    }

    void start_downloading(const std::string &url) const {
        std::cout << "Downloading: " << url << '\n';
//        this->state_machine.on_trigger(stm_example::DownloadTrigger::START_DOWNLOADING);
    }

    void download_failed(const stm_example::FailureReason &reason) {
        this->failure_reason = reason;
    }
};


#endif //STATEY_EXAMPLE_CONTEXT_H
