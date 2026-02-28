#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include "config.hpp"

int main() {
    
    auto a = create_matrix();
    read_input(a, M, N, P); // sequential

    // results
    int min_val = INT_MAX, max_val = INT_MIN;
    int min_i=0, min_j=0, min_k=0;
    int max_i=0, max_j=0, max_k=0;

    // Parallel region: share the matrix and result variables
    #pragma omp parallel default(none) shared(a, min_val, max_val, min_i, min_j, min_k, max_i, max_j, max_k)
    {
        // Split work into two independent tasks
        #pragma omp sections 
        {
            // ---- Section 1: compute MIN ----
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

                // Write results (only this section writes min_* so no race)
                min_val = lmin;
                min_i = li; min_j = lj; min_k = lk;
            }

            // ---- Section 2: compute MAX ----
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

                // Write results (only this section writes max_* so no race)
                max_val = lmax;
                max_i = li; max_j = lj; max_k = lk;
            }
        }
    } // implicit barrier at end of parallel region ensures both sections finished

    std::cout << "Min: " << min_val << " at (" << min_i << "," << min_j << "," << min_k << ")\n";
    std::cout << "Max: " << max_val << " at (" << max_i << "," << max_j << "," << max_k << ")\n";
    return 0;
}