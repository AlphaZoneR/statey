#ifndef STATEY_STARTING_STATE_H
#define STATEY_STARTING_STATE_H

#include "abstract_example_state.h"
#include "downloading_state.h"

class StartingState : public AbstractExampleState {
public:
    std::shared_ptr<AbstractExampleState>
    on_start_downloading(std::shared_ptr<ExampleContext> context, const std::string &url) override {
//        context->start_downloading(url);
        return std::shared_ptr<DownloadingState>();
    }

    std::shared_ptr<AbstractExampleState>
    on_download_failed(std::shared_ptr<ExampleContext> context, const stm_example::FailureReason &failre_reason) override {
        return std::shared_ptr<AbstractExampleState>();
    }

    std::shared_ptr<AbstractExampleState>
    on_download_succeeded(std::shared_ptr<ExampleContext> context, const stm_example::FileContents &contents) override {
        return std::shared_ptr<AbstractExampleState>();
    }

    std::string name() const override {
        return "StaringState";
    }
};


#endif //STATEY_STARTING_STATE_H
