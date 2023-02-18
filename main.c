#include "kernel.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define quanta 10
#define RUNNING 0
#define BLOCK 1
#define ever ;;
uint32_t c0,c1,c2,c3;
int count_p=0;
int32_t semaphore1,semaphore2; 

#define DEBUG __asm__("BKPT #0"/);
void task0(){    // task 0 stack size 100
	for(ever){	
		
		c0++;
	}
	
}

void task1(){    //task 1 stack size 100
	for(ever){	
		
		c1++;
	}
}

void task2(){   //task 2 stack size 100
	for(ever){
	c2++;
	}
}

void task3(){   //task 3 stack size 100
	for(ever){
	if(c3 == 1000){
	block_thread();
	}
		c3++;		
	
	}
	
}


int main(){
	
	kernel_init();
	kernel_add_thread(&task0);
	kernel_add_thread(&task1);
	kernel_add_thread(&task2);
	kernel_add_thread(&task3);
	kernel_launch(quanta);

}	