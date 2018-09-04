//
// Created by Michael on 8/24/2018.
//

#ifndef MEX_METAWEAR_CONFIGURATIONHANDLER_H
#define MEX_METAWEAR_CONFIGURATIONHANDLER_H

#include <map>
#include <mex.hpp>
#include "FunctionWrapper.h"

class FunctionWrapper;
class ConnectionHandler;
class ConfigurationHandler {
private:
    ConnectionHandler* m_connectionHandler;
public:
    ConfigurationHandler (ConnectionHandler* connectionHandler,FunctionWrapper* wrapper);
    ~ConfigurationHandler();


    static void mexConfigureGyroSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureGyroRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureGyroWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);


    static void mexConfigureAccelerometerSample(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureAccelerometerRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureAccelerometerWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexConfigureMagnetometerPowerPreset(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexConfigureBarometerOversampling(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureBarometerStandby(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureBarometerImpulseFilter(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureBarometerWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexConfigureSensorFusionMode(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureSensorFusionRange(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureSensorFusionWrite(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);

    static void mexConfigureProximity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureAmbientLight(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureTemperature(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);
    static void mexConfigureHumidity(std::shared_ptr<matlab::engine::MATLABEngine> engine,void *context,  ParameterWrapper& outputs, ParameterWrapper& inputs);


};

#endif //MEX_METAWEAR_CONFIGURATIONHANDLER_H
