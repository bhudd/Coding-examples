
/**
 * Given an array of numbers, 
 * replace each number with the product of all the 
 * numbers in the array except the number itself *without* using division.
 */

#include <vector>
#include <stdio.h>
#include <algorithm>


std::vector<int> multiply(std::vector<int>& A)
{
    std::vector<int> B(A.size());
    int accumulator = 1;

    for(int i = 0; i < A.size()-1; ++i)
    {
        accumulator *= A[i];
        B[i+1] = accumulator;
    }

    accumulator = 1;

    for(int i = A.size()-1; i >= 1; --i)
    {
        accumulator *= A[i];
        B[i-1] *= accumulator;
    }
    return B;
}

std::vector<int> multiply_n_squared(std::vector<int>& A)
{
    std::vector<int> B(A.size());
    std::fill(B.begin(), B.end(), 1);
    for(int i = 0; i < A.size(); ++i)
    {
        for(int j = 0; j < A.size(); ++j)
        {
            if(i != j)
            {
                B[i] *= A[j];
            }
        }
    }
    return B;
}

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto b = multiply(v);
    // expected output: {120, 60, 40, 30, 24}

    for(int i : b)
    {
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}