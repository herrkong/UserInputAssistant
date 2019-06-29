#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

namespace hk
{

class IndexProducer
{
public:
    IndexProducer(const string & dictfilepath,const string & indexfilepath);
    void read_dict();   
    void build_index();
    int  nBytesCode(const char ch);//获取每个字所占字节数(新增)
    void store_index();
    void show_index();

private:
    string _dictfilepath;  //字典文件存放路径
    string _indexfilepath; //索引文件存放地址
    vector<pair<string,int>> _vecdict;//建个新字典接受文件中的字典内容
    map< string,set<int> > _mapindex;
    //字母,中文不止一个char,set是出现该字母的所有单词在vector中的下标。
};

}//end of namespace hk
