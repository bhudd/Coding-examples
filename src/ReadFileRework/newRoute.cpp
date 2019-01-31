//============================================================================
// Name        : hackerRank.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <vector>
#include <algorithm>

#define GLPM_PREAMBLE1                                  0x4B
#define GLPM_PREAMBLE2                                  0x4E
#define GLPM_PREAMBLE3                                  0x47

#define GLPM_BYTE_POSITION_PREAMBLE1                    0
#define GLPM_BYTE_POSITION_PREAMBLE2                    1
#define GLPM_BYTE_POSITION_PREAMBLE3                    2
#define GLPM_BYTE_POSITION_MAJOR_VERSION                3
#define GLPM_BYTE_POSITION_MINOR_VERSION                4
#define GLPM_BYTE_POSITION_CHECKSUM                     5

#define BK_PACKET_HEADER_LENGTH 4

#define GLPM_HEADER_LENGTH 6

#define GL_MAKE_USHORT(msByte, lsByte)      ((((msByte) << 8) | (lsByte)) & 0xFFFF)

bool VerifyHeader(uint8_t* packet)
{
    if(packet[GLPM_BYTE_POSITION_PREAMBLE1] == GLPM_PREAMBLE1 &&
       packet[GLPM_BYTE_POSITION_PREAMBLE2] == GLPM_PREAMBLE2 &&
       packet[GLPM_BYTE_POSITION_PREAMBLE3] == GLPM_PREAMBLE3)
    {
        //m_majorVersion = packet[GLPM_BYTE_POSITION_MAJOR_VERSION];
        //m_minorVersion = packet[GLPM_BYTE_POSITION_MINOR_VERSION];
        return true;
    }

    return false;
}

bool VerifyFileChecksum(std::ifstream& buf)
{
    uint8_t checksum = 0;
    
    // make sure we're at the start of the buffer
    buf.seekg(0, buf.beg);
    
    if(buf)
    {
        std::vector<uint8_t> fileData;

        buf.seekg(0, buf.end);
        int fileSize = buf.tellg();
        // seek back to beginning
        buf.seekg(0, buf.beg);
        fileData.reserve(fileSize);
        buf.read(reinterpret_cast<char*>(fileData.data()), fileSize);
        checksum = 0;
        if(buf)
        {
            for(auto a : fileData)
            {
                checksum += a;
            }
        }
    }

    // seek back to beginning
    buf.seekg(0, buf.beg);
    return (checksum == 0);
}

bool IsHeaderAndChecksumOkay(std::ifstream& buf)
{
    uint8_t header[GLPM_HEADER_LENGTH];
    if(buf)
    {
        buf.read(reinterpret_cast<char*>(header), GLPM_HEADER_LENGTH);
        if(buf)
        {
            // read successfully
            if(VerifyHeader(header))
            {
                if(VerifyFileChecksum(buf))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

struct Packet
{
    Packet() {}

    Packet(uint8_t* dataptr, int length)
    {
        data.assign(dataptr, dataptr+length);
    }

    size_t size() const noexcept
    {
        return data.size();
    }

    uint8_t& operator[] (size_t index)
    {
        // assert(index < data.size());
        return data[index];
    }

    void readData(std::ifstream& is, unsigned length)
    {
        // from cppreference: "std::istreambuf_iterator is more efficient, since it avoids 
        // the overhead of constructing and destructing the sentry object once per character."
        // This comes at the cost of us having to manage the state of the ifstream.

        if(is.good())
        {
            for(int i = 0; i < length; ++i)
            {
                uint8_t byte;
                is >> byte;
                if(!is.good()) break;
                data.push_back(byte);
            }
        }
    }

    friend std::ifstream& operator>> (std::ifstream& is, Packet& packet)
    {
        uint16_t length = 0;
        packet.readData(is, BK_PACKET_HEADER_LENGTH);
        if(is.good())
        {
            length = GL_MAKE_USHORT(packet[2], packet[3]);
            packet.readData(is, length);
        }
        return is;
    }

    std::vector<uint8_t> data;
};

void ParseParameterPacket(Packet& packet)
{
    int cat = packet[0];
    int type = packet[1];
    int length = GL_MAKE_USHORT(packet[2],packet[3]);
}

int main(){
    auto begin = std::chrono::high_resolution_clock::now();
    uint8_t header[6];
    int length = 0;
    bool okay = true;
    std::ifstream file1("/home/bhudd/workspace/hackerRank/src/Chula_Vista_Codeplug.neo", std::ifstream::binary);

    okay = IsHeaderAndChecksumOkay(file1);

    file1.seekg(GLPM_HEADER_LENGTH, file1.beg);
    length = GLPM_HEADER_LENGTH;

    while(file1.good())
    {
        Packet packet;
        file1 >> packet;
        ParseParameterPacket(packet);
        length += packet.size();
    }

    std::cout << "length: " << length << std::endl;

    auto end= std::chrono::high_resolution_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;

}
