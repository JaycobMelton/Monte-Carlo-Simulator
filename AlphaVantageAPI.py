import requests as rq
import json
import csv
import pandas as pd

def GetData(func: str, symbol: str, key: str, file_type: str) -> dict:
    url = 'https://www.alphavantage.co/query?function=' + func + '&symbol=' + symbol + '&apikey=' + key
    r = rq.get(url)
    if r.status_code == 200:
        return r.json()
    else:
        print(f'Error: {r.status_code}')
        return None


def WriteToJSON(data: dict, filename: str):
    with open(filename, 'w') as json_file:
        json.dump(data, json_file, indent = 4)

def WriteToCSV(data: dict, json_filename: str, filename: str):
    # Read JSON data from the file
    with open(json_filename, 'r') as json_file:
        data = json.load(json_file)

    # Specify the CSV file name
    csv_file_name = 'SPY_daily_prices.csv'

    # Write data to CSV file
    with open(csv_file_name, 'w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)

    # Write header
    header = data[0].keys()
    csv_writer.writerow(header)

    # Write data rows
    for row in data:
        csv_writer.writerow(row.values())


def FormatToDataFrame(data: dict):
    closing_prices = [float(entry["4. close"]) for entry in data["Time Series (Daily)"].values()]
    dates = list(data["Time Series (Daily)"].keys())
    
    df = {"Date": pd.to_datetime(dates), "Closing Price": closing_prices}
    df = pd.DataFrame(df)
    return df
    

def GetAnnualVolatility(df) -> float:
    df.set_index("Date", inplace=True)
    
    # Only looking for the volatility in the last year
    start_date = df.index.min() - pd.DateOffset(years=1)
    
    df = df[df.index >= start_date]
    
    # Calculate daily change in price
    df["Daily Returns"] = df["Closing Price"].pct_change()
    
    # Calculate annual volatility
    annual_volatility = df['Daily Returns'].std() * (252 ** 0.5)

    return annual_volatility


def GetAnnualReturn(df) -> float:
    # Calculate daily returns
    df["Daily Returns"] = df["Closing Price"].pct_change()

    # Calculate annual return
    last_price = df["Closing Price"].iloc[-1]
    first_price = df["Closing Price"].iloc[0]
    years = (df.index[-1] - df.index[0]).days / 365
    annual_return = ((last_price / first_price) ** (1 / years)) - 1

    return annual_return

def WriteGreeksToCSV(annual_volatility: float, annual_return: float):
    header = ["Annual Volatility", "Annual Return"]
    values = [annual_volatility, annual_return]
    with open("greeks.csv", "w", newline="") as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(header)
        csv_writer.writerow(values)
    csvfile.close()
        
        
def main():
    func = 'TIME_SERIES_DAILY'
    symbol = 'SPY'
    key = 'YOUR_KEY'
    data = GetData(func, symbol, key, 'json')
    filename = 'security_data.json'
    WriteToJSON(data, filename)
    
    with open("security_data.json") as file:
        data = json.load(file)
    df = FormatToDataFrame(data)
    
    
    annual_volatility = GetAnnualVolatility(df)
    
    annual_return = GetAnnualReturn(df)
    
    WriteGreeksToCSV(annual_volatility, annual_return)
    
    
  
if __name__ == '__main__':
    main()
  
  

        
