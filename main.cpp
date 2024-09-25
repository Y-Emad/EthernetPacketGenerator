#include "EthernetPacket.h"

int main() {
    int packet_size = 44;         // in Bytes
    unsigned char* dest_mac = StrToArr("0x010101010101");
    unsigned char*  src_mac = StrToArr("0x333333333333");
    int minIFG = 12;                // Number of IFGs at packet end

    int lineRate = 10;                // in Gbps
    long long int captureSize = 1*1e9;         // Capture size in PicoSeconds (10 ms)
    int BurstSize = 3;                          // Packets per Burst
    long long int BurstPeriodicity = 100*1e6;   // Periodicity of Bursts in PicoSeconds (100 us)

    // Time Simulation
    long long int byteTime = 1e3/lineRate; // Byte Time in PicoSeconds
    long long int pktTime = (packet_size + minIFG)*byteTime; // Packet Time in PicoSeconds
    long long int time = 0; // time in PicoSeconds
    while (time < captureSize) {        
        if ((time % BurstPeriodicity) == 0) {   // Time to send Burst Reached
            // Generate Required BurstSize
            for (int i = 0; i < BurstSize; i++) {
                time += pktTime;
                cout <<  setw(10) << setfill('0') << time << " : ";
                if (time < captureSize) {
                    // Generate within Capture window
                    generatePacket(packet_size, dest_mac, src_mac);
                    genIFG(minIFG, "ethernet_packet.bin");
                } else 
                    genIFG(packet_size + minIFG, "ethernet_packet.bin");
            }
        }
        else {  // send IFGs when not sending packets
            int remainder = BurstPeriodicity - time%BurstPeriodicity;
            genIFG(remainder/byteTime, "ethernet_packet.bin");
            time += remainder;
        }
    }
    return 0;
}
