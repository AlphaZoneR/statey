//
// Created by split on 10/1/21.
//

#ifndef STATEY_EXAMPLE_CONTEXT_H
#define STATEY_EXAMPLE_CONTEXT_H

#include <string>
#include <optional>
#include <memory>

#include "abstract_example_state.h"
#include "starting_state.h"
#include "state_machine.h"

namespace stm_example {
    enum class FailureReason {
        NONE,
        NO_NETWORK,
        FILE_NOT_FOUND
    };

    enum class DownloadTrigger {
        START_DOWNLOADING,
        DOWNLOAD_FAILED,
        DOWNLOAD_FINISHED
    };

    using FileContents = std::string;

    class ExampleContext {
    private:
        FailureReason failure_reason = FailureReason::NONE;
        std::optional<FileContents> file_context = {};
        std::shared_ptr<stm::StateMachine<AbstractExampleState, ExampleContext, DownloadTrigger>> state_machine;

    public:

        void start_downloading(const std::string &url) const {
            std::cout << "Downloading: " << url << '\n';
            this->state_machine.on_trigger(DownloadTrigger::START_DOWNLOADING);
        }

        void download_failed(const FailureReason &reason) {
            this->failureReason = reason;
        }
    };
}

#endif //STATEY_EXAMPLE_CONTEXT_H
