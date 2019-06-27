#include "MyTask.h"
#include "EditDistance.h"

namespace  hk
{


MyTask::MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn)
:_dict(dict)
,_queryWord(queryWord)
,_conn(conn)
{
   // cout<<"MyTask()"<<endl;
}

void MyTask::execute()
{
    queryIndexTable();//先检查索引 再计算编辑距离
    insertQueue();
    response();
}

void MyTask::statistic(set<int> & iset)
{
    auto dict = _dict.getDict();
    //防止单词重复push 先将位置插入set中过滤一遍
    
    for(auto & location :iset)
    {
        if((distance(dict[location].first)) <= 4)
        {
            _words.insert(location);//没重复的单词下标
        }
    }
}

void MyTask::insertQueue()
{
    auto dict = _dict.getDict();
    MyResult temp;

    for(auto & it:_words)
    {
        temp._word = dict[it].first;
        temp._iFreq = dict[it].second;
        temp._iDist = distance(temp._word);
        _resultQue.push(temp);
    }
}


void MyTask::queryIndexTable()
{
    auto indexTable = _dict.getIndexTable();

    set<string> WordChars;
    //去除重复字母
    for(size_t idx =0 ; idx != _queryWord.size() ; ++idx)
    {
        string ch = string(1,_queryWord[idx]);
       
        WordChars.insert(ch);
    }
    
    //遍历每个字母对应的set 做statistic操作
    for(auto & ch:WordChars)
    {
        statistic(indexTable[ch]);        
    }
}

int MyTask::distance(const string & rhs)
{
    EditDistance editdist(_queryWord,rhs);
    return editdist.get();
}


void MyTask::response()
{   
    //采用json数据格式发送
    Json::Value root;
    
    if(_resultQue.empty())
    {  
        _conn->sendInLoop("No match words");
    }
    else
    {
        root["word1"] = Json::Value(_resultQue.top()._word);
        _resultQue.pop();
        root["word2"] = Json::Value(_resultQue.top()._word);
        _resultQue.pop();
        root["word3"] = Json::Value(_resultQue.top()._word);

        Json::FastWriter fw;
        stringstream ss;
        ss<<fw.write(root);
        string  ans; 
        ss>>ans;
        
        _conn->sendInLoop(ans);
        ResultQue empty;
        _resultQue.swap(empty);//清空队列
    }
}

}//end of namespace hk
