#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

// Prototyper
bool is_it_in_word(std::string last_four, std::string d);
void create_node_system(const std::vector<std::string> &input);
// Vi ändrar init så den returnerar data eller tar emot referenser
void init(int argc, char *argv[], std::vector<std::string> &word_list,
          std::vector<std::vector<std::string>> &pair_list);

int main(int argc, char *argv[]) {
  std::vector<std::string> word_list;
  std::vector<std::vector<std::string>> pair_list;

  init(argc, argv, word_list, pair_list);

  // Nu kör vi systemet med orden vi faktiskt läste in!
  if (!word_list.empty()) {
    create_node_system(word_list);
  }

  return 0;
}

void init(int argc, char *argv[], std::vector<std::string> &word_list,
          std::vector<std::vector<std::string>> &pair_list) {
  if (argc < 2)
    return;

  std::ifstream file(argv[1]);
  if (!file.is_open())
    return;

  int N, Q;
  if (!(file >> N >> Q))
    return;

  // Läs in ord
  for (int i = 0; i < N; i++) {
    std::string word;
    if (file >> word) {
      word_list.push_back(word);
    }
  }

  // Läs in par
  for (int j = 0; j < Q; j++) {
    std::string w1, w2;
    if (file >> w1 >>
        w2) { // >> hoppar över ALLA mellanslag och radbrytningar automatiskt
      pair_list.push_back({w1, w2});
    }
  }

  // Bekräftelse för debug
  std::cout << "DEBUG: Last in " << word_list.size() << " ord." << std::endl;
}

void create_node_system(const std::vector<std::string> &input) {
  for (const std::string &s : input) {
    if (s.length() < 5)
      continue; // substr(1,4) kräver minst 5 tecken!

    std::string last_four = s.substr(1, 4);
    std::cout << "Kollar bokstaver: " << last_four << " fran ordet " << s << ":"
              << std::endl;

    for (const std::string &d : input) {
      if (d != s && is_it_in_word(last_four, d)) {
        std::cout << "  -> Hittade match i: " << d << std::endl;
      }
    }
  }
}

bool is_it_in_word(std::string last_four, std::string d) {
  std::unordered_set<char> word_set(d.begin(), d.end());
  for (char c : last_four) {
    if (word_set.find(c) == word_set.end()) {
      return false;
    }
  }
  return true;
}
