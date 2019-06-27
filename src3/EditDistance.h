#pragma once
#include <iostream>
#include <string>

using namespace std;

namespace hk
{

class EditDistance
{
public:
    EditDistance(const string & word1,const string & word2);
    int get();

private:
    string _word1;
    string _word2;

};


}//end of namespace hk;
