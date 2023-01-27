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
};

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

int selection_roulette(vector<double> fitnesses) {
    double reduced = reduce(fitnesses.begin(), fitnesses.end());
    uniform_real_distribution<double> dist(0, reduced);

    double random = dist(mt_generator);
    int i = 0;

    while (random > 0) {
        random = random - fitnesses[i];
        i++;
    }

    return i - 1;
}

population_t crossover_one_point(population_t allChromosomes) {
    int chromosome_size = allChromosomes[0].size();
    uniform_int_distribution<int> uni(0, chromosome_size);

    for (int i = uni(mt_generator); i < chromosome_size; i++) {
        swap(allChromosomes[0][i], allChromosomes[1][i]);
    }
    return allChromosomes;
}

chromosome_t mutation(chromosome_t chromosome, double p_mutation) {
    chromosome_t mutated_chromosome = chromosome;

    for (int i = 0; i < chromosome.size(); i++) {
        uniform_real_distribution<double> uni(0.0, 1.0);
        if (uni(mt_generator) > p_mutation) mutated_chromosome[i] = 1 - mutated_chromosome[i];
    }
    return mutated_chromosome;
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
    vector<double> fitnessed_chromosomes;
    population_t population = generate_population(100 + (23317 % 10) * 2);

    cout << "fitness: \n";
    for (auto &chromosome: population) {
        auto decoded = decode(chromosome);
        double fit_chromo = fitness_f(chromosome);
        fitnessed_chromosomes.push_back(fit_chromo);

        cout << decoded.first << ", " << decoded.second << " | " << fit_chromo << "\n";
    }


    cout << "\n---Crossover--- \n";
    population_t test = generate_population(10);
    population_t crossed_test = crossover_one_point(test);

    cout << "Before: \n";
    for (auto &gen: test[0]) {
        cout << gen;
    }
    cout << "\n";
    for (auto &gen: test[1]) {
        cout << gen;
    }
    cout << "\nAfter: \n";
    for (auto &gen: crossed_test[0]) {
        cout << gen;
    }
    cout << "\n";
    for (auto &gen: crossed_test[1]) {
        cout << gen;
    }
    cout << "\n\n";


    cout << "---Mutation---\n";
    chromosome_t mutated_test = mutation(test[0], 0.5);

    cout << "Before: \n";
    for (auto &gen: test[0]) {
        cout << gen;
    }
    cout << "\nAfter: \n";
    for (auto &gen: mutated_test) {
        cout << gen;
    }
    cout << "\n\n";


    cout << "---Roulette Selection: " << selection_roulette(fitnessed_chromosomes) << "---\n";

}
