#pragma once
#include <unordered_map>
#include <string>

using namespace std;

namespace  hk
{

class MyConf
{
    using ConfigMap = unordered_map<string,string>;
public:
    explicit MyConf(const string & filepath);
    ConfigMap & getConfigMap();  

private:
    string _filepath;  //配置文件路径
    ConfigMap _configMap; //配置文件内容
};

}//end of namespace hk
