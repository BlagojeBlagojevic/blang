#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BVM_IMPLEMENTATION
#include "bvm.h"

#define NOT_IMPLEMENTED()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;NOT IMPLEMENTED %s\n.LABEL%zu:\n", instructionNames[vm.instruction[i].type], numOfInstruction);\
    fputs(buffer, f);}  
    
#define NOP_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;nop\n.LABEL%zu:\n", numOfInstruction);\
    fputs(buffer, f);}  

#define PUSH_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;push %ld\n.LABEL%zu:\n\tpush %ld\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}
   
#define PUSHSP_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;pushsp\n.LABEL%zu:\n\tmov rax, rsp\n\t\n\tmov rbx,  new_stack + 1000000\n\tsub rax, rbx\n\tshr rax, 3\n\tpush rax\n", numOfInstruction);\
    fputs(buffer, f);}


#define POP_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;pop %ld\n.LABEL%zu:\n\tpop rax\n", vm.instruction[i].operand._asI64, numOfInstruction);\
    fputs(buffer, f);}

#define DUP_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;dup\n.LABEL%zu:\n\tpop rax\n\tpush rax\n\tpush rax\n", numOfInstruction);\
    fputs(buffer, f);}



#define MEM_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;mem %ld\n.LABEL%zu:\n\tpop rax\n\tmov qword [new_stack + (%ld * 8)], rax  ; Store at explicit address\n\tpush rax\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}    

#define MEM_STACK_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;memstack %ld\n.LABEL%zu:\n\tpop rax\n\tshl rax, 3\n\tpop rbx\n\tmov [new_stack + rax], rbx\n\tpush rbx\n", vm.instruction[i].operand._asI64, numOfInstruction);\
    fputs(buffer, f);}



#define COPY_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;copy %ld\n.LABEL%zu:\n\tmov rax, qword [new_stack + (%ld * 8)]   ; Load from explicit address\n\tpush rax\n", vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
    fputs(buffer, f);}


#define COPY_STACK_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;copystack %ld\n.LABEL%zu:pop rax\n\tshl rax, 3\n\tmov rbx, [new_stack + rax]\n\tpush rbx\n", vm.instruction[i].operand._asI64, numOfInstruction);\
    fputs(buffer, f);}

#define SETSPSTACK_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;setspstack %ld\n.LABEL%zu:\n\tpop rsp\n\timul rsp, 8\n\tadd rsp, new_stack + 1000000\n", vm.instruction[i].operand._asI64, numOfInstruction);\
    fputs(buffer, f);}



#define ADD_INT_ASM()\
{char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;add\n.LABEL%zu:\n\tpop rax\n\tpop rbx\n\tadd rax, rbx\n\tpush rax\n", numOfInstruction);\
    fputs(buffer, f);}

#define SUB_INT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;sub\n.LABEL%zu:\n\tpop rax\n\tpop rbx\n\tsub rax, rbx\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}

#define MUL_INT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;mul\n.LABEL%zu:\n\tpop rax\n\tpop rdi\n\timul rax, rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}

#define DIV_INT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;div\n.LABEL%zu:\n\tpop rax\n\tpop rdi\n\tcqo\n\tidiv rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}   

#define MOD_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;MOD\n.LABEL%zu:\n\tpop rdi\n\tpop rsi\n\tcall MOD\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}   

#define SHL_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;shl\n.LABEL%zu:\n\tpop rcx\n\tpop rax\n\tshl rax, cl\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}   
    

#define SHR_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;shr\n.LABEL%zu:\n\t\n\tpop rcx\n\tpop rax\n\tshr rax, cl\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}   
        
#define XOR_ASM()\
    {char buffer[256];\
            memset(buffer, 0, sizeof(char)*256);\
            sprintf(buffer, "\n\t;;xor\n.LABEL%zu:\n\tpop rdi\n\tpop rax\n\txor rax, rdi\n\tpush rax\n", numOfInstruction);\
            fputs(buffer, f);}   

#define OR_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;or\n.LABEL%zu:\n\tpop rdi\n\tpop rax\n\tor rax, rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);} 

#define AND_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;and\n.LABEL%zu:\n\tpop rdi\n\tpop rax\n\tand rax, rdi\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}  

#define NOT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;not\n.LABEL%zu:\n\tpop rax\n\txor rax, -1\n\tpush rax\n", numOfInstruction);\
        fputs(buffer, f);}         

#define OVER_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;over\n.LABEL%zu:\n\tpop rax\n\tpop rdx\n\tpush rdx\n\tpush rax\n\tpush rdx\n", numOfInstruction);\
        fputs(buffer, f);}         

#define ROT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;rot\n.LABEL%zu:\n\tpop rax\n\tpop rdx\n\tpop rdi\n\tpush rax\n\tpush rdi\n\tpush rdx\n", numOfInstruction);\
        fputs(buffer, f);}    

#define SWAPNO_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;swap_no\n.LABEL%zu:\n\tpop rax\n\tpop rdx\n\tpush rax\n\tpush rdx\n", numOfInstruction);\
        fputs(buffer, f);}    
    

#define IF_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;if %ld\n.LABEL%zu:\n\tpop rdi\n\tpop rsi\n\tmov rdx, %ld\n\tcall COMPARE\n\tpush rax\n",vm.instruction[i].operand._asI64, numOfInstruction, vm.instruction[i].operand._asI64);\
        fputs(buffer, f);}             
    

