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
    int length = price.size();
    if (length < period) {
        std::cout << "The length of 'prices' is less than 'period'." << std::endl;
        return xarray<double>();
    }

    int sma_size = length - period + 1;
    xarray<double> sma = empty<double>({sma_size});
    double currSum = 0.0;
    
    // Calculating SMA for 1st window
    for (int i=0; i<period; i++) {
        currSum += price[i];
    }
    sma(0) = currSum / period;

    // Calculating the Rolling sum for the rest of the 'prices' vector
    for (int i=1; i<sma_size; i++) {
        currSum = currSum - price[i-1] + price[i+period-1];
        sma(i) = currSum / period;
    }
    return sma;
}

// Calculating the standard Deviation over the price of the stock
xarray<double> StandardDeviation(int period, vector<double> price) {
    int length = price.size();
    if (length < period || period == 0) {
        std::cout << "The length of 'prices' is less than 'period' OR 'period' is 0." << std::endl;
        return xarray<double>();
    }
    int stddev_size = length - period + 1;
    xarray<double> stdDev = empty<double>({stddev_size});

    double mean = 0.0;
    double m2 = 0.0;

    // Initialize the first window using standard Welford
    for (int i = 0; i < period; ++i) {
        double delta = price[i] - mean;
        mean += delta / (i + 1);
        m2 += delta * (price[i] - mean);
    }
    stdDev(0) = sqrt(m2 / period);

    // Rolling Window Welford
    for (int i = 1; i < stddev_size; ++i) {
        double old_val = price[i - 1];
        double new_val = price[i + period - 1];

        double old_mean = mean;
        mean += (new_val - old_val) / period;
        // This accounts for the point entering AND the point leaving
        m2 += (new_val - old_val) * (new_val - mean + old_val - old_mean);
        stdDev(i) = sqrt(std::max(0.0, m2 / period));
    }
    return stdDev;
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