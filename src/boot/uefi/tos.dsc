[Defines]
PLATFORM_NAME                  = UefiAppBootloaderPkg
PLATFORM_GUID                  = e1fcd28c-f219-11e6-b657-d89d67f40bd7
PLATFORM_VERSION               = 0.01
DSC_SPECIFICATION              = 0x00010005
OUTPUT_DIRECTORY               = UefiAppBootloaderPkg/Build/
SUPPORTED_ARCHITECTURES        = X64
BUILD_TARGETS                  = DEBUG
SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
# Entry point
UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
# Basic
BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
# UEFI & PI
UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
# Generic Modules
PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
# Misc
DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

[LibraryClasses.common.UEFI_APPLICATION]
MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

[Components]
UefiAppBootloaderPkg/tos.inf

[BuildOptions]
*_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES