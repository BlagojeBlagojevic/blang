#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BVM_IMPLEMENTATION
#include "bvm.h"

#define NOT_IMPLEMENTED()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;NOT IMPLEMENTED %s\n.LABEL%zu:\n", instructionNames[vm.instruction[i].type], numOfInstruction);\
    fputs(buffer, f);}    

#define PUSH_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;push %ld\n.LABEL%zu:\n\tpush %ld\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}

#define POP_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;pop %ld\n.LABEL%zu:\n\tpop rax\n", vm.instruction[i].operand._asI64, numOfInstruction);\
    fputs(buffer, f);}

#define DUP_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;dup\n.LABEL%zu:\n\tpop rax\n\tpush rax\n\tpush rax\n", numOfInstruction);\
    fputs(buffer, f);}


#define MEM_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;mem %ld\n.LABEL%zu:\n\tpop rax\n\tmov QWORD [-%ld], rax\n\tpush rax\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}

#define MEM_STACK_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;memstack\n.LABEL%zu:\n\tpop rax\n\timul rax, -1\n\tpop rbx\n\tpush rbx\n\tmov QWORD [rax], rbx\n", numOfInstruction);\
    fputs(buffer, f);}


#define COPY_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;copy %ld\n.LABEL%zu:\n\tmov rax,  QWORD [-%ld]\n\tpush rax\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}

#define COPY_STACK_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;copystack\n.LABEL%zu:\n\tpop rax\n\timul rax, -1\n\tmov rbx, QWORD [rax]\n\tpush rbx\n",numOfInstruction);\
    fputs(buffer, f);}


#define ADD_INT_ASM()\
{char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;add\n.LABEL%zu:\n\tpop rax\n\tpop rbx\n\tadd rax, rbx\n\tpush rax\n", numOfInstruction);\
    fputs(buffer, f);}

#define SUB_INT_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;sub\n.LABEL%zu:\n\tpop rax\n\tpop rbx\n\tsub rax, rbx\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}

#define MUL_INT_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;mul\n.LABEL%zu:\n\tpop rax\n\tpop rdi\n\timul rax, rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}

#define DIV_INT_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;div\n.LABEL%zu:\n\tpop rax\n\tpop rdi\n\tcqo\n\tidiv rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}   

#define IF_ASM()\
        {char buffer[128];\
            memset(buffer, 0, sizeof(char)*128);\
            sprintf(buffer, "\n\t;;if\n.LABEL%zu:\n\tpop rax\n\tpop rbx\n\tcmp rax, rbx\n", numOfInstruction);\
            fputs(buffer, f);}             
    

#define JMP_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;jmp\n.LABEL%zu:\n\tpop rdi\n\tjmp .LABEL%zu\n", numOfInstruction, vm.instruction[i].operand._asI64 - 1);\
        fputs(buffer, f);}             

#define JMPF_ASM()\
    {char buffer[128];\
        memset(buffer, 0, sizeof(char)*128);\
        sprintf(buffer, "\n\t;;jmpf\n.LABEL%zu:\n\tpop rdi\n\tjnz .LABEL%zu\n", numOfInstruction, vm.instruction[i].operand._asI64 - 1);\
        fputs(buffer, f);}           

#define PRINT_INT_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;printInt\n.LABEL%zu:\n\tpop rdi\n\tcall PRINT_INT\n\tpush rdi\n", numOfInstruction);\
    fputs(buffer, f);}

