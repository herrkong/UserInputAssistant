#pragma once
#include "SplitTool.h"

namespace  hk
{

class Configuration ;

class SplitToolCppJieba:public SplitTool
{
public:
    SplitToolCppJieba(Configuration & conf);
    virtual ~SplitToolCppJieba(); 
    virtual Word & cut(const string &) = 0;

private:
    Configuration & _conf;

};

}//end of namespace hk
