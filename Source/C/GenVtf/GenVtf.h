/*++

Copyright (c) 1999 - 2007 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:  

  GenVtf.h

Abstract:

  This file contains the relevant declarations required
  to generate Boot Strap File

--*/

//
// Module Coded to EFI 2.0 Coding Conventions
//
#ifndef   _EFI_GEN_VTF_H
#define   _EFI_GEN_VTF_H

//
// External Files Referenced
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
// #include "TianoCommon.h"
#include "Common/PiFirmwareFileSystem.h"
#include "Common/PiFirmwareVolumeHeader.h"
#include "ParseInf.h"

//
// Internal Constants
//
#define EFI_IPF_VTF1_GUID \
  { \
    0xfa371c9b, 0x5a86, 0x4198, 0xab, 0xc2, 0xed, 0x3f, 0xaa, 0xce, 0xb0, 0x8b \
  };

#define EFI_IPF_VTF2_GUID \
  { \
    0x624a0d5a, 0x315f, 0x40b6, 0xa6, 0x33, 0xe5, 0xf7, 0xde, 0x58, 0x20, 0xa0 \
  };

#define EFI_IA32_BOOT_STRAP_GUID \
  { \
    0xd4260a8d, 0x356, 0x4f45, 0x85, 0xe9, 0xad, 0x1d, 0x79, 0x22, 0x79, 0xf0 \
  };

#define CV_N_TYPE(a,b)            (UINT8)(((UINT8)a << 7) + (UINT8)b)  // Keeps the CV and Type in same byte field
#define MAKE_VERSION(a,b)         (UINT16)(((UINT16)a << 8) + (UINT16)b)

#define   FILE_NAME_SIZE          256
#define   COMPONENT_NAME_SIZE     128
#define   VTF_INPUT_FILE          "VTF.INF"
#define   VTF_OUTPUT_FILE         "Vtf.RAW"
#define   VTF_SYM_FILE            "Vtf.SYM"
#define   FIT_SIGNATURE           "_FIT_   "
#define   VTF_OUTPUT               1

//
// This is IA32 seccore
//
#define   COMP_TYPE_SECCORE             0x0F

//
//Fit Type Definition
//
#define   COMP_TYPE_FIT_HEADER          0x00
#define   COMP_TYPE_FIT_PAL_B           0x01

//
// This is generic PAL_A
//
#define   COMP_TYPE_FIT_PAL_A           0x0F
#define   COMP_TYPE_FIT_PEICORE         0x10
#define   COMP_TYPE_FIT_AUTOSCAN        0x30
#define   COMP_TYPE_FIT_FV_BOOT         0x7E

//
//This is processor Specific PAL_A
//
#define   COMP_TYPE_FIT_PAL_A_SPECIFIC  0x0E
#define   COMP_TYPE_FIT_UNUSED    0x7F

#define   FIT_TYPE_MASK           0x7F
#define   CHECKSUM_BIT_MASK       0x80

//
// IPF processor address is cached bit
//
#define IPF_CACHE_BIT 0x8000000000000000ULL

//
// Size definition to calculate the location from top of address for
// each component
//
#define SIZE_IA32_RESET_VECT      0x10        // 16 Bytes
#define SIZE_SALE_ENTRY_POINT     0x08        // 8 Byte
#define SIZE_FIT_TABLE_ADD        0x08        // 8 Byte
#define SIZE_FIT_TABLE_PAL_A      0x10     
#define SIZE_RESERVED             0x10


#define SIZE_TO_OFFSET_PAL_A_END  (SIZE_IA32_RESET_VECT + SIZE_SALE_ENTRY_POINT + \
                                  SIZE_FIT_TABLE_ADD + SIZE_FIT_TABLE_PAL_A + \
                                  SIZE_RESERVED)
#define SIZE_TO_PAL_A_FIT         (SIZE_IA32_RESET_VECT + SIZE_SALE_ENTRY_POINT + \
                                  SIZE_FIT_TABLE_ADD + SIZE_FIT_TABLE_PAL_A)

#define SIZE_OF_PAL_HEADER        0x40        //PAL has 64 byte header

//
// Utility Name
//
#define UTILITY_NAME  "GenVtf"

//
// Utility version information
//
#define UTILITY_MAJOR_VERSION   0
#define UTILITY_MINOR_VERSION   1
#define UTILITY_DATE            __DATE__

//
// The maximum number of arguments accepted from the command line.
//
#define ONE_VTF_ARGS  7
#define TWO_VTF_ARGS  9
#define THREE_VTF_ARGS 11
#define FOUR_VTF_ARGS  13

//
// The number of IA32 vtf arguments accepted from the command line.
//
#define IA32_ARGS  4

#define IA32_SOFT_FIT "IA32VtfAddress.inf"

//
// Internal Data Structure
//
typedef enum _LOC_TYPE 
{
  NONE,                   // In case there is - INF file
  FIRST_VTF,              // First VTF
  SECOND_VTF,             // Outside VTF
} LOC_TYPE;

typedef struct _PARSED_VTF_INFO {
  CHAR8       CompName[COMPONENT_NAME_SIZE];
  LOC_TYPE    LocationType;
  UINT8       CompType;
  UINT8       MajorVer;
  UINT8       MinorVer;
  UINT8       CheckSumRequired;
  BOOLEAN     VersionPresent;                // If it is TRUE, then, Version is in INF file
  BOOLEAN     PreferredSize;
  BOOLEAN     PreferredAddress;
  CHAR8       CompBinName[FILE_NAME_SIZE];
  CHAR8       CompSymName[FILE_NAME_SIZE];
  UINTN       CompSize;
  UINT64      CompPreferredAddress;
  UINT32      Align;

  //
  //  Fixed - warning C4133: '=' : incompatible types - from 'struct _ParsedVtfInfo *' to 'struct _PARSED_VTF_INFO *'
  //  Fixed - warning C4133: '=' : incompatible types - from 'struct _ParsedVtfInfo *' to 'struct _PARSED_VTF_INFO *'
  //  Fixed - warning C4133: '=' : incompatible types - from 'struct _ParsedVtfInfo *' to 'struct _PARSED_VTF_INFO *'
  //  Fixed - warning C4133: '=' : incompatible types - from 'struct _ParsedVtfInfo *' to 'struct _PARSED_VTF_INFO *'
  //
  struct      _PARSED_VTF_INFO   *NextVtfInfo;
} PARSED_VTF_INFO;

#pragma pack (1)
typedef struct {
  UINT64      CompAddress;
  UINT32      CompSize;
  UINT16      CompVersion;
  UINT8       CvAndType;
  UINT8       CheckSum;
} FIT_TABLE;
#pragma pack ()

//
// The function that displays general utility information
//
VOID
PrintUtilityInfo (
  VOID
  )
/*++

Routine Description:

  Displays the standard utility information to SDTOUT

Arguments:

  None

Returns:

  None

--*/
;

//
// The function that displays the utility usage message.
//
VOID
PrintUsage (
  VOID
  )
/*++

Routine Description:

  Displays the utility usage syntax to STDOUT

Arguments:

  None

Returns:

  None

--*/
;

//
// Other Function Prototype Declarations
//

EFI_STATUS
UpdateVtfBuffer(
  IN  UINT64    StartAddress,
  IN  UINT8     *Buffer,
  IN  UINT64    DataSize,
  IN  LOC_TYPE  LocType
  )
/*++

Routine Description:

  Update the Firmware Volume Buffer with requested buffer data
  
Arguments:

  StartAddress   - StartAddress in buffer. This number will automatically
                  point to right address in buffer where data needed 
                  to be updated.
  Buffer         - Buffer pointer from data will be copied to memory mapped buffer.
  DataSize       - Size of the data needed to be copied.
  LocType        - The type of the VTF

Returns:
  
  EFI_ABORTED  - The input parameter is error
  EFI_SUCCESS  - The function completed successfully

--*/
;

