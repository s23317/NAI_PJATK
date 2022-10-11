#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using mojamapa_t = std::map<std::string, double>;
using mojafunkcja_t = std::function<double(mojamapa_t)>;


void wypisz(mojamapa_t mapa, mojafunkcja_t fun) {
    using namespace std;
    cout << fun(mapa) << endl;
}

int main(int argc, char **argv) {
    using namespace std;

    vector<string> argfirst(argv, argv + argc);
    vector<double> argumenty;
    map<string, double> mapa;
    map<string, mojafunkcja_t> formatery;

    for (int i = 2; i < argc; i++) {
        argumenty.push_back(stod(argv[i]));
    }

    for (int i = 0; i < argumenty.size(); i++) {
        mapa.insert({{"arg" + to_string(i + 1), argumenty.at(i)}});
    }

    formatery["sin"] = [](map<string, double> map) { return sin(map.at("arg1") * (3.14159 / 180)); };
    formatery["add"] = [](map<string, double> map) { return map.at("arg1") + map.at("arg2"); };
    formatery["mod"] = [](map<string, double> map) { return fmod(map.at("arg1"), map.at("arg2")); };

    try {
        auto selected_f = argfirst[1];
        cout << selected_f << endl;
        wypisz(mapa, formatery.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "podaj argument. Dostepne to: ";
        for (auto [k, v]: formatery) cout << " " << k;
        cout << endl;
    }
    return 0;
}
