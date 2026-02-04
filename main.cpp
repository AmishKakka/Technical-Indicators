#include <iostream>
#include <fstream>
#include <chrono>
#include "csv.hpp"
#include "indicators.hpp"

using namespace xt;
using std::vector;


int main() {
    struct DataFrame csvData = readCSV("AAPL_10y_1d.csv");
    long int total_points = csvData.Close.size();

    // for (long int i=0; i<total_points; i++) {
    //     std::cout<<csvData.Close[i]<<" |   "<<
    //     csvData.High[i]<<" |   "<<
    //     csvData.Low[i]<<" |   "<<
    //     csvData.Open[i]<<" |   "<<
    //     csvData.Volume[i]<<std::endl;
    // }
    std::cout << "Total data points: " << total_points << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // 20-SMA
    // xarray<double> sma_20 = SimpleMovingAverage(20, csvData.Close);
    // std::cout << "20-SMA vector size: " << sma_20.size() << std::endl;
    // 100-SMA
    // xarray<double> sma_100 = SimpleMovingAverage(100, csvData.Close);
    // std::cout << "100-SMA vector size: " << sma_100.size() << std::endl;
    // 250-SMA
    // xarray<double> sma_250 = SimpleMovingAverage(250, csvData.Close);
    // std::cout << "250-SMA vector size: " << sma_250.size() << std::endl;

    // Bollinger Bands
    // xarray<double> bollinger = BollingerBands(20, csvData.Close);    
    // std::ofstream file("bollinger_results.csv");
    // dump_csv(file, bollinger);
    // std::cout << bollinger << std::endl;

    // VWAP
    // xarray<double> vwap = VolumeWeightedAveragePrice(csvData.Close, csvData.Volume);
    // std::cout << "VWAP vector size: " << vwap.size() << std::endl;

    // EMA
    xarray<double> ema = ExponentialMovingAverage(20, csvData.Close);
    std::cout << "EMA vector size: " << ema.size() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    return 0;   
}