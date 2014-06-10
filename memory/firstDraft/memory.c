#include <stdio.h>
#include <stdlib.h>


/****************************************************************************/
/*                                                                          */
/* 			     Module MEMORY                                  */
/* 			External Declarations 	                            */
/*                                                                          */
/****************************************************************************/


/* OSP constants */

#define MAX_PAGE       16                 /* max size of page tables        */
#define MAX_FRAME      32                 /* size of the physical memory    */
#define PAGE_SIZE      512                /* size of a page in bytes        */

#define   COST_OF_PAGE_TRANSFER      6  /* cost of reading page  from drum  */

#define FIFO true
#define INCR_LRU_COUNTER 128  //in binary: 10000000


/* external enumeration constants */

typedef enum {
    false, true                         /* the boolean data type            */
} BOOL;

typedef enum {
    read, write                         /* type of actions for I/O requests */
} IO_ACTION;

typedef enum {
    load, store                         /* types of memory reference        */
} REFER_ACTION;

typedef enum {
    running, ready, waiting, done       /* types of status                  */
} STATUS;

typedef enum {
    iosvc, devint,                      /* types of interrupt               */
    pagefault, startsvc,
    termsvc, killsvc,
    waitsvc, sigsvc, timeint
} INT_TYPE;

/* external type definitions */

typedef struct page_entry_node PAGE_ENTRY;
typedef struct page_tbl_node PAGE_TBL;
typedef struct event_node EVENT;
typedef struct ofile_node OFILE;
typedef struct pcb_node PCB;
typedef struct iorb_node IORB;
typedef struct int_vector_node INT_VECTOR;
typedef struct frame_node FRAME;
typedef struct Frame_list_node FRAME_NODE;

/* external data structures */

extern struct frame_node {
    BOOL   free;        /* = true, if free                                  */
    PCB    *pcb;        /* process to which the frame currently belongs     */
    int    page_id;     /* vitrual page id - an index to the PCB's page tbl */
    BOOL   dirty;       /* indicates if the frame has been modified         */
    int    lock_count;  /* number of locks set on page involved in an       */
                        /* active I/O                                       */
    int    *hook;       /* can hook up anything here                        */
};

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

extern struct iorb_node {
    int    iorb_id;     /* iorb id                                          */
    int    dev_id;      /* associated device; index into the device table   */
    IO_ACTION action;   /* read/write                                       */
    int    block_id;    /* block involved in the I/O                        */
    int    page_id;     /* buffer page in the main memory                   */
    PCB    *pcb;        /* PCB of the process that issued the request       */
    EVENT  *event;      /* event used to synchronize processes with I/O     */
    OFILE  *file;       /* associated entry in the open files table         */
    IORB   *next;       /* next iorb in the device queue                    */
    IORB   *prev;       /* previous iorb in the device queue                */
    int    *hook;       /* can hook up anything here                        */
};

extern struct int_vector_node {
    INT_TYPE cause;           /* cause of interrupt                         */
    PCB    *pcb;              /* PCB to be started (if startsvc) or pcb that*/
			      /* caused page fault (if fagefault interrupt) */
    int    page_id;           /* page causing pagefault                     */
    int    dev_id;            /* device causing devint                      */
    EVENT  *event;            /* event involved in waitsvc and sigsvc calls */
    IORB   *iorb;             /* IORB involved in iosvc call                */
};

/* extern variables */

extern INT_VECTOR Int_Vector;           /* interrupt vector         	     */
extern PAGE_TBL *PTBR;                  /* page table base register 	     */
extern FRAME Frame_Tbl[MAX_FRAME];      /* frame table              	     */
extern int Prepage_Degree;		/* global degree of prepaging (0-10) */

/* external routines */

extern siodrum(/* action, pcb, page_id, frame_id */);
/*  IO_ACTION   action;
    PCB         *pcb; 
    int         page_id, frame_id;  */
extern int get_clock();
extern gen_int_handler();


