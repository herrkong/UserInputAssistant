#include "UserInputServer.h"

using namespace hk;

MyDict * MyDict::_pInstance = new MyDict();  //饱汉模式


int main(void)
{
    UserInputServer server("/home/schalke04/UserInputServer/conf");
    server.start();
    return 0;
}

