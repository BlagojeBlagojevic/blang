//TBD interface for device system generates code
#ifndef DEVICE_H
#define DEVICE_H
//#define BVM_IMPLEMENTATION
//#define SDL_DEVICE
#include "bvm_type.h"

#ifdef DEVICE
#ifdef SDL_DEVICE
    #include<SDL2/SDL.h>
#endif
#ifdef WEB_DEVICE
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<netinet/tcp.h>
    #include<arpa/inet.h>
    #include<unistd.h>
    #include <fcntl.h>

#endif

typedef enum {
#ifdef SDL_DEVICE
    INIT_SDL,
    CHECK_EVENT,
    PUSH_EVENT_FROM_QUEUE,
    RENDERER_CLEAR,
    SET_RENDER_DRAW_COLOR,
    SDL_RENDER_FILL_RECT,
    SDL_RENDER_PRESENT,
    SDL_DELAY,
    SDL_MOUSE,
#endif
#ifdef WEB_DEVICE
    WEB_SOCKET,
    WEB_BIND,
    WEB_LISTEN,  
    WEB_ACCEPT,
    WEB_FCNTL,
#endif
    NUM_OF_DEVICES
}DeviceType;

static const char* device_name[] = {
#ifdef SDL_DEVICE
    "initSdl",
    "checkEvent",
    "pushEvent",
    "renderClear",
    "drawColor",
    "renderRect",
    "renderPresent",
    "sdlDelay",
    "sdlMouse",
#endif
#ifdef WEB_DEVICE
    "socket",
    "bind",
    "listen",
    "accept",
    "fcntl",               
 #endif   
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
#ifdef SDL_DEVICE
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
#endif
#ifdef WEB_DEVICE

//SOCKET(protocol, type, domain -- socketFd)
static inline void socketInit(Stack *stack){
    Word protocol = stackPop(stack);
    Word type = stackPop(stack);
    Word domain = stackPop(stack);
    stackPush(stack, (i64)socket((i32)domain._asI64, (i32)type._asI64, (i32)protocol._asI64));   
}
//BIND(socketFd, protocol port pernitenAddr -- host error)
static void __attribute__((noinline)) bindInit(Stack *stack){
    
    Word permitAddr = stackPop(stack);
    Word port = stackPop(stack);
    Word protocol = stackPop(stack);
    Word socketFd = stackPop(stack); 
    
    struct sockaddr_in host = {
        .sin_family = (i32)protocol._asI64,
        .sin_port = htons((u16)port._asU64),
        .sin_addr.s_addr = htonl((u32)permitAddr._asU64),
    };
    Word err;
    err._asI64 = (i64)bind((i32)socketFd._asI64, (struct sockaddr *)&host, (socklen_t)sizeof(host));
    stackPush(stack, (i64)&host); 
    stackPush(stack, err._asI64);
}
//LISTEN(socketFd, &host, -- error)
static void  listenInit(Stack *stack){
    Word maxConections = stackPop(stack);
    Word socketFd = stackPop(stack);
    stackPush(stack, (i64)listen((i32)socketFd._asI64, (i32)maxConections._asI64));
}

//ACCEPT(socketFd, &HOST, -- newSocketFd)
static void  acceptInit(Stack *stack){
    Word host = stackPop(stack);
    Word socketFd = stackPop(stack);
    Word sizeHost;
    stackPush(stack, (i64)accept((i32)socketFd._asI64, (struct sockaddr *)host._asI64, (socklen_t*)&sizeHost._asI64));
    (void)sizeHost;
    //stackPush(stack, sizeHost);
}

//FCNTL(socketFd, F_SETFL  O_NONBLOCK, -- newSocketFd)
static void  fcntlInit(Stack *stack){
    
    Word isBlocking = stackPop(stack);
    Word flag = stackPop(stack);
    Word socketFd = stackPop(stack);

    stackPush(stack, (i64)fcntl((i32)socketFd._asI64, flag._asI64, isBlocking._asI64));    
    //stackPush(stack, sizeHost);
}






#endif



static inline void initDevices(Stack *stack){
    (void)stack;
    printf("Devices init\n");
   //INIT SDL
#ifdef SDL_DEVICE
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
#endif
#ifdef WEB_DEVICE
    //SOCKET socket
    devices[WEB_SOCKET].func_pointer = &socketInit;
    devices[WEB_SOCKET].stackSize = -2; 
    //BIND bind
    devices[WEB_BIND].func_pointer = &bindInit;
    devices[WEB_BIND].stackSize = -2; 
    //LISTEN listen
    devices[WEB_LISTEN].func_pointer = &listenInit;
    devices[WEB_LISTEN].stackSize = -1;
    //ACCEPT accept
    devices[WEB_ACCEPT].func_pointer = &acceptInit;
    devices[WEB_ACCEPT].stackSize = -2;

#endif    
    
}
#endif
#ifndef DEVICE
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
