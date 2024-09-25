#include "EthernetPacket.h"

// Constructor
EthernetPacket::EthernetPacket(int packet_size)
    : packet_size(packet_size) {  // Initialize payload_size with the value passed to the constructor

    // Allocate buffer with total size
    packet.resize(packet_size, 0);

    // Assign memory location pointers according to Header Struct
    eth_header = reinterpret_cast<EthHeader*>(packet.data()); 

    // Finding Maximum payload size
    payload_size = packet_size - ETH_HDRLEN - CRC_LEN;

    // Assign memory location pointers for payload and CRC
    payload = packet.data() + ETH_HDRLEN;   // Pointer to Payload Start Byte
    crc = payload + payload_size;           // Pointer to CRC Start Byte

    // Assign Constant Parameters
    unsigned char preamble[7] = { 0xFB, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
    this->setPreamble(preamble);

    unsigned char sfd[1] = { 0xD5 };
    this->setSFD(sfd);
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
    this->setLength();
}

void EthernetPacket::setLength() {
    eth_header->length[0] = (packet_size >> 8) & 0xFF;
    eth_header->length[1] = packet_size & 0xFF;
}

void EthernetPacket::setPayload(const unsigned char* payload_data) {
    memcpy(payload, payload_data, payload_size);
    this->applyCRC();
}

// void EthernetPacket::applyCRC() {
//     unsigned char crc_data[CRC_LEN] = {0xA1, 0xA2, 0xA3, 0xA4};
//     memcpy(crc, crc_data, CRC_LEN);
// }
void EthernetPacket::applyCRC() {
    // Calculate the CRC using the provided payload
    uint32_t CRC = 0xFFFFFFFF; // Initial CRC value
    CRC = crc32(CRC, payload, payload_size);

    // Convert the CRC value to a byte array
    unsigned char crc_bytes[4];
    crc_bytes[0] = (CRC >> 24) & 0xFF;
    crc_bytes[1] = (CRC >> 16) & 0xFF;
    crc_bytes[2] = (CRC >> 8) & 0xFF;
    crc_bytes[3] = CRC & 0xFF;

    // Copy the CRC bytes to the CRC field in the packet
    memcpy(crc, crc_bytes, CRC_LEN);
}

// Dump packet to a file
void EthernetPacket::dumpToFile(const char* filename) {
    ofstream outfile(filename, ios::app | ios::binary);
    for (int i = 0; i < packet_size; i++) {
        outfile << hex << setw(2) << setfill('0') << (int)packet[i];
        if ((i + 1) % 4 == 0) 
            outfile << "\n";    // New Line every 4 Bytes
        else
            outfile << " ";     // Space after every Byte
    }
    outfile.close();
    cout << "Packet Generated in file: " << filename << endl;
}

// IFG Generator
void genIFG(int minIFG, const char* filename) {
    int remainder = minIFG % 4;
    if (remainder) minIFG = minIFG + 4 - remainder;
    ofstream outfile(filename, ios::app | ios::binary);
    for (int i = 0; i < minIFG; i++) {
        outfile << hex << setw(2) << setfill('0') << IFG;
        if ((i + 1) % 4 == 0) 
            outfile << "\n";    // New Line every 4 Bytes
        else
            outfile << " ";     // Space after every Byte
    }
    outfile.close();
    // cout << "IFG Generated" << endl;
}

// Additional Functions
unsigned char* StrToArr(const string& hexString) {
    unsigned char* byteArray = new unsigned char[(hexString.length() - 2) / 2];

    // Iterate over the hexString in pairs of characters
    for (size_t i = 0, j = 0; i < hexString.length() - 2; i += 2, j++) {

        // Convert the hex byte to an unsigned char
        byteArray[j] = stoi(hexString.substr(i + 2, 2), nullptr, 16);
    }

    return byteArray;
}

uint32_t crc32(uint32_t crc, const void *buf, size_t len) {
    const uint8_t *p = (const uint8_t *)buf;
    while (len--) {
        crc ^= *p++;
        for (int i = 0; i < 8; i++) {
            crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320 : 0);
        }
    }
    return crc;
}

void generatePacket(int packet_size, unsigned char* dest_mac, unsigned char* src_mac) {
    EthernetPacket packet(packet_size);

    packet.setDestinationMAC(dest_mac);

    packet.setSourceMAC(src_mac);

    unsigned char payload[packet.payload_size] = {0};
    packet.setPayload(payload);

    packet.dumpToFile("ethernet_packet.bin");
}