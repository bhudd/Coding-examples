#include <variant>
#include <cstdint>
#include <vector>
#include <iostream>
#include <array>

struct Message1
{
    uint8_t byte;
    uint16_t sh;
    uint32_t in;
};

struct Message2
{
    std::vector<uint8_t> data;
};

struct Message3
{
    std::array<uint8_t, 5> data;
};

struct Message4
{
    uint16_t sh;
    uint16_t sh2;
    uint16_t sh3;
};

struct OldMessage
{
    uint8_t data[1100];
};

using MessageType = std::variant<Message1, Message2, Message3, Message4>;

int main()
{
    int temp[sizeof(OldMessage)];
    std::cout << sizeof(MessageType) << std::endl;
    std::cout << sizeof(Message1) << std::endl;
    std::cout << sizeof(Message2) << std::endl;
    std::cout << sizeof(Message3) << std::endl;
    std::cout << sizeof(Message4) << std::endl;

    return 0;
}