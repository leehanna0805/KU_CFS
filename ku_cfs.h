#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

struct itimerval itimer;

typedef struct PCB{
	int pid;
	int nice;
	float vrun;
	struct PCB * next;
	struct PCB * prev;
}PCB;

PCB* head;
PCB* tail;

void init(){
	head = (PCB*)malloc(sizeof(PCB));
	head->pid = 0; head->nice = 0; head->vrun = 0;
	tail = (PCB*)malloc(sizeof(PCB));
	tail->pid = 0; tail->nice = 0; tail->vrun = 999;
	
	head->next = tail;
	head->prev = NULL;
	tail->next = NULL;
	tail->prev = head;
}

void add(int _pid, int _nice, float _vrun){
	PCB* new_pcb = (PCB*)malloc(sizeof(PCB));
	new_pcb->pid = _pid;
	new_pcb->nice = _nice;
	new_pcb->vrun = _vrun;
	
	PCB * tmp = head->next;

	while(tmp != NULL){
		if(tmp->vrun <= new_pcb->vrun){
			tmp = tmp->next;
		}else{
			tmp->prev->next = new_pcb;
			new_pcb->prev = tmp->prev;
			tmp->prev = new_pcb;
			new_pcb->next = tmp;
			return;
		}
	}
}

PCB* front(){
	return head->next;
}

void * delete(){
	PCB* del = head->next;
	head->next = del->next;
	del->next->prev = head;
	free(del);
}

void delete_all(){
	PCB* cur=head->next;
	PCB* nex;
	while(cur != tail){
		nex = cur->next;
		free(cur);
		cur = nex;
	}
	free(head);
	free(tail);
}

void init_timer(){
	itimer.it_value.tv_sec = 1;
	itimer.it_value.tv_usec = 0;
	itimer.it_interval.tv_sec = 1;
	itimer.it_interval.tv_usec = 0;
}

