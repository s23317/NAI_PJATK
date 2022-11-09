#include <math.h>
#include <iostream>
#include <vector>
#include <functional>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());

double findMinimum(auto f, std::vector<double> domain, int iterations) {
    using namespace std;
    uniform_real_distribution<double> dist(domain.at(0), domain.at(1));
    double min = f(domain.at(0), domain.at(1));
    for (int i = 0; i < iterations - 1; i++) {
        double rand1 = dist(mt_generator);
        double rand2 = dist(mt_generator);
        double comp;
        comp = f(rand1, rand2);
        if (comp < min) {
            min = comp;
        }
    }
    return min;
}

int main() {
    using namespace std;
    double result = 0;

    auto booth = [](double x, double y) {
        return pow(x + 2 * y - 7, 2) +
               pow(2 * x + y - 5, 2);
    };

    auto beale = [](double x, double y) {
        return pow((1.5 - x + x * y), 2) +
               pow(2.25 - x + pow(x * y, 2), 2) +
               pow(2.625 - x + pow(x * y, 3), 2);
    };

    auto mccormick = [](double x, double y) {
        return sin(x + y) +
               pow(x - y, 2) -
               1.5 * x + 2.5 * y + 1;
    };

    vector<double> domainBeale = {-4.5, 4.5};
    vector<double> domainBooth = {-10, 10};
    vector<double> domainMc = {-1.5, 4};

    int iterations = 100000;

    cout << "Beale: " << findMinimum(beale, domainBeale, iterations) << endl;
    cout << "Booth: " << findMinimum(booth, domainBooth, iterations) << endl;
    cout << "McCormick: " << findMinimum(mccormick, domainMc, iterations) << endl;

    return 0;
}
