#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

namespace  hk
{


struct CacheNode 
{
    string key ;  //查询词
    string value; //候选词 jason格式
    CacheNode * pnext,* ppre;
    //bug key(0),value(0)
    CacheNode():key(),value(),pnext(NULL),ppre(NULL){}
};


using HashMap = unordered_map<string ,CacheNode * >;

class Cache
{

public:

     Cache(int capacity);
    
    //Cache(const Cache & cache);
    
    ~Cache();

    //往缓存中添加数据 
    void addElement(const string & key,const string  & value);
    //插入链表头部
    void insertToFront(CacheNode * node);
    //cache满了 淘汰一些缓存
    void removeElement(); 
    //查找某个键值
    string get(string & key);
    //删除结点
    void detachNode(CacheNode * node);
    //更新缓存信息 
   // void update(const Cache & rhs);
    //打印当前缓存信息
    void print();
    

    HashMap & gethashMap();

private:
    HashMap _hashMap; 
    CacheNode * _head; //指向双向链表的头结点
    CacheNode * _tail; //指向双向链表的尾结点
    int _capacity; //Cache的容量 其实就是双向链表的长度
    int _count ; //计数
};


}//end of namespace hk
