#include <iostream>
#include <iomanip>



#include <string.h>

#include "genlclient.hpp"
#include <json11.hpp>
#include <cmath>
#include <random>

//A q&d request generator with random actions (including incorrect one) and random operands (clamped to 10)
std::string actions[]{"add","sub","mul","ukn"};
std::string getRandAct()
{
    json11::Json::object obj;
    obj["action"] = actions[std::rand()%4];
    obj["argument_1"] = std::round(std::rand()%10);
    obj["argument_2"] = std::round(std::rand()%10);
    
    return json11::Json(obj).dump();
}

int main ( int argc, char **argv )
{
    srand(time(NULL));
    
    uint32_t dstPid{2};
    std::string err;
    if(argc>=2)
    {
        dstPid = atol(argv[1]);
    }
    
    Mwl2::GenlClient client;
    std::string resp;
    
    
    auto res = client.exchange(dstPid, getRandAct(), resp,3);
    if(res<0)
    {
        return res;
    }
    printf("Received string %s\n", resp.c_str());

    return 0;
}
