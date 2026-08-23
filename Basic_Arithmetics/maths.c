#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

//creating function to take input
UINT64 input(IN EFI_SYSTEM_TABLE *SystemTable){
  EFI_STATUS Status;
  EFI_INPUT_KEY key;
  UINT64 value=0;
  
  SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  while (TRUE){
    Status= SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key); //reading keyboard input via ASCII
    if(!EFI_ERROR(Status)){
      if(key.UnicodeChar == CHAR_CARRIAGE_RETURN||key.UnicodeChar == 0x0A){
        Print(L"\n"); 
        break; //locks the values in memory once enter key is pressed and moves on to next input
      }
      if(key.UnicodeChar>=L'0'&& key.UnicodeChar<=L'9'){
        Print(L"%c",key.UnicodeChar);
        value=(value*10)+(key.UnicodeChar - L'0'); //converts from ASCII to display the integers in the QEMU on EDK2
      }
    }
  }
  return value;
}


EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  UINT64 A,B;
  UINT64 add,sub,mul,div;
  
  gST->ConOut->ClearScreen(gST->ConOut); //clear screen
  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_RED, EFI_BLACK)); //red text, black back
  Print(L"EDK2 BASIC ARITHMETICS\n");
  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_LIGHTGRAY)); //white text, gray back
  Print(L"Enter input num 1 and press enter: ");
  A=input(SystemTable); //calling from func
  Print(L"Enter input num 2 and press enter: ");
  B=input(SystemTable); //calling from func
  
  if(B==0){
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_LIGHTRED,EFI_BLACK)); //lightred text, black back
    Print(L"\n FATAL ERROR, Division by 0 is undefined! Halting operations. \n");
    return EFI_INVALID_PARAMETER;
  }
  add=A+B;
  sub=A-B;
  mul=A*B;
  div=A/B;
  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_GREEN, EFI_BLACK)); //green text, black back
  Print(L"->Add= %d\n",add); //printing output
  Print(L"->Sub= %d\n",sub);
  Print(L"->Mul= %d\n",mul);
  Print(L"->Div= %d\n",div);
  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK));//white text, black back
  Print(L"Execution Complete, Press any key to exit back to the shell.\n");
  
  EFI_INPUT_KEY key;
  gST->ConIn->Reset(gST->ConIn, FALSE);
  while(EFI_ERROR(gST->ConIn->ReadKeyStroke(gST->ConIn,&key))); //reading key input
  
  return EFI_SUCCESS;
}
  
  
  
  
