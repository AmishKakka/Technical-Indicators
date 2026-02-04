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
xarray<double> SimpleMovingAverage(int period, const vector<double>& price) {
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
xarray<double> StandardDeviation(int period, const vector<double>& price) {
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
xarray<double> BollingerBands(int period, const vector<double>& price) {
    xarray<double> sma = SimpleMovingAverage(period, price);
    xarray<double> stdDev = StandardDeviation(period, price);

    xarray<double> upper = sma + (2.0 * stdDev);
    xarray<double> lower = sma - (2.0 * stdDev);

    auto bands_expr = stack(xtuple(lower, upper), 1);
    xarray<double> bands = bands_expr.reshape({-1, 2});
    return bands;
}

// Calculating the Volume Weighted Average Price (VWAP) of the stock
xarray<double> VolumeWeightedAveragePrice(const vector<double>& price, const vector<long long int>& volume) {
    xarray<double> price_volume;
    int length = price.size();
    xarray<double> vwap = empty<double>({length});

    double cumulativePV = 0.0;
    double cumulativeVol = 0.0;

    for (int i = 0; i < length; i++) {
        cumulativePV += price[i] * (double)volume[i];
        cumulativeVol += (double)volume[i];
        
        if (cumulativeVol > 0) {
            vwap(i) = cumulativePV / cumulativeVol;
        } else {
            vwap(i) = 0.0;
        }
    }
    return vwap;
}

// Calculating the Exponential Moving Average (EMA) over the price of the stock
xarray<double> ExponentialMovingAverage(int period, const vector<double>& price) {
    int length = price.size();
    if (length < period) {
        std::cout << "The length of 'prices' is less than 'period'." << std::endl;
        return xarray<double>();
    }

    int ema_size = length - period + 1;
    xarray<double> ema = empty<double>({ema_size});

    double smoothing = 2.0 / (period + 1);
    double prevEMA = 0.0;
    double currEMA = 0.0;
    
    // Setting the inital EMA as the SMA for given period.
    for (int i=0; i<period; i++) {
        prevEMA += price[i];
    }
    ema(0) = prevEMA / period;

    for (int i=1; i<ema_size; i++) {
        currEMA = (price[period+i-1] * smoothing) + (prevEMA * (1-smoothing));
        ema(i) = currEMA;
        prevEMA = currEMA;
    }
    return ema;
}