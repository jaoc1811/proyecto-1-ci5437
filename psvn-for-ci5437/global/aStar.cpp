#include <limits>
#include <iostream>
#include "node.hpp"
#include <queue>
#include <map>
#include "heuristics.hpp"
#include <time.h>

using namespace std;

#define MAX_LINE_LENGTH 999

const int infinity = std::numeric_limits<int>::max();

unsigned state_count = 0;

node *aStar(state_t init, unsigned (*h)(state_t *))
{

  map<uint64_t, unsigned> distance;
  priority_queue<pair<unsigned, node *>, vector<pair<unsigned, node *>>, greater<pair<unsigned, node *>>> q;
  q.push({h(&init), node::make_root_node(&init)});

  while (!q.empty())
  {
    node *n = q.top().second;
    q.pop();
    state_t *ns = n->state;

    if (distance.count(hash_state(ns)) == 0 || n->g < distance[hash_state(ns)])
    {
      distance[hash_state(ns)] = n->g;

      if (is_goal(ns))
        return n;

      int ruleid;
      ruleid_iterator_t iter;
      init_fwd_iter(&iter, ns);

      while ((ruleid = next_ruleid(&iter)) >= 0)
      {
        state_t *s = new state_t;
        apply_fwd_rule(ruleid, ns, s);

        if (h(s) < infinity)
        {
          int cost = n->g + get_fwd_rule_cost(ruleid) + h(s);
          q.push({cost, n->make_node(s, ruleid)});
        }
      }
    }
  }
  return NULL;
}

int main()
{
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


  // cout << manhattan(&state_init) <<endl;
  printf("selected heuristic: \n"
          "1) manhattan\n"
          "2) pdb\n");
  char input;
  cin >> input;

  node *solution;
  clock_t t;
  t = clock();
  switch (input){
    case '1':
      solution = aStar(state_init, manhattan);
      break;
    case '2':
      load_pdb();
      solution = aStar(state_init, pdb);
      break;
  }
  t = clock() - t;
  if (!solution){
    cout << "No solution found" << endl;
    return 0;
  }
  vector<int> path;
  solution->extract_path(path);
  cout << "Solution: ";
  for (int i : path)
  {
    cout << get_fwd_rule_label(i) << " ";
  }
  
  cout << endl;
  cout << "Movements: " << path.size() << endl;
  printf ("It took: %f seconds.\n",((float)t)/CLOCKS_PER_SEC);
  cout << "States generated: " << state_count << " States" << endl;

  return 0;
}
