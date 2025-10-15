#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

// Funcție pentru procesarea unui interval de obiecte
void procesare_interval(const vector<int>& cost, const vector<int>& valoare, int start, 
                                        int end, int capacitate, int& valoare_max_local) {
    valoare_max_local = 0;
    
    // Generăm toate subseturile posibile pentru obiectele din interval
    for (int subset = 0; subset < (1 << (end - start)); ++subset) {
        int cost_total = 0, valoare_total = 0;
        for (int i = 0; i < (end - start); ++i) {
            if (subset & (1 << i)) {
                cost_total += cost[start + i];
                valoare_total += valoare[start + i];
            }
        }
        if (cost_total <= capacitate) {
            valoare_max_local = max(valoare_max_local, valoare_total);
        }
    }
}

int main() {
    int capacitate = 5;
    vector<int> cost = {2, 3, 4, 5}; // Greutățile obiectelor
    vector<int> valoare = {3, 4, 5, 80}; // Valorile obiectelor
    int n = cost.size(); // Numărul de obiecte

    int num_threads = thread::hardware_concurrency();
    thread threads[num_threads];
    int valori_maxime[num_threads] = {0};

    // Divizăm obiectele în intervale și lansăm fire de execuție
    int chunk_size = (n + num_threads - 1) / num_threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = min(start + chunk_size, n);
        if (start < n) {
            threads[t] = thread(procesare_interval, cref(cost), cref(valoare), start, 
                                                end, capacitate, ref(valori_maxime[t]));
        }
    }

    // Așteptăm finalizarea firelor de execuție
    for (int t = 0; t < num_threads; ++t) {
        if (threads[t].joinable()) {
            threads[t].join();
        }
    }

    // Calculăm valoarea maximă globală
    int valoare_max = 0;
    for (int val : valori_maxime) {
        valoare_max = max(valoare_max, val);
    }

    cout << "Valoarea maximă care se poate obține: " << valoare_max << endl;
    return 0;
}
