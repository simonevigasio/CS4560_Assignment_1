#include <iostream>
#include <vector>
#include <chrono>
#include "config.hpp"

int main() {

    auto a = create_matrix();
    read_input(a, M, N, P);   

    using clock = std::chrono::steady_clock;

    auto t0 = clock::now();

    int min_i=0, min_j=0, min_k=0;
    int max_i=0, max_j=0, max_k=0;

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<P;k++){

                if(a[i][j][k] > a[max_i][max_j][max_k]){
                    max_i = i;  
                    max_j = j;
                    max_k = k;	  
                }

                if(a[i][j][k] < a[min_i][min_j][min_k]){
                    min_i = i;  
                    min_j = j;
                    min_k = k;	  
                }
            }
        }
    }

    auto t1 = clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::cout << "Elapsed: " << sec.count() << " s\n";
    
    std::cout << "Min: " << a[min_i][min_j][min_k] << "\n";
    std::cout << "Max: " << a[max_i][max_j][max_k] << "\n";
    return 0;
}