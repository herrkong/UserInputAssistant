#include "Cache.h"

namespace hk
{


void Cache::addElement(const string & key,const string & value)
{
    
    //cache中没找到记录
    if(_hashMap.find(key) == _hashMap.end())
    {
        
         CacheNode * node = new CacheNode ;
         if(_count == _capacity)
         {
             removeElement();
         }
         node->key = key ;
         node->value = value ;
         _hashMap.insert(make_pair(key,node)) ;//插入hashMap
         insertToFront(node); //插入链表头部
         _count++ ;
    }
    else //在cache中找到了
    {
        CacheNode * node = _hashMap[key];
        detachNode(node);
        insertToFront(node);
        node->value = value ;
    }
}


void Cache::removeElement()
{
    CacheNode * node = _tail->ppre ;
    detachNode(node);

    _hashMap.erase(node->key);
    --_count ;
}

void Cache::insertToFront(CacheNode * node)
{
    //带头结点的头插法 
    node->pnext = _head->pnext ;
    node->ppre = _head ;
   _head->pnext = node ;
    node->pnext->ppre = node ;
}

string Cache::get(string & key)
{
    if(_hashMap.find(key) == _hashMap.end())
    {
         string miss = "No such key";
         return miss;
    }
    CacheNode * node = _hashMap[key] ;
    detachNode(node);
    insertToFront(node); //命中就移动到头部
    return _hashMap[key]->value ;
}

void Cache::detachNode(CacheNode * node)
{
    node->ppre->pnext = node->pnext ;
    node->pnext->ppre = node->ppre ;
}

void Cache::print()
{
    for(auto & pair:_hashMap)
    {
        cout<<pair.first<<endl;
        cout<<pair.second->value<<endl;
    }
}

HashMap & Cache::gethashMap()
{
    return _hashMap ;
}


}//end of namespace hk
