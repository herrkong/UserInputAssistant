#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/IndexProducer.h"

using namespace hk;

int main()
{
    Configuration conf("../conf");
    
    auto configMap = conf.getConfigMap();

    DictProducer dictProducer(configMap["dir"],configMap["ch_dir"], configMap["dict"]); 

    dictProducer.build_dict();
    dictProducer.build_chdict();
    dictProducer.store_dict();
    //dictProducer.show_dict();
    
    IndexProducer indexProducer(configMap["dict"], configMap["index"]);
    indexProducer.read_dict();
    indexProducer.build_index();
    //indexProducer.show_index();
    indexProducer.store_index();

    return 0;
}

