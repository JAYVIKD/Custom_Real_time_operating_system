#ifndef _kernel_h
#define _kernel_h

#include <stdint.h>
#include "stm32f7xx.h"                  // Device header

void kernel_launch(uint32_t quanta);

void kernel_init(void);

void thread_yeild(void);

void kernel_skiptick(uint32_t i);

void semaphore_init(int32_t *semaphore,int value);

void wait(int32_t *semaphore);

void signal(int32_t *semaphore);

uint8_t kernel_add_thread(void(*task0)(void));

uint8_t kernel_create_thread(void(*task0)(void),
													void(*task1)(void),
													void(*task2)(void));
void block_thread();

#endif	