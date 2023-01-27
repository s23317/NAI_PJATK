#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;

random_device rd;
mt19937 mt_generator(rd());

int globalIterations = 1000000;

auto brute_force = [](
        const function<double(pair<double, double>)> &f,
        const function<pair<double, double>(int, int)> &domain,
        int iterations, int a, int b) {

    auto current_p = domain(a, b);
    auto best_point = current_p;

    for (int i = 0; i < iterations; ++i) {
        if (f(current_p) < f(best_point)) {
            best_point = current_p;
        }
        current_p = domain(a, b);
    }

    return best_point;
};

auto hill_climbing = [](
        const function<double(pair<double, double> pair)> &f,
        const function<pair<double, double>(int, int)> &start_point,
        const function<vector<pair<double, double>>(pair<double, double>, int, int)> &get_close_points,
        int max_iterations, int a, int b) {

    pair<double, double> best_p = start_point(a, b);

    for (int i = 0; i < max_iterations; i++) {
        auto close_points = get_close_points(best_p, a, b);
        auto best_neighbour =
                *min_element(close_points.begin(), close_points.end(),
                                  [f](auto a, auto b) { return f(a) > f(b); });
        if (f(best_neighbour) < f(best_p)) best_p = best_neighbour;
    }
    return best_p;
};

auto simulated_annealing = [](
        const function<double(pair<double, double> pair)> &f,
        const function<pair<double, double>(int, int)> &domain,
        const function<pair<double, double>(pair<double, double>, int, int)> &neighbour,
        int iterations, int a, int b) {

    vector<pair<double, double>> pairsVector;
    uniform_real_distribution<double> uk(0.0, 1.0);

    auto best_point = domain(a, b);
    pairsVector.push_back(best_point);

    for (int i = 0; i < iterations; ++i) {
        double ukValue = uk(mt_generator);
        auto tk = neighbour(best_point, a, b);
        if (f(tk) <= f(best_point)) {
            best_point = tk;
            pairsVector.push_back(best_point);
        } else {
            if (ukValue < exp(-(abs(f(tk) - f(best_point)) / (1 / log(i))))) {
                best_point = tk;
                pairsVector.push_back(best_point);
            }
        }
    }
    return best_point;
};

bool isInDomain(double x, int a, int b) {
    return (a <= x) && (x <= b);
}

auto xy_generator = [](int a, int b) {
    uniform_real_distribution<> dis(a, b);
    return pair<double, double>(dis(mt_generator), dis(mt_generator));
};

auto xy_generator2 = [](pair<double, double> p, int a, int b) -> vector<pair<double, double>> {
    double precision = 1.0 / 128.0;
    vector<pair<double, double>> ret;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            pair<double, double> new_point = {p.first + (i * precision),
                                              p.second + (j * precision)};
            if (isInDomain(new_point.first, a, b) &&
                isInDomain(new_point.second, a, b)) {
                ret.push_back(new_point);
            }
        }
    }
    return ret;
};

auto xy_generator3 = [](pair<double, double> p, int a, int b) {
    normal_distribution<> neighbour;
    double firstPair, secondPair;

    do {
        firstPair = p.first + neighbour(mt_generator) * 0.01;
    } while (firstPair < a && firstPair > b);

    do {
        secondPair = p.second + neighbour(mt_generator) * 0.01;
    } while (secondPair < a && secondPair > b);

    return pair<double, double>(firstPair, secondPair);
};

int main() {
    using namespace chrono;

    auto Beale = [](pair<double, double> pair) {
        return pow((1.5 - pair.first + pair.first * pair.second), 2) +
               pow(2.25 - pair.first + pow(pair.first * pair.second, 2), 2) +
               pow(2.625 - pair.first + pow(pair.first * pair.second, 3), 2);
    };
    auto Booth = [](pair<double, double> pair) {
        return pow(pair.first + 2 * pair.second - 7, 2) +
               pow(2 * pair.first + pair.second - 5, 2);
    };
    auto McCormick = [](pair<double, double> pair) {
        return sin(pair.first + pair.second) +
               pow(pair.first - pair.second, 2) -
               1.5 * pair.first + 2.5 * pair.second + 1;
    };


    cout << "Beale Function\n";

    auto start = high_resolution_clock::now();
    auto ackley_burte = brute_force(Beale, xy_generator, globalIterations, -4.5, 4.5);
    auto stop = high_resolution_clock::now();
    cout << "brute_force (x,y) = " << ackley_burte.first << ", " << ackley_burte.second
         << " | result: " << Beale(ackley_burte)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto ackley_hill = hill_climbing(Beale, xy_generator, xy_generator2, globalIterations, -4.5, 4.5);
    stop = high_resolution_clock::now();
    cout << "hill_climbing (x,y) = " << ackley_hill.first << ", " << ackley_hill.second
         << " | result: " << Beale(ackley_hill)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto ackley_annealing = simulated_annealing(Beale, xy_generator, xy_generator3, globalIterations, -4.5, 4.5);
    stop = high_resolution_clock::now();
    cout << "simulated_annealing (x,y) = " << ackley_annealing.first << ", " << ackley_annealing.second
         << " | result: " << Beale(ackley_annealing)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n\n";

    cout << "Booth Function\n";

    start = high_resolution_clock::now();
    auto himmelblau_burte = brute_force(Booth, xy_generator, globalIterations, -10, 10);
    stop = high_resolution_clock::now();
    cout << "brute_force (x,y) = " << himmelblau_burte.first << ", " << himmelblau_burte.second
         << " | result: " << Booth(himmelblau_burte)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto himmelblau_hill = hill_climbing(Booth, xy_generator, xy_generator2, globalIterations, -10, 10);
    stop = high_resolution_clock::now();
    cout << "hill_climbing (x,y) = " << himmelblau_hill.first << ", " << himmelblau_hill.second
         << " | result: " << Booth(himmelblau_hill)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto himmelblau_annealing = simulated_annealing(Booth, xy_generator, xy_generator3, globalIterations, -10, 10);
    stop = high_resolution_clock::now();
    cout << "simulated_annealing (x,y) = " << himmelblau_annealing.first << ", " << himmelblau_annealing.second
         << " | result: " << Booth(himmelblau_annealing)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n\n";


    cout << "McCormick Function\n";

    start = high_resolution_clock::now();
    auto holderTable_burte = brute_force(McCormick, xy_generator, globalIterations, -1.5, 4);
    stop = high_resolution_clock::now();
    cout << "brute_force (x,y) = " << holderTable_burte.first << ", " << holderTable_burte.second
         << " | result: " << McCormick(holderTable_burte)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto holderTable_hill = hill_climbing(McCormick, xy_generator, xy_generator2, globalIterations, -1.5, 4);
    stop = high_resolution_clock::now();
    cout << "hill_climbing (x,y) = " << holderTable_hill.first << ", " << holderTable_hill.second
         << " | result: " << McCormick(holderTable_hill)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n";
    start = high_resolution_clock::now();
    auto holderTable_annealing = simulated_annealing(McCormick, xy_generator, xy_generator3, globalIterations, -1.5, 4);
    stop = high_resolution_clock::now();
    cout << "simulated_annealing (x,y) = " << holderTable_annealing.first << ", " << holderTable_annealing.second
         << " | result: " << McCormick(holderTable_annealing)
         << " | time: " << duration_cast<microseconds>(stop - start).count()
         << "\n\n";

    cout
            << "Metoda najlepsza dla podobnej jakosci wynikow: BruteForce \n Metoda ktora sie nie nadaje - Hill Climbing \n";

    return 0;
}
