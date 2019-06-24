#include "MyDict.h"
#include <fstream>
#include <sstream>

namespace  hk
{

void MyDict::init(const char * dictFilePath,const char * indexFilePath)
{
    cout<<"进入MyDict了么？nmb"<<endl;
    ifstream ifsDict(dictFilePath);
    string word;
    int wordFreq;
    while(ifsDict>>word>>wordFreq)
    {
        _dict.push_back(make_pair(word,wordFreq));
    }
    ifsDict.close();

    ifstream ifsIndex(indexFilePath);
    string ch;
    while(ifsIndex>>ch)
    {
        string line;
        getline(ifsIndex,line);//读取字母后面的换行符
        getline(ifsIndex,line);
        stringstream ss(line);
        int num;
        while(ss>>num)
        {
            _indexTable[ch].insert(num);
        }
        ifsIndex.close();
    }
    cout<<"Debug2-->MyDict::init()"<<endl;
}

Dict & MyDict::getDict()
{
    return _dict;
}

IndexTable & MyDict::getIndexTable()
{
    return _indexTable;
}

}//end of namespace hk
