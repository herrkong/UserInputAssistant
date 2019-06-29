#pragma once
#include "Configuration.h"
#include "SplitTool.h"
#include <iostream>
#include <vector>
#include <memory>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <map>
using std::endl;
using std::cout;

namespace hk
{

class DictProducer
{

    using YuLiao_FILE = vector<string>;
    using Dict =map<string,int>;

public:
    explicit DictProducer(const string & dir,const string & ch_dir,const string & filepath);
    
    void build_dict();  //创建英文词典
    void build_chdict();//创建中文词典
    void store_dict(); //将词典写入文件
    void show_files() const ; //查看文件路径，做测试用
    void show_dict() const; //查看词典，做测试用
    
    //private:
    //void push_dict(const string & word);//存储某个单词


private:
    string _dir;           //英文语料库文件存放路径../data/english_file
    string _ch_dir;        //中文语料库存放路径 ../data/chinese_file
    YuLiao_FILE _files;    //保存英文语料库文件的文件名vector
    YuLiao_FILE _ch_files;//保存中文语料库文件的文件名vector
    Dict _dict;            //所有文件导出成词典 中文英文大词典
    string _filepath;     //存放词典内容的地址 
    
};

}//end of namespace hk