#define PRINT_CHAR_ASM()\
    {char buffer[128];\
    memset(buffer, 0, sizeof(char)*128);\
    sprintf(buffer, "\n\t;;printChar\n.LABEL%zu:\n\tpop rdi\n\tcall PRINT_CHAR\n", numOfInstruction);\
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

#define PRINT_CHAR_INIT_ASM()\
   {fputs("\nPRINT_CHAR:", f);\
    fputs("\n\tsub     rsp, 24", f);\
    fputs("\n\tmov     edx, 1", f);\
    fputs("\n\tmov     BYTE  [rsp+15], dil", f);\
    fputs("\n\tlea     rsi, [rsp+15]", f);\
    fputs("\n\tmov     edi, 1", f);\
    fputs("\n\tmov rax, 1", f);\
    fputs("\n\tsyscall", f);\
    fputs("\n\tadd     rsp, 24", f);\
    fputs("\n\tret\n\n", f);}\

#define STR_LABEL_RDI()\
  {fputs("\nSTR_LABEL_RDI:", f);\
   fputs("\n\tmov     BYTE buffer[rip+4], 76", f);\
   fputs("\n\tlea     rsi, [rsp-32]", f);\
   fputs("\n\tmov     rax, rdi", f);\
   fputs("\n\tmov     edi, 10", f);\
   fputs("\n\tmov     DWORD buffer[rip], 1161969996", f);\
   fputs("\n\tmov     rcx, rsi", f);\
   fputs("\n\t.L2:", f);\
   fputs("\n\tcqo", f);\
   fputs("\n\tidiv    rdi", f);\
   fputs("\n\tadd     edx, 48", f);\
   fputs("\n\tmov     BYTE [rcx+31], dl", f);\
   fputs("\n\tmov     rdx, rcx", f);\
   fputs("\n\tdec     rcx", f);\
   fputs("\n\ttest    rax, rax", f);\
   fputs("\n\tjne     .L2", f);\
   fputs("\n\tmov     rax, rsi", f);\
   fputs("\n\tsub     rax, rdx", f);\
   fputs("\n\tmov     edx, buffer+5", f);\
   fputs("\n\tlea     rcx, [rax+1]", f);\
   fputs("\n\tmov     rdi, rdx", f);\
   fputs("\n\trep movsb", f);\
   fputs("\n\tmov     BYTE buffer[rax+6], 0", f);\
   fputs("\n\tmov     eax, buffer", f);\
   fputs("\n\tret\n\n", f);}\


int main(){
    Bvm vm = initBVM(); 
    binToProgram("code.vm", vm.instruction);
    FILE *f = fopen("nasmAsm.asm", "w");
    if(f == NULL) return -1;
    fputs("segment .text\n", f);
    PRINT_INT_ASM_INIT();
    PRINT_CHAR_INIT_ASM();
    //STR_LABEL_RDI();
    fputs("global _start\n", f);
    fputs("_start:\n", f);

    //fputs(headerBuffer, f);
    size_t numOfInstruction = 0;
    for(u64 i = 0; vm.instruction[i].type != END; i++){
        switch (vm.instruction[i].type){
        case PUSH:{
            PUSH_ASM();
            break;
        }
        case POP:{
            POP_ASM();
            break;
        }
        case DUP:{
            DUP_ASM();
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
        case MUL:{
            if(vm.instruction[i].operand._asI64 == 0){
                MUL_INT_ASM();
            }
            else{
                NOT_IMPLEMENTED();
            }
            break;
        }
        case DIV:{
            if(vm.instruction[i].operand._asI64 == 0){
                DIV_INT_ASM();
            }
            else{
                NOT_IMPLEMENTED();
            }
            break;
        }
        case PRINT:{
            if(vm.instruction[i].operand._asI64 == 0){
                PRINT_INT_ASM();
            }else if(vm.instruction[i].operand._asI64 == 3){
                PRINT_CHAR_ASM();
            }
            break;
        }
        case IF:{
            IF_ASM();
            break;
        }
        case JMP:{
            JMP_ASM();
            break;
        }
        case JMPF:{
            JMPF_ASM();
            break;
        }
        case MEM:{
            MEM_ASM();
            break;
        }
        case MEMSTACK:{
            MEM_STACK_ASM();
            break;
        }
        case COPY:{
            COPY_ASM();
            break;
        }
        case COPYSTACK:{
            COPY_STACK_ASM();
            break;
        }
        default:{
            NOT_IMPLEMENTED();
            break;
        }
        }
        numOfInstruction++;  
    }
    fputs("\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n", f);    
    fclose(f);
    system("nasm -f -ggdb elf64 nasmAsm.asm -o nasmAsm.o ");
    system("ld nasmAsm.o -o nativeApp");
    printf("Native Executable\n");
    return 0;

}
