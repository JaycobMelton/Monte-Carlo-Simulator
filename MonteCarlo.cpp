#include "MonteCarlo.h"

MonteCarlo::MonteCarlo(Option option, int num_simulation) {
  option_ = option;
  num_simulation_ = num_simulation;  
  call_val_ = put_val_ = 0.0;
  Run();
}

void MonteCarlo::Run() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  for (int i = 0; i < num_simulation_; i++) {
    double random_p = distribution(rng);
    double sim_ret = InvCDF(random_p);
    double sim_price = option_.spot_price * (1 + sim_ret);
    sim_price_.push_back(sim_price);

    double sim_call_val = std::max(sim_price - option_.strike_price, 0.0);
    sim_call_.push_back(sim_call_val);
    call_val_ += sim_call_val;

    double sim_put_val = std::max(option_.strike_price - sim_price, 0.0);
    sim_put_.push_back(sim_put_val);
    put_val_ += sim_put_val;
  }

  call_val_ /= num_simulation_;
  put_val_ /= num_simulation_;
}

double MonteCarlo::CDF(double x, double mu, double sigma) {
  return 0.5 * (1 + std::erf((x - mu) / (std::sqrt(2) * sigma)));
}

double MonteCarlo::InvCDF(double p) {
  double mu = option_.ret;
  double sigma = option_.vol;
  
  double lower_bound = mu - 5.0 * sigma;
  double upper_bound = mu + 5.0 * sigma;
  double epsilon = 1e-6;

  while (upper_bound - lower_bound > epsilon) {
    double mid_point = (lower_bound + upper_bound) / 2.0;
    double mid_prob = CDF(mid_point, mu, sigma);

    if (mid_prob < p) {
      lower_bound = mid_point;
    } else {
      upper_bound = mid_point;
    }
  }

  return (lower_bound + upper_bound) / 2.0;
}

double MonteCarlo::GetCallVal() {
  return call_val_;
}

double MonteCarlo::GetPutVal() {
  return put_val_;
}

void MonteCarlo::WriteToCSV() {
  std::ofstream output_file("simulated_prices.csv");

  if (!output_file.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    return;
  }
  for (const auto& price : sim_price_) {
    output_file << price << ",";
  }
  output_file.close();
}