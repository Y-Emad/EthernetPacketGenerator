#ifndef ETHERNETPACKET_H
#define ETHERNETPACKET_H

#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdint.h>

using namespace std;

// Define Ethernet frame sizes
#define ETH_HDRLEN 22       // Ethernet header length (including Preamble, SFD, and Length)
#define CRC_LEN 4           // Cyclic Redundancy Check (4 bytes)
#define IFG 0x07            // IFG byte shape

// Structure for Ethernet header
struct EthHeader {
    unsigned char preamble[7];  // Synchronization
    unsigned char sfd[1];       // Start of Frame Delimiter
    unsigned char dest_mac[6];  // Destination MAC address
    unsigned char src_mac[6];   // Source MAC address
    unsigned char length[2];    // Ethertype Packet Length
};

// Class for Ethernet Packet
class EthernetPacket {
public:
    EthernetPacket(int packet_size);

    int packet_size;
    int payload_size;
    
    void setPreamble(const unsigned char preamble[7]);
    void setSFD(const unsigned char sfd[1]);
    void setDestinationMAC(const unsigned char dest_mac[6]);
    void setSourceMAC(const unsigned char src_mac[6]);
    void setLength();
    void setPayload(const unsigned char* payload_data);
    void applyCRC();
    void dumpToFile(const char* filename);

private:
    int padding_size;
    int total_size;
    std::vector<unsigned char> packet;
    EthHeader* eth_header;
    unsigned char* payload;
    unsigned char* crc;
};

// Additional Functions
void genIFG(int minIFG, const char* filename);
unsigned char* StrToArr(const std::string& hexString);
uint32_t crc32(uint32_t crc, const void *buf, size_t len);
void generatePacket(int packet_size, unsigned char* dest_mac, unsigned char* src_mac);

#endif
