//THIS FILE IS FOR BUILDING executable FIRST  make writeHeader
//IT GENERATES generatedInst.h -Wall -Wextra fail i do not now why
//Then buildExe or buildExeSdl dependhing what you want

#define BVM_IMPLEMENTATION
#include "bvm.h"
#include "generatedInst.h"

int main(){
    Bvm vm = initBVM();
    memcpy(vm.instruction, inst, sizeof(inst));
   
    loop(&vm);
    return 0;
}
