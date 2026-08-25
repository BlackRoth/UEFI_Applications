#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
  UINT32 address,rawdata,device;
  UINT16 vendorid,deviceid;
  EFI_INPUT_KEY key;
  
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  
  Print(L"EDK2 BARE METAL PCI SCANNER (ALL SLOTS)\n\n");
  for(device=0;device<32;device++){
    address=(UINT32)((1<<31)|(device<<11));
    
    IoWrite32(0xCF8,address);
    rawdata=IoRead32(0xCFC);
    
    vendorid=(UINT16)(rawdata&0xFFFF);
    deviceid=(UINT16)(rawdata>>16);
    
    if(vendorid!=0xFFFF){
      Print(L"Found device in slot %02d: vendor [0x%04X] | device [0x%04X]\n\n",device,vendorid,deviceid);
    }
  }
  Print(L"Press any key to return to the shell\n");
  SystemTable->ConIn->Reset(SystemTable->ConIn,FALSE);
  while(EFI_ERROR(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&key)));
  
  return EFI_SUCCESS;
}