EFI_STATUS
UpdateSymFile (
  IN UINT64 BaseAddress,
  IN CHAR8  *DestFileName,
  IN CHAR8  *SourceFileName
  )
/*++

Routine Description:

  This function adds the SYM tokens in the source file to the destination file.
  The SYM tokens are updated to reflect the base address.

Arguments:

  BaseAddress    - The base address for the new SYM tokens.
  DestFileName   - The destination file.
  SourceFileName - The source file.

Returns:

  EFI_SUCCESS             - The function completed successfully.
  EFI_INVALID_PARAMETER   - One of the input parameters was invalid.
  EFI_ABORTED             - An error occurred.

--*/
;

EFI_STATUS
CalculateFitTableChecksum (
  VOID
  )
/*++
  
Routine Description:

  This function will perform byte checksum on the FIT table, if the the checksum required
  field is set to CheckSum required. If the checksum is not required then checksum byte
  will have value as 0;.
  
Arguments:

  NONE
  
Returns:

  Status       - Value returned by call to CalculateChecksum8 ()
  EFI_SUCCESS  - The function completed successfully
    
--*/
;

EFI_STATUS
GenerateVtfImage (
  IN  UINT64  StartAddress1,
  IN  UINT64  Size1,
  IN  UINT64  StartAddress2,
  IN  UINT64  Size2,
  IN  FILE    *fp
  )
/*++

Routine Description:

  This is the main function which will be called from application.

Arguments:

  StartAddress1  - The start address of the first VTF      
  Size1          - The size of the first VTF
  StartAddress2  - The start address of the second VTF      
  Size2          - The size of the second VTF

Returns:
 
  EFI_OUT_OF_RESOURCES - Can not allocate memory
  The return value can be any of the values 
  returned by the calls to following functions:
      GetVtfRelatedInfoFromInfFile
      ProcessAndCreateVtf
      UpdateIA32ResetVector
      UpdateFfsHeader
      WriteVtfBinary
  
--*/
;

EFI_STATUS
PeimFixupInFitTable (
  IN  UINT64  StartAddress
  )
/*++

Routine Description:

  This function is an entry point to fixup SAL-E entry point.

Arguments:

  StartAddress - StartAddress for PEIM.....
    
Returns:
 
  EFI_SUCCESS   - The function completed successfully
  EFI_ABORTED   - Error Opening File 

--*/
;

EFI_STATUS
Generate32VtfImage (
IN  UINT64  BootFileStartAddress
  )
/*++

Routine Description:

  This is the main IA32 function which will be called from application.
  (Now this tool is not used for IA32 platform, if it will be used in future,
  the relative functions need to be updated, the updating can refer to IPF 
  functions)

Arguments:

  BootFileStartAddress   - Top Address of Boot File

Returns:
 
  The return value can be any of the values 
  returned by the calls to following functions:
      Get32VtfRelatedInfoFromInfFile
      CreateVtfBuffer
      ProcessAndCreate32Vtf
      Update32FfsHeader
      WriteVtfBinary
  
--*/
;

EFI_STATUS
GetTotal32VtfSize(
  IN  UINT32  *VtfSize 
  )
/*++

Routine Description:

  This function calculates total size for IA32 VTF which would be needed to create
  the buffer. This will be done using Passed Info link list and looking for the
  size of the components which belong to VTF. The addtional file header is accounted.

Arguments:

  VTFSize     - Pointer to the size of IA32 VTF 

Returns:

  EFI_ABORTED - Returned due to one of the following resons:
                (a) Error Opening File
  EFI_SUCCESS - The fuction completes successfully

--*/
;

EFI_STATUS
ProcessAndCreate32Vtf (
  IN  UINT64  Size
  )
