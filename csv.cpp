#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::vector;
using std::string;

// Defining my own data-type for a defined schema DataFrame.
// A DataFrame is a collection of different vectors of different data-types.
// This allows more easy use and access of a single column's values.
struct DataFrame {
    vector<double> Close;
    vector<double> High;
    vector<double> Open;
    vector<double> Low;
    vector<long long int> Volume;
};

// Function to read the csv files
struct DataFrame readCSV(const string& filename) {
    DataFrame df;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return df;
    }

    string line;
    string header;
    std::getline(file, header);
    std::cout<<"Headers: "<<header<<std::endl;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        string field;
        vector<string> ss_field;
        int i = 0;

        // Getting the row's elements in a temp. vector<string>
        while (std::getline(ss, field, ',')) {
            if (i != 0) {
                ss_field.push_back(field);
            }
            i++;
        }

        // Assigning them to struct df
        df.Close.push_back(std::stod(ss_field[0]));
        df.High.push_back(std::stod(ss_field[1]));
        df.Open.push_back(std::stod(ss_field[2]));
        df.Low.push_back(std::stod(ss_field[3]));
        df.Volume.push_back(std::stoll(ss_field[4]));
    }
    file.close();
    return df;
}
