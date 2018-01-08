#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "project_euler.h"

class Matrix {
 public:
  Matrix();
  void print_data();
  void solve();
  int get_minimum_sum();

 private:
  std::vector<std::vector<int>> data;
  std::vector<std::vector<int>> minimum_paths;
  int size;
};

void Matrix::print_data() {
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < data.size(); j++) {
      std::cout << data[i][j] << " ";
    }
    std::cout << "\n";
  }
}

Matrix::Matrix() {
  std::string line;
  std::string file_name = "p081_matrix.txt";
  // std::string file_name = "081_sample.txt";
  std::ifstream file(file_name);
  while (true) {
    auto tokens = Parse::get_tokens(file, '#', ',');
    if (tokens.empty()) break;
    std::vector<int> row;
    for (auto token : tokens) {
      row.push_back(std::stoi(token));
    }
    data.push_back(row);
  }
  size = data.size();
  std::vector<int> empty(size, 0);
  minimum_paths.resize(size, empty);
  minimum_paths[size - 1][size - 1] = data[size - 1][size - 1];
  solve();
}

void Matrix::solve() {
  // fill in bottom row
  for (int col = size - 2; col >= 0; col--) {
    minimum_paths[size - 1][col] =
        minimum_paths[size - 1][col + 1] + data[size - 1][col];
  }
  // fill in the right column
  for (int row = size - 2; row >= 0; row--) {
    minimum_paths[row][size - 1] =
        minimum_paths[row + 1][size - 1] + data[row][size - 1];
  }
  // now just fill in the rest :)
  int row = size - 2;
  while (row >= 0) {
    for (int col = size - 2; col >= 0; col--) {
      minimum_paths[row + 1][col] < minimum_paths[row][col + 1]
          ? minimum_paths[row][col] =
                data[row][col] + minimum_paths[row + 1][col]
          : minimum_paths[row][col] =
                data[row][col] + minimum_paths[row][col + 1];
    }
    row--;
  }
}

int Matrix::get_minimum_sum() { return minimum_paths[0][0]; }

int main() {
  auto matrix = Matrix();
  std::cout << matrix.get_minimum_sum() << std::endl;
  return 0;
}
