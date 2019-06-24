#include "EditDistance.h"
#include <vector>

using namespace std;

namespace hk
{


EditDistance::EditDistance(const string & word1,const string & word2)
:_word1(word1)
,_word2(word2)
{
    cout<<"EditDistance(const string & _word1,const string & _word2)"<<endl;
}

int EditDistance::get()
{
    int m = _word1.length();
    int n = _word2.length();

    //m+1行 n+1列
    vector<vector<int>>  cost(m+1,vector<int>(n+1));

    for(int i=0;i<=m;++i)
    {
        cost[i][0] = i;
    }

    for(int j=0;j<=n;++j)
    {
        cost[0][j]= j;
    }

    for(int i=1; i<=m ;++i)
    {
        for(int j=1;j<=n;++j)
        {
            if(_word1[i-1] == _word2[j-1])
            {
                cost[i][j] = cost[i-1][j-1];
            }
            else
            {
                cost[i][j] = 1 + min(cost[i-1][j-1],min(cost[i][j-1],cost[i-1][j]));
            }
        }
    }
    return cost[m][n];
}


}//end of namespace hk
