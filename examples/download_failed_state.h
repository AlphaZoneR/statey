//
// Created by split on 10/1/21.
//

#ifndef STATEY_DOWNLOAD_FAILED_STATE_H
#define STATEY_DOWNLOAD_FAILED_STATE_H

#include "abstract_example_state.h"
#include "downloading_state.h"

namespace stm_example {

    class DownloadFailedState : public AbstractExampleState {
    public:
        std::shared_ptr<AbstractExampleState>
        on_start_downloading(std::shared_ptr<ExampleContext> context, const std::string &url) override {
            std::cout << "Retrying download.\n";
            return std::make_shared<DownloadingState>();
        }

        std::shared_ptr<AbstractExampleState>
        on_download_failed(std::shared_ptr<ExampleContext> context, const FailureReason &failre_reason) override {
            return std::shared_ptr<AbstractExampleState>();
        }

        std::shared_ptr<AbstractExampleState>
        on_download_succeeded(std::shared_ptr<ExampleContext> context, const FileContents &contents) override {
            return std::shared_ptr<AbstractExampleState>();
        }

        std::string name() const override {
            return std::string();
        }
    };
}
#endif //STATEY_DOWNLOAD_FAILED_STATE_H
