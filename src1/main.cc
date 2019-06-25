#include "UserInputAssitant.h"

using namespace hk;

int main()
{
    //UserInputAssitant server("/home/schalke04/UserInputAssitant/conf"); 
    UserInputAssitant server("../conf"); 
    server.start();
    return 0;
}

