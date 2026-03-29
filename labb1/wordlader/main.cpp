#include <algorithm>
#include <any>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory_resource>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class node {

public:
  std::string name;
  node(std::string n) : name(n) {};
  node() : name(""), visited(0), dist(0) {};
  void ad_edge(std::string linked_node) { edges.push_back(linked_node); }
  int visited;
  std::vector<std::string> edges;
  std::string pred;
  int dist;
};

struct problem_data {
  std::unordered_map<std::string, node> graph;
  std::vector<std::vector<std::string>> queries;
};

struct read_data {
  std::vector<std::string> word_list;
  std::vector<std::vector<std::string>> pair_list;
};

bool is_it_in_word(std::string last_four, std::string d);
std::unordered_map<std::string, node>
create_node_system(std::vector<std::string> &input);
problem_data init(int argc, char *argv[]);
read_data reed_input(int argc, char *argv[]);
void run(problem_data &data);
int BFS(std::unordered_map<std::string, node> &Graf, std::string s,
        std::string t);

int main(int argc, char *argv[]) {
  problem_data data = init(argc, argv);
  run(data);
  return 1;
}

problem_data init(int argc, char *argv[]) {
  read_data from_input = reed_input(argc, argv);
  problem_data data;
  data.graph = create_node_system(from_input.word_list);
  data.queries = from_input.pair_list;
  return data;
}

// Reeds file and gives word list and all the pairs in a seperate list
read_data reed_input(int argc, char *argv[]) {
  int N = 0;
  int Q = 0;
  std::vector<std::string> word_list;
  std::vector<std::vector<std::string>> pair_list;
  read_data data;
  // Checking if input was given
  if (argc < 2) {
    std::cout << "FÖr får arg" << std::endl;
    return {};
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "Kan ej öppna" << std::endl;
    return {};
  }
  file >> N >> Q;
  for (int i = 0; i < N; i++) {
    std::string word;
    file >> word;
    word_list.push_back(word);
  }

  for (int j = 0; j < Q; j++) {
    std::vector<std::string> word_pair;
    std::string w1;
    std::string w2;
    file >> w1 >> w2;
    word_pair.push_back(w1);
    word_pair.push_back(w2);
    pair_list.push_back(word_pair);
  }
  data.word_list = word_list;
  data.pair_list = pair_list;
  // std::cout << "Vi har " << N << " ord och " << Q << " par" << std::endl;
  // for (std::string w : word_list) {
  //   std::cout << w << std::endl;
  // }
  // for (std::vector<std::string> p : pair_list) {
  //   std::cout << p[0] + p[1] << std::endl;
  // }
  return data;
}

// Takes the word_list and gives a graf
std::unordered_map<std::string, node>
create_node_system(std::vector<std::string> &input) {
  std::unordered_map<std::string, node> graf;
  for (std::string &s : input) {
    std::string last_four = s.substr(1, 4);
    node new_node(s);
    for (std::string &d : input) {
      bool word_contains_last_four = is_it_in_word(last_four, d);
      if (d != s && word_contains_last_four == true) {
        // std::cout << "<True>" << std::endl;
        new_node.ad_edge(d);
      }
    }
    graf.emplace(s, new_node);
  }
  return graf;
}

// Takes the last four letters in a word and another word and checks if should
// be connected with another
bool is_it_in_word(std::string last_four, std::string d) {
  std::unordered_set<char> word_set(d.begin(), d.end());
  for (char c : last_four) {
    if (!word_set.count(c)) {
      return false;
    }
  }
  return true;
}

void run(problem_data &data) {
  std::unordered_map<std::string, node> &Graf = data.graph;
  for (std::vector<std::string> &query : data.queries) {
    std::string s = query[0];
    std::string t = query[1];
    std::cout << BFS(Graf, s, t) << std::endl;
  }
}

int BFS(std::unordered_map<std::string, node> &Graf, std::string s,
        std::string t) {
  for (auto &[id, v] : Graf) {
    v.visited = 0;
    v.dist = 0;
  }

  Graf[s].visited = 1;
  Graf[s].dist = 0;

  std::queue<std::string> q;
  q.push(s);

  while (!q.empty()) {
    std::string current_name = q.front();
    q.pop();

    node &n = Graf[current_name];

    for (std::string &w : n.edges) {
      if (Graf[w].visited == 0) {
        Graf[w].visited = 1;
        Graf[w].dist = n.dist + 1;
        Graf[w].pred = n.name;

        if (w == t) {
          return Graf[w].dist;
        }
        q.push(w);
      }
    }
  }
  return -1;
}
