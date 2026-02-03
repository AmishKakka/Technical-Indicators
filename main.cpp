#include <iostream>
#include <fstream>
#include "csv.hpp"
#include "indicators.hpp"

using namespace xt;
using std::vector;


int main() {
    struct DataFrame csvData = readCSV("AAPL_6mo_1h.csv");
    long int total_points = csvData.Close.size();

    // for (long int i=0; i<total_points; i++) {
    //     std::cout<<csvData.Close[i]<<" |   "<<
    //     csvData.High[i]<<" |   "<<
    //     csvData.Low[i]<<" |   "<<
    //     csvData.Open[i]<<" |   "<<
    //     csvData.Volume[i]<<std::endl;
    // }
    std::cout << "Total data points: " << total_points << std::endl;

    xarray<double> sma = SimpleMovingAverage(100, csvData.Close);
    std::cout << "100-SMA vector size: " << sma.size() << std::endl;

    xarray<double> bollinger = BollingerBands(20, csvData.Close);    
    std::ofstream file("bollinger_results.csv");
    dump_csv(file, bollinger);
    std::cout << bollinger << std::endl;

    xarray<double> vwap = VolumeWeightedAveragePrice(20, csvData.Close, csvData.Volume);
    for (auto i:vwap) {
        std::cout << i <<   "  " << std::endl;
    }
    std::cout << "VWAP vector size: " << vwap.size() ;
    return 0;   
}