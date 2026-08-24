#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>


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
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    Print(L"\n=====================================================================\n");
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_MAGENTA,EFI_BLACK));
    Print(L"CHoose between 1-5 : ");
    
    key=menu(SystemTable);
    Print(L"%c\n\n",key.UnicodeChar);
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_CYAN,EFI_BLACK));
    switch(key.UnicodeChar){
      case L'1':
        Print(L"Firmware vendor : %s\n",SystemTable->FirmwareVendor);
        Print(L"Firmware revision : 0x%08X\n",SystemTable->FirmwareRevision);
        Print(L"UEFI spec version : %d.%02d\n\n",(SystemTable->Hdr.Revision>>16)& 0xFFFF,SystemTable->Hdr.Revision & 0xFFFF);
        break;
      case L'2':
        #if defined(MDE_CPU_X64)
          Print(L"System Architecture: X64 (64-bit long mode)\n");
        #endif
        UINT32 RegEax,RegEbx,RegEcx,RegEdx;
        CHAR8 VendorString[13];
        
        AsmCpuid(0, &RegEax,&RegEbx,&RegEcx,&RegEdx);
        *(UINT32 *)(VendorString)=RegEbx;
        *(UINT32 *)(VendorString+4)=RegEdx;
        *(UINT32 *)(VendorString+8)=RegEcx;
        VendorString[12]=0;
        
        Print(L"CPU silicon brand : %a\n",VendorString);
        Print(L"Active Config Tables : %d mappings initialized in RAM\n\n",SystemTable->NumberOfTableEntries);
        break;
      case L'3':
        EFI_TIME time;
        status=gRT->GetTime(&time,NULL);
        
        if(EFI_ERROR(status)){
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_LIGHTRED,EFI_BLACK));
          Print(L"[ERROR] Bypassed core channels: Unable to query CMOS RTC chip. \n\n");
        } else{
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
          Print(L"Current hardware date : %02d/%02d/%04d\n",time.Day,time.Month,time.Year);
          Print(L"Current hardware time : %02d:%02d:%02d (GMT)\n\n",time.Hour,time.Minute,time.Second);
          }
          break;
        case L'4':
          Print(L"Refreshing system canvas layers...\n\n");
          break;
        case L'5':
          Print(L"Exiting dashboard, goodbye\n\n");
          keeprun=FALSE;
          continue;
        default:
          SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_BROWN,EFI_BLACK));
          Print(L"Invalid choice. Choose between 1 and 5.\n\n");
      }
      SystemTable->ConOut->SetAttribute(SystemTable->ConOut,EFI_TEXT_ATTR(EFI_GREEN,EFI_BLACK));
      Print(L"Press any key to return to menu.\n\n");
      menu(SystemTable);
    }
    return EFI_SUCCESS;
  }
