#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <json/json.h>
#include <string>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

using namespace std;


void do_service(int sockfd);

int recvCycle(int newFd,void * p,int len)
{
    int total = 0;
    int ret ;
    char * pStart = (char *)p;
    while(total < len)
    {
        ret = recv (newFd,pStart + total ,len -total,0);
        if(0 == ret)
        {
            return -1;
        }
        total =total + ret ;
    }
    return total;
}

typedef struct
{
    int dataLen;
    char buf[1024];

}Train_t;


void sendTrain(string buf,int sockfd)
{
    Train_t train;
    memset(&train,0,sizeof(train));
    train.dataLen = sizeof(buf);
    strcpy(train.buf,buf.c_str());
    send(sockfd,&train,4+train.dataLen,0);
}


int recvTrain(string & buff,int sockfd)
{
    Train_t train;
    memset(&train,0,sizeof(train));
    recvCycle(sockfd,&train.dataLen,4);
    int ret = recvCycle(sockfd,&train.buf,train.dataLen);
    buff =string(train.buf);
    return ret;
}



int main()
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    //PF_INET
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.182.128"); //localhost
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(8888);
    //去连接服务器端的8888端口
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");

    string buf;
    recvTrain(buf,peerfd);

    do_service(peerfd);

    return 0;

#if 0
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    read(peerfd, buf, sizeof(buf));
prin:tf("%s\n", buf);

    do_service(peerfd);
    return 0;
#endif

}



void do_service(int sockfd)
{
    //char recvbuf[1024] = {0};//接受缓冲区
    // char sendbuf[1024] = {0};//发送缓冲区
    string recvbuf;
    string sendbuf;
    recvbuf.resize(1024);
    recvbuf.resize(1024);



    while(1)
    {
        cout<<"请输入查询词:"<<endl;
        //fgets会将换行符也读取。
        // fgets(sendbuf, sizeof sendbuf, stdin);
        cin>>sendbuf;
        // write(sockfd, sendbuf, strlen(sendbuf));
        //sleep(5);
        sendTrain(sendbuf,sockfd);
        //read
        int nread = recvTrain(recvbuf,sockfd);

        if(nread == -1)
        {
            if(errno == EINTR)//中断操作
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("服务器断开!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        //else if(nread == 4)
        //{
        // cout<<"No match words"<<endl;
        //}
        string str= "No match words";
        if((recvbuf == str))
        {
            cout<<"没有找到匹配词语。"<<endl;
        }
        else
        {
            Json::Reader reader;
            Json::Value root;
            //parse用于从一个字符串中解析出json对象
#if 0
            if(reader.parse(recvbuf,root))
            {
                string word1 = root["word1"].asString();
                string word2 = root["word2"].asString();
                string word3 = root["word3"].asString();
                cout<<"从服务器接收到候选词:"<<endl;
                cout<< word1<<" "<<word2<<" "<<word3<<endl;
            }
            //stringify用于从一个对象解析出字符串，
#endif 
            //解析收到的jason字符串
            size_t n = 3 ;
            //vector<string> messages(3);
            cout<<"从服务器接收到候选词: "<<endl;
            if(reader.parse(recvbuf,root))
            {
                //cout<<"233"<<endl;
                for(size_t idx = 0;idx < n ;++idx)
                {


                    cout<<root[idx].asString()<<" ";
                   // cout<<"666"<<endl;
                   // messages[idx] = root[idx].asString();
                   // cout<<"777"<<endl;
                   // cout<<messages[idx]<<endl;
                   // cout<<"999"<<endl;
                }
            }

           // for(auto & word:messages)
           // {
               // cout<<word<<" ";
           // }
            cout<<endl;

            recvbuf.clear();
            sendbuf.clear();

            //printf("receive msg : %s\n", recvbuf);

            // memset(recvbuf, 0, sizeof recvbuf);
            // memset(sendbuf, 0, sizeof sendbuf);

        }
    }

}

