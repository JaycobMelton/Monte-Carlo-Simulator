#%%
import pandas as pd
import matplotlib.pyplot as plt

prices_file = pd.read_csv('simulated_prices.csv', header=None)
# prices_file = prices_file.dropna()
prices = prices_file.values.tolist()

plt.hist(prices, bins=30, edgecolor='black', color='blue')

plt.title('Monte Carlo Simulated Prices')
plt.xlabel('Price')
plt.ylabel('Frequency')

plt.show()

# print(prices)
# %%
