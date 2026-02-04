# Technical-Indicators

## Overview
1. A python script gets data for a stock using the **yfinance** library with specified period and intervals.
Data is saved into a CSV file.

2. Then, **readCSV()** in csv.cpp reads the file and formats the data.

3. A list of indicators like **Bollinger Bands, SMA (Simple Moving Average), VWAP (Volume Weighted Average Price), and EMA (Exponential Moving Average)** are defined to run on the stock's OHLC and Volume data.

4. Use cmake command to compile and then run.


## Speed Test

20-SMA, 100-SMA, and 250-SMA on Apple's 10 year data with a single data as interval.

**get_data.py** ->
```py
df = download_data("AAPL", "10y", "1d")

st = time()
moving_avg_20 = df['Close'].rolling(window=20).mean()
moving_avg_100 = df['Close'].rolling(window=100).mean()
moving_avg_250 = df['Close'].rolling(window=250).mean()
et = time()
print(f"Execution time : {et-st} seconds")
```

```sh  
Technical-Indicators % python3 get_data.py
Got data for AAPL
Price           Close       High        Low       Open     Volume
Date                                                             
2016-02-04  21.898483  22.063969  21.578848  21.730731  185886800
2016-02-05  21.313620  21.971028  21.238812  21.880351  185672400
2016-02-08  21.538048  21.694464  21.091463  21.111865  216085600
2016-02-09  21.533508  21.748867  21.293215  21.374824  177324800
2016-02-10  21.370287  21.841807  21.331750  21.744330  169374400
Downloaded
Execution time : 0.002521991729736328 seconds
```

**main.cpp** ->
```cpp
int main() {
    // ... Reading CSV file

    // 20-SMA
    xarray<double> sma_20 = SimpleMovingAverage(20, csvData.Close);
    std::cout << "20-SMA vector size: " << sma_20.size() << std::endl;
    // 100-SMA
    xarray<double> sma_100 = SimpleMovingAverage(100, csvData.Close);
    std::cout << "100-SMA vector size: " << sma_100.size() << std::endl;
    // 250-SMA
    xarray<double> sma_250 = SimpleMovingAverage(250, csvData.Close);
    std::cout << "250-SMA vector size: " << sma_250.size() << std::endl;
    
    // ... Rest of the code
}
```

```sh
Technical-Indicators % cmake --build ./build --config Release && ./build/main
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[100%] Linking CXX executable main
[100%] Built target main
Headers: Date,Close,High,Low,Open,Volume
Total data points: 2514
20-SMA vector size: 2495
100-SMA vector size: 2415
250-SMA vector size: 2265
Execution time: 0.099959 ms
```

So, we can see here for the same query Python code using Numpy at the backend to calculate the MA is *fast*, only **2.5 ms**.
But, the xtensor framework in C++ is ***just faster***, taking **0.099 ms**.

## Installation Steps
1. I have used the **xtensor** library (it's like Numpy but for C++) to handle arrays and calculations. To install xtensor - 
    ```sh
    git clone https://github.com/microsoft/vcpkg.git

    ./vcpkg/bootstrap-vcpkg.sh

    ./vcpkg/vcpkg install xtensor
    ```
    

2. Create a 'build' folder and move in it.
    ```mkdir build && cd cuild```

3. Configure the project
    1. Debug mode ( Takes longer to run as compared to Release mode)
        ```sh 
        cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake && cd ..
        ```

    2. Release mode
        ```sh
        cmake .. -DCMAKE_BUILD_TYPE=Release \           
            -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake && cd..
        ```

4. Compile and Run
    1. Debug mode
        ```sh 
        cmake --build ./build && ./build/main
        ```
    2. Release mode
        ```sh 
        cmake --build ./build --config Release && ./build/main
        ```

5. Removing the 'build' folder 
    ```rm -rf build```

6. To get the financial data, we use yfinance and for plotting we use matplotlib.
    ```sh
    pip3 install yfinance matplotlib
    ```