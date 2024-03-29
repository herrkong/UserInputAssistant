
#pragma once
#include "MyTask.h"
#include "EditDistance.h"
#include  "Thread.h"
namespace  hk
{

MyTask::MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn)
:_dict(dict)
,_queryWord(queryWord)
,_conn(conn)
,_pCacheManager(CacheManager::getCacheManager()) 
//拿到cacheManager对象的指针
{
    cout<<"MyTask()"<<endl;
}

//运行在线程池的某个子线程中
void MyTask::execute()
{
    cout<<"我是 "<<threadNum<<" 线程,线程ID = "<<pthread_self()<<endl;
    //decode
   // CacheManager * _pCacheManager = CacheManager::getCacheManager();
    string results;
    Cache & cache = _pCacheManager->getCache(threadNum);
    //在子线程缓存中查找
    //cout<<"xxxx"<<endl;
    string hotdata = cache.get(_queryWord);
    cout<<"debug 1"<<endl;
   
    if(!results.empty())
    {
        //子线程缓存命中
        cout<<threadNum<<"号子线程缓存命中"<<endl;
       // _conn->sendInLoop(results);
        results = hotdata ;
    }
    else
    {
        //子线程缓存未命中
        string retjson = _pCacheManager->getCache(0).get(_queryWord);
        if(!retjson.empty())
        {
            cout<<threadNum<<"号子线程缓存未命中,主线程缓存命中."<<endl;
            //_conn->sendInLoop(retjson);
            results = retjson ;
        }
        else
        {
            cout<<threadNum<<"号子线程和主线程缓存都没有命中"<<endl;
            queryIndexTable();
            //compute
            insertQueue();
            //response(cache);//重载了response nb
            //写进缓存
            createJson(results);
            cache.addElement(_queryWord,results);
        }
    }//else

    cout<<results<<endl;
    _conn->sendInLoop(results);

}

void MyTask::createJson(string & results)
{
    Json::Value data ;
    for(int i = 0 ; !_resultQue.empty() && i <3 ; ++i)
    {
        data[_queryWord].append(_resultQue.top()._word);
        _resultQue.pop();
    }
    Json::FastWriter fw ;
    results = fw.write(data);
}



int MyTask::length(const string & str)
{
    int len = 0 ;
    for(size_t i = 0;i != str.size();++i)
    {
        int nBytes = nBytesCode(str[i]);
        i+=(nBytes -1);
        ++len;
    }
    return len;
}


void MyTask::queryIndexTable()
{
    //先拿到词典和索引表
    auto dict = _dict.getDict();
    auto indexTable =_dict.getIndexTable();    
    
    set<string>  WordChars;//去重后的单词和汉字
    size_t cur = 0;

    while(cur != _queryWord.size())
    {
        //去重单词和汉字
        int nBytes = nBytesCode(_queryWord[cur]);
        string ch = _queryWord.substr(cur,nBytes);
        cur += nBytes;
        WordChars.insert(ch);
    }

    for(auto & ch:WordChars)
    {
        //每一个字母和汉字都对应一个set,保存着相应单词的行号。
        statistic(indexTable[ch]);
    }
}


void MyTask::statistic(set<int> & iset)
{

    auto dict = _dict.getDict();
    //计算出下标对应的单词并存储
    for(auto & location:iset)
    {
        if((distance(dict[location].first) <=4))
        {
            _words.insert(location); //没有重复的单词下标           
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


#if 0
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
#endif

int MyTask::nBytesCode(const char ch)
{
    if(ch & (1<<7))
    {
        int nBytes = 1;
        for(int i =0 ; i!=6;++i)
        {
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






int MyTask::distance(const string & rhs)
{
    //计算编辑距离修改为中英文都可以
   // EditDistance editdist(_queryWord,rhs);
   // return editdist.get();
    
    
    string word1=_queryWord;
    string word2= rhs;
    int m=length(word1);//客户端传来的单词或词语
    int n=length(word2);//词典中的单词或词语
    vector<string> words1;//将每个字符或汉字存入数组
    vector<string> words2;//将每个字符或汉字存入数组
    size_t cur=0;
    while(cur!=word1.size())
    {//如果是单词，则转为小写
        int n=nBytesCode(word1[cur]);
        if(n==1)
            word1[cur]=tolower(word1[cur]);
        string ch=word1.substr(cur,n);
        words1.push_back(ch);//将Word1存到数组中
        cur+=n;
    }
    cur=0;
    while(cur!=word2.size())
    {
        int n=nBytesCode(word2[cur]);
        string ch=word2.substr(cur,n);
        words2.push_back(ch);//将Word2存到数组中
        cur+=n;
    }
    if(m*n==0)  return m+n;
    vector<vector<int> > d(m+1,vector<int>(n+1));
    for(int i=0;i<=m;++i){  d[i][0]=i;  }
    for(int j=0;j<=n;++j){  d[0][j]=j;  }
    for(int i=1;i<=m;++i)
    {
        for(int j=1;j<=n;++j)
        {
            if(words1[i-1]==words2[j-1]){
                d[i][j]=d[i-1][j-1];
            }else{
                d[i][j]=1+min(d[i-1][j-1],min(d[i-1][j],d[i][j-1]));
            }
        }
    }
    return d[m][n];

}

# if 0 
void MyTask::response(Cache & cache)
{   
    //采用json数据格式发送
    Json::Value root;
    
    if(_resultQue.empty())
    {  
        _conn->sendInLoop("No match words");
    }
    else
    {

        size_t n = 3 ;//发送三个候选词
        for(size_t idx = 0 ;idx < n ;idx++)
        {
            root[idx] =Json::Value(_resultQue.top()._word);
            cout<<root[idx].asString()<<" ";
            //带引号的string 
            _resultQue.pop();
        }

#if 0
        root["word1"] = Json::Value(_resultQue.top()._word);
        _resultQue.pop();
        root["word2"] = Json::Value(_resultQue.top()._word);
        _resultQue.pop();
        root["word3"] = Json::Value(_resultQue.top()._word);
#endif

        Json::FastWriter fw;
        //stringstream ss;
       // ss<<fw.write(root);
       // string  ans; 
       // ss>>ans;
        string ans = fw.write(root);
        _conn->sendInLoop(ans);
        ResultQue empty;
        cache.addElement(_queryWord,ans);
        _resultQue.swap(empty);//清空队列
   
    }//else
}
#endif

}//end of namespace hk
