#include <limits>
#include <iostream>
#include "heuristics.hpp"
#include <time.h>

using namespace std;

#define MAX_LINE_LENGTH 999

const int infinity = std::numeric_limits<int>::max();
state_t state;
state_t state_aux;
state_t state_prunning_aux;
vector<int> path;
unsigned state_count = 0;
unsigned bound_state_count;

pair<bool, unsigned> f_bounded_dfs_visit(unsigned bound, unsigned g, unsigned (*heu)(state_t *), int history)
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
        if (fwd_rule_valid_for_history(history, ruleid) == 0)
            continue;

        unsigned cost = g + get_fwd_rule_cost(ruleid);
        copy_state(&state_aux, &state);
        apply_fwd_rule(ruleid, &state_aux, &state);

        if (heu(&state) < infinity)
        {
            bound_state_count++;
            state_count++;
            path.push_back(ruleid);
            int new_history = next_fwd_history(history, ruleid); // next history for child
            pair<bool, unsigned> p = f_bounded_dfs_visit(bound, cost, heu, new_history);
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
        bound_state_count = 0;
        cout << "Running with bound: " << bound;
        pair<bool, unsigned> p = f_bounded_dfs_visit(bound, 0, heu, init_history);
        cout << " (" << bound_state_count << " states generated)" << endl;
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

    printf("Choose heuristic: \n"
           "1) manhattan\n"
           "2) pdb additive\n"
           "3) pdb max\n"
           "Selected: ");
    char input;
    cin >> input;

    clock_t t;
    t = clock();

    switch (input)
    {
    case '1':
        ida_search(state_init, manhattan);
        break;
    case '2':
        load_pdb();
        ida_search(state_init, pdb_additive);
        break;
    case '3':
        load_pdb();
        ida_search(state_init, pdb_max);
        break;
    }
    t = clock() - t;

    cout << "Solution: ";
    for (int i : path)
    {
        cout << get_fwd_rule_label(i) << " ";
    }
    cout << endl;
    cout << "Movements: " << path.size() << endl;
    printf("It took: %f seconds.\n", ((float)t) / CLOCKS_PER_SEC);
    cout << "States generated: " << state_count << " States" << endl;

    return 0;
}