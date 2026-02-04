import yfinance as yf
from time import time
import matplotlib.dates as mdates
import matplotlib.pyplot as plt

def download_data(ticker, period, interval): 
    '''
        Arguments:
            ticker = The stock ticker you want to backtest
            period = Must be one of: 1d, 5d, 1mo, 3mo, 6mo, 1y, 2y, 5y, 10y, ytd, max
            interval = Valid intervals: [1m, 2m, 5m, 15m, 30m, 60m, 90m, 1h, 4h, 1d, 5d, 1wk, 1mo, 3mo]
    ''' 
    try:
        df = yf.download(ticker, 
                        period=period,
                        interval=interval)
        df.columns = df.columns.droplevel('Ticker')
        print("Columns: ", df.columns)
        print(f"\nGot data for {ticker}")
        print(df.head())
        df.to_csv(f"{ticker}_{period}_{interval}.csv")
        print("Downloaded")
        return df
    except:
        print(f"Unable to download data for {period} and {interval}.")
    
df = download_data("AAPL", "10y", "1d")


st = time()
moving_avg_100 = df['Close'].rolling(window=100).mean()
moving_avg_250 = df['Close'].rolling(window=250).mean()
et = time()
print("Execution time : ", et-st)



def plot(features: list, legend_labels: list):
  plt.figure(figsize=(14, 7))
  for i, f in enumerate(features):
    plt.plot(f.index, f, label=legend_labels[i])

  plt.title(f'Stock Price and Moving Averages')
  plt.xlabel('Date')
  plt.ylabel('Price')
  plt.grid(True)
  plt.legend()

  ax = plt.gca()
  ax.xaxis.set_major_locator(mdates.DayLocator(interval=1))
  ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
  plt.xticks(rotation=45, ha='right')
  plt.tight_layout()
  plt.show()