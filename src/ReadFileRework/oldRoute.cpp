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

#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

#define GLPM_PREAMBLE1                                  0x4B
#define GLPM_PREAMBLE2                                  0x4E
#define GLPM_PREAMBLE3                                  0x47

#define GLPM_BYTE_POSITION_PREAMBLE1                    0
#define GLPM_BYTE_POSITION_PREAMBLE2                    1
#define GLPM_BYTE_POSITION_PREAMBLE3                    2
#define GLPM_BYTE_POSITION_MAJOR_VERSION                3
#define GLPM_BYTE_POSITION_MINOR_VERSION                4
#define GLPM_BYTE_POSITION_CHECKSUM                     5

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

int CalculateChecksum(int bufferLength)
{
    uint8_t checksum = 0;
    uint8_t fileArr[bufferLength];
    uint8_t temp = 0;
    int fd = open("/home/bhudd/workspace/hackerRank/src/Chula_Vista_Codeplug.neo", O_RDONLY);

    while(read(fd, &temp, 1))
    {
        checksum += temp;
    }

    printf("checksum: %d\n", checksum);

    close(fd);
    return checksum;
}

int GetStreamSize()
{
    uint8_t dummy = 0;
    int length = 0;
    int fd = open("/home/bhudd/workspace/hackerRank/src/Chula_Vista_Codeplug.neo", O_RDONLY);

    while(read(fd, &dummy, 1))
    {
        length++;
    }
    close(fd);
    return length;
}

bool VerifyFileChecksum(int fd)
{
    int length = GetStreamSize();
    uint8_t checksum = CalculateChecksum(length);
    return (checksum == 0);
}

bool IsHeaderAndChecksumOkay(int fd)
{
    uint8_t header[GLPM_HEADER_LENGTH];
    if(fd)
    {
        int len = read(fd, header, GLPM_HEADER_LENGTH);
        if(len)
        {
            // read successfully
            if(VerifyHeader(header))
            {
                if(VerifyFileChecksum(fd))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

uint8_t m_buffer[1000];
int m_head = 0;
int m_length = 0;
bool m_foundEOFInThisBlock = false;
int m_fileDescriptorParms;

bool GetNextByte(uint8_t* byte)
{
    int value;
    bool foundEOF = false;

    if (m_head >= m_length)
    {
        if (!m_foundEOFInThisBlock)
        {
            value = read (m_fileDescriptorParms, m_buffer, 1000);
            if (value == -1)
            {
                std::cout << "ERROR" << std::endl;
            }
            else
            {
                m_length = (uint16_t) value;
                m_head = 0;
                if (m_length < 1000)
                {
                    m_foundEOFInThisBlock = true;
                    if (m_length == 0)
                    {
                        foundEOF = true;
                    }
                }
            }
        }
        else
        {
            foundEOF = true;
        }
    }

    if (!foundEOF)
    {
        *byte = m_buffer[m_head];
        m_head++;
    }

    return foundEOF;
}

bool GetNextPacket(uint8_t* packet, unsigned* packetLength, unsigned maximumPacketLength)
{
    bool foundPossiblePacket = false;
    bool foundEOF = false;
    bool packetLengthError = false;
    uint16_t index = 0;
    uint16_t length;
    uint8_t lengthMsb;
    uint8_t lengthLsb;
    uint16_t i;

    // Category
    foundEOF = GetNextByte(&(packet[index]));
    index++;

    if (!foundEOF)
    {
        // Parameter
        foundEOF = GetNextByte(&(packet[index]));
        index++;
    }

    if (!foundEOF)
    {
        // Length MSB
        foundEOF = GetNextByte(&(packet[index]));
        lengthMsb = packet[index];
        index++;
    }

    if (!foundEOF)
    {
        // Length LSB
        foundEOF = GetNextByte(&(packet[index]));
        lengthLsb = packet[index];
        index++;
    }

    if (!foundEOF)
    {
        length = GL_MAKE_USHORT(lengthMsb, lengthLsb);

        if(length > maximumPacketLength)
        {
            packetLengthError = true;
        }
        else
        {

            i = 0;

            while ((i < length) && (!foundEOF))
            {
                foundEOF = GetNextByte(&(packet[index]));
                index++;
                i++;
            }
        }

        if (!foundEOF)
        {
            foundPossiblePacket = true;
            *packetLength = index;
        }

        if(packetLengthError)
        {
            foundPossiblePacket = false;
        }
    }

    return foundPossiblePacket;
}

#define GLPM_MAX_PARAMETER_PACKET_LENGTH            1586

void ParseParameterPacket(uint8_t* packet)
{
    int cat = packet[0];
    int type = packet[1];
    int length = GL_MAKE_USHORT(packet[2], packet[3]);

    std::cout << "cat: " << cat << " type: " << type << std::endl;
    std::cout << "data: ";
    for(int i = 2; i < length; ++i)
    {
        printf("%02x ", packet[i]);
    }
    printf("\n");
}

int main(){
    auto begin = std::chrono::high_resolution_clock::now();
    uint8_t header[6];
    int length = 0;
    bool okay = true;
    m_fileDescriptorParms = open("/home/bhudd/workspace/hackerRank/src/Chula_Vista_Codeplug.neo", O_RDWR);
    unsigned packetLength = 0;
    uint8_t packet[GLPM_MAX_PARAMETER_PACKET_LENGTH];


    okay = IsHeaderAndChecksumOkay(m_fileDescriptorParms);

    if(okay)
    {
        length = GetStreamSize();
    }

    while(GetNextPacket(packet, &packetLength, sizeof(packet)))
    {
        ParseParameterPacket(packet);
    }

    std::cout << "length: " << length << std::endl;

    auto end= std::chrono::high_resolution_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;

}
