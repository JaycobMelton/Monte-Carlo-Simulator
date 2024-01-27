#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "MonteCarlo.h"
#include "Option.h"

std::vector<double> ReadCSV(std::string);

int main(int argc, char *argv[]) {
  std::cout << "Program name: " << argv[0] << std::endl;

  std::vector<double> greeks;
  greeks.insert(greeks.end(), ReadCSV(argv[1]).begin(), ReadCSV(argv[1]).end());
  
  for (int i = 2; i <= argc; i++) {
    greeks.push_back(std::stod(argv[i]));
  }

  Option option(greeks);

  MonteCarlo monte_carlo(option, 100000);
  std::cout << "Call Value: $" << monte_carlo.GetCallVal() << ", Put Value: $" << monte_carlo.GetPutVal() << std::endl;
  monte_carlo.WriteToCSV(); 
  
  return 0;
}

std::vector<double> ReadCSV(std::string file_name) {
  std::ifstream file(file_name);

  if (!file.is_open()) {
    std::cerr << "Error: Unable to open the file: " << file_name << std::endl;
    return {};
  }

  std::string line;
  std::getline(file, line);

  std::vector<double> greeks;

  if (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) {
      greeks.push_back(std::stod(token));
    }  
  } else {
    std::cerr << "Error: File has fewer than two lines." << std::endl;
    return {};
  }
  file.close();

  return greeks;
}