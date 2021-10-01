#ifndef STATEY_DOWNLOAD_SUCCEEDED_STATE_H
#define STATEY_DOWNLOAD_SUCCEEDED_STATE_H

#include "abstract_example_state.h"

class DownloadSucceededState : public AbstractExampleState, std::enable_shared_from_this<AbstractExampleState> {
public:
    std::shared_ptr<AbstractExampleState>
    on_start_downloading(std::shared_ptr<ExampleContext> context, const std::string &url) override {
        std::cout << "Download already completed.\n";
        return std::enable_shared_from_this<AbstractExampleState>::shared_from_this();
    }

    std::shared_ptr<AbstractExampleState>
    on_download_failed(std::shared_ptr<ExampleContext> context, const stm_example::FailureReason &failre_reason) override {
        std::cout << "Download already completed.\n";
        return std::enable_shared_from_this<AbstractExampleState>::shared_from_this();
    }

    std::shared_ptr<AbstractExampleState>
    on_download_succeeded(std::shared_ptr<ExampleContext> context, const stm_example::FileContents &contents) override {
        std::cout << "Download already completed.\n";
        return std::enable_shared_from_this<AbstractExampleState>::shared_from_this();
    }

    std::string name() const override {
        return "DownloadSucceededState";
    }
};

#endif //STATEY_DOWNLOAD_SUCCEEDED_STATE_H
