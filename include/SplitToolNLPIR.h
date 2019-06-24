#pragma once
#include "SplitTool.h"
#include <iostream>


namespace hk
{

class Configuration;

class SplitToolNLPIR:public SplitTool
{
public:
    SplitToolNLPIR(Configuration & conf);
    ~SplitToolNLPIR();
    virtual Word & cut(const string &);

private:
    Configuration & _conf;
    //field has incomplete type 'hk::Configuration'
    //前向声明class Configuration;
    //前向声明的类或者结构体只能用来定义指针或者引用。
    
};

}//end of namespace hk
