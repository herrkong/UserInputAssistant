#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
using namespace std;

//改动 将Configuration写成单例模式 造成后面改动太大啊？
//暂时先别改。

namespace hk
{    
   //这里如果用using shared_ptr 下面解引用*_configMap.insert()就出错了。
    using ConfigMap = unordered_map<string,string>;
class Configuration
{
public:
   // static Configuration * getInstance();
    
    Configuration(const string & filepath);
    ConfigMap & getConfigMap();
    void print() const;//打印conf中保存的内容

//private:
    // Configuration(const string & filepath);
   // ~Configuration(){}

private:
    string  _filepath;     //配置文件路径conf
    ConfigMap  _configMap; //配置文件内容
    
};

}//end of namespace hk
