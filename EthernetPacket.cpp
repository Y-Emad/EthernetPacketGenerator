#include "EthernetPacket.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

// Constructor
EthernetPacket::EthernetPacket(int packet_size)
    : packet_size(packet_size) {  // Initialize payload_size with the value passed to the constructor 

    // Total size calculation: header + payload + CRC + IFG + padding
    payload_size = packet_size - ETH_HDRLEN - CRC_LEN;
    if ((packet_size + IFG_LEN) % 4)
        padding_size = 4 - (packet_size + IFG_LEN) % 4;
    else
        padding_size = 0;
    total_size = packet_size + IFG_LEN + padding_size;

    // Allocate buffer with total size and assign IFG byte
    packet.resize(total_size, 0x07);

    // Assign memory location pointers according to Header Struct
    eth_header = reinterpret_cast<EthHeader*>(packet.data());

    // Assign memory location pointers for payload and CRC
    payload = packet.data() + ETH_HDRLEN;   // Pointer to Payload Start Byte
    crc = payload + payload_size;           // Pointer to CRC Start Byte
}

// Setters for Ethernet Fields
void EthernetPacket::setPreamble(const unsigned char preamble[7]) {
    memcpy(eth_header->preamble, preamble, 7);
}

void EthernetPacket::setSFD(const unsigned char sfd[1]) {
    memcpy(eth_header->sfd, sfd, 1);
}

void EthernetPacket::setDestinationMAC(const unsigned char dest_mac[6]) {
    memcpy(eth_header->dest_mac, dest_mac, 6);
}

void EthernetPacket::setSourceMAC(const unsigned char src_mac[6]) {
    memcpy(eth_header->src_mac, src_mac, 6);
}

void EthernetPacket::setLength() {
    eth_header->length[0] = (packet_size >> 8) & 0xFF;
    eth_header->length[1] = packet_size & 0xFF;
}

void EthernetPacket::setPayload(const unsigned char* payload_data) {
    memcpy(payload, payload_data, payload_size);
}

void EthernetPacket::applyCRC(const unsigned char crc_data[CRC_LEN]) {
    memcpy(crc, crc_data, CRC_LEN);
}

// Dump packet to a file
void EthernetPacket::dumpToFile(const char* filename) {
    ofstream outfile(filename, ios::app | ios::binary);
    for (int i = 0; i < total_size; i++) {
        outfile << hex << setw(2) << setfill('0') << (int)packet[i];
        if ((i + 1) % 4 == 0) 
            outfile << "\n";    // New Line every 4 Bytes
        else
            outfile << " ";     // Space after every Byte
    }
    outfile.close();
    cout << "Packet dumped to file: " << filename << endl;
}
