#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <chrono>
#include "config.hpp"

int main() {

    auto a = create_matrix();
    read_input(a, M, N, P);  

    int T = 8;                         
    omp_set_dynamic(0);                
    omp_set_num_threads(T);            
    
    using clock = std::chrono::steady_clock;

    auto t0 = clock::now();

    int gmin = INT_MAX, gmax = INT_MIN;
    int gmin_i = 0, gmin_j = 0, gmin_k = 0;
    int gmax_i = 0, gmax_j = 0, gmax_k = 0;

    #pragma omp parallel
    {
        int lmin = INT_MAX, lmax = INT_MIN;
        int lmin_i = 0, lmin_j = 0, lmin_k = 0;
        int lmax_i = 0, lmax_j = 0, lmax_k = 0;

        #pragma omp for collapse(3) nowait
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < P; ++k) {
                    int v = a[i][j][k];
                    if (v < lmin) { lmin = v; lmin_i = i; lmin_j = j; lmin_k = k; }
                    if (v > lmax) { lmax = v; lmax_i = i; lmax_j = j; lmax_k = k; }
                }

        #pragma omp critical
        {
            if (lmin < gmin) { gmin = lmin; gmin_i = lmin_i; gmin_j = lmin_j; gmin_k = lmin_k; }
            if (lmax > gmax) { gmax = lmax; gmax_i = lmax_i; gmax_j = lmax_j; gmax_k = lmax_k; }
        }
    }

    auto t1 = clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::cout << "Elapsed: " << sec.count() << " s\n";
    
    std::cout << "Min: " << gmin << " at (" << gmin_i << "," << gmin_j << "," << gmin_k << ")\n";
    std::cout << "Max: " << gmax << " at (" << gmax_i << "," << gmax_j << "," << gmax_k << ")\n";
    return 0;
}