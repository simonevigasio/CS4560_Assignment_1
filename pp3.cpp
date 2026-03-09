#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <algorithm>
#include <chrono>
#include "config.hpp"

int main(int argc, char** argv) {
    int T = 2;
    if (argc > 1) T = std::stoi(argv[1]);
    
    auto a = create_matrix();
    read_input(a, M, N, P); 

    omp_set_dynamic(0);                
    omp_set_num_threads(T);            
    omp_set_max_active_levels(2);      

    int outer_threads = 2;
    int inner_threads = std::max(1, T / outer_threads); // T/2

    using clock = std::chrono::steady_clock;

    auto t0 = clock::now();

    int gmax_i=0, gmax_j=0, gmax_k=0;
    int gmin_i=0, gmin_j=0, gmin_k=0;

    #pragma omp parallel sections num_threads(outer_threads) 
    {
        #pragma omp section
        {
            #pragma omp parallel num_threads(inner_threads) 
            {
                int lmax_i=0, lmax_j=0, lmax_k=0;

                #pragma omp for collapse(3) nowait
                for (int i=0;i<M;i++)
                    for (int j=0;j<N;j++)
                        for (int k=0;k<P;k++) {
                            int v = a[i][j][k];
                            if (a[i][j][k] > a[lmax_i][lmax_j][lmax_k]) { 
                                lmax_i=i; 
                                lmax_j=j; 
                                lmax_k=k; 
                            }
                        }

                #pragma omp critical
                {
                    if (a[lmax_i][lmax_j][lmax_k] > a[gmax_i][gmax_j][gmax_k]) { 
                        gmax_i = lmax_i; 
                        gmax_j = lmax_j; 
                        gmax_k = lmax_k; 
                    }
                }
            }
        }

        #pragma omp section
        {
            #pragma omp parallel num_threads(inner_threads) 
            {
                int lmin_i=0, lmin_j=0, lmin_k=0;

                #pragma omp for collapse(3) nowait
                for (int i=0;i<M;i++)
                    for (int j=0;j<N;j++)
                        for (int k=0;k<P;k++) {
                            if (a[i][j][k] < a[lmin_i][lmin_j][lmin_k]) { 
                                lmin_i = i; 
                                lmin_j = j; 
                                lmin_k = k; 
                            }
                        }

                #pragma omp critical
                {
                    if (a[lmin_i][lmin_j][lmin_k] < a[gmin_i][gmin_j][gmin_k]) { 
                        gmin_i = lmin_i; 
                        gmin_j = lmin_j; 
                        gmin_k = lmin_k; 
                    }
                }
            }
        }
    }

    auto t1 = clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::cout << "Elapsed: " << sec.count() << " s\n";

    std::cout << "Min: " << a[gmin_i][gmin_j][gmin_k] << "\n";
    std::cout << "Max: " << a[gmax_i][gmax_j][gmax_k] << "\n";
    return 0;
}