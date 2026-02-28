#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include "config.hpp"

int main() {
    
    auto a = create_matrix();
    read_input(a, M, N, P); // SEQUENZIALE

    int maxval = a[0][0][0], minval = a[0][0][0];
    int max_i=0, max_j=0, max_k=0;
    int min_i=0, min_j=0, min_k=0;

    // ======= Qui scegli T = 2,4,8,16 in un loop di benchmark =======
    int T = 8;                         // esempio
    omp_set_dynamic(0);                // non far cambiare T al runtime
    omp_set_num_threads(T);            // imposta default threads per "parallel"
    omp_set_max_active_levels(2);      // abilita nested parallelism

    // Distribuzione controllata dei thread:
    int outer_threads = 2;
    int inner_threads = std::max(1, T / outer_threads); // T/2

    double t0 = omp_get_wtime();

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

    double t1 = omp_get_wtime();

    std::cout << "T=" << T << " time=" << (t1 - t0) << " s\n";
    std::cout << "Min: " << minval << " at ("<<min_i<<","<<min_j<<","<<min_k<<")\n";
    std::cout << "Max: " << maxval << " at ("<<max_i<<","<<max_j<<","<<max_k<<")\n";
}