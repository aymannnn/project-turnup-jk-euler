// fun BFS use as template for later search things
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <map>

class Map {
private:
  std::unordered_map<int, std::unordered_set<int>> connections_;
  std::vector<std::unordered_set<int>> islands_;
  std::unordered_map<int, int> distances_;
public:
  Map(const int n) {
    n_nodes_ = n;
    for (int i = 1; i <= n; i++) {
      connections_.insert({ i, std::unordered_set<int>() });
    }
  };
  void add_edge(const int a, const int b);
  std::vector<int> solve_map();
  void solve_distances(const std::unordered_set<int> &island);
  int n_nodes_;
  int start_node_;
};

void Map::add_edge(const int a, const int b) {

  connections_[a].insert(b);
  connections_[b].insert(a);

  int position_a = -1, position_b = -1;

  for (auto i = 0; i < islands_.size(); i++) {
    if (islands_[i].count(a)) {
      position_a = i;
    }
    if (islands_[i].count(b)) {
      position_b = i;
    }
  }

  if (position_a == -1 &&
      position_b == -1) {
    islands_.push_back({ a, b });
  }
  else if (position_a == -1) {
    islands_[position_b].insert(a);
  }
  else if (position_b == -1) {
    islands_[position_a].insert(b);
  }
  else if (position_a != position_b) {
    islands_[position_a].insert(islands_[position_b].begin(),
                                islands_[position_b].end());
    islands_.erase(islands_.begin() + position_b,
                   islands_.begin() + position_b + 1);
  }
}

void Map::solve_distances(const std::unordered_set<int> &island) {
  // let's do this a different way...
  // all the items in the island are connected and we're starting
  // at the first node
  std::unordered_set<int> min_distances_found;
  std::vector<int> nodes_in_level = { start_node_ };
  std::vector<int> nodes_in_next_level = {};
  int distance = 0;
  // island size - 1 because we don't care about the starting node
  while (min_distances_found.size() < island.size() - 1) {
    distance += 1;
    for (auto node : nodes_in_level) {
      auto next_steps = connections_[node];
      for (auto next_step : next_steps) {
        if (min_distances_found.count(next_step) == 0 &&
            next_step != start_node_) {
          min_distances_found.insert(next_step);
          distances_[next_step] = distance * 6;
          nodes_in_next_level.push_back(next_step);
        }
      }
    }
    nodes_in_level = nodes_in_next_level;
    nodes_in_next_level.clear();
  }
}

std::vector<int> Map::solve_map() {
  // connections AND islands are already set
  // start with distance defaults
  for (int i = 1; i <= n_nodes_; i++) {
    if (i != start_node_) {
      distances_.insert({ i, -1 });
    }
  }
  // get island of start
  int position_island = -1;
  for (int i = 0; i < islands_.size(); i++) {
    if (islands_[i].count(start_node_)) {
      position_island = i;
      break;
    }
  }
  if (position_island >= 0) {
    solve_distances(islands_[position_island]);
  }
  std::vector<int> answer;
  for (int i = 1; i <= n_nodes_; i++) {
    if (i != start_node_) {
      answer.push_back(distances_[i]);
    }
  }
  return answer;
}

int main() {
  int q = -1;
  std::cin >> q;
  std::vector<std::vector<int>> distances;
  for (int i = 0; i < q; i++) {
    int n = -1, m = -1, s = -1;
    std::cin >> n >> m;
    Map map(n);
    for (int j = 0; j < m; j++) {
      int a, b;
      std::cin >> a >> b;
      map.add_edge(a, b);
    }
    std::cin >> s;
    map.start_node_ = s;
    distances.push_back(map.solve_map());
  }
  for (auto const &solution : distances) {
    for (auto const val : solution) {
      std::cout << val << " ";
    }
    std::cout << "\n";
  }
  bool stop;
  std::cin >> stop;
}
