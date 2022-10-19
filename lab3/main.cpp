#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <optional>

/**
 * @brief random number generator
 * 
 * @param std::random_device 
 * @return std::mt19937 
 */
std::mt19937 mt_generator((std::random_device()) ());
/**
 * @brief goal function domain.
 */
using domain_t = std::vector<double>;

/**
 * @brief calculate minimum point using hill climbing algorithm
 * 
 * @param f goal function
 * @param start_point the start point for calculations
 * @param get_close_points function generating neighbours
 * @param max_iterations number of iterations
 * @return domain_t the domain ponint where the function f has minimum
 */
domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t start_point,
                       std::function<std::vector<domain_t>(domain_t)> get_close_points, int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points(best_p);
        auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(),
                                                [f](auto a, auto b) { return f(a) > f(b); });
        if (f(best_neighbour) < f(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

/**
 * @brief full review method that will check every domain point
 *
 * @param f goal function
 * @param domain_generator the function that will generate consecutive points from the domain and it will return empty when there are no more points to check
 * @return domain_t the point where f has its minimum
 */
domain_t brute_force_method(const std::function<double(domain_t)> &f,
                            const std::function<std::optional<domain_t>()> &domain_generator) {

    auto best_p = domain_generator();
    for (auto current_p = best_p; current_p.has_value(); current_p = domain_generator()) {
        if (f(current_p.value()) < f(best_p.value())) {
            best_p = current_p;
        }
    }
    return best_p.value_or(domain_t());
}

int main() {
    const double precision = 1.0 / 128;
    auto sphere_f_v = [](domain_t x) { return x[0] * x[0] + x[1] * x[1]; };
    auto sphere_f_generator = [precision]() -> std::optional<domain_t> {
        static domain_t p = {-10, -10};
        int i = 0;
        for (i; i < p.size(); i++) {
            p[i] = p[i] + precision;
            if (p[i] < 10) return std::optional(p);
            p[i] = -10;
        }
        return {};
    };
    auto get_random_point = []() -> domain_t {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {distr(mt_generator), distr(mt_generator)};
    };
    auto get_close_points_random = [](domain_t p0) -> std::vector<domain_t> {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {{distr(mt_generator), distr(mt_generator)}};
    };

    auto best1 = hill_climbing(sphere_f_v, get_random_point(), get_close_points_random, 1000000);
    std::cout << "hill_climbing x = " << best1[0] << " " << best1[1] << std::endl;
    auto best2 = brute_force_method(sphere_f_v, sphere_f_generator);
    std::cout << "hill_climbing x = " << best2[0] << " " << best2[1] << std::endl;
    return 0;
}
