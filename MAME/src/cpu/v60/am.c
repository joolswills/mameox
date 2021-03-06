#pragma code_seg("CC8")
#pragma data_seg("CD8")
#pragma bss_seg("CB8")
#pragma const_seg("CK8")
#pragma comment(linker, "/merge:CD8=CPU8")
#pragma comment(linker, "/merge:CC8=CPU8")
#pragma comment(linker, "/merge:CB8=CPU8")
#pragma comment(linker, "/merge:CK8=CPU8")
// NOTE for bit string/field addressing
// ************************************
// modDim must be passed as 10 for bit string instructions,
// and as 11 for bit field instructions



// Output variables for ReadAMAddress()
UINT8 amFlag;
UINT32 amOut;
UINT32 bamOffset;

// Appo temp var
UINT32 amLength1,amLength2,amLength3,amLength4;


// Global vars used by AM functions
UINT32 modAdd;
UINT8 modM;
UINT8 modVal;
UINT8 modVal2;
UINT8 modWriteValB;
UINT16 modWriteValH;
UINT32 modWriteValW;
UINT8 modDim;

// Addressing mode functions and tables
#include "am1.c" // ReadAM
#include "am2.c" // ReadAMAddress
#include "am3.c" // WriteAM

/*
  Input:
  modAdd
	modDim

  Output:
	amOut
	amLength
*/

UINT32 ReadAM(void)
{
	modM=modM?1:0;
	modVal=OpRead8(modAdd);
	return AMTable1[modM][modVal>>5]();
}

UINT32 BitReadAM(void)
{
	modM=modM?1:0;
	modVal=OpRead8(modAdd);
	return BAMTable1[modM][modVal>>5]();
}



/*
  Input:
  modAdd
	modDim

  Output:
	amOut
	amFlag
	amLength
*/

UINT32 ReadAMAddress(void)
{
	modM=modM?1:0;
	modVal=OpRead8(modAdd);
	return AMTable2[modM][modVal>>5]();
}

UINT32 BitReadAMAddress(void)
{
	modM=modM?1:0;
	modVal=OpRead8(modAdd);
	return BAMTable2[modM][modVal>>5]();
}

/*
  Input:
  modAdd
	modDim
	modWriteValB/H/W

  Output:
	amOut
	amLength
*/

UINT32 WriteAM(void)
{
	modM=modM?1:0;
	modVal=OpRead8(modAdd);
	return AMTable3[modM][modVal>>5]();
}


#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
