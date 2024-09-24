# EthernetPacketGenerator
## This is the First Milestone of my 5G Diploma Final Project.

1) Use PacketConfig file to modify packet parameters
2) Generate packets using PacketGenerator.exe
3) Read the packets using PacketParser.exe

Steps 2 and 3 can be automated using makefile
### 1. Navigate to the Project Directory
Use **PowerShell** or the **terminal** to navigate to your project directory:
```bash
cd <path_to_your_project>
```

### 2. Build the Project
If you have modified any configuration files (like `PacketConfig`), recompile the project using:
```bash
make
```

### 3. Run the Programs
To execute the compiled programs, use the following command:
```bash
make run
```

### 4. Clean Up
To delete the generated executables and any `.bin` files, run:
```bash
make clean
```


### Additional Information
- Ensure you have the necessary tools installed (like `g++` and `make`).