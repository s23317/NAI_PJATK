#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
using mojamapa_t = std::map<std::string, std::string>;
using mojafunkcja_t = std::function<double(std::vector<double>)>;


//void wypisz(mojamapa_t mapa, mojafunkcja_t fun) {
//    using namespace std;
//    for (auto kv : mapa) {
//        auto [k, v] = kv;
//        cout << "klucz: " << k << "; wartosc " << fun(v) << endl;
//    }
//}



int main(int argc, char **argv) {
    using namespace std;
    vector<double> argumentyf;
//    transform(argv+1, argv+argc,
//              back_inserter(argumentyf),
//              [](auto s){return stod(s);});

    for(int i=2; i<argc; i++) {
        argumentyf.push_back(stod(argv[i]));
    }

    for (auto e : argumentyf) {
        cout<<e<<endl;
    }

//
//    map<string, string> mapa = {{"sin", "Jan"}};
    map<string, mojafunkcja_t> formatery;
//
//
//
    formatery["sin"] = [](vector<double> argumentyf) { return sin(argumentyf[0]); };
    formatery["add"] = [](vector<double> argumentyf) { return argumentyf[0] + argumentyf[1]; };
    formatery["mod"] = [](vector<double> argumentyf) { return argumentyf[0] / argumentyf[1]; };
//
//
//
//
//    try {
//        vector<string> argumenty(argv, argv + argc);
//        auto selected_f = argumenty.at(1);
//        wypisz(mapa, formatery.at(selected_f));
//    } catch (std::out_of_range aor) {
//        cout << "podaj argument. Dostepne to: ";
//        for (auto [k, v] : formatery) cout << " " << k;
//        cout << endl;
//    }
    return 0;
}
