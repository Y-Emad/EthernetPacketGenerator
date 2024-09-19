#include "EthernetPacket.h"
#include <iostream>

void generatePacket(int packet_size, unsigned char* dest_mac, unsigned char* src_mac) {
    EthernetPacket packet(packet_size);

    unsigned char preamble[7] = { 0xFB, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
    packet.setPreamble(preamble);

    unsigned char sfd[1] = { 0xD5 };
    packet.setSFD(sfd);

    packet.setDestinationMAC(dest_mac);

    packet.setSourceMAC(src_mac);

    packet.setLength();

    unsigned char payload[packet.payload_size] = {0};
    packet.setPayload(payload);

    unsigned char crc[CRC_LEN] = {0xA1, 0xA2, 0xA3, 0xA4};
    packet.applyCRC(crc);

    packet.dumpToFile("ethernet_packet.bin");
}

int main() {
    int packet_size = 1500;         // in Bytes
    unsigned char dest_mac[6] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
    unsigned char  src_mac[6] = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };
    int minIFG = 12;                // Number of IFGs at packet end

    long long int lineRate = 10;                          // in Gbps
    long long int captureSize = 10*1e9;         // Capture size in PicoSeconds (10 ms)
    int BurstSize = 3;                          // Packets per Burst
    long long int BurstPeriodicity = 100*1e6;   // Periodicity of Bursts in PicoSeconds (100 us)

    // Timer
    // for (int time = 0; time < captureSize*1e3; time++) { // Timer moves from zero to end in MicroSeconds
    //     if ((time % BurstPeriodicity) == 0) {
    //         for (int i = 0; i < 1; i++) generatePacket(packet_size, dest_mac, src_mac);
    //     }
    // }

    // Time Simulation
    long long int pktTime = (packet_size + minIFG)*1e12 / lineRate*1e9; // Packet Time in PicoSeconds
    long long int time = 0; // time in PicoSeconds
    while (time < captureSize) {
        // Burst Time Reached
        if ((time % BurstPeriodicity) == 0) {
            // Generate Required BurstSize
            for (int i = 0; i < BurstSize; i++) {
                time += pktTime;
                if (time < captureSize) // Generate within Capture window
                    generatePacket(packet_size, dest_mac, src_mac);
                // else 
                    /* code to print IFGs */
            }
        }
        else time = captureSize;
    }

    return 0;
}
