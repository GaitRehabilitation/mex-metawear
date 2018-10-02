//
// Created by Michael on 10/1/2018.
//

#ifndef MEX_METAWEAR_METAWEARSTATEDATA_H
#define MEX_METAWEAR_METAWEARSTATEDATA_H

#include <vector>
#include <map>

class MetawearWrapper;
class MetawearStateData{
private:
    uint8_t* m_data;
    uint32_t m_size;
    std::vector<uint8_t> m_handlers;
public:
    MetawearStateData(MetawearWrapper* wrapper);
    void restore(MetawearWrapper* wrapper);
};

#endif //MEX_METAWEAR_METAWEARSTATEDATA_H
