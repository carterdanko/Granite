/**
 * main module for the TSP solver
 */
 
#include "include/tsp.h"
#include "include/eax.h"
#include "include/fitness.h"

// "global" variables. I try to start these with capital letters
tour_t* Cities; // the "tour" that contains every city in their provided order. Not really a tour, just used as the master array of cities
 
int main(int argc, char** argv)
{
	/*
	printf("numargs: %i\n", argc);
	printf("filename? '%s'\n", argv[0]);
	*/
	
	// check number of parameters
	if (argc < 2)
	{
		printf("Usage: tsp <filename of cities text document> [flags]\n");
		exit(1); // ERROR: must supply a filename for the cities
	}
	
	// search input for cities file, and any relevant flags
	char* citiesFile = 0;
	int i;
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			//TODO: process the flags
			if (strcmp(argv[i], "--help") == 0)
			{
				printf("Usage: tsp <filename of cities text document> [flags]\n");
				printf("The first line of the cities text document is the number of cities.\n");
				printf("The following lines are each city. 2 integers, space separated, are the x and y of that city. Example: \n");
				printf("23 45\n");
			}
		}
		else if (citiesFile)
		{
			printf("Please only supply one city file. Use tsp --help for help\n");
			exit(2); // ERROR: must supply exactly one city file
		}
		else
		{
			citiesFile = argv[i];
			printf("City file: '%s'\n", citiesFile);
		}
	} // for each parameter
	// check to make sure we got a city file
	if (!citiesFile)
	{
		printf("no city file present. halting\n");
		exit(3); // ERROR: no city file present
	}
	
	// load the cities specified by the file
	printf("Loading cities...");
	Cities = loadCities(citiesFile);
	if (!Cities)
	{
		printf("Error while loading cities. refer to error log? halting.\n");
		exit(5); // ERROR: error while loading cities
	}
	printf("done! (loaded %i cities from the file)\n", Cities->size);
	
	///////////////////////////////////////////////////////////////////////////
	// process the cities
	int N = Cities->size;
	construct_distTable(Cities,N);// compute distances as soon as we can (now)

	// output the city information to the console
	printf("\nNum Cities: %04i\n", Cities->size);
	printf("---------------------------\n");
	for (i=0; i < Cities->size; i++)
	{
		printf("City[%04i] at %04i, %04i   [id: %04i]\n", i, Cities->city[i]->x, Cities->city[i]->y, Cities->city[i]->id);
	}

	// create two new tours by some arbitrary but reproducible means
	tour_t tourA, tourB;
	tour_t* tstar;
	tourA.size = Cities->size;
//	for (i=0; i < N; i++)
//	{
//		tourA.city[i] = Cities->city[(i*2)%N];
//	}

//Manually set cities so that there are a lot of crossovers.
//Only works for dataset 3
	tourA.city[0] = Cities->city[1];
	tourA.city[1] = Cities->city[2];
	tourA.city[2] = Cities->city[0];
	tourA.city[3] = Cities->city[7];
	tourA.city[4] = Cities->city[4];
	tourA.city[5] = Cities->city[6];
	tourA.city[6] = Cities->city[3];
	tourA.city[7] = Cities->city[5];
	
	
	tstar = create_tour_nn(Cities->city[0], Cities->size, Cities);
	tourB = *tstar;
	
	print_tour(&tourA);
	print_tour(&tourB);
	
	printf("Begining 2-opt fixin\n");

	for(i = 0; i <= Cities->size-4; i+=2)
	{
		LineSegment outsideSeg;
		outsideSeg.x1 = tourA.city[i]->x;
		outsideSeg.y1 = tourA.city[i]->y;
		outsideSeg.x2 = tourA.city[i+1]->x;
		outsideSeg.y2 = tourA.city[i+1]->y;
		int j;
		//This might only work for an even amount of cities?
		for(j = i+2;j <= Cities->size-2; j+=2)
		{
			LineSegment insideSeg;
			insideSeg.x1 = tourA.city[j]->x;
			insideSeg.y1 = tourA.city[j]->y;
			insideSeg.x2 = tourA.city[j+1]->x;
			insideSeg.y2 = tourA.city[j+1]->y;	
			if(doIntersect(outsideSeg, insideSeg) == 1)
			{	
				printf("Intersecting segments i:%d j:%d\n", i,j);

				city_t *temp = tourA.city[i+1];
				tourA.city[i+1] = tourA.city[j];
				tourA.city[j] = temp;				
								
			}//End of if the segments intersect
		}//End of the inside j loop	
		
	}//End of outside loop

	printf("\nPrinting tour after 2-opt\n");
	print_tour(&tourA);

