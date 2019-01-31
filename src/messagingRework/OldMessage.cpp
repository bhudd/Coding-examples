
#define messageSize 1100


#include <cstdint>
#include <cstring>
#include <iostream>
#include <chrono>

int variable;

// don't let the optimizer remove the loop
void whatever(uint8_t v1, uint8_t v2)
{
    uint8_t v3 = v1 + v2;
    variable += v3;
}

void sendMessage(uint8_t* msg)
{
    uint8_t arr[messageSize];
    memcpy(arr, msg, messageSize);
    whatever(arr[5], arr[10]);
}

int main()
{
    uint8_t tempArr[messageSize];
    auto begin = std::chrono::high_resolution_clock::now();

    // copy 1000 messages of size 100
    for(int i = 0; i < 1000; ++i)
    {
        // assumed average message size is 100
        for(int j = 0; j < 100; ++j)
        {
            tempArr[j] = 5;
        }
        sendMessage(tempArr);
    }

    // copy 10000 messages of size 0
    for(int i = 0; i < 10000; ++i)
    {
        sendMessage(tempArr);
    }
    
    std::cout << variable << std::endl;
    auto end= std::chrono::high_resolution_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;
}