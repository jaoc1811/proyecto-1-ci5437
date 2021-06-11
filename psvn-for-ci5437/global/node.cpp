#include "node.hpp"

node::node(state_t *state, node *parent, int ruleid, unsigned g)
{
    this->state = state;
    this->parent = parent;
    this->ruleid = ruleid;
    this->g = g;
}

node *node::make_node(state_t state, int ruleid)
{
    return new node(state, this, ruleid, this->g + get_fwd_rule_cost(ruleid));
}

void node::extract_path(std::vector<int> &reversed_path)
{
    node* n = this;
    while(n != NULL && n->parent != NULL)
    {
        reversed_path.push_back(n->ruleid);
        n = n->parent;
    }
}

node *node::make_root_node(state_t state)
{
    return new node(state, NULL, -1, 0);
}