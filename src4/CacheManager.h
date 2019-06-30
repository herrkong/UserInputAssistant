#pragma once
#include "Cache.h"
#include <vector>

namespace  hk
{

class CacheManager
{

public:
    
    //从磁盘文件中读取缓存信息
    static void initCache(size_t,const string & filename);
    //获取某个缓存
    static Cache & getCache(size_t idx);
    //定时更新所有缓存
    static void periodicUpdateCaches();


private:
    static vector<Cache> _cacheList;//缓存的数量和线程个数一致。

};


}//end of namespace hk
