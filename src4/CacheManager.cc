
#pragma once
#include "Cache.h"
#include "CacheManager.h"
namespace hk
{

CacheManager::CacheManager()
:_cacheNum(4)
,_cacheCapacity(104)
,_mainCache(_cacheCapacity)
{
    cout<<"CacheManager()"<<endl;
}

void CacheManager::readFromFile()
{
    string filename = "cache.txt";//之后写到conf中间去
    string filepath ="../bin/cache/" + filename ;
    ifstream ifs(filepath);
    string queryWord,resultWords;//resultWords是json字符串
    while(ifs>>queryWord>>resultWords)
    {
        //cout<<queryWord<<endl;
       // cout<<resultWords<<endl;
       // CacheNode * node = new CacheNode ;
       // node->key =queryWord ;
       // node->value = resultWords ; 
        //_hashMap[queryWord] = resultWords;
        _mainCache.addElement(queryWord,resultWords);
    }
    ifs.close();
}

void CacheManager::writeToFile()
{
    string filename = "cache.txt";
    string filepath = "../bin/cache/" + filename;
    ofstream ofs(filepath);
    HashMap & hashMap =_mainCache.gethashMap();
    for(auto & pair: hashMap)
    {
        ofs<<pair.first<<" "<<pair.second->value<<endl; 
    }
    ofs.close();
}


CacheManager *  CacheManager::initCache()
{
    cout<<"init all Caches"<<endl;
    _cacheNum = 4 ;  //和开的线程数保持一致之后写到conf中去。
    for(int i = 0 ;i != _cacheNum ; i++)
    {
        _cacheList.push_back(Cache(_cacheCapacity));
    }
    readFromFile();//初始化cache 
    cout<<"init main Cache finished."<<endl;
    return _pCacheManager ;   
}

Cache & CacheManager::getCache(size_t idx)
{
    if(idx == 0)
    {
        return _mainCache ; 
    }
    else 
    {
        return _cacheList[idx-1];
    }
}

void CacheManager::periodicUpdateCaches()
{
    cout<<"周期轮询子线程cache"<<endl;
        for(int i=1 ; i<=_cacheNum ;++i)
        {
            Cache temp(_cacheCapacity);;
            temp = _cacheList[i-1];
            //读取子线程的热数据,清空原有热数据
            HashMap & hashMap = temp.gethashMap();
             for(auto & pair:hashMap)
             {
                _mainCache.addElement(pair.first,pair.second->value);     
             }
        }//for 
    cout << "读取子线程Cache完毕"<<endl;
    writeToFile();//将更新后的cache写回文件
}

CacheManager * CacheManager::_pCacheManager = nullptr ;//饿汉模式

}//end of namespace hk
