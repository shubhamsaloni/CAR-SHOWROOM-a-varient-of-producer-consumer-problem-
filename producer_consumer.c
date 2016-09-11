#include"stdio.h"
#include"unistd.h"
#include"sys/types.h"
#include"sys/sem.h"
#include"sys/shm.h"
#include"sys/wait.h"
#include"fcntl.h"
#include"sys/ipc.h"
#include"stdlib.h"
#define size 5
#define EMPTY 0
#define FULL 1
#define MUTEX 2
////////////////////

void upsem(int sem_id,int snum,struct sembuf * semaphore)
{ semaphore->sem_num=snum;
semaphore->sem_op=1;
semaphore->sem_flg=0;
semop(sem_id,semaphore,1);
}
///////////////////////////////////
void downsem(int sem_id,int snum,struct sembuf * semaphore)
{ semaphore->sem_num=snum;
semaphore->sem_op=-1;
semaphore->sem_flg=0;
semop(sem_id,semaphore,1);
}
///////////
void initsem(int sem_id,int sem_num,int val)
{
union semum
{ int val;
struct semid_ds * buf;
unsigned short *array;
}argument;
argument.val=val;
semctl(sem_id,sem_num,SETVAL,argument);
}
////////////////////////
void main()
{ key_t shm_key=1234,sem_key=5678;
int * shm;int item;int i,count=0,flag=1;
int shm_id,sem_id;
struct sembuf semaphore;
shm_id=shmget(shm_key,sizeof(int),IPC_CREAT|0666);
shm=shmat(shm_id,NULL,0);
sem_id=semget(sem_key,3,IPC_CREAT|0666);
initsem(sem_id,FULL,0);
initsem(sem_id,EMPTY,size);
initsem(sem_id,MUTEX,1);
shm[0]=0;
if(fork()==0)
{
while(flag)
{ //consumer
sleep(1);
downsem(sem_id,FULL,&semaphore);
downsem(sem_id,MUTEX,&semaphore);
if(count==10)
{ printf("\n 10 cars purchased");
flag=0; shm[0]=-1;
upsem(sem_id,MUTEX,&semaphore);
upsem(sem_id,EMPTY,&semaphore);
}
else
{
item=shm[1];
for(i=1;i {shm[i]=shm[i+1];}
shm[0]--;

count++;

printf("\n consumer buys car no. %d ",count);
}
upsem(sem_id,MUTEX,&semaphore);
upsem(sem_id,EMPTY,&semaphore);
}
}
else
{
while(flag)
{//producer)

item=random()%10;
sleep(1);
downsem(sem_id,EMPTY,&semaphore);
downsem(sem_id,MUTEX,&semaphore);
if(shm[0]==-1)
{flag=0;}
else
{shm[++shm[0]]=item;

printf("\n producer adds car ");
}
upsem(sem_id,MUTEX,&semaphore);
upsem(sem_id,FULL,&semaphore);
}
}
}//end of main 
