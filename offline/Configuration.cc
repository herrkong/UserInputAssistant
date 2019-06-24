#include "../include/Configuration.h"
#include <iostream>

using std::endl;
using std::cout;

namespace hk
{
//初始化配置文件
Configuration::Configuration(const string & filepath)
:_filepath(filepath)
{
    ifstream ifs(_filepath);
    string s1,s2;
    while(ifs>>s1>>s2)
    {
        _configMap.insert(make_pair(s1,s2));
    }       
    cout<<"Configuration(const string & filepath)"<<endl;
}


ConfigMap &  Configuration::getConfigMap()
{
    return _configMap;
}

void Configuration::print()  const
{
    for(auto it = _configMap.begin();it!=_configMap.end();++it)
    {
        cout<<it->first<<"   "<<it->second<<endl;
    }
    cout<<endl;
}

}//end of namespace hk
