#include "kernel.h"
#define MAX_thread;
#define max_threads 10 
#define stack_size 100
#define bus_freq 16000000
#define RUNNING 0
#define BLOCK 1

uint8_t  no_of_thread=-1;
//register def
#define SysPri3   (*((volatile uint32_t *)0xE000ED20)) // for systick timer
#define INTCTRL 	(*((volatile uint32_t *)0xE000ED04))  // for Pendsv handler.
#define ICSR      (*((volatile uint32_t *)0xE000ED04))
void scheduler_launch(void);
void periodic(void);
void kernel_scheduler(void);
void pendsv(void);

uint32_t milli_prescalar ;
uint32_t tick=0;
uint32_t time_quanta;

struct tcb{
	int32_t *stackpt;
	uint8_t thread_no;
	uint8_t thread_status;
	struct tcb *nextpt;
};


typedef struct tcb tcbtype;
tcbtype tcbs[max_threads];
tcbtype *currentPt;
uint32_t kernel_tick=0;
int32_t tcb_stack[max_threads][stack_size];

void kernel_stackinit(int i){
		tcbs[i].stackpt = &tcb_stack[i][stack_size-16];				// -16 because of the 16 inbuilt register 
		tcb_stack[i][stack_size - 1] = 0x01000000;						// to keep the processor  in thumb mode
}

uint8_t kernel_create_thread(void(*task0)(void),void(*task1)(void),void(*task2)(void)){
	__disable_irq();
	tcbs[0].nextpt = &tcbs[1];
	tcbs[1].nextpt = &tcbs[2];
	tcbs[2].nextpt = &tcbs[0];
	kernel_stackinit(0);
	tcb_stack[0][stack_size-2]=(int32_t)(task0);  // to add it to the program counter
	kernel_stackinit(1);
	tcb_stack[1][stack_size-2]=(int32_t)(task2);
	kernel_stackinit(2);
	tcb_stack[2][stack_size-2]=(int32_t)(task1);
	currentPt= &tcbs[0];
	__enable_irq();
	return 1;
}
// kernel add thread
uint8_t kernel_add_thread(void(*task)(void)){
	__disable_irq();
	no_of_thread++;
	tcbs[no_of_thread].thread_no = no_of_thread;
	tcbs[no_of_thread].thread_status = RUNNING ; 
	if(no_of_thread == 0){
				tcbs[no_of_thread].nextpt= &tcbs[no_of_thread];
	}
	else{ tcbs[no_of_thread-1].nextpt = &tcbs[no_of_thread];
				tcbs[no_of_thread].nextpt= &tcbs[0];
	}
	kernel_stackinit(no_of_thread);
	tcb_stack[no_of_thread][stack_size-2]=(int32_t)(task);
	currentPt= &tcbs[0];
	__enable_irq();
	return no_of_thread;
	
}

// initialise the kernel 
void kernel_init(void){
	__disable_irq();
	milli_prescalar = bus_freq/1000;
}

// launch the kernel 
void kernel_launch(uint32_t quanta){
		SysTick->CTRL = 0;
		SysTick->VAL = 0;
		SysTick->LOAD = (milli_prescalar)-1;
		time_quanta = quanta;
		SysPri3 = (SysPri3&0x00FFFFFF) | 0xE0000000; // changing the systick's timer priority to the highest
		
		SysTick->CTRL = 0x00000007;                 // systick enable 
		scheduler_launch();
	
}



void thread_yeild(void){ 		 // yeilding the current task via calling the interrupt of the systick
	SysTick->VAL = 0;		
	INTCTRL = 0X04000000;
}


// kernel delay by skiping tick unrelaiable and in multiples of quanta seconds 
void kernel_skiptick(uint32_t i){
	int j;
	for(j=0;j<i;j++){
			thread_yeild();
	}
}
int perodic=0;

//kernel's round robin scheduler

void kernel_scheduler(){
	
	
	currentPt = currentPt->nextpt;
	if(currentPt->thread_status == BLOCK){
		currentPt = currentPt->nextpt;
	}
	
}

inline void block_thread(){
			__disable_irq();
			currentPt->thread_status = BLOCK;
			__enable_irq();
			thread_yeild();
	
}