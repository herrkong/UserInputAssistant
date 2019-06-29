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

int IndexProducer::nBytesCode(const char ch)
{
    //计算每个字的长度
    if(ch & (1<<7))
    {
        int nBytes = 1;
        for(int i =0;i!=6;++i)
        {   //utf-8最大占6个字节
            if(ch & (1<<(6-i)))
            {
                ++nBytes;
            }
            else
            {
                break;
            }
        }//for
        return nBytes;
    }//if
    return 1;
}


#if 0
//需要修改这一块代码了。中英文索引。
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
#endif

void IndexProducer::build_index()
{
    string word;
    int location=0; //单词或字的下标
    for(auto & pair :_vecdict )
    {
        word=pair.first;
        int nBytes;
        string ch;
        size_t cur = 0;
        while(cur != word.size())
        {
           nBytes = nBytesCode(word[cur]);//获取当前字所占字符数
           ch = word.substr(cur,nBytes);
           //将word前nBytes个字符解析成单个字
           cur+=nBytes;
           _mapindex[ch].insert(location);
        }//while
        location++;
    }
   // cout<<"创建索引完成。"<<endl;
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

