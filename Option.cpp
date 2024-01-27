#include "Option.h"

Option::Option() {
  annual_vol = annual_ret = strike_price = spot_price = time = ret = vol = 0;
}

Option::Option(std::vector<double> greeks) {
  annual_vol = greeks[0];
  annual_ret = greeks[1];
  strike_price = greeks[2];
  spot_price = greeks[3];
  time = greeks[4];
  ret = pow(1 + annual_ret, time) - 1;
  vol = annual_vol * sqrt(time);
}
