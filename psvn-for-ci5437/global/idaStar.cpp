#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <limits>
#include <iostream>
#include "node.hpp"
using namespace std;

#define  MAX_LINE_LENGTH 999

const int infinity = std::numeric_limits<int>::max();
state_t state;
state_t state_aux;
vector<int> path;

unsigned pupu(state_t *state)
{
    if(is_goal(state)) return 0;
    else return 1;
}

pair<bool, unsigned> f_bounded_dfs_visit(unsigned bound, unsigned g)
{
    // base cases
    unsigned h = pupu(&state);
    unsigned f = g + h;
    if(f > bound) return {false, f};
    if(h == 0) return {true, g};

    unsigned t = infinity;
    int ruleid;
    ruleid_iterator_t iter;
    init_fwd_iter(&iter, &state);
    while( (ruleid = next_ruleid(&iter)) >= 0 )
    {
        unsigned cost = g + get_fwd_rule_cost(ruleid);
        copy_state(&state_aux, &state);
        apply_fwd_rule(ruleid, &state_aux, &state);
        if(pupu(&state) < infinity)
        {
            path.push_back(ruleid);
            pair<bool, unsigned> p = f_bounded_dfs_visit(bound, cost);
            if(p.first) return p;
            t = min(t, p.second);
            path.pop_back();
        }
        copy_state(&state_aux, &state);
        apply_bwd_rule(ruleid, &state_aux, &state);
    }
    return {false, t};
}

void ida_search(state_t init)
{
    state = init;
    unsigned bound = pupu(&init);
    // search with increasing f-value bounds
    while(true)
    {
        pair<bool, unsigned> p = f_bounded_dfs_visit (bound, 0);
        if(p.first) return;
        bound = p.second;
    }
}

int main() 
{
    // VARIABLES FOR INPUT
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    state_t state_init; // state_t is defined by the PSVN API. It is the type used for individual states.

    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &state_init);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    printf("The state you entered is: ");
    print_state(stdout, &state_init);
    printf("\n");

    ida_search(state_init);
    for(int i : path)
    {
        cout << get_fwd_rule_label(i) << endl;
    }
    cout << endl;
    return 0;
}
