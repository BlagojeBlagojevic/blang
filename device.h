//STA BI DEVICE TREBAO DA RADI 
//PRVO DA DODAMO TOKEN TYPE DEVICE !
//DODATI U VM DA MOZE DA INTERFESUJE SA
//MOZDA KAO C FAJL PA DA BUDE EMBEDABLE
#ifndef DEVICE_H
#define DEVICE_H
//#define BVM_IMPLEMENTATION
#include "bvm_type.h"
#ifdef SDL_DEVICE
#include<SDL2/SDL.h>

typedef enum {
    INIT_SDL,
    None,
    NUM_OF_DEVICES
}DeviceType;

static const char* device_name[] = {
    "initSdl",
    "NUM_OF_DEVICES",
};

typedef struct device{
    void (*func_pointer)(Stack*); 
}Devices;

//ALL STUF ARE PASED AS GLOBAL VARS FOR EXAMPLE WINDOW, RENDERER itd...

//NEKA SA SAD BUDE GLOBALNO 
static Devices devices[NUM_OF_DEVICES];

//MAKE THIS STUF BE PASED SOMEHOW
//PROBLY PASS A STACK

static SDL_Window *window;
static SDL_Renderer *renderer;
#define WIDTH  800
#define HEIGHT 600
static inline void initSdl(Stack *stack){
    //Stack *s;
    printf("Inited sdl!!!\n");
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, NULL, &window, &renderer);
    while(1){

    }
    return;
}

static inline void initDevices(Stack *stack){
    printf("Devices init\n");
    devices[INIT_SDL].func_pointer = &initSdl;
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
    void (*func_pointer)(int); 
}Devices;

//ALL STUF ARE PASED AS GLOBAL VARS FOR EXAMPLE WINDOW, RENDERER itd...

//NEKA SA SAD BUDE GLOBALNO 
static Devices devices[NUM_OF_DEVICES];
static inline void initDevices(Stack *stack){
    printf("Devices init non\n");
    
}



#endif


#endif

