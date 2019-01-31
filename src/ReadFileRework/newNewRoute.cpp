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
#include <memory>

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
#define GL_USHORT_MS_BYTE(word)             ((word) >> 8)
#define GL_USHORT_LS_BYTE(word)             ((word) & 0x00FF)

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
        dataPtr->assign(dataptr, dataptr+length);
    }

    Packet(uint8_t cat, uint8_t parm, uint8_t* dataptr, int length) :
        category(cat),
        parameter(parm)
    {
        dataPtr = std::make_unique<std::vector<uint8_t>>(dataptr, dataptr+length);
    }

    Packet(uint8_t cat, uint8_t parm, int length) :
        category(cat),
        parameter(parm)
    {
        dataPtr = std::make_unique<std::vector<uint8_t>>(length);
    }

    size_t size() const noexcept
    {
        return dataPtr->size();
    }

    uint8_t& operator[] (size_t index)
    {
        // assert(index < data.size());
        return (*dataPtr)[index];
    }

    friend std::ifstream& operator>> (std::ifstream& is, Packet& packet)
    {
        uint8_t header[BK_PACKET_HEADER_LENGTH];
        is.read(reinterpret_cast<char*>(header), BK_PACKET_HEADER_LENGTH);
        if(is.good())
        {
            packet.category = header[0];
            packet.parameter = header[1];
            auto length = GL_MAKE_USHORT(header[2], header[3]);
            packet.dataPtr = std::make_unique<std::vector<uint8_t>>(length);
            is.read(reinterpret_cast<char*>(packet.dataPtr->data()), length);
        }
        else
        {
            printf("ERROR\n");    
        }
        
        return is;
    }
    
    friend std::ofstream& operator<< (std::ofstream& os, Packet& packet)
    {
        uint8_t header[BK_PACKET_HEADER_LENGTH];
        header[0] = packet.category;
        header[1] = packet.parameter;
        header[2] = GL_USHORT_MS_BYTE(packet.size());
        header[3] = GL_USHORT_LS_BYTE(packet.size());
        os.write(reinterpret_cast<char*>(header), BK_PACKET_HEADER_LENGTH);
        if(os.good())
        {
            os.write(reinterpret_cast<char*>(packet.dataPtr->data()), packet.dataPtr->size());
        }
    }

    uint8_t category;
    uint8_t parameter;

    std::unique_ptr<std::vector<uint8_t>> dataPtr = nullptr;
};

void ParseParameterPacket(Packet& packet)
{
    int cat = packet.category;
    int type = packet.parameter;
    int length = packet.size();

    // std::cout << "cat: " << cat << " type: " << type << std::endl;
    // std::cout << "data: ";
    // for(int i = 0; i < length; ++i)
    // {
    //     printf("%02x ", packet[i]);
    // }
    // printf("\n");
}

int main(){
    auto begin = std::chrono::high_resolution_clock::now();
    uint8_t header[6];
    int length = 0;
    bool okay = true;
    std::ifstream file1("/home/bhudd/workspace/hackerRank/src/Chula_Vista_Codeplug.neo", std::ifstream::binary);
    std::ofstream outFile("/home/bhudd/workspace/hackerRank/src/output.neo", std::ios::binary | std::ios::trunc);

    okay = IsHeaderAndChecksumOkay(file1);

    file1.seekg(GLPM_HEADER_LENGTH, file1.beg);
    length = GLPM_HEADER_LENGTH;

    while(file1.good())
    {
        Packet packet;
        file1 >> packet;
        if(file1.good())
        {
            ParseParameterPacket(packet);
            length += packet.size();
            outFile << packet;
        }
    }
    std::cin.get();
    outFile.close();

    std::cout << "length: " << length << std::endl;

    Packet packet(0, 0, 15);
    std::cout << "packet size: " << packet.size() << std::endl;
    packet[4] = 200;
    printf("packet[0]: %d\n", packet[0]);
    printf("packet[4]: %d\n", packet[4]);

    auto end= std::chrono::high_resolution_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;

}
