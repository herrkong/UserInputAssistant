#include "Cache.h"

namespace hk
{

Cache::Cache(int capacity)
:_head(new CacheNode)
,_tail(new CacheNode)
,_capacity(capacity)
,_count(0)
{
    
        cout<<"Cache(int capacity)"<<endl;
        _head->pnext = _tail ;
        _head->ppre = NULL ;
        _tail->ppre = _head ;
        _tail ->pnext = NULL ;
}

Cache::~Cache()
{
    //只剩下一个结点还delete tail 就double free了
    if(_head)
    {
        delete _head;
    }
    cout<<"~Cache()"<<endl;
}

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
