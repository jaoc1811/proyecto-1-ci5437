#include <vector>
#include <algorithm>
class node
{
private:
    /* data */
public:
    state_t *state;
    node *parent;
    int ruleid;
    unsigned g;

    node(state_t *state, node *parent, int ruleid, unsigned g);
    node *make_node(state_t *state, int ruleid);
    void extract_path(std::vector<int> &reversed_path);
    static node *make_root_node(state_t *state);
};
