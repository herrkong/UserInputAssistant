#include "MyConf.h"
#include <iostream>
#include <fstream>
using namespace std;


namespace hk
{

using ConfigMap = unordered_map<string,string>;

MyConf::MyConf(const string & filepath)
:_filepath(filepath)
{
    ifstream ifs(_filepath);
    string s1,s2;
    while(ifs>>s1>>s2)
    {
       // cout<<s1<<" "<<s2<<endl;
        _configMap.insert(make_pair(s1,s2));
    }
   // cout<<"MyConf(const string & filepath)"<<endl;
}

ConfigMap & MyConf::getConfigMap()
{
    return _configMap;
}

}//end of namespace hk
