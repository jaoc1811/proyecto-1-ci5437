/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout the number of states at each distance from goal and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <vector>
#include "priority_queue.hpp"
#include "sys/sysinfo.h"
#include <iostream>
std::vector<double> numAtDarr;
std::vector<double> numAtDarr2;
int64_t totalNodes, numAtD;  // counters

/*
  Imprime la memoria virtual (ram y swap) usada actualmente.
*/
void print_memory_used(void) {
  struct sysinfo memInfo;
  sysinfo (&memInfo);
  double virtualMemUsed = memInfo.totalram - memInfo.freeram;
  //Add other values in next statement to avoid int overflow on right hand side...
  virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
  virtualMemUsed *= memInfo.mem_unit;
  std::cout << virtualMemUsed / (1024*1024*1024) << " Gb\n";
}

void print_results(void){
    printf("%"PRIu64" states in total.\n", totalNodes);
    for(int i=1;i < numAtDarr.size();i++) {
        numAtDarr2.push_back(numAtDarr[i]/numAtDarr[i-1]);
    }
    double p = 0;
    for(int i=0;i < numAtDarr2.size();i++) {
        p += numAtDarr[i]*numAtDarr2[i];
    }
    double sum = 0;
    for(int i=0;i < numAtDarr.size()-1;i++){
       sum += numAtDarr[i];
    }
    double result = p/sum;
    printf("Ramification Factor: %lf\n", result);
}


/*
  Imprime los resultados actuales y finaliza la ejecucion.
*/
void handler_ctrl_c(int s){
  printf("No solution found.\n");
  print_results();
  exit(1);
}

/*
  Para poder parar la ejecucion.
*/
void set_handler(void) {
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = handler_ctrl_c;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
}

int main(int argc, char **argv) {

    set_handler();

    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int d, ruleid;
    ruleid_iterator_t iter;

    PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    // add goal states
    first_goal_state(&state, &d);
    do {
        state_map_add(map, &state, 0);
        open.Add(0, 0, state);
    } while( next_goal_state(&state, &d) );

    d = 0;
    numAtD = 0;
    totalNodes = 0;

    while( !open.Empty() ) {
        // get current distance from goal; since operator costs are
        // non-negative this distance is monotonically increasing
        if( open.CurrentPriority() > d ) {
            numAtDarr.push_back((double)numAtD);
            printf("%"PRId64" states at distance %d\n", numAtD, d);
            d = open.CurrentPriority();
            numAtD = 0;
        }

        // remove top state from priority state
        state = open.Top();
        open.Pop();
        
        // check if we already expanded this state.
        // (entries on the open list are not deleted if a cheaper path to a state is found)
        const int *best_dist = state_map_get(map, &state);
        assert(best_dist != NULL);
        if( *best_dist < d ) continue;
        
        numAtD++;
        totalNodes++;

        // look at all predecessors of the state
        init_bwd_iter(&iter, &state);
        while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
            apply_bwd_rule(ruleid, &state, &child);
            const int child_d = d + get_bwd_rule_cost(ruleid);

            // check if either this child has not been seen yet or if
            // there is a new cheaper way to get to this child.
            // const int *old_child_d = state_map_get(map, &child);
            // if( (old_child_d == NULL) || (*old_child_d > child_d) ) {
                // add to open with the new distance
                state_map_add(map, &child, child_d);
                open.Add(child_d, child_d, child);
            // }
        }
    }
    // print last level and total states
    if( numAtD > 0 ) {
        numAtDarr.push_back((double)numAtD);
        printf("%"PRId64" states at distance %d\n", numAtD, d);
    }
    print_results();
    // write the state map to a file
    if( argc >= 2 ) {
        file = fopen(argv[1], "w");
        if( file == NULL ) {
            fprintf(stderr, "could not open %s for writing\n", argv[1]);
            exit(-1);
        }
        write_state_map(file, map);
        fclose(file);
    }
    
    return 0;
}

