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

    int min_val = INT_MAX, max_val = INT_MIN;
    int min_i=0, min_j=0, min_k=0;
    int max_i=0, max_j=0, max_k=0;

    #pragma omp parallel default(none) shared(a, min_val, max_val, min_i, min_j, min_k, max_i, max_j, max_k)
    {
        #pragma omp sections 
        {
            #pragma omp section
            {
                int lmin = INT_MAX;
                int li=0, lj=0, lk=0;

                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j)
                        for (int k = 0; k < P; ++k) {
                            int v = a[i][j][k];
                            if (v < lmin) { lmin = v; li = i; lj = j; lk = k; }
                        }

                min_val = lmin;
                min_i = li; min_j = lj; min_k = lk;
            }

            #pragma omp section
            {
                int lmax = INT_MIN;
                int li=0, lj=0, lk=0;

                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j)
                        for (int k = 0; k < P; ++k) {
                            int v = a[i][j][k];
                            if (v > lmax) { lmax = v; li = i; lj = j; lk = k; }
                        }

                max_val = lmax;
                max_i = li; max_j = lj; max_k = lk;
            }
        }
    } 
    
    auto t1 = clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::cout << "Elapsed: " << sec.count() << " s\n";
    
    std::cout << "Min: " << min_val << " at (" << min_i << "," << min_j << "," << min_k << ")\n";
    std::cout << "Max: " << max_val << " at (" << max_i << "," << max_j << "," << max_k << ")\n";
    return 0;
}