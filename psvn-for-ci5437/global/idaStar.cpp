#include <limits>
#include <vector>
#include <iostream>
#include "heuristics.hpp"
#include <map>
#include <time.h>

using namespace std;

#define MAX_LINE_LENGTH 999

const int infinity = std::numeric_limits<int>::max();
state_t state;
state_t state_aux;
state_t state_prunning_aux;
vector<int> path;
map<string, int> labels {{"RIGHT",1},{"LEFT",-1},{"UP",2},{"DOWN", -2}};

pair<bool, unsigned> f_bounded_dfs_visit(unsigned bound, unsigned g, unsigned (*heu)(state_t *))
{
    // base cases
    unsigned h = heu(&state);
    unsigned f = g + h;
    if (f > bound)
        return {false, f};
    if (h == 0)
        return {true, g};

    unsigned t = infinity;
    int ruleid;
    ruleid_iterator_t iter;
    init_fwd_iter(&iter, &state);

    while ((ruleid = next_ruleid(&iter)) >= 0)
    {
        if (path.size() && !(labels[get_fwd_rule_label(ruleid)] + labels[get_fwd_rule_label(path.back())])){
            continue;
        }
        unsigned cost = g + get_fwd_rule_cost(ruleid);
        copy_state(&state_aux, &state);
        apply_fwd_rule(ruleid, &state_aux, &state);
        if (heu(&state) < infinity)
        {
            path.push_back(ruleid);
            pair<bool, unsigned> p = f_bounded_dfs_visit(bound, cost, heu);
            if (p.first)
                return p;
            t = min(t, p.second);
            path.pop_back();
        }
        copy_state(&state_aux, &state);
        apply_bwd_rule(ruleid, &state_aux, &state);
    }
    return {false, t};
}

void ida_search(state_t init, unsigned (*heu)(state_t *))
{
    state = init;
    unsigned bound = heu(&init);
    // search with increasing f-value bounds
    while (true)
    {
        cout << "Running with bound: " << bound << endl;
        pair<bool, unsigned> p = f_bounded_dfs_visit(bound, 0, heu);
        if (p.first)
            return;
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
    if (fgets(str, sizeof str, stdin) == NULL)
    {
        printf("Error: empty input line.\n");
        return 0;
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &state_init);
    if (nchars <= 0)
    {
        printf("Error: invalid state entered.\n");
        return 0;
    }

    printf("The state you entered is: ");
    print_state(stdout, &state_init);
    printf("\n");

    printf("selected heuristic: \n"
            "1) manhattan\n"
            "2) pupu\n");
    char input;
    cin >> input;

    clock_t t;
    t = clock();

    switch (input){
        case '1':
            ida_search(state_init, manhattan);
            break;     
        case '2':
            ida_search(state_init, pupu);
            break;     
    }
    t = clock() - t;

    for (int i : path)
    {
        cout << get_fwd_rule_label(i) << endl;
    }
    cout << endl;
    cout << "Movements: " << path.size() << endl;
    printf ("It took: %f seconds.\n",((float)t)/CLOCKS_PER_SEC);

    return 0;
}