# EthernetPacketGenerator
This is the First Milestone of my 5G Diploma Final Project.

1) Use PacketConfig file to modify packet parameters
2) Generate packets using PacketGenerator.exe
3) Read the packets using PacketParser.exe

Steps 2 and 3 are automated using makefile, use powershell or terminal to execute it
    cd /* path */

In case PacketConfig was modified, recompile using:
    make

To run the programs
    make run

To delete the generated executables and .bin file
    make clean