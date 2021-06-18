#include <iostream>
#include <vector>

extern std::vector<abstraction_t *> abstractions;
extern std::vector<state_map_t *> state_maps;
void load_pdb();
unsigned pdb_additive(state_t *state);
unsigned pdb_max(state_t *state);
unsigned manhattan(state_t *state);
unsigned pupu(state_t *state);