# Monte-Carlo-Simulator
Basic Monte Carlo simulation for options pricing with a histogram plotter. Gets data from Alpha Vantage API.

# How to Use
* Get daily time series data from AlphaVantageAPI.py (calculates annual volatility and annual return and puts them in greeks.csv)
* Run main.cpp with command line arguments greeks.csv, (strike price), (spot price), (time to maturity)
* Run PricePlotter.py to see the price distribution