/* end of module */

/* external routines */

//extern prepage(/* pcb */);

extern int start_cost(/* pcb */);
/*  PCB    *pcb; */    

extern set_timer(/* time_quantum */);
/*  int    time_quantum; */

extern int get_clock();

/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*                              Module MEMORY                               */
/*                            Internal Routines                             */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
int FreePages;
FRAME_NODE fifoHead;
FRAME_NODE LRUHead;
FRAME_NODE *tempFRAME_NODE;

struct Frame_list_node {
    
	FRAME *frame;
    FRAME_NODE *next;          
    FRAME_NODE *prev;
	int listSize;
	int bitCounter;
    
};

//adds to end of FIFO list
void addFifo(FRAME_NODE *newNode)
{
	FRAME_NODE *frontNode;
		
	frontNode = fifoHead.next;
	//frontNode->prev = fifoHead;

	//if nothing in list
	if(fifoHead.next==0)
	{
		newNode->next =0;
		newNode->prev = &fifoHead;
		return;
	}

	//step through list to last node
	while(frontNode->next!=0)
	{
		
		frontNode = frontNode->next;

	}
	//frontNode should now point to last item in list
	//now set newNode as last item
	frontNode->next = newNode;
	newNode->prev = frontNode;
	
	//add to counter
	fifoHead.listSize++;
}

//for FIFO 
FRAME_NODE *getNextFIFO()
{
	FRAME_NODE *frontNode;
	frontNode = 0;
	
	//if only one FRAME in queue, the choice is easy
	if(fifoHead.listSize==0)
	{
		frontNode = fifoHead.next;
		
		fifoHead.next = 0;
		fifoHead.listSize--;
		return frontNode;
	}
	//if nothing in node, returns null
	else if(fifoHead.next==0)
	{
        	return 0;
	}
	//returns first frame in queue
	else 
    {
        	frontNode =fifoHead.next;
			fifoHead.next = frontNode->next;
        	fifoHead.listSize--;
			return frontNode;
    }
}

//for LRU 
FRAME_NODE *getNextLRU()
{
	FRAME_NODE *frontNode;
	frontNode = 0;
	
	FRAME_NODE *oldestNode;
	oldestNode = 0;

	
	//if only one FRAME in list, the choice is easy
	if(LRUHead.listSize==0)
	{
		frontNode = LRUHead.next;
		
		LRUHead.next = 0;
		LRUHead.listSize--;
		return frontNode;
	}//end if

	//if nothing in list, returns null
	else if(LRUHead.next==0)
	{
        	return 0;
	}// end else if
	//returns oldest frame in list
	else 
    {
       	frontNode = LRUHead.next;
    	oldestNode = LRUHead.next;

    	while(frontNode->next!=0 )
    	{
        	frontNode = frontNode->next;

			if(frontNode->bitCounter < oldestNode->bitCounter)
        	{
				oldestNode=frontNode;

        	}//end if
        	
    	}//end while
    	(oldestNode->prev)->next = oldestNode->next;
		if(oldestNode->next!=0)
			(oldestNode->next)->prev = oldestNode->prev;

    	LRUHead.listSize--;
    }//end else

	return oldestNode;
    
}

void addLRU(FRAME_NODE *newNode)
{
	FRAME_NODE *frontNode;
		
	frontNode = LRUHead.next;
	//frontNode->prev = &LRUHead;
	
	//if nothing in list
	if(LRUHead.next==0)
	{
		newNode->next =0;
		newNode->prev = &LRUHead;
		return;
	}
	
	//step through list to last node
	while(frontNode->next!=0)
	{
		
		frontNode = frontNode->next;

	}
	
	//frontNode should now point to last item in list
	//now set newNode as last item
	frontNode->next = newNode;
	newNode->prev = frontNode;
	
	//add to counter
	LRUHead.listSize++;	

}

