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
  std::string file_name = "p082_matrix.txt";
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
  solve();
}

void Matrix::solve() {
  // Trival final column fill
  for (int row = 0; row < size; row++) {
    minimum_paths[row][size - 1] = data[row][size - 1];
  }
  for (int col = size - 2; col >= 0; col--) {
    for (int row = 0; row < size; row++) {
      std::vector<int> possible_values;
      // go straight right
      possible_values.push_back(minimum_paths[row][col + 1]);
      // check up
      for (int top_row = 0; top_row < row; top_row++) {
        int sum = minimum_paths[top_row][col + 1];  // to the right of top row
        for (int sum_me = top_row; sum_me < row; sum_me++) {
          // add up the values that go up to that row
          sum += data[sum_me][col];
        }
        possible_values.push_back(sum);
      }
      // check down
      for (int bottom_row = size - 1; bottom_row > row; bottom_row--) {
        int sum =
            minimum_paths[bottom_row][col + 1];  // to the right of bottom row
        for (int sum_me = bottom_row; sum_me > row; sum_me--) {
          // add up the values that go up to that row
          sum += data[sum_me][col];
        }
        possible_values.push_back(sum);
      }
      std::sort(possible_values.begin(), possible_values.end());
      minimum_paths[row][col] = possible_values[0] + data[row][col];
    }
  }
}

int Matrix::get_minimum_sum() {
  std::vector<int> first_column_values;
  for (int row = 0; row < size; row++) {
    first_column_values.push_back(minimum_paths[row][0]);
  }
  std::sort(first_column_values.begin(), first_column_values.end());
  return first_column_values[0];
}

int main() {
  auto matrix = Matrix();
  std::ofstream file("answer.txt");
  file << matrix.get_minimum_sum() << std::endl;
  return 0;
}
