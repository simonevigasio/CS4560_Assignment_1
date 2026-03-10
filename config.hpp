#ifndef READ_INPUT_HPP
#define READ_INPUT_HPP

#include <vector>
#include <random>

constexpr int M = 500;
constexpr int N = 500;
constexpr int P = 500; 

using Matrix3D = std::vector<std::vector<std::vector<int>>>;

inline Matrix3D create_matrix()
{
    return Matrix3D(
        M, std::vector<std::vector<int>>(
            N, std::vector<int>(P)
        )
    );
}

inline void read_input(std::vector<std::vector<std::vector<int>>>& a,
                       int M, int N, int P,
                       unsigned seed = 42)
{
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 1000);

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < P; ++k)
                a[i][j][k] = dist(gen);
}

#endif 