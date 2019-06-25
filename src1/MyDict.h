#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

namespace  hk
{
    using Dict = vector<pair<string,int>>;
    using IndexTable = map< string,set<int> >; 

//单例模式 只创建一份词典 
//不需要每次执行查询 都要在线程中创建词典
class MyDict  
{
public:
    static MyDict * getInstance()
    {
        if(!_pInstance)
        {
            _pInstance = new MyDict();
            atexit(destroy);
        }
        return _pInstance;
    }
    
    //初始化词典
    void init(const char * dictFilePath,const char * indexFilePath);
   //获取词典
    Dict & getDict();
    //获取索引表
    IndexTable & getIndexTable();
    
    static void destroy()
    {
        if(_pInstance)
        {
           delete _pInstance; 
        }
    }

private:
    MyDict(){ cout<<"MyDict()"<<endl; }
    ~MyDict() { cout<<"~MyDict()"<<endl; }
private:
   static  MyDict * _pInstance;
   Dict _dict;  //词典
   IndexTable _indexTable;//索引表
};


}//end of namespace hk
