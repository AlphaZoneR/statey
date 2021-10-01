//
// Created by Albert Ferencz on 01.10.2021.
//

#ifndef STATEY_DATA_H
#define STATEY_DATA_H

#include <string>

namespace stm_example {
    enum class FailureReason {
        NONE,
        NO_NETWORK,
        FILE_NOT_FOUND
    };

    enum class DownloadTrigger {
        START_DOWNLOADING = 0,
        DOWNLOAD_FAILED,
        DOWNLOAD_SUCCEEDED
    };

    using FileContents = std::string;
}

#endif //STATEY_DATA_H
