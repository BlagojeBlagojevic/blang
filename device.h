//TBD interface for device system generates code
#ifndef DEVICE_H
#define DEVICE_H
//#define BVM_IMPLEMENTATION
//#define SDL_DEVICE
#include "bvm_type.h"
#ifdef SDL_DEVICE
#include<SDL2/SDL.h>

typedef enum {
    INIT_SDL,
    CHECK_EVENT,
    PUSH_EVENT_FROM_QUEUE,
    RENDERER_CLEAR,
    SET_RENDER_DRAW_COLOR,
    SDL_RENDER_FILL_RECT,
    SDL_RENDER_PRESENT,
    SDL_DELAY,
    SDL_MOUSE,
    NUM_OF_DEVICES
}DeviceType;

static const char* device_name[] = {
    "initSdl",
    "checkEvent",
    "pushEvent",
    "renderClear",
    "drawColor",
    "renderRect",
    "renderPresent",
    "sdlDelay",
    "sdlMouse",
    "NUM_OF_DEVICES",
};

typedef struct device{
    void (*func_pointer)(Stack*);
    int stackSize; 
}Devices;

//ALL STUF ARE PASED AS GLOBAL VARS FOR EXAMPLE WINDOW, RENDERER itd...

//NEKA SA SAD BUDE GLOBALNO 
static volatile Devices devices[NUM_OF_DEVICES];

//MAKE THIS STUF BE PASED SOMEHOW
//PROBLY PASS A STACK

static SDL_Window *window;
static SDL_Renderer *renderer;

#define WIDTH  800
#define HEIGHT 600

//(WIDTH HEIGHT --)
static inline void initSdl(Stack *stack){
    //Stack *s;
    printf("Inited sdl!!!\n");
    Word width = stackPop(stack);
    Word height = stackPop(stack);
    printf("Width: %ld Height %ld\n", width._asI64, height._asI64);
    SDL_CreateWindowAndRenderer(width._asI64, height._asI64, 0, &window, &renderer);
    //while(1){

    //}
    return;
}

//(-- key event.type)
static inline void pushEventFromQueue(Stack *stack){
    //Word typeOfEvent = stackPop(stack);
    SDL_Event event;
    if(SDL_PollEvent(&event)){
        stackPush(stack, (i64)event.key.keysym.sym);
        stackPush(stack, (i64)event.type);
 
    }
    else{
        stackPush(stack, -1);
        stackPush(stack, -1); //NO EVENT IN A QUEUE
    }
}

//(err +)
static inline void renderClear(Stack *stack){
    int err = SDL_RenderClear(renderer);//MAYBE PUSHES ERROR CODE 
    stackPush(stack, (i64)err);
}


//(Event +)
static inline void checkEventInLoop(Stack *stack){
    Word typeOfEvent = stackPop(stack);
    
    SDL_Event event;
    //printf("Event loop\n");
    if(SDL_PollEvent(&event)){
        if(event.type == typeOfEvent._asI64){
            //MAYBE SEND DATA TO A STACK
            exit(-1);

        }
    }
}
//(a, g, b, r ----)
static inline void drawColor(Stack *stack){
    Word a = stackPop(stack);
    Word b = stackPop(stack);
    Word g = stackPop(stack);
    Word r = stackPop(stack);
    SDL_SetRenderDrawColor(renderer, (u8)r._asI64, (u8)g._asI64, (u8)b._asI64, (u8)a._asI64);
}
//(h,w,y,x ----)
static inline void rednerRect(Stack *stack){
    Word h = stackPop(stack);
    Word w = stackPop(stack);
    Word y = stackPop(stack);
    Word x = stackPop(stack);
    SDL_Rect r;
    r.h =  (int)h._asI64; 
    r.w =  (int)w._asI64;
    r.y =  (int)y._asI64; 
    r.x =  (int)x._asI64;
    if(SDL_RenderFillRect(renderer, &r) != 0){
        printf("Error in drawing of a rectangle\n");
    }
    //SDL_Delay(1);
}
//(--)
static inline void renderPresent(Stack *stack){
    (void)stack;
    //printf("Drawing\n");
    SDL_RenderPresent(renderer);
    //printf("Drawing\n"); 
}
//DELAY(dealy --) 
static inline void sdlDelay(Stack *stack){
    Word a = stackPop(stack);
    SDL_Delay((u32)a._asI64);
}
//MOUSE(-- x, y)
static inline void sdlMouse(Stack *stack){
    int X, Y;
    
    SDL_GetMouseState(&X, &Y);
    stackPush(stack, (i64)X);
    stackPush(stack, (i64)Y);
    
}



static inline void initDevices(Stack *stack){
    (void)stack;
    printf("Devices init\n");
   //INIT SDL
    devices[INIT_SDL].func_pointer = &initSdl;
    devices[INIT_SDL].stackSize = -2;
   //CHECK EVENT
    devices[CHECK_EVENT].func_pointer = &checkEventInLoop;
    devices[CHECK_EVENT].stackSize = -1;
   //PUSH EVENT FROM QUUES
    devices[PUSH_EVENT_FROM_QUEUE].func_pointer = &pushEventFromQueue;
    devices[PUSH_EVENT_FROM_QUEUE].stackSize = +2;
   //CLEAR RENDERER
    devices[RENDERER_CLEAR].func_pointer = &renderClear;
    devices[RENDERER_CLEAR].stackSize = +1;
   //SET DRAW COLOR
    devices[SET_RENDER_DRAW_COLOR].func_pointer = &drawColor;
    devices[SET_RENDER_DRAW_COLOR].stackSize = -4;
  //DRAW RECR
    devices[SDL_RENDER_FILL_RECT].func_pointer = &rednerRect;
    devices[SDL_RENDER_FILL_RECT].stackSize = -4;
  //RENDER *renderer
    devices[SDL_RENDER_PRESENT].func_pointer = &renderPresent;
    devices[SDL_RENDER_PRESENT].stackSize = 0;
  //DELAY Sdl_Delay
    devices[SDL_DELAY].func_pointer = &sdlDelay;
    devices[SDL_DELAY].stackSize = -1;

  //MOUSE Sdl_Delay
    devices[SDL_MOUSE].func_pointer = &sdlMouse;
    devices[SDL_MOUSE].stackSize = +2;
    
    
}
#endif
#ifndef SDL_DEVICE
typedef enum {
    NUM_OF_DEVICES
}DeviceType;

static const char* device_name[] = {
    
    "NUM_OF_DEVICES",
};

typedef struct device{
    void (*func_pointer)(Stack*);
    int stackSize; 
}Devices;

//ALL STUF ARE PASED AS GLOBAL VARS FOR EXAMPLE WINDOW, RENDERER itd...

//NEKA SA SAD BUDE GLOBALNO 
static Devices devices[NUM_OF_DEVICES];
static inline void initDevices(Stack *stack){
    (void)stack;
    printf("Devices init non\n");
    
}



#endif


#endif
