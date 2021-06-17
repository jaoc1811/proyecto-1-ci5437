#include "heuristics.hpp"
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

vector<abstraction_t *> abstractions;
vector<state_map_t *> state_maps;

void load_pdb() {
    int i = 1;
    while(true){
        string file_name = "abs"+to_string(i);
        FILE *f = fopen((file_name+".pdb").c_str(),"r");
        if (!f){
            break;
        }
        state_maps.push_back(read_state_map(f));
        abstractions.push_back(read_abstraction_from_file((file_name+".abst").c_str()));
        fclose(f);
        i++;
    }
}

vector<int> split (const string &s, char delim) {
    vector<int> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        if ((item) == "B"){
            result.push_back (0);
        }
        else{
            result.push_back(stoi(item));
        }
    }
    return result;
}

unsigned pdb(state_t *state) {
    state_t abs_state;
    unsigned total = 0;
    for(unsigned i = 0; i < abstractions.size(); i++) {
        abstract_state(abstractions[i], state, &abs_state);
        total += *state_map_get(state_maps[i], &abs_state);
    }
    return total;
}

unsigned manhattan(state_t *state) {
    char str[512];
    sprint_state(str,512,state);
    vector<int> v = split(str, ' ');

    int i = 0;
    int sum = 0;
    int n = (int) v.size();
    int n_sqrt = sqrt(n);

    while (i < n)
    {
        if (v[i] == 0){
            i++;
            continue;
        }
        sum += abs(v[i]%n_sqrt - i%n_sqrt) + abs(v[i]/n_sqrt - i/n_sqrt);
        i++;
    }
    return (unsigned) sum;
}

unsigned pupu(state_t *state) {
    if (is_goal(state))
        return 0;
    else
        return 1;
}