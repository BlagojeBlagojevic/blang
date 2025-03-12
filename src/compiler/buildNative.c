#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BVM_IMPLEMENTATION
#include "bvm.h"

#define NOT_IMPLEMENTED()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;NOT IMPLEMENTED %s\n", instructionNames[vm.instruction[i].type]);\
    fputs(buffer, f);}    

#define PUSH_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;push %ld\n\tpush %ld\n", vm.instruction[i].operand._asI64,vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}


#define ADD_INT_ASM()\
{char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;add\n\tpop rax\n\tpop rbx\n\tadd rax, rbx\n\tpush rax\n");\
    fputs(buffer, f);}

#define SUB_INT_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;sub\n\tpop rax\n\tpop rbx\n\tsub rax, rbx\n\tpush rax\n");\
        fputs(buffer, f);}

#define PRINT_INT_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;print\n\tpop rdi\n\tcall PRINT_INT\n");\
    fputs(buffer, f);}
    
        

#define PRINT_INT_ASM_INIT()\
    {fputs("\nPRINT_INT:",f);\
    fputs("\n\tsub     rsp, 40", f);\
    fputs("\n\txor     ecx, ecx", f);\
    fputs("\n\tmov     r8d, 10", f);\
    fputs("\n\tmov     rsi, rsp", f);\
    fputs("\n\tlea     r10, [rsp+31]", f);\
    fputs("\n.L2:", f);\
    fputs("\n\tmov     rax, rdi", f);\
    fputs("\n\txor     edx, edx", f);\
    fputs("\n\tmov     r9, r10", f);\
    fputs("\n\tdiv     r8", f);\
    fputs("\n\tsub     r9, rcx", f);\
    fputs("\n\tinc     rcx", f);\
    fputs("\n\tadd     edx, 48", f);\
    fputs("\n\tmov     BYTE [r9], dl", f);\
    fputs("\n\tmov     rdx, rdi", f);\
    fputs("\n\tmov     rdi, rax", f);\
    fputs("\n\tcmp     rdx, 9", f);\
    fputs("\n\tja      .L2", f);\
    fputs("\n\tsub     rsi, rcx", f);\
    fputs("\n\tmov     rdx, rcx", f);\
    fputs("\n\tmov     edi, 1", f);\
    fputs("\n\tadd     rsi, 32", f);\
    fputs("\n\tmov rax, 1", f);\
    fputs("\n\tsyscall", f);\
    fputs("\n\tadd     rsp, 40", f);\
    fputs("\n\tret\n", f);}

int main(){
    Bvm vm = initBVM(); 
    binToProgram("code.vm", vm.instruction);
    FILE *f = fopen("nasmAsm.asm", "w");
    if(f == NULL) return -1;
    fputs("segment .text\n", f);
    PRINT_INT_ASM_INIT();
    fputs("global _start\n", f);
    fputs("_start:\n", f);

    //fputs(headerBuffer, f);
    for(u64 i = 0; vm.instruction[i].type != END; i++){
        switch (vm.instruction[i].type){
        case PUSH:{
            PUSH_ASM();
            break;
        }
        case ADD:{
            if(vm.instruction[i].operand._asI64 == 0){
                ADD_INT_ASM();
            }
            else{
                NOT_IMPLEMENTED();
            }
            break;
        }
        case DEC:{
            if(vm.instruction[i].operand._asI64 == 0){
                SUB_INT_ASM();
            }
            else{
                NOT_IMPLEMENTED();
            }
            break;
        }
        case PRINT:{
            if(vm.instruction[i].operand._asI64 == 0){
                PRINT_INT_ASM();
            }
            break;
        }
        default:{
            NOT_IMPLEMENTED();
            break;
        } 
        }
    }
    fputs("\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n", f);    
    fclose(f);
    system("nasm -f elf64 nasmAsm.asm -o nasmAsm.o");
    system("ld nasmAsm.o -o nativeApp");
    printf("Native Executable\n");
    return 0;

}
