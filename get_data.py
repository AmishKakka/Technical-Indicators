import yfinance as yf
from time import time

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
    except:
        print(f"Unable to download data for {period} and {interval}.")
    
download_data("AAPL", "1mo", "90m")