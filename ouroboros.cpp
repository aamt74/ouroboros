// Find all (2, N)-partitions in which each (cyclic) N consecutive bits occurs only once.
// Note: Throughout this implementation we use the fact that a vertex name = vertex index = N-bit pattern

#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/graph/directed_graph.hpp>

using namespace std;
using namespace boost;

using vertex_t = unsigned;
using edge_t = pair<vertex_t, vertex_t>;
using graph_t = adjacency_list<vecS, vecS, bidirectionalS>;
using path_t = vector<vertex_t>;
using GraphTraits = graph_traits<graph_t>;

struct puzzle_t
{
  puzzle_t(int n) : N(n) { }
  const int N;                              // N consecutive bits (cyclic) to be unique in the total (2, N)-partition
  const long SLEN = 1 << N;                 // The total partition length is 2^N
  const vertex_t VMIN = 0;                  // The pattern 00...0, i.e. N zeros
  const vertex_t VMAX = (1 << N) - 1;       // The pattern 11...1, i.e. N ones
  const vertex_t VLST = (1 << (N-1)) - 1;   // The pattern 01...1 is the LaST before cycling to VMAX again
  const vertex_t MASK = VMAX;               // So we can shift vertices and then mask to keep at length of N bits
};

struct state_t
{
  state_t(puzzle_t const &ouroboros) : 
    path({ ouroboros.VMAX }), 
    done(string(ouroboros.SLEN, '0')), 
    count(0) { }
  path_t path;  // Path under construction. We start with pattern 11...1.
  string done;  // done[i] == '1' means that V_i is in P
  long count;   // nbr of solutions found
};

// Digraph where an edge A->B means that B is a consecutive pattern of A
// [00...00] -> {[00...01]}
// [b1 b2 ... b_k-1 b_k] -> {[b2 ... b_k-1 b_k 0], [b2 ... b_k-1 b_k 1]}
// [11...11] -> {[11...10]}
auto create_graph(puzzle_t const &ouroboros) -> graph_t
{
  graph_t G(ouroboros.N);
  cout << ouroboros.VMIN << " -> " << ouroboros.VMIN + 1 << endl;
  add_edge(ouroboros.VMIN, ouroboros.VMIN + 1, G);
  for (vertex_t v = 1; v < ouroboros.VMAX; ++v)
  {
    vertex_t v_next_1 = (v << 1) & ouroboros.MASK;
    vertex_t v_next_2 = v_next_1 | 1;
    cout << v << " -> " << v_next_1 << endl;
    add_edge(v, v_next_1, G);
    cout << v << " -> " << v_next_2 << endl;
    add_edge(v, v_next_2, G);
  }
  cout << ouroboros.VMAX << " -> " << ouroboros.VMAX - 1 << endl;
  add_edge(ouroboros.VMAX, ouroboros.VMAX - 1, G);
  return std::move(G);
}

// Note this doesn't print all patterns in the path, because of the cyclic nature
void print_permutation(path_t const &P, int N, long SLEN)
{
  string permutation(N, '1');
  for (int i = 1; permutation.length() < SLEN; ++i)
  {
    permutation += (P[i] & 0x01) == 1 ? "1" : "0";
  }
  cout << permutation << endl;
}

// Backtrack search for all hamiltonian cycles, starting from pattern 11...1
void solve_ouroboros(puzzle_t const &ouroboros, graph_t const &G, state_t &state)
{
  typename GraphTraits::out_edge_iterator out_i, out_end;
  long SLEN = ouroboros.SLEN;
  vertex_t v = vertex(state.path.back(), G);
  
  if (state.path.size() == SLEN)
  {
    print_permutation(state.path, ouroboros.N, SLEN);
    state.count++;
    return;
  }
  
  for (tie(out_i, out_end) = out_edges(v, G); out_i != out_end; ++out_i)
  {
    vertex_t v_out = target(*out_i, G);
    bool is_last_too_soon = v_out == ouroboros.VLST && state.path.size() + 1 < SLEN;
    bool is_already_visited = state.done[v_out] == '1';
    if (!is_last_too_soon && !is_already_visited)
    {
      state.path.push_back(v_out);
      state.done[v_out] = '1';

      solve_ouroboros(ouroboros, G, state);

      state.done[v_out] = '0';
      state.path.pop_back();
    }
  }
}

int main(int argc, char *argv[])
{
  int N = (argc == 2) ? atoi(argv[1]) : 2;

  cout << "Ouroboros puzzle (N=" << N << "):" << endl;
  puzzle_t ouroboros(N);
    
  cout << "Graph:" << endl;
  auto G = create_graph(ouroboros);

  cout << "Solutions:" << endl;
  state_t state(ouroboros);
  solve_ouroboros(ouroboros, G, state);
  cout << state.count << " solutions found" << endl;

  return 0;
}
