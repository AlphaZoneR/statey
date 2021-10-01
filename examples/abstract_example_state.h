//
// Created by split on 10/1/21.
//

#ifndef STATEY_ABSTRACT_EXAMPLE_STATE_H
#define STATEY_ABSTRACT_EXAMPLE_STATE_H

#include "state_machine.h"
#include "example_context.h"

namespace stm_example {
    class AbstractExampleState : public stm::VirtualAbstractState<ExampleContext> {
    public:
        virtual std::shared_ptr<AbstractExampleState>
        on_start_downloading(std::shared_ptr<ExampleContext> context, const std::string &url) = 0;

        virtual std::shared_ptr<AbstractExampleState>
        on_download_failed(std::shared_ptr<ExampleContext> context, const FailureReason &failre_reason) = 0;

        virtual std::shared_ptr<AbstractExampleState>
        on_download_succeeded(std::shared_ptr<ExampleContext> context, const FileContents &contents) = 0;
    };
}

#endif //STATEY_ABSTRACT_EXAMPLE_STATE_H
