#include "../include/DictProducer.h"
#include <iostream>
#include <fstream>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <iterator>

namespace  hk
{
//初始化词典 传入语料库地址 待装入词典路径
DictProducer::DictProducer(const string & dir,const string & filepath)
:_dir(dir)
,_filepath(filepath)
{
    //把语料库中的所有文件名搞到vector中
    DIR * d = opendir(_dir.c_str());
    //cout<<"debug2"<<endl;
    //cout<<_dir<<endl;
    if(!d)
    {
        perror("opendir");

    }
   // cout<<"debug3"<<endl;
    struct dirent * p ;
    while((p = readdir(d)))
    {
       // cout<<"debug4"<<endl;//没进入while循环
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
        {
            continue;
        }                                                                      
        _files.push_back(p->d_name);

    }
    cout<<"DictProducer(const string & dir)"<<endl;

}


void DictProducer::build_dict() //创建英文词典
{

    for(auto it=_files.begin() ;it!=_files.end(); ++it )
    {
        //cout<<"debug1"<<endl;
        string filepath =
        ("../data/english_file/")+ *it;
        //cout<<"filepath: "<<filepath<<endl;
        ifstream ifs;
        ifs.open(filepath);
        if(!ifs.good())
        {
            perror("open");
        }
        string word;
        while(ifs>>word)
        {
            if(!isalpha(word[0])) continue;
            while(!isalpha(word[word.size()-1]))
            {
                word.erase(word.end() -1);
            }
            _dict[word]++;
        }   
    }
}

#if 0
//待补充
void DictProducer::build_cn_dict()//创建中文词典
{

}
#endif

void DictProducer::store_dict()//将词典写入文件
{
    ofstream ofs;
    ofs.open(_filepath);
    for(auto it = _dict.begin();it!=_dict.end();++ it)
    {
        ofs<<it->first<<"  "<<it->second <<endl;
    }
    ofs.close();
}

void DictProducer::show_files() const //查看文件路径
{
    for(auto & filename:_files)
    {
        cout<<("../data/english_file/")+filename<<endl;
    }
    cout<<endl;
}

void DictProducer::show_dict() const //查看词典
{
    for(auto it =_dict.begin();it!=_dict.end();++it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }
}

#if 0
void DictProducer::push_dict(const string & word) //存储某个单词
{}
#endif


}//end of namespace hk
