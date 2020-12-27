#ifndef GNUEFI_BUILD

// Code for compilation with EDK2 Toolkit
#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>

EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable){
  Print(L"Hello World \n");
  while(1){}
  return EFI_SUCCESS;
}

#else

// Code for compilation with GNU-EFI Toolkit
#include <efi.h>
#include <efilib.h>
EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world!\n");

  while(1){}
  return EFI_SUCCESS;
}

#endif