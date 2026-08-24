This is a simple system information menu driven UEFI application where a switch case is used using a while loop in a bare-metal pre-OS execution environment.
The menu includes viewing firmware and bios specifications, system core architecture data, real time motherboard hardware clock, clear screen and exit to shell.
The user is asked for an input between 1-5. The output is given if the user inputs an valid number through interactive input bounds validation and throws an error if it is an invalid input.

For Case 1:
The memory-mapped SystemTable pointer structure is used to get the information as it is stored default.

For Case 2:
It first checks if the CPU is 64-bit using compile-time preprocessor macro guards. Then 32-bit register variables are created to store the collected information in the VendorString array queried via the AsmCpuId CPU intrinsic function.
Each register take 4 bytes of data and then it is parsed natively as an 8-bit ASCII string (%a) using explicit pointer-casting arithmetic to display the information on QEMU.
Number of entries initialized from RAM is also taken from the SystemTable.

For Case 3:
Uses the UefiRuntimeServicesTableLib library to call persistent runtime service structures and get the real time motherboard clock synchronized in GMT mode by querying the CMOS battery-backed NVRAM oscillator crystal.
Used it to produce day,month,year and hour,minute,second.

For Case 4:
A simple refresh screen option that references the text output protocol to clear the active console frame buffer coordinates and initialize a blank terminal canvas on command.

For Case 5:
Stops the while loop by toggling the conditional boolean flag constraint and exits to the shell.


##NOTE##
Customized text color attributes and hexadecimal color bitmasks are used for different outputs.

