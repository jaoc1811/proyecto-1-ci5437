#include "heuristics.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

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

unsigned manhattan(state_t *state){
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

unsigned pupu(state_t *state)
{
    if (is_goal(state))
        return 0;
    else
        return 1;
}