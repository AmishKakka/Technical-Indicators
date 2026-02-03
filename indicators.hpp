#include <iostream>
#include "xtensor/containers/xarray.hpp"
#include "xtensor/containers/xadapt.hpp"
#include "xtensor/views/xview.hpp"
#include "xtensor/io/xio.hpp"
#include "xtensor/io/xcsv.hpp"
#include "xtensor/generators/xbuilder.hpp"

using namespace xt;
using std::vector;

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

// Calculating the Volume Weighted Average Price (VWAP) of the stock
xarray<double> VolumeWeightedAveragePrice(int period, vector<double> price, vector<long long int> volume) {
    xarray<double> price_volume;
    vector<double> vwap;

    vector<std::size_t> pshape = {price.size(), 1};
    xarray<double> xprice = adapt(price, pshape);

    vector<std::size_t> vshape = {volume.size(), 1};
    xarray<double> xvolume = adapt(volume, vshape);

    price_volume = xprice * xvolume;

    for (int i=period; i<price.size(); i++) {
        double vwap_i = sum(view(price_volume, range(i-period, i)))() / sum(view(xvolume, range(i-period, i)))();
        vwap.push_back(vwap_i);
    }
    
    vector<std::size_t> vwap_shape = {vwap.size(), 1};
    auto xvwap = adapt(vwap, vwap_shape);
    return xvwap;
}