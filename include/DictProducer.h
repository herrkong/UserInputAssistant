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
   explicit DictProducer(const string & dir,const string & filepath);

#if 0
    DictProducer(const string & dir,SplitTool * splitTool)//中文处理
        :_dir(dir)
         ,_splitTool(splitTool)
    {
        cout<<"DictProducer(const string & dir,SplitTool * splitTool)"<<endl;
    }
#endif  

    void build_dict();  //创建英文词典
    //void build_cn_dict(); //创建中文词典
    void store_dict(); //将词典写入文件
    void show_files() const ; //查看文件路径，做测试用
    void show_dict() const; //查看词典，做测试用

    //private:
    //void push_dict(const string & word);//存储某个单词


private:
    string _dir;           //语料库文件存放路径../data/english_file
    YuLiao_FILE _files;    //保存语料库文件的文件名vector
    Dict _dict;            //所有文件导出成词典
    string _filepath;     //存放词典内容的地址 
    //SplitTool * _splitTool; //分词工具

};

}//end of namespace hk
