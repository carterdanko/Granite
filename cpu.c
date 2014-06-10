#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/****************************************************************************/
/*                                                                          */
/* 			     Module CPU                                     */
/* 			External Declarations 				    */
/*                                                                          */
/****************************************************************************/


/* OSP constant      */

#define   MAX_PAGE                   16 /* max size of page tables          */

/* OSP enumeration constants */

typedef enum {
    false, true                         /* the boolean data type            */
} BOOL;

typedef enum {
    running, ready, waiting, done       /* types of status                  */
} STATUS;



/* external type definitions */

typedef struct page_entry_node PAGE_ENTRY;
typedef struct page_tbl_node PAGE_TBL;
typedef struct event_node EVENT;
typedef struct pcb_node PCB;



/* external data structures */

extern struct page_entry_node {
    int    frame_id;    /* frame id holding this page                       */
    BOOL   valid;       /* page in main memory : valid = true; not : false  */
    int    *hook;       /* can hook up anything here                        */
};

extern struct page_tbl_node {
    PCB    *pcb;        /* PCB of the process in question                   */
    PAGE_ENTRY page_entry[MAX_PAGE];
    int    *hook;       /* can hook up anything here                        */
};

extern struct pcb_node {
    int    pcb_id;         /* PCB id                                        */
    int    size;           /* process size in bytes; assigned by SIMCORE    */
    int    creation_time;  /* assigned by SIMCORE                           */
    int    last_dispatch;  /* last time the process was dispatched          */
    int    last_cpuburst;  /* length of the previous cpu burst              */
    int    accumulated_cpu;/* accumulated CPU time                          */
    PAGE_TBL *page_tbl;    /* page table associated with the PCB            */
    STATUS status;         /* status of process                             */
    EVENT  *event;         /* event upon which process may be suspended     */
    int    priority;       /* user-defined priority; used for scheduling    */
    PCB    *next;          /* next pcb in whatever queue                    */
    PCB    *prev;          /* previous pcb in whatever queue                */
    int    *hook;          /* can hook up anything here                     */
};


/* external variables */

extern PAGE_TBL *PTBR;		/* page table base register */

extern int    Quantum;		/* global time quantum; contains the value
				   entered at the beginning or changed 
				   at snapshot. Has no effect on timer
				   interrupts, unless passed to set_timer() */


PCB head;	//head node for queue
int numProc;


/* external routines */

extern prepage(/* pcb */);

extern int start_cost(/* pcb */);
/*  PCB    *pcb; */    

extern set_timer(/* time_quantum */);
/*  int    time_quantum; */

extern int get_clock();


void enque(PCB *newNode)
{
	PCB *currNode;

	if(head.next == 0)
	{
		head.next= newNode;
		printf("\n\nAdding item at the first Position\n\n");
	}
	else
	{
		currNode=head.next;
		while(currNode->next!=0)
		{
			currNode=currNode->next;
		}
		currNode->next = newNode;
		printf("\n\nAdding item at not first Position\n\n");
	}
	numProc++;
}




//removes and returns next node
PCB *deque()
{
	PCB *currNode = 0;

	if(numProc==1)
	{

          currNode=head.next;
          head.next=0;
          printf("\n\nRemoving last node in the list\n\n");
          numProc--;
	}
	else if(numProc==0)
	{
		printf("\n\nThere is nothing in the queue\n\n");
	        return 0;
	}
   	else if(numProc>1)
    	{
       		currNode = head.next;
	       	head.next = currNode->next;
       		printf("removing item:  id = %d\n\n", (currNode->pcb_id));
	       	numProc--;
		printf("\n\nRemoving a node that isn't the last one\n\n");
	}
	//printf("%d \n\n", currNode->pcb_id);
	return  currNode;

}

int equals(PCB a, PCB b)
{
	if(a.pcb_id ==b.pcb_id)
		return 1;
	else

		return 0;

}

int isEmpty()
{
	if(head.next==0)
		return 1;
	else
		return 0;
}

/****************************************************************************/
/*                                                                          */
/*				Module CPU				    */
/*			     Internal Routines				    */
/*                                                                          */
/****************************************************************************/





void cpu_init()
{
	head.next = 0;
	head.prev = 0;
	numProc = 0;
	printf("\n\nInit Complete\n\n");
}


void insert_ready(pcb)
PCB *pcb;
{
	PCB *currNode;
	int isDuplicate=0;  //0 for false, 1 for true
	
	if(head.next==0)
	{
		pcb->status = ready;		
		enque(pcb);
		printf("\n\nProcess Inserte at head\n\n");
		
	}
	else
	{
		currNode= head.next;
		while(currNode!=0)
		{
			if(equals(*currNode, *pcb))
			{
				isDuplicate = 1;
				break;
			}
			currNode=currNode->next;
		}	
		
		if(!isDuplicate)
		{
			pcb->status = ready;
			enque(pcb);
			printf("\n\nProcess Inserted into Queue\n\n");
		}
	}	

}

void dispatch()
{
	//checks if proc was running prior to dispatch() being called
	if(PTBR !=0 && PTBR->pcb->status == running)
//Changed from ready to running
{	
		PTBR->pcb->status = ready;
		insert_ready(PTBR->pcb);



}	
	//checks if queue is empty, otherwise sets next proc in queue to be run
	//printf("Entering trouble area");
	
	if(isEmpty())
	{
		//PTBR->pcb = 0;
		PTBR = 0;
		printf("\n\nThis means queue is empty and PTBR is null\n\n");
	}
	else
	{
		printf("\n\nAssigning something to run\n\n");
		PCB *temp;
		temp = deque();

		PTBR = temp->page_tbl;
		PTBR->pcb = temp;
		PTBR->pcb->status = running;
		PTBR->pcb->last_dispatch = get_clock();
	}

}




/* end of module */
