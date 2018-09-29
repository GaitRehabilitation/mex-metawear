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

#ifndef MEX_METAWEAR_CAPTUREHANDLER_H
#define MEX_METAWEAR_CAPTUREHANDLER_H

#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"


class FunctionWrapper;
class ConnectionHandler;
class MexPrintStream;
class CaptureHandler{
private:
    ConnectionHandler* m_connectionHandler;
    MexPrintStream* m_printStream;
public:
    CaptureHandler(ConnectionHandler* connectionHandler,FunctionWrapper* wrapper);
    ~CaptureHandler();

    /**
     * query sensor for data based off of the subscribed signal
     * @param engine
     * @param context
     * @param outputs
     * @param inputs
     */
    static void mexQuery(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    /**
     * start logging data on signals that are marked for logging
     * @param engine
     * @param context
     * @param outputs
     * @param inputs
     */
    static void mexStartLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    /**
     * stops logging and download data in the queues
     * @param engine
     * @param context
     * @param outputs
     * @param inputs
     */
    static void mexStopLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexDownloadLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexClearLogger(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);


    static void mexEnableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexEnableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexEnableSensorFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexSetSensorFusionFlag(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexDisableGyro(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexDisableAccelerometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexDisableSensorFusion(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexEnableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexDisableMagnetometer(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
};
#endif //MEX_METAWEAR_CAPTUREHANDLER_H
