#include "../include/IndexProducer.h"
#include <iostream>
#include <fstream>

namespace hk
{

IndexProducer::IndexProducer(const string & dictfilepath,const string & indexfilepath)
:_dictfilepath(dictfilepath)
,_indexfilepath(indexfilepath)
{
    cout<<"IndexProducer(const string & dictfilepath,const string & indexfilepath)"<<endl;
}

void IndexProducer::read_dict()
{
    ifstream ifs(_dictfilepath);
    string word,num;
    int location =0;
    while(ifs>>word>>num)
    {
        _vecdict.push_back(make_pair(word,location++)); 
    }
    ifs.close();
}


void IndexProducer::build_index()
{
    for(char ch = 'a';ch<='z';++ch) 
    //debug5
    //检查data/index.dat发现少了z 这里少了等于号。 
    {
        for(size_t idx =0 ;idx<_vecdict.size();++idx)
        {
            //查找没有匹配返回npos
            if(_vecdict[idx].first.find(ch) != string::npos)
            {
                _mapindex[string(1,ch)].insert(_vecdict[idx].second);
                //包含这个字母的单词所在下标。
            }
        }
    }
}

void IndexProducer::store_index()
{
    ofstream ofs;
    ofs.open(_indexfilepath);
    for(auto it=_mapindex.begin();it!=_mapindex.end();++it)
    {
        ofs<<it->first<<endl;
        for(auto iter = it->second.begin();iter !=it->second.end();++iter)
        {
            ofs<<*iter<<" ";
        }
        ofs<<endl;
    }
    ofs.close();
}

void IndexProducer::show_index()
{
    for(auto it = _mapindex.begin();it!=_mapindex.end();++it)
    {
        cout<<it->first<<endl;
        for(auto iter = it->second.begin();iter!=it->second.end();++iter)
        {
            cout<<*iter<<" ";
        }
        cout<<endl;
    }
}


}//end of namespace hk

