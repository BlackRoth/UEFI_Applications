This is a simple PCI scanner UEFI Application where all the hardware slots are shown from the primary bus 0 within QEMU.<br>

The code iterates 32-bit hardware addresses by applying bitwise shifting operations to align with the physical bus pins.
Target Addresses are written directly to the motherboard register port 0xCF8 and the resulting data is streamed back to the port 0xCFC to extract the raw 4 byte device signature blocks.<br>

The data is isolated by using low level bitwise masking configurations.
It isolates the lower 16-bits to extract vendorid and upper 16-bits to isolate the deviceid.<br>

Active slots are filtered out by filtering the non responsive 0xFFFF blank addresses to map the real world silicon layouts such as:

Intel 440FX HOST Bridge 0x1237 at Vendor 0x8086<br>

Intel 82371SB PCI-to-ISA Bridge 0x7000 at Vendor 0x8086<br>

Qemu standard VGA adpater 0x1111 at Vendor 0x1234<br>


