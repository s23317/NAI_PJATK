#include <iostream>
#include <vector>
#include <functional>
#include <random>

using namespace std;

random_device rd;
mt19937 mt_generator(rd());

using chromosome_t = vector<int>;
using population_t = vector<chromosome_t>;

auto ackley_f = [](pair<double, double> pair) {
    return -20 * exp(-0.2 * sqrt(0.5 * (pow(pair.first, 2) + pow(pair.second, 2)))) -
           exp(0.5 * (cos(2 * M_PI * pair.first) + cos(2 * M_PI * pair.second))) + exp(1) + 20;
}; // domain = -5;5

auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation, auto mutation) {

    uniform_real_distribution<double> uniform(0.0, 1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);

    while (!term_condition(population, population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;

        for (int i = 0; i < parents_indexes.size(); i += 2) {
            decltype(initial_population) offspring = {population[i], population[i + 1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (auto chromosome: offspring) new_population.push_back(chromosome);
        }

        for (auto &chromosome: new_population) {
            chromosome = mutation(chromosome, p_mutation);
        }

        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};

vector<int> selection_empty(vector<double> fitnesses) {
    return {};
}

vector<chromosome_t> crossover_empty(vector<chromosome_t> parents) {
    return parents;
}

chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    return parents;
}

population_t generate_population(int size) {
    population_t pop;

    for (int i = 0; i < size; ++i) {
        chromosome_t ch;

        for (int j = 0; j < size; ++j) {
            uniform_int_distribution<int> uni(0, 1);
            ch.push_back(uni(mt_generator));
        }
        pop.push_back(ch);
    }
    return pop;
}


pair<double, double> decode(chromosome_t chromosome) {
    int half = chromosome.size() / 2;
    double x = 0.0;
    double y = 0.0;

    for (int i = 1; i < half; i++) {
        x += chromosome[i] * (1 / pow(2, i));
    }

    for (int i = half + 1; i < chromosome.size(); i++) {
        y += chromosome[i] * (1 / pow(2, i - (half)));
    }

    if (chromosome[0] == 1) x *= -1.0;
    if (chromosome[half] == 1) y *= -1.0;

    return {x * 5, y * 5};
}


double fitness_f(chromosome_t chromosome) {
    return 1.0 / (1.0 + abs(ackley_f(decode(chromosome))));
}

int main() {
    population_t population = generate_population(100 + (23317 % 10) * 2);

    for (auto &chromosome: population) {
        auto decoded = decode(chromosome);
        cout << decoded.first << ", " << decoded.second << endl;
    }

    return 0;
}