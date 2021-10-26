#include "ku_cfs.h"

float vrun_arr[5]={0.64,0.8,1,1.25,1.5625};
void timer_handler(int sig);
PCB * temp; int t_pid, t_nice; float t_vrun;
int pid_arr[26]; int idx=0;
int time=0;

int main(int argc, char* argv[]){
	char* alpha[]={"A","B","C","D","E","F","G","H","I","J","K","L"
	,"M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
	int argv_arr[5];
	for(int i=0;i<5;i++){
		argv_arr[i] = atoi(argv[i+1]);
	}
	int end_time = atoi(argv[6]);

	signal(SIGALRM, timer_handler);
	init();
	
	for(int n=0;n<5;n++){
		for(int p=0;p<argv_arr[n];p++){
			pid_arr[idx] = fork();
			if(pid_arr[idx]==0){
				execl("./ku_app","ku_app",alpha[idx],NULL);
			}
			add(pid_arr[idx],n-2,0);
			idx++;			
		}
	}
	sleep(5);
	init_timer();
	setitimer(ITIMER_REAL, &itimer, NULL);

	temp = front();
	t_pid = temp->pid; t_vrun = temp->vrun; t_nice = temp->nice;
	
	kill(head->next->pid, SIGCONT);
	delete();

	while(1){
		if(time == end_time){
			kill(t_pid,SIGKILL);

			PCB* p = head->next;
			while(p!=tail){
				kill(p->pid,SIGKILL);
				p = p->next;
			}
			delete_all();
			return 0;
		}
	}
}

void timer_handler(int sig){
	time++;
	kill(t_pid, SIGSTOP);
	t_vrun += vrun_arr[t_nice+2];
	add(t_pid, t_nice, t_vrun);

	temp = front();
	t_pid = temp->pid, t_vrun = temp->vrun, t_nice = temp->nice;
	kill(head->next->pid,SIGCONT);
	delete();
}
