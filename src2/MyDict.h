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

//这里使用了之前封装好的自动释放的单例模式 只创建一份词典 
//不需要每次执行查询 就在线程中创建词典
class MyDict  
{
    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease()
        {
            if(_pInstance)
                delete _pInstance;
            cout<<"~AutoRelease()"<<endl;
        }
    };

public:
    static MyDict * getInstance()
    {
        if(!_pInstance)
           _pInstance = new MyDict();
        return _pInstance;
    }
    
    void init(const char * dictFilePath,const char * indexFilePath);
    Dict & getDict();
    IndexTable & getIndexTable();

private:
    MyDict(){}
    ~MyDict() {}
    friend class AutoRelease;

private:
   static  MyDict * _pInstance;
   static AutoRelease _autoRelease;
   Dict _dict;  //词典
   IndexTable _indexTable;//索引表
};
//没有在这里初始化全局静态变量
//MyDict::AutoRelease MyDict::_autoRelease; 
}//end of namespace hk
