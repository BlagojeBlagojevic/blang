#ifndef BVM_TYPE
#define BVM_TYPE

#include <stdint.h>
//#define SDL_DEVICE
typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;
typedef  float    f32;
typedef  double   f64;
typedef union {

	u64 _asU64;
	i64 _asI64;
	f64 _asF64;
	void *_asP;

	} Word;

typedef struct {
	Word *stack;
	i64  SP;
	} Stack;
static inline void stackPush(Stack *stack,i64 value);
static inline void stackPushF64(Stack *stack,f64 value);
static inline void stackPushWord(Stack *stack, Word value);
static inline Word stackPop(Stack *stack);
static inline void initStack(Stack *stack);
#define STACK_CAPACITIY 200024

#endif