//After this point there are two tours created. Since I'm creating a 2 opt function, ignore everything
//after this part
/*
	// now, find fitness of the tours.
	set_tour_fitness(&tourA,N);
	set_tour_fitness(&tourB,N);
	
	DPRINTF("fitness of A,B is %f,%f.\n", tourA.fitness,tourB.fitness);
	// output the two tours
	printf("TourA: [%i]", tourA.city[0]->id);
	for (i=1; i < N; i++)
		printf(", [%i]", tourA.city[i]->id);
	printf("\nTourB: [%i]", tourB.city[0]->id);
	for (i=1; i < N; i++)
		printf(", [%i]", tourB.city[i]->id);
	printf("\n");

	// now, testing print tour function for A and B.
	print_tour(&tourA);
	print_tour(&tourB);

	// now, testing roulette wheel 5 times.
	srand(0);
	tours[0] = tourA;
	tours[1] = tourB;
	tour_t* tempt;
	DPRINTF(" ~~~ ROULETTE WHEEL ~~\n");
	for (i=0;i<5;i++) {
		tempt = roulette_select(tours, 2);
		dprint_tour(tempt);
	}
	DPRINTF(" ~~~  END ROULETTE  ~~\n");
	
	// merge the two tours
	printf("\nMerging A with B...");
	graph_t* R = mergeTours(&tourA, &tourB);
	printf("done!\n");
	
	// output the merged graph
	printf("\nGraph R contains: \n");
	for (i=0; i < N; i++)
	{
		printf("%04i [id:%04i] -> edges: ", i, R->node[i]->id);
		int e;
		for (e=0; e < R->node[i]->size; e++)
			printf((e>0) ? ", [%04i:t%01i]" : "[%04i:t%01i]", R->node[i]->edge[e]->id, R->node[i]->tour[e]);
		printf("\n");
	}

	// create A-B cycles on R
	printf("Allocating cycles...");
	tour_t** cycles;
	cycles = (tour_t**)malloc(sizeof(tour_t *) * MAX_ABCYCLES);
	for (i=0; i < MAX_ABCYCLES; i++)
	{
		cycles[i] = (tour_t*)malloc(sizeof(tour_t));
	}
	printf("done!\n");
	int nCycles;
	printf("Generating AB Cycles....");
	// REMEMBER! R gets defiled by this call, and the contents of cycles isn't important, it all gets overwritten
	nCycles = generateABCycles(Cities, R, cycles);
	printf("done!\n");

	// output the cycles
	printf("Printing all %i cycles...\n", nCycles);
	for (i=0; i < nCycles; i++)
	{
		printf("Cycle[%i]: [%i]", i, cycles[i]->city[0]->id);
		int a;
		for (a=1; a < cycles[i]->size; a++)
			printf(", [%i]", cycles[i]->city[a]->id);
		printf("\n");
	}

	// create E-sets from the cycles
	nCycles = generateESetRAND(Cities, cycles, nCycles);
	// output the E-set
	printf("Printing all %i cycles in the \033[32mE-set\033[0m...\n", nCycles);
	for (i=0; i < nCycles; i++)
	{
		printf("Cycle[%i]: [%i]", i, cycles[i]->city[0]->id);
		int a;
		for (a=1; a < cycles[i]->size; a++)
			printf(", [%i]", cycles[i]->city[a]->id);
		printf("\n");
	}

	// apply E-sets to generate intermediates
	graph_t* T = createGraph(&tourA);
	// output the created graph from tourA
	printf("\n\033[32mIntermediate Tour T\033[0m contains: \n");
	for (i=0; i < N; i++)
	{
		printf("%04i [id:%04i] -> edges: ", i, T->node[i]->id);
		int e;
		for (e=0; e < T->node[i]->size; e++)
			printf((e>0) ? ", [%04i:t%01i]" : "[%04i:t%01i]", T->node[i]->edge[e]->id, T->node[i]->tour[e]);
		printf("\n");
	}
	// apply the eset to the graph
	printf("allocating edges array...\n");
	edge_t** edges = (edge_t**)malloc(sizeof(edge_t *) * Cities->size);
	for (i=0; i < Cities->size; i++)
	{
		edges[i] = (edge_t*)malloc(sizeof(edge_t));
	}
	printf("Applying the E-set.\n");
	int disjointCycles = applyESet(Cities, T, cycles, nCycles, edges);
	printf("there were \033[32m%i\033[0m disjoint cycles.\n", disjointCycles);
	// output the intermediate
	printf("\n\033[32mIntermediate Tour T\033[0m contains: \n");
	for (i=0; i < N; i++)
	{
		printf("%04i [id:%04i] -> edges: ", i, T->node[i]->id);
		int e;
		for (e=0; e < T->node[i]->size; e++)
			printf((e>0) ? ", [%04i:t%01i]" : "[%04i:t%01i]", T->node[i]->edge[e]->id, T->node[i]->tour[e]);
		printf("\n");
	}
	// output the edges
	printf("Printing all %i edges in the graph: \n", Cities->size);
	for (i=0; i < Cities->size; i++)
	{
		printf("Edge[%i] = {%i -> %i : i%i : c%f}\n", i, edges[i]->v1->id, edges[i]->v2->id, edges[i]->cycle, edges[i]->cost);
	}
	// output the sub-disjointCycles
	printf("Printing all %i cycles in the \033[32mIntermediate Tour\033[0m...\n", disjointCycles);
	for (i=0; i < disjointCycles; i++)
	{
		printf("Cycle[%i]: [%i]", i, cycles[i]->city[0]->id);
		int a;
		for (a=1; a < cycles[i]->size; a++)
			printf(", [%i]", cycles[i]->city[a]->id);
		printf("\n");
	}
	
	//TODO: turn intermediates into valid tours
	
	// clean up
	printf("\nClean up...");
	freeGraph(R);
	freeGraph(T);
	printf("graph gone...");
	freeCities(Cities);
	printf("main cities gone...");
	//for (i=0; i < MAX_ABCYCLES; i++)
	//	freeCities(cycles[i]);
	printf("AB cycles structure gone...");
	printf("done!\n");
*/	
	// done (just used to make sure that the program ran to completion)
	printf("Program ran to completion (done).\n");
	exit(0);
}