void memory_init()
{

	FreePages = MAX_FRAME;

	fifoHead.listSize = 0;
	LRUHead.listSize = 0;

	int i;
	for(i=0;i<MAX_PAGE;i++)
	{
		Frame_Tbl[i].free = true;
		Frame_Tbl[i].pcb = 0;
		Frame_Tbl[i].page_id = -1;
		Frame_Tbl[i].dirty=false;
		Frame_Tbl[i].lock_count = 0;		

	}
}

void prepage(pcb)
PCB *pcb;
{


}

int start_cost(pcb)
PCB *pcb;
{

	int numPages = pcb->size / PAGE_SIZE;
	int remainingPage = pcb->size % PAGE_SIZE;
	if(remainingPage > 0)
		numPages++;
	
	//GLOBAL FreePages;
	int cost;
	
	if(MAX_FRAME - FreePages + numPages > MAX_FRAME)	
		cost = (numPages - FreePages) * COST_OF_PAGE_TRANSFER;
		//This is the cost of swapping the pages out of mem if there isn't 
		//Enough free pages

	cost += numPages * COST_OF_PAGE_TRANSFER;	
	//The cost of swapping pages in	

	//Start_Cost = #swapOut * COST_OF_PAGE_TRANSFER + swapIn * COST_OF_PAGE_TRANSFER
	return cost;
}

void deallocate(pcb)
PCB *pcb;
{
	int i=0;
	for(i;i<MAX_PAGE;i++)
	{
		if(pcb->page_tbl->page_entry[i].valid)
		{
			Frame_Tbl[pcb->page_tbl->page_entry[i].frame_id].free = true;
			Frame_Tbl[pcb->page_tbl->page_entry[i].frame_id].pcb = 0;

			//Update freepages (our thing)
			pcb->page_tbl->page_entry[i].valid = false;
			if(Frame_Tbl[pcb->page_tbl->page_entry[i].frame_id].dirty == true)
			{
				//write to disk			
				siodrum(write, pcb, Frame_Tbl[pcb->page_tbl->page_entry[i].frame_id].page_id , pcb->page_tbl->page_entry[i].frame_id); 
				Frame_Tbl[pcb->page_tbl->page_entry[i].frame_id].dirty = false;

			}
			
		}		
	}
}


void get_page(pcb,page_id)
PCB *pcb;
int page_id;
{
	
	int frameIDToEvict;
	
	int i = 0;
	for (;i < MAX_FRAME; i++)
	{
		if(Frame_Tbl[i].pcb == pcb && Frame_Tbl[i].page_id == page_id)
		//TODO: Do you need to also check the valid bit?
			return;
		
	}
	
	//If the func gets here, that means the page wasn't found
	if(FreePages > 0)
	{
		for(i = 0; i < MAX_FRAME; i++)
		{
			if(Frame_Tbl[i].free == true && Frame_Tbl[i].lock_count==0)
			//if the frame table is free
			{
				siodrum(read, pcb, page_id, i);
				//TODO:Do I need to set the frame info or does siodrum do that?
	
				pcb->page_tbl->page_entry[page_id].frame_id = i;
				Frame_Tbl[i].pcb = pcb;
				Frame_Tbl[i].page_id = page_id;
				Frame_Tbl[i].free = false;
				FreePages--;
				printf("frame %d          pageID %d ********************************************************\n\n", i, page_id);
				
				frameIDToEvict = i;
				break;
			}
		}	
	}
	else
	{
		
		//must evict a page
		if(FIFO)
		{
			//gets frame id of next frame to evict
			FRAME_NODE *temp = getNextFIFO();
			frameIDToEvict = temp->frame->pcb->page_tbl->page_entry[temp->frame->page_id].frame_id;
		}
		else	//LRU
		{
			FRAME_NODE *temp = getNextLRU();
			frameIDToEvict = temp->frame->pcb->page_tbl->page_entry[temp->frame->page_id].frame_id;
			
		}

		if(Frame_Tbl[frameIDToEvict].dirty == true)
		{
			siodrum(write, Frame_Tbl[frameIDToEvict].pcb, Frame_Tbl[frameIDToEvict].page_id, frameIDToEvict);
			Frame_Tbl[frameIDToEvict].dirty = false;
		}
		
		siodrum(read, pcb, page_id, frameIDToEvict);
		pcb->page_tbl->page_entry[page_id].frame_id=frameIDToEvict;
		Frame_Tbl[frameIDToEvict].pcb = pcb;
		Frame_Tbl[frameIDToEvict].page_id = page_id;
	}//end else
	
	//creates new node and points it to the FRAME
	tempFRAME_NODE = (FRAME_NODE*)malloc(sizeof(FRAME_NODE));
		
	tempFRAME_NODE->listSize = 0;

	tempFRAME_NODE->next = 0;
	tempFRAME_NODE->prev = 0;

	tempFRAME_NODE->frame = &Frame_Tbl[frameIDToEvict];
		
	if(FIFO)
	{	
		addFifo(tempFRAME_NODE);
	}
	else
	{
		addLRU(tempFRAME_NODE);
	}
	FreePages--;
}



void lock_page(iorb)
IORB *iorb;
{
	
	if(iorb->pcb->page_tbl->page_entry[iorb->page_id].valid == false)
	{
		Int_Vector.cause = pagefault;
		Int_Vector.page_id = iorb->page_id;
		Int_Vector.pcb = PTBR->pcb;
		gen_int_handler();	
		
	}
	if(iorb->action==read)
	{
		
		Frame_Tbl[iorb->pcb->page_tbl->page_entry[iorb->page_id].frame_id].dirty = true;
	}
	
	Frame_Tbl[iorb->pcb->page_tbl->page_entry[iorb->page_id].frame_id].lock_count++;
	
}


void unlock_page(iorb)
IORB  *iorb;
{
/* decrement the lock count */
	Frame_Tbl[iorb->pcb->page_tbl->page_entry[iorb->page_id].frame_id].lock_count--;	
	
}



void refer(logic_addr,action)
int logic_addr;
REFER_ACTION action;
{
		printf("Entered refer****************************************************************\n\n");
		int pagenum = logic_addr / PAGE_SIZE;
		int frameID = PTBR->pcb->page_tbl->page_entry[pagenum].frame_id;
		PCB *temp=PTBR->pcb;		
		
		if((PTBR->page_entry[pagenum].valid == true && PTBR->pcb == Frame_Tbl[frameID].pcb) || PTBR->page_entry[pagenum].valid == false)
		{
			Int_Vector.cause = pagefault;
			Int_Vector.page_id = pagenum;
			Int_Vector.pcb = PTBR->pcb;
			gen_int_handler();		
						
			//use saved pcb after interupt			
			//PTBR->pcb = temp;
			frameID = temp->page_tbl->page_entry[pagenum].frame_id;
			
			temp->page_tbl->page_entry[pagenum].valid = true;
			
			Frame_Tbl[frameID].free = false;
			Frame_Tbl[frameID].pcb = temp;
			Frame_Tbl[frameID].page_id = pagenum;
		}	
		
		if(action==store)
			Frame_Tbl[frameID].dirty = true;

		//now inc LRU bit counters
		if(!FIFO)
		{
			FRAME_NODE *index;
			
			index = &LRUHead;
	    	
    		while(index->next!=0 )
    		{
        		//step to next node, then shift counter bit one position to right
				index = index->next;
				index->bitCounter =  index->bitCounter >> 1;
	       		
				//for the node being referenced, also inc its counter
				if(index->frame->pcb->page_tbl->page_entry[index->frame->page_id].frame_id == frameID)
					index->bitCounter+=INCR_LRU_COUNTER;
    		}//end while
			
			Frame_Tbl[frameID].pcb->page_tbl->page_entry[index->frame->page_id].frame_id;
			
		}

		
		printf("Exit refer.......................\n");
}
