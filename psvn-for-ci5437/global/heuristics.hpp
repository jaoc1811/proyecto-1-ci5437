#include <iostream>
#include <vector>

extern std::vector<abstraction_t *> abstractions;
extern std::vector<state_map_t *> state_maps;
void load_pdb();
unsigned manhattan(state_t *state);
unsigned pupu(state_t *state);