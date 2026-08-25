#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
  UINT32 address,rawdata,device;
  UINT16 vendorid,deviceid;
  EFI_INPUT_KEY key;
  
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut); //clear screen
  
  Print(L"EDK2 BARE METAL PCI SCANNER (ALL SLOTS)\n\n");
  for(device=0;device<32;device++){ //loop through all possible 32 hardware slots on primary bus 0
    //construct a strict 32 bit PCI address structure
    //enable config bit 31 and shift device index left by 11 bits to align to hardware pins
    address=(UINT32)((1<<31)|(device<<11)); 
    
    IoWrite32(0xCF8,address); //write target address to hardware configuration port 0XCF8
    rawdata=IoRead32(0xCFC); //read from target address to data port 0xCFC
    
    //isolating ids, lower 16 bits for vendorid and upper 16 bits for deviceid
    vendorid=(UINT16)(rawdata&0xFFFF);
    deviceid=(UINT16)(rawdata>>16);
    
    if(vendorid!=0xFFFF){ //valid chip is responding if vendorid!=0xFFFF
      Print(L"Found device in slot %02d: vendor [0x%04X] | device [0x%04X]\n\n",device,vendorid,deviceid);
    }
  }
  Print(L"Press any key to return to the shell\n");
  SystemTable->ConIn->Reset(SystemTable->ConIn,FALSE);
  while(EFI_ERROR(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&key)));
  
  return EFI_SUCCESS;
}