/*++

Routine Description:

  This function process the link list created during INF file parsing
  and create component in IA32 VTF
  
Arguments:

  Size   - Size of the Firmware Volume of which, this VTF belongs to.

Returns:
  
  EFI_UNSUPPORTED - Unknown component type
  EFI_SUCCESS     - The function completed successfully                 

--*/
;

EFI_STATUS
CreateAndUpdateSeccore (
  IN  PARSED_VTF_INFO   *VtfInfo
  )
/*++
  
Routine Description:

  This function reads the binary file for seccore and update them
  in IA32 VTF Buffer
  
Arguments:

  VtfInfo    - Pointer to Parsed Info
  
Returns:

  EFI_ABORTED           - Due to one of the following reasons:
                           (a)Error Opening File
                           (b)The PAL_A Size is more than specified size status
                              One of the values mentioned below returned from 
                              call to UpdateSymFile
  EFI_SUCCESS           - The function completed successfully.
  EFI_INVALID_PARAMETER - One of the input parameters was invalid.
  EFI_ABORTED           - An error occurred.UpdateSymFile
   
--*/
;

EFI_STATUS
CreateAndUpdate32Component (
  IN  PARSED_VTF_INFO   *VtfInfo
  )
/*++
  
Routine Description:

  This function reads the binary file for each components. Add it at aligned address.
  
Arguments:

  VtfInfo    - Pointer to Parsed Info
  
Returns:

  EFI_SUCCESS              - The function completed successful
  EFI_ABORTED              - Aborted due to one of the many reasons like:
                              (a) Component Size greater than the specified size.
                              (b) Error opening files.
  EFI_INVALID_PARAMETER    - Value returned from call to UpdateEntryPoint()
  
--*/
;

EFI_STATUS
Update32FfsHeader(
  IN      UINT32 VtfSize
  )
/*++

Routine Description:

  Update the Firmware Volume Buffer with requested buffer data

Arguments:

  VtfSize     - Size of the IA32 VTF

Returns:
  
  EFI_SUCCESS            - The function completed successfully
  EFI_INVALID_PARAMETER  - The Ffs File Header Pointer is NULL

--*/
;

EFI_STATUS
Get32VtfRelatedInfoFromInfFile (
  IN  CHAR8 *FileName
  )
/*++
  
Routine Description:

  This function reads the input file, parse it and create a list of tokens
  which is parsed and used, to intialize the data related to IA32 VTF
  
Arguments:

  FileName  FileName which needed to be read to parse data

Returns:
   
  EFI_ABORTED            Error in opening file
  EFI_INVALID_PARAMETER  File doesn't contain any valid informations
  EFI_OUT_OF_RESOURCES   Malloc Failed
  EFI_SUCCESS            The function completed successfully 

--*/
;

VOID
Initialize32InFileInfo (
  VOID                     
  )
/*++

Routine Description:

  This function intializes the relevant global variable which is being
  used to store the information retrieved from IA32 INF file.

Arguments:

  NONE

Returns:

  NONE

--*/
;

VOID 
ParseAndUpdate32Components (
  IN  PARSED_VTF_INFO   *VtfInfo
  )
/*++

Routine Description:

  This function intializes the relevant global variable which is being
  used to store the information retrieved from INF file.
  
Arguments:

  VtfInfo   - A pointer to the VTF Info Structure
  

Returns:

  None

--*/
;

EFI_STATUS
Write32SoftFit(
  IN CHAR8              *FileName,
  IN  PARSED_VTF_INFO   *VtfInfo
  )
/*++

Routine Description:

  Write IA32 Firmware Volume component address from memory to a file.
  
Arguments:

  FileName      Output File Name which needed to be created/
  VtfInfo       Parsed info link
  
Returns:

  EFI_ABORTED  - Returned due to one of the following resons:
                  (a) Error Opening File
                  (b) Failing to copy buffers
  EFI_SUCCESS  - The fuction completes successfully

--*/
;

#endif
