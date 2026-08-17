#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE     ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_INPUT_KEY Key;
  
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  
  Print(L"Hello from edk2 from zorin, tis rohith\n");
  Print(L"Press any key to exit this app..\n");
  
  SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  
  while (TRUE){
    Status= SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    if (!EFI_ERROR(Status)){
      break;
    }
  }
  Print(L"Key detected! Returning to UEFI SHELL.\n");
  return EFI_SUCCESS;
}
