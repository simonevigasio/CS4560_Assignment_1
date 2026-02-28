#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <chrono>
#include "config.hpp"

int main() {
    
    auto a = create_matrix();
    read_input(a, M, N, P); 

    int T = 16;                         
    omp_set_dynamic(0);                
    omp_set_num_threads(T);            
    omp_set_max_active_levels(2);      

    int outer_threads = 2;
    int inner_threads = std::max(1, T / outer_threads); // T/2

    using clock = std::chrono::steady_clock;

    auto t0 = clock::now();

    int maxval = a[0][0][0], minval = a[0][0][0];
    int max_i=0, max_j=0, max_k=0;
    int min_i=0, min_j=0, min_k=0;

    #pragma omp parallel sections num_threads(outer_threads) default(none) \
        shared(a,M,N,P,maxval,minval,max_i,max_j,max_k,min_i,min_j,min_k,inner_threads)
    {
        // ---------- MAX ----------
        #pragma omp section
        {
            #pragma omp parallel num_threads(inner_threads) default(none) \
                shared(a,M,N,P,maxval,max_i,max_j,max_k)
            {
                int local_max = a[0][0][0];
                int li=0, lj=0, lk=0;

                #pragma omp for collapse(3) nowait
                for (int i=0;i<M;i++)
                    for (int j=0;j<N;j++)
                        for (int k=0;k<P;k++) {
                            int v = a[i][j][k];
                            if (v > local_max) { local_max = v; li=i; lj=j; lk=k; }
                        }

                #pragma omp critical(max_update)
                {
                    if (local_max > maxval) { maxval = local_max; max_i=li; max_j=lj; max_k=lk; }
                }
            }
        }

        // ---------- MIN ----------
        #pragma omp section
        {
            #pragma omp parallel num_threads(inner_threads) default(none) \
                shared(a,M,N,P,minval,min_i,min_j,min_k)
            {
                int local_min = a[0][0][0];
                int li=0, lj=0, lk=0;

                #pragma omp for collapse(3) nowait
                for (int i=0;i<M;i++)
                    for (int j=0;j<N;j++)
                        for (int k=0;k<P;k++) {
                            int v = a[i][j][k];
                            if (v < local_min) { local_min = v; li=i; lj=j; lk=k; }
                        }

                #pragma omp critical(min_update)
                {
                    if (local_min < minval) { minval = local_min; min_i=li; min_j=lj; min_k=lk; }
                }
            }
        }
    }

    auto t1 = clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::cout << "Elapsed: " << sec.count() << " s\n";

    std::cout << "Min: " << minval << " at ("<<min_i<<","<<min_j<<","<<min_k<<")\n";
    std::cout << "Max: " << maxval << " at ("<<max_i<<","<<max_j<<","<<max_k<<")\n";
}