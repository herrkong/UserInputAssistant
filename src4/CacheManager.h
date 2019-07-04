#pragma once
#include "Cache.h"
#include <vector>

namespace  hk
{

class CacheManager
{

public:
    
    static CacheManager * getCacheManager()
    {
        if(!_pCacheManager)
        {
            _pCacheManager = new CacheManager ;
            atexit(destroy);
        }
        return _pCacheManager ;
    }
    
    static void destroy()
    {
        if(_pCacheManager)
        {
            delete _pCacheManager ;
        }
    }

    //初始化Cache数量 并首次同步Cache 
    CacheManager *  initCache();
    //获取某个缓存
    Cache & getCache(size_t);
    //定时更新所有缓存
    void periodicUpdateCaches(); 
    //从文件中读取缓存信息
    void readFromFile();
    //将缓存信息写入文件
    void writeToFile();


private:
    CacheManager();
    ~CacheManager(){}

private:
    int _cacheNum ; //cache数目
    int _cacheCapacity ;//cache容量
    Cache _mainCache ;//总Cache
    vector<Cache> _cacheList;//缓存的数量和线程个数一致。
    static CacheManager * _pCacheManager ;
}; 

//CacheManager * CacheManager::_pCacheManager = nullptr ; //饿汉模式

}//end of namespace hk
