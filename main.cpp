#include <iostream>
#include <fstream>
#include "xtensor/containers/xarray.hpp"
#include "xtensor/containers/xadapt.hpp"
#include "xtensor/views/xview.hpp"
#include "xtensor/io/xio.hpp"
#include "xtensor/io/xcsv.hpp"
#include "csv.cpp"

using namespace xt;

// Calculating Simple Moving Average over the price of the stock
xarray<double> SimpleMovingAverage(int period, vector<double> price) {
    vector<double> sma;
    vector<std::size_t> shape = {price.size(), 1};
    // Creating a tensor interface for - vector<double>
    auto window = adapt(price, shape);

    // Going through windows of size period (20, 50, 100) and calculate the mean
    for (int i=period; i<price.size(); i++) {
        double windowMean = mean(view(window, range(i-period, i)))();
        sma.push_back(windowMean);
    }

    vector<std::size_t> sshape = {sma.size(), 1};
    auto xsma = adapt(sma, sshape);
    return xsma;
}

// Calculating the standard Deviation over the price of the stock
xarray<double> StandardDeviation(int period, vector<double> price) {
    vector<double> stdDev;
    vector<std::size_t> shape = {price.size(), 1};
    // Creating a tensor interface for - vector<double>
    auto window = adapt(price, shape);

    // Going through windows of size period (20, 50, 100) and calculate the std. deviation
    for (int i=period; i<price.size(); i++) {
        double windowStdDev = stddev(view(window, range(i-period, i)))();
        stdDev.push_back(windowStdDev);
    }

    vector<std::size_t> sshape = {stdDev.size(), 1};
    auto xstdDev = adapt(stdDev, sshape);
    return xstdDev;
}

// Calculating the upper and lower price ranges of the Bollinger Band
xarray<double> BollingerBands(int period, vector<double> price) {
    xarray<double> sma = SimpleMovingAverage(period, price);
    xarray<double> stdDev = StandardDeviation(period, price);

    xarray<double> upper = sma + (2.0 * stdDev);
    xarray<double> lower = sma - (2.0 * stdDev);

    auto bands_expr = stack(xtuple(lower, upper), 1);
    xarray<double> bands = bands_expr.reshape({-1, 2});
    return bands;
}




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


    xarray<double> bollinger = BollingerBands(20, csvData.Close);    
    std::ofstream file("bollinger_results.csv");
    dump_csv(file, bollinger);
    std::cout << bollinger << std::endl;
    return 0;   
}