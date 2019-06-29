#include "../include/DictProducer.h"
#include "../include/cppjieba/Jieba.hpp"
#include <iostream>
#include <fstream>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <iterator>


namespace  hk
{


const char * const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";////最大概率法分词所使用的词典路径
const char * const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型
const char * const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char * const IDF_PATH = "../include/cppjieba/dict/idf.utf8"; //IDF路径
const char * const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";//分割词路径



//初始化词典 传入语料库地址 待装入词典路径
DictProducer::DictProducer(const string & dir,const string & ch_dir,const string & filepath)
:_dir(dir)
,_ch_dir(ch_dir)
,_filepath(filepath)
{
    //把语料库中的英文文件名搞到vector中
    DIR * d = opendir(_dir.c_str());
    if(!d)
    {
        perror("opendir");

    }
    struct dirent * p ;

    while((p = readdir(d)))
    {
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
        {
            continue;
        }                                                                      
        _files.push_back(p->d_name);

    }
    closedir(d);
    
    //处理中文语料
    DIR * d_ch = opendir(_ch_dir.c_str());
    if(!d_ch)
    {
        perror("opendir");
    }    
    
    struct dirent * p_ch;

    while((p_ch = readdir(d_ch)))
    {
        if(!strcmp(p_ch->d_name,".") || !strcmp(p_ch->d_name,".."))
        {
            continue;
        }
        _ch_files.push_back(p_ch->d_name);
    }
    closedir(d_ch);
    
    cout<<"DictProducer(const string & dir,const string & ch_dir,const string & filepath )"<<endl;

}


void DictProducer::build_dict() //创建英文词典
{
 
    for(auto it=_files.begin() ;it!=_files.end(); ++it )
    {
        string filepath =
        ("../data/english_file/")+ *it;
        cout<<filepath<<endl;
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
    }//for

}

void DictProducer::build_chdict()  //创建中文词典
{
    using namespace cppjieba;
    for(auto & file:_ch_files)//遍历一份中文文件
    {
        string filepath = ("../data/chinese_file/") + file ;
        cout<<filepath<<endl;
        ifstream ifs;
        ifs.open(filepath);
        if(!ifs.good())
        {
            perror("open");
        }
        Jieba jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,
                        STOP_WORD_PATH);
        string fulltext;
        while(ifs>>fulltext) //对中文进行分词
        {
            vector<string> result;
            jieba.Cut(fulltext,result,true);
            for(auto & word:result)
            {
               // cout<<word<<endl;
                ++_dict[word];
            }
        }//while
        ifs.close();
    }//for

}


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

    for(auto & filename:_ch_files)
    {
        cout<<("../data/chinese_file/")+filename<<endl;
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
