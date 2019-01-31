

#include <cstdint>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <stdexcept>

int variable;

// #define DEBUG

constexpr bool isRelease()
{
#ifdef DEBUG
    return false;
#else
    return true;
#endif
}

struct Message
{
    Message(int minSize = 1100) : data(minSize) {
    }
    std::vector<uint8_t> data;

    uint8_t& operator[](int index)
    {
        return data[index];
        // try {
        //     return data.at(index);
        // } catch(std::out_of_range& e)
        // {
        //     if(isRelease())
        //     {
        //         data.emplace_back(0);
        //         return data.at(index);
        //     }
        //     else
        //     {
        //         throw;
        //     }
        // }
    }
};

// don't let the optimizer remove the loop
void whatever(Message& msg)
{
    if(msg.data.size() > 10)
    {
        uint8_t v3 = msg[5] + msg[10];
        variable += v3;
    }
}

int main()
{
    Message src1, dest1;
    auto begin= std::chrono::high_resolution_clock::now();
    // copy 1000 messages of size 100
    for(int i = 0; i < 1000; ++i)
    {
        // assumed average message size is 100
        for(int j = 0; j < 100; ++j)
        {
            src1[j] = 5;
        }
        std::copy(dest1.data.begin(), dest1.data.end(), src1.data.begin());
        whatever(dest1);
    }

    Message src2(0), dest2(0);
    // copy 10000 messages of size 0
    for(int i = 0; i < 10000; ++i)
    {
        std::copy(dest2.data.begin(), dest2.data.end(), src2.data.begin());
        whatever(dest2);
    }

    auto end= std::chrono::high_resolution_clock::now();
    std::cout << variable << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;
}