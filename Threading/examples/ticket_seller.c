#include "thread.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

/* Binary Semaphore Use Example
 * ----------------------------
 *  1 global variable - number tickets,
 *  threads of ticket sellers will try and sell tickets until
 *  none remain.
 */

#define NUM_TICKETS 35
#define NUM_SELLERS 4

static int numTickets = NUM_TICKETS;
static thread_mutex_T  ticketsLock;
list_L ticket_sellers;

struct data {
	char *string;
	int index;
};

static THREAD_RETURN ticket_sellerworker(void *data) {
	thread_paramload(data);

	struct data *inp = data;

	int done = 0;
	int numSoldByThread = 0;
	int randomgen = inp->index + 1;

	while(!done){ 
		int ttw = (((randomgen * 11)%13)*23)%17;
		
		thread_sleep(ttw > 5 ? ttw : 5);
		randomgen = ttw;
		
		thread_mutexlock(ticketsLock);
			if(numTickets == 0){
				done = 1;
			} else {
				numTickets--;
				numSoldByThread++;
				printf("%s sold one(%d left)\n", inp->string, numTickets);
			}
		thread_mutexrelease(ticketsLock);
	}
	printf("%s noticed all tickets sold! (I sold %d myself)\n", inp->string, numSoldByThread);

	thread_return(NULL);
}

int main() {
	int i;
	char name[NUM_SELLERS][32];
	struct data items[NUM_SELLERS];
	ticketsLock = thread_mutexnew();
	ticket_sellers = list_new();
	

	for(i = 0; i < NUM_SELLERS; i++) {
		sprintf(name[i], "Seller #%d", i);
		items[i].index = i;
		items[i].string = name[i];
		thread_T thread = thread_new(ticket_sellerworker, &items[i]);
		list_push(ticket_sellers, thread);
	}

	struct L_iterator iter = list_iterator(ticket_sellers);

	while(list_iteratorhasnext(&iter)) {
		thread_T thread = list_iteratornext(&iter);
		thread_join(thread);
		thread_delete(thread);
	}
	thread_mutexdelete(ticketsLock);

}
