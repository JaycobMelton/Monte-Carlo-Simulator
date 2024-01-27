#include <cmath>
#include <fstream>
#include <iostream>
#include "Option.h"
#include <random>
#include <vector>

class MonteCarlo {
public:
  MonteCarlo(Option, int);
  void Run();
  double CDF(double, double, double);
  double InvCDF(double);
  double GetCallVal();
  double GetPutVal();
  void WriteToCSV();
private:
  Option option_;
  int num_simulation_;
  std::vector<double> sim_price_, sim_call_, sim_put_;
  double call_val_, put_val_;
};