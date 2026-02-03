# Technical-Indicators

## Overview
1. A python script gets data for a stock using the **yfinance** library with specified period and intervals.
Data is saved into a CSV file.

2. Then, **readCSV()** in csv.cpp reads the file and formats the data.

3. A list of indicators like **Bollinger Bands and Simple Moving Average** are defined to run on the stock's OHLC and Volume data.

4. Use cmake command to compile and then run.


## Installation Steps
1. I have used the **xtensor** library (it's like Numpy but for C++) to handle arrays and calculations. To install xtensor - 
    ```sh
    git clone https://github.com/microsoft/vcpkg.git

    ./vcpkg/bootstrap-vcpkg.sh

    ./vcpkg/vcpkg install xtensor
    

2. Create a 'build' folder
    ```mkdir build```


3. Configure the project
   ``` cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake```


4. Compile and Run
   ``` cmake --build ./build && ./build/main```


5. Cleaning the 'build' folder 
    ```rm -rf build```