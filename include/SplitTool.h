#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace  hk
{

class SplitTool
{
public:
    using Word = vector<string>;

public:
    SplitTool();
    virtual ~SplitTool();
    virtual Word & cut(const string &) = 0;
   //分词函数,纯虚函数，提供接口

};

}//end of namespace hk
