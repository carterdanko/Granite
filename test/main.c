
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NThread 4	
#define MaxSize 10

pthread_mutex_t mutex;


void  deque(int *belt)
{
	int i = 0;
	for (;i<9;i++)	{
		belt[i]=belt[i+1];
	}
	belt[9]=0;
}


typedef struct 
{
	int conBelt[MaxSize];	//Conveyor belt to hold sopes and molletes
	int sopeCount;		//Running count of how many sopes are on the belt
	int beltTotal;		//Running count of all items on the belt
	int total;		//The total number of items produced
	int  mollete;		//Boolean to check if producer is a mollete or not


} threadInfo;


void *producerFunc (threadInfo * param)
{
	if(param->mollete == 0)
	{
		printf("Entered Sope part\n");
		while(param->total < 100){
			pthread_mutex_lock(&mutex);
			while((param->beltTotal < MaxSize) && (param->sopeCount < 3)){
				printf("Making some sopes\n");//Add sope to belt
				param->conBelt[param->beltTotal] = 1;
				param->sopeCount++;
				param->total++;
				param->beltTotal++;
			}
			pthread_mutex_unlock(&mutex);
		}
		printf("%d", param->total);
		pthread_exit(0);
	}
	else if( param->mollete == 1)
	{
		printf("Entered Mollete Part\n");
		while(param->total < 100){
			pthread_mutex_lock(&mutex);
			while((param->beltTotal < MaxSize)){
				printf("Making some molletes\n");//Add mollete to belt
				param->conBelt[param->beltTotal] = 2;
				param->total++;
				param->beltTotal++;
			}
			pthread_mutex_unlock(&mutex);
		}
		printf("%d", param->total);
		pthread_exit(0);
	}
}


void *consumerFunc (threadInfo * param)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while((param->beltTotal)){
			int consume = param->conBelt[0];
			if(consume == 1)
			{	param->sopeCount--;		
				printf("Eating Sope\n");

			}
			else
				printf("Eating some Molletes\n");

			param->beltTotal--;
			deque(param->conBelt);
		}
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_exit(0);
}



int main (int argc, char *argv[])
{
	pthread_mutex_init(&mutex,0);

        int status, junk;
        pthread_t threads[NThread];

        int producerID = 0;
        int i = 0;

	threadInfo param;
	param.mollete = 0;


//SOPE PRODUCER
        status = pthread_create(&threads[0], 0, (void*)producerFunc, &param );
        if(status != 0){
                printf("Error");
                exit(-1);
        }
       	else{
                printf("Main created Sope Producer\n");
        }

	
//MOLLETE PRODUCER
        status = pthread_create(&threads[1], 0, (void*)producerFunc, &param);
        if(status != 0){
                printf("Error");
                exit(-1);
        }
	else{
                printf("Main created Mollete Producer\n");
        }


//SOPE CONSUMER
        status = pthread_create(&threads[2], 0,(void *) consumerFunc, &param);
        if(status != 0){
                printf("Error");
                exit(-1);
        }
	else{
                printf("Main created Sope Consumer\n");
        }


//MOLLETE CONSUMER
        status = pthread_create(&threads[3], 0, (void *)consumerFunc, &param);
        if(status != 0){
                printf("Error");
                exit(-1);
        }
	else{
                printf("Main created Mollete Consumer\n");
        }

        scanf("%d", &junk);
	exit(0);


}



