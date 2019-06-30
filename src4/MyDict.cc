#include "MyDict.h"
#include <fstream>
#include <sstream>

namespace  hk
{


//初始化中英文词典和索引表
void MyDict::en_init(const char * EnDictFilePath,const char * EnIndexFilePath)
{

    ifstream ifsEnDict(EnDictFilePath);//文件流
    string word;
    int wordFreq;
    while(ifsEnDict>>word>>wordFreq)
    {
        _dict.push_back(make_pair(word,wordFreq));
    }
    ifsEnDict.close();



    string line;
    ifstream ifsEnIndex(EnIndexFilePath);
    int location,k=1;
    while(getline(ifsEnIndex,line)) //换行符也拿到了
    {
        stringstream ss(line);//字符串流
        if(k%2 != 0)  //奇数行是字母
        {
            while(ss>>word);
        }
        else         //偶数行是索引下标
        {
            while(ss>>location)
            {
                _indexTable[word].insert(location);
            }
        }
        k++;
    }//while
    ifsEnIndex.close();
}




#if 0    //有些瑕疵
void MyDict::init(const char * dictFilePath,const char * indexFilePath)
{

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
}

#endif

Dict & MyDict::getDict()
{
    return _dict;
}

IndexTable & MyDict::getIndexTable()
{
    return _indexTable;
}

MyDict * MyDict::_pInstance = MyDict::getInstance(); //饱汉模式

}//end of namespace hk
