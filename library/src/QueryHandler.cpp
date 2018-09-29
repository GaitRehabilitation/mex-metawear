/**
* Copyright 2018 GaitRehabilitation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "handlers/QueryHandler.h"
#include "handlers/ConnectionHandler.h"
#include "FunctionWrapper.h"
#include <MetawearWrapper.h>
#include <MexUtility.h>
#include <metawear/platform/memory.h>
#include "StreamTypes.h"
#include "MatlabDataArray.hpp"

QueryHandler::QueryHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper):
        m_connectionHandler(connectionHandler) {
    std::map<std::string, WrapperMethod *> functions = {
            {"query_meta",mexQueryBoardInfo}
    };
    wrapper->registerMethod(this, functions);
}

void QueryHandler::mexQueryBoardInfo(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs) {
    auto *handler = static_cast<QueryHandler *>(context);

    if (inputs.size() != 2)
        MexUtility::error(engine, "Two Inputs Required");

    matlab::data::CharArray address = inputs[1];
    MetawearWrapper *wrapper = handler->m_connectionHandler->mexGetDeviceAndVerify(engine,address.toAscii());

    MblMwMetaWearBoard *board = wrapper->getBoard();
    auto dev_info = mbl_mw_metawearboard_get_device_information(board);
    MexUtility::printf(engine, "Firmware: " + std::string(dev_info->firmware_revision) + "\n");
    MexUtility::printf(engine, "Model Number: " + std::string(dev_info->model_number) + "\n");
    MexUtility::printf(engine, "Serial Number: " + std::string(dev_info->serial_number) + "\n");
    MexUtility::printf(engine, "Hardware Revision: " + std::string(dev_info->hardware_revision) + "\n");
    MexUtility::printf(engine, "Manufacture:  " + std::string(dev_info->manufacturer) + "\n");
    mbl_mw_memory_free((void *) dev_info);

    for (auto it = board->module_config.begin(); it != board->module_config.end(); ++it) {
        MexUtility::printf(engine, "module configs:  " + std::to_string(it->first) + "\n");
    }
}

QueryHandler::~QueryHandler() {
}