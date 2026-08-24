#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//creating function to read input
EFI_INPUT_KEY menu(IN EFI_SYSTEM_TABLE *SystemTable){
  EFI_STATUS status;
  EFI_INPUT_KEY key;
  
  SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  while(TRUE){
    status= SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&key);
    if(!EFI_ERROR(status)){
      if(key.UnicodeChar!=0){
        return key;
      }
    }
  }
}

EFI_STATUS
EFIAPI
UefiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
  EFI_INPUT_KEY key;
  EFI_STATUS status;
  BOOLEAN keeprun=TRUE;
  
  while(keeprun){
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    
    Print(L"FIRMWARE SYSTEM INFORMATION\n\n");
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_GREEN,EFI_BLACK));
    Print(L" [1] View firmware & bios specifications\n");
    Print(L" [2] View system core architecture data\n");
    Print(L" [3] View real time motherboard hardware clock\n");
    Print(L" [4] Clear screen\n");
    Print(L" [5] Exit to shell\n");
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK)); //white text,black back
    Print(L"\n=====================================================================\n");
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_MAGENTA,EFI_BLACK)); //magenta text,black back
    Print(L"CHoose between 1-5 : ");
    
    key=menu(SystemTable);
    Print(L"%c\n\n",key.UnicodeChar); //what number is given as input
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_CYAN,EFI_BLACK)); //cyan text,black back
    switch(key.UnicodeChar){
      case L'1': //view firmware and bios architecture
        Print(L"Firmware vendor : %s\n",SystemTable->FirmwareVendor); //uses pointer to get the numbers
        Print(L"Firmware revision : 0x%08X\n",SystemTable->FirmwareRevision);
        Print(L"UEFI spec version : %d.%02d\n\n",(SystemTable->Hdr.Revision>>16)& 0xFFFF,SystemTable->Hdr.Revision & 0xFFFF);
        break;
      case L'2': //view system core architecture data
        #if defined(MDE_CPU_X64)
          Print(L"System Architecture: X64 (64-bit long mode)\n"); //checks if the cpu is 64 bit
        #endif
        UINT32 RegEax,RegEbx,RegEcx,RegEdx; //creating variables as 32 bit registers 
        CHAR8 VendorString[13]; //creating to store the data taken from the registers
        
        AsmCpuid(0, &RegEax,&RegEbx,&RegEcx,&RegEdx);
        *(UINT32 *)(VendorString)=RegEbx; //dumped to bytes 0,1,2,3
        *(UINT32 *)(VendorString+4)=RegEdx; //dumped to bytes 4,5,6,7
        *(UINT32 *)(VendorString+8)=RegEcx; //dumped to bytes 8,9,10,11
        VendorString[12]=0; //0 is put to avoid garbage value and crash
        
        Print(L"CPU silicon brand : %a\n",VendorString); //converts the ASCII to unicode to display on the emulator
        Print(L"Active Config Tables : %d mappings initialized in RAM\n\n",SystemTable->NumberOfTableEntries); //points to number of entries on the ram used
        break;
      case L'3': //view the real time motherboard clock in GMT 
        EFI_TIME time;
        status=gRT->GetTime(&time,NULL);
        
        if(EFI_ERROR(status)){
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_LIGHTRED,EFI_BLACK)); //lightred text, black back
          Print(L"[ERROR] Bypassed core channels: Unable to query CMOS RTC chip. \n\n"); //error if chip not read or present
        } else{
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK)); //white text,black back
          Print(L"Current hardware date : %02d/%02d/%04d\n",time.Day,time.Month,time.Year);
          Print(L"Current hardware time : %02d:%02d:%02d (GMT)\n\n",time.Hour,time.Minute,time.Second);
          }
          break;
        case L'4': //Refreshes the menu
          Print(L"Refreshing system canvas layers...\n\n");
          break;
        case L'5': //Exits to shell
          Print(L"Exiting dashboard, goodbye\n\n");
          keeprun=FALSE;
          continue;
        default:
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_BROWN,EFI_BLACK)); //brown text,black back
          Print(L"Invalid choice. Choose between 1 and 5.\n\n");
      }
      SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_GREEN,EFI_BLACK)); //green text,black back
      Print(L"Press any key to return to menu.\n\n");
      menu(SystemTable);
    }
    return EFI_SUCCESS;
  }