#define JMP_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;jmp\n.LABEL%zu:\n\tjmp .LABEL%zu\n", numOfInstruction, vm.instruction[i].operand._asI64);\
        fputs(buffer, f);}             

#define JMPF_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;jmpf\n.LABEL%zu:\n\tpop rax\n\ttest rax, rax   ; Check if false (0)\n\tjz .LABEL%zu\n", numOfInstruction, vm.instruction[i].operand._asI64);\
        fputs(buffer, f);}           

#define JMPT_ASM()\
    {char buffer[256];\
        memset(buffer, 0, sizeof(char)*256);\
        sprintf(buffer, "\n\t;;jmpf\n.LABEL%zu:\n\tpop rax\n\ttest rax, rax   ; Check if false (0)\n\tjz .LABEL%zu\n", numOfInstruction, vm.instruction[i].operand._asI64);\
        fputs(buffer, f);}           
    

#define PRINT_INT_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;printInt\n.LABEL%zu:\n\tpop rdi\n\tcall PRINT_INT\n\tpush rdi\n", numOfInstruction);\
    fputs(buffer, f);}

#define PRINT_CHAR_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t;;printChar\n.LABEL%zu:\n\tmov rax, 1\n\tmov rdi, 1\n\tmov rsi, rsp\n\tmov rdx, 1\n\tsyscall\n", numOfInstruction);\
    fputs(buffer, f);}

#define END_ASM()\
    {char buffer[256];\
    memset(buffer, 0, sizeof(char)*256);\
    sprintf(buffer, "\n\t; END,\n.LABEL%zu:\n\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n", numOfInstruction);\
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

#define COMPARE_INIT()\
    fputs("\nCOMPARE:", f);\
    fputs("\n\ttest    rdx, rdx", f);\
    fputs("\n\tjne     .L2", f);\
    fputs("\n\tcmp     rsi, rdi", f);\
    fputs("\n\tjmp     .L6", f);\
    fputs("\n.L2:", f);\
    fputs("\n\tdec     rdx", f);\
    fputs("\n\tjne     .L4", f);\
    fputs("\n\tcmp     rdi, rsi", f);\
    fputs("\n.L6:", f);\
    fputs("\n\tsetb    al", f);\
    fputs("\n\tjmp     .L5", f);\
    fputs("\n.L4:", f);\
    fputs("\n\tcmp     rdi, rsi", f);\
    fputs("\n\tsete    al", f);\
    fputs("\n.L5:", f);\
    fputs("\n\tmovzx   eax, al   ; Zero-extend AL into EAX", f);\
    fputs("\n\tret               ; Return to the caller\n", f);\

#define MOD_INIT()\
    fputs("\nMOD:", f);\
    fputs("\n\tmov     rax, rdi", f);\
    fputs("\n\tcdq", f);\
    fputs("\n\tidiv    rsi", f);\
    fputs("\n\tmov     rax, rdx", f);\
    fputs("\n\tret\n", f);\


int main(){
    Bvm vm = initBVM(); 
    binToProgram("code.vm", vm.instruction);
    FILE *f = fopen("nasmAsm.asm", "w");
    if(f == NULL) return -1;
    fputs("section .bss\n\tnew_stack resq 1000000  ; Reserve 1MB of qword for the new stack\n", f); //PROB SEND THIS AS A CONST WHEN WE CREATE A PROGRAM
    fputs("segment .text\n", f);
    PRINT_INT_ASM_INIT();
    PRINT_CHAR_INIT_ASM();
    COMPARE_INIT();
    MOD_INIT();
    //STR_LABEL_RDI();
    fputs("global _start\n", f);
    fputs("_start:\n", f);
    fputs("; Switch to new stack (aligned)\n\tmov rsp, new_stack + 1000000\n", f);

    //fputs(headerBuffer, f);
    size_t numOfInstruction = 0;
    for(u64 i = 0; vm.instruction[i].type != END; i++){
        switch (vm.instruction[i].type){
        case PUSH:{
            PUSH_ASM();
            break;
        }
        case PUSHSP:{
            PUSHSP_ASM();
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
            
                ADD_INT_ASM();
            
            break;
        }
        case DEC:{
            
                SUB_INT_ASM();
            
            break;
        }
        case MUL:{
            
                MUL_INT_ASM();
            break;
        }
        case DIV:{
            
                DIV_INT_ASM();
            
            break;
        }
        case MOD:{
            MOD_ASM();
            break;
        }
        case SHL:{
            SHL_ASM();
            break;
        }
        case SHR:{
            SHR_ASM();
            break;
        }
        case AND:{
            AND_ASM();
            break;
        }
        case OR:{
            OR_ASM();
            break;
        }
        case BNOT:{
            NOT_ASM();
            break;
        }
        case OVER:{
            OVER_ASM();
            break;
        }
        case ROT:{
            ROT_ASM();
            break;
        }
        case SWAP_NO:{
            SWAPNO_ASM();
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
        case JMPT:{
            JMPT_ASM();
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
        case SETSPSTACK:{
            SETSPSTACK_ASM();
            break;
        }
        case NOP:{
            NOP_ASM();
            break;
        }
        default:{
            NOT_IMPLEMENTED();
            break;
        }
        }
        numOfInstruction++;  
    }
    END_ASM();    
    fclose(f);
    system("nasm -f elf64 nasmAsm.asm -o nasmAsm.o ");
    system("ld nasmAsm.o -o nativeApp");
    printf("Native Executable\n");
    return 0;

}
