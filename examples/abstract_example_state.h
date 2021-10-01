//
// Created by split on 10/1/21.
//

#ifndef STATEY_ABSTRACT_EXAMPLE_STATE_H
#define STATEY_ABSTRACT_EXAMPLE_STATE_H

#include <map>

#include "data.h"
#include "example_context.h"

class ExampleContext;

class AbstractExampleState : public stm::VirtualAbstractState<ExampleContext> {
public:
    virtual std::shared_ptr<AbstractExampleState>
    on_start_downloading(std::shared_ptr<ExampleContext> context, const std::string &url) = 0;

    virtual std::shared_ptr<AbstractExampleState>
    on_download_failed(std::shared_ptr<ExampleContext> context,
                       const stm_example::FailureReason &failre_reason) = 0;

    virtual std::shared_ptr<AbstractExampleState>
    on_download_succeeded(std::shared_ptr<ExampleContext> context, const stm_example::FileContents &contents) = 0;


    using CALLBACK_TYPE = stm::FUNC_TYPE<AbstractExampleState, ExampleContext>;

    static std::map<stm_example::DownloadTrigger, CALLBACK_TYPE> trigger_map() {
        auto resulting_trigger_map = std::map<stm_example::DownloadTrigger, CALLBACK_TYPE>();
        resulting_trigger_map[stm_example::DownloadTrigger::START_DOWNLOADING] = AbstractExampleState::on_start_downloading;
        resulting_trigger_map[stm_example::DownloadTrigger::DOWNLOAD_FAILED] = &AbstractExampleState::on_download_failed;
        resulting_trigger_map[stm_example::DownloadTrigger::DOWNLOAD_SUCCEEDED] = &AbstractExampleState::on_download_succeeded;

        return resulting_trigger_map;
    }
};

#endif //STATEY_ABSTRACT_EXAMPLE_STATE_H
