
#include <iostream>
#include <cstring>
#include <optional>

struct Base
{
    uint16_t m_talkerIdentifier;
    unsigned m_sentenceIdentifier;
    unsigned m_miscData[10];
    uint8_t m_miscDataLength;
    unsigned m_dataFields[10];
    uint8_t m_dataFieldCount;
    uint8_t m_checksum;

    bool m_hasChecksum;    // Checksums are optional
    bool m_isChecksumCorrect;  // true if no checksum
};

struct Derived : public Base
{
    Derived(Base& other) : Base(other)
    {
        ParseFields();
    }

    void ParseFields()
    {
        val = 5;
    }

    int val = 0;
};

int main()
{
    Base b;
    b.m_talkerIdentifier = 1;
    b.m_sentenceIdentifier = 2;
    b.m_miscData[0] = 3;

    Derived d(b);

    std::cout << d.m_talkerIdentifier << std::endl;
    std::cout << d.m_sentenceIdentifier << std::endl;
    std::cout << d.m_miscData[0] << std::endl;
}