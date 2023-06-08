#include "tree.h"
#include <vector>


using namespace std;


int main(){
    auto* obj1 = new TreeBuilder(5, 10, "test.bin");
    obj1->findSubtreeMostCommonSum();

    obj1->serialize();
    auto* new_root = obj1->deserialize();


    return 0;
}