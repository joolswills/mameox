#pragma code_seg("CC4")
#pragma data_seg("CD4")
#pragma bss_seg("CB4")
#pragma const_seg("CK4")
#pragma comment(linker, "/merge:CD4=CPU4")
#pragma comment(linker, "/merge:CC4=CPU4")
#pragma comment(linker, "/merge:CB4=CPU4")
#pragma comment(linker, "/merge:CK4=CPU4")
void InitTables(void)
{
	UINT32 t;

	for (t=0; t<256; t++)
	{
		OpCodeTable[t] = opUNHANDLED;
	}

	OpCodeTable[0x00] = opHALT;
	OpCodeTable[0x02] = opSTPR;
	OpCodeTable[0x08] = opRVBIT;
	OpCodeTable[0x09] = opMOVB;
	OpCodeTable[0x0a] = opMOVSBH;
	OpCodeTable[0x0b] = opMOVZBH;
	OpCodeTable[0x0c] = opMOVSBW;
	OpCodeTable[0x0d] = opMOVZBW;
	OpCodeTable[0x10] = opCLRTLBA;
	OpCodeTable[0x12] = opLDPR;
	OpCodeTable[0x13] = opUPDPSWW;
	OpCodeTable[0x19] = opMOVTHB;
	OpCodeTable[0x1b] = opMOVH;
	OpCodeTable[0x1c] = opMOVSHW;
	OpCodeTable[0x1d] = opMOVZHW;
	OpCodeTable[0x20] = opINB;
	OpCodeTable[0x21] = opOUTB;
	OpCodeTable[0x22] = opINH;
	OpCodeTable[0x23] = opOUTH;
	OpCodeTable[0x24] = opINW;
	OpCodeTable[0x25] = opOUTW;
	OpCodeTable[0x29] = opMOVTWB;
	OpCodeTable[0x2b] = opMOVTWH;
	OpCodeTable[0x2c] = opRVBYT;
	OpCodeTable[0x2d] = opMOVW;
	OpCodeTable[0x38] = opNOTB;
	OpCodeTable[0x39] = opNEGB;
	OpCodeTable[0x3a] = opNOTH;
	OpCodeTable[0x3b] = opNEGH;
	OpCodeTable[0x3c] = opNOTW;
	OpCodeTable[0x3d] = opNEGW;
	OpCodeTable[0x3f] = opMOVD;
	OpCodeTable[0x40] = opMOVEAB;
	OpCodeTable[0x41] = opXCHB;
	OpCodeTable[0x42] = opMOVEAH;
	OpCodeTable[0x43] = opXCHH;
	OpCodeTable[0x44] = opMOVEAW;
	OpCodeTable[0x45] = opXCHW;
	OpCodeTable[0x47] = opSETF;
	OpCodeTable[0x48] = opBSR;
	OpCodeTable[0x49] = opCALL;
	OpCodeTable[0x4a] = opUPDPSWH;
	OpCodeTable[0x4b] = opCHLVL;
	OpCodeTable[0x4d] = opCHKAR;
	OpCodeTable[0x4e] = opCHKAW;
	OpCodeTable[0x4f] = opCHKAE;
	OpCodeTable[0x50] = opREMB;
	OpCodeTable[0x51] = opREMUB;
	OpCodeTable[0x52] = opREMH;
	OpCodeTable[0x53] = opREMUH;
	OpCodeTable[0x54] = opREMW;
	OpCodeTable[0x55] = opREMUW;
	OpCodeTable[0x58] = op58;
	OpCodeTable[0x59] = op59;
	OpCodeTable[0x5A] = op5A;
	OpCodeTable[0x5B] = op5B;
	OpCodeTable[0x5C] = op5C;
	OpCodeTable[0x5D] = op5D;
	OpCodeTable[0x5F] = op5F;
	OpCodeTable[0x60] = opBV8;
	OpCodeTable[0x61] = opBNV8;
	OpCodeTable[0x62] = opBL8;
	OpCodeTable[0x63] = opBNL8;
	OpCodeTable[0x64] = opBE8;
	OpCodeTable[0x65] = opBNE8;
	OpCodeTable[0x66] = opBNH8;
	OpCodeTable[0x67] = opBH8;
	OpCodeTable[0x68] = opBN8;
	OpCodeTable[0x69] = opBP8;
	OpCodeTable[0x6A] = opBR8;
	OpCodeTable[0x6C] = opBLT8;
	OpCodeTable[0x6D] = opBGE8;
	OpCodeTable[0x6E] = opBLE8;
	OpCodeTable[0x6F] = opBGT8;
	OpCodeTable[0x70] = opBV16;
	OpCodeTable[0x71] = opBNV16;
	OpCodeTable[0x72] = opBL16;
	OpCodeTable[0x73] = opBNL16;
	OpCodeTable[0x74] = opBE16;
	OpCodeTable[0x75] = opBNE16;
	OpCodeTable[0x76] = opBNH16;
	OpCodeTable[0x77] = opBH16;
	OpCodeTable[0x78] = opBN16;
	OpCodeTable[0x79] = opBP16;
	OpCodeTable[0x7A] = opBR16;
	OpCodeTable[0x7C] = opBLT16;
	OpCodeTable[0x7D] = opBGE16;
	OpCodeTable[0x7E] = opBLE16;
	OpCodeTable[0x7F] = opBGT16;
	OpCodeTable[0x80] = opADDB;
	OpCodeTable[0x81] = opMULB;
	OpCodeTable[0x82] = opADDH;
	OpCodeTable[0x83] = opMULH;
	OpCodeTable[0x84] = opADDW;
	OpCodeTable[0x85] = opMULW;
	OpCodeTable[0x86] = opMULX;
	OpCodeTable[0x87] = opTEST1;
	OpCodeTable[0x88] = opORB;
	OpCodeTable[0x89] = opROTB;
	OpCodeTable[0x8A] = opORH;
	OpCodeTable[0x8B] = opROTH;
	OpCodeTable[0x8C] = opORW;
	OpCodeTable[0x8D] = opROTW;
	OpCodeTable[0x90] = opADDCB;
	OpCodeTable[0x91] = opMULUB;
	OpCodeTable[0x92] = opADDCH;
	OpCodeTable[0x93] = opMULUH;
	OpCodeTable[0x94] = opADDCW;
	OpCodeTable[0x95] = opMULUW;
	OpCodeTable[0x96] = opMULUX;
	OpCodeTable[0x97] = opSET1;
	OpCodeTable[0x98] = opSUBCB;
	OpCodeTable[0x99] = opROTCB;
	OpCodeTable[0x9a] = opSUBCH;
	OpCodeTable[0x9b] = opROTCH;
	OpCodeTable[0x9c] = opSUBCW;
	OpCodeTable[0x9d] = opROTCW;
	OpCodeTable[0xa0] = opANDB;
	OpCodeTable[0xa1] = opDIVB;
	OpCodeTable[0xa2] = opANDH;
	OpCodeTable[0xa3] = opDIVH;
	OpCodeTable[0xa4] = opANDW;
	OpCodeTable[0xa5] = opDIVW;
	OpCodeTable[0xa6] = opDIVX;
	OpCodeTable[0xa7] = opCLR1;
	OpCodeTable[0xa8] = opSUBB;
	OpCodeTable[0xa9] = opSHLB;
	OpCodeTable[0xaa] = opSUBH;
	OpCodeTable[0xab] = opSHLH;
	OpCodeTable[0xac] = opSUBW;
	OpCodeTable[0xad] = opSHLW;
	OpCodeTable[0xb0] = opXORB;
	OpCodeTable[0xb1] = opDIVUB;
	OpCodeTable[0xb2] = opXORH;
	OpCodeTable[0xb3] = opDIVUH;
	OpCodeTable[0xb4] = opXORW;
	OpCodeTable[0xb5] = opDIVUW;
	OpCodeTable[0xb6] = opDIVUX;
	OpCodeTable[0xb7] = opNOT1;
	OpCodeTable[0xb8] = opCMPB;
	OpCodeTable[0xb9] = opSHAB;
	OpCodeTable[0xba] = opCMPH;
	OpCodeTable[0xbb] = opSHAH;
	OpCodeTable[0xbc] = opCMPW;
	OpCodeTable[0xbd] = opSHAW;
	OpCodeTable[0xc6] = opC6;
	OpCodeTable[0xc7] = opC7;
	OpCodeTable[0xc8] = opBRK;
	OpCodeTable[0xc9] = opBRKV;
	OpCodeTable[0xca] = opRSR;
	OpCodeTable[0xcb] = opTRAPFL;
	OpCodeTable[0xcc] = opDISPOSE;
	OpCodeTable[0xcd] = opNOP;
	OpCodeTable[0xd0] = opDECB_0;
	OpCodeTable[0xd1] = opDECB_1;
	OpCodeTable[0xd2] = opDECH_0;
	OpCodeTable[0xd3] = opDECH_1;
	OpCodeTable[0xd4] = opDECW_0;
	OpCodeTable[0xd5] = opDECW_1;
	OpCodeTable[0xd6] = opJMP_0;
	OpCodeTable[0xd7] = opJMP_1;
	OpCodeTable[0xd8] = opINCB_0;
	OpCodeTable[0xd9] = opINCB_1;
	OpCodeTable[0xda] = opINCH_0;
	OpCodeTable[0xdb] = opINCH_1;
	OpCodeTable[0xdc] = opINCW_0;
	OpCodeTable[0xdd] = opINCW_1;
	OpCodeTable[0xde] = opPREPARE_0;
	OpCodeTable[0xdf] = opPREPARE_1;
	OpCodeTable[0xe0] = opTASI_0;
	OpCodeTable[0xe1] = opTASI_1;
	OpCodeTable[0xe2] = opRET_0;
	OpCodeTable[0xe3] = opRET_1;
	OpCodeTable[0xe4] = opPOPM_0;
	OpCodeTable[0xe5] = opPOPM_1;
	OpCodeTable[0xe6] = opPOP_0;
	OpCodeTable[0xe7] = opPOP_1;
	OpCodeTable[0xe8] = opJSR_0;
	OpCodeTable[0xe9] = opJSR_1;
	OpCodeTable[0xea] = opRETIU_0;
	OpCodeTable[0xeb] = opRETIU_1;
	OpCodeTable[0xec] = opPUSHM_0;
	OpCodeTable[0xed] = opPUSHM_1;
	OpCodeTable[0xee] = opPUSH_0;
	OpCodeTable[0xef] = opPUSH_1;
	OpCodeTable[0xf0] = opTESTB_0;
	OpCodeTable[0xf1] = opTESTB_1;
	OpCodeTable[0xf2] = opTESTH_0;
	OpCodeTable[0xf3] = opTESTH_1;
	OpCodeTable[0xf4] = opTESTW_0;
	OpCodeTable[0xf5] = opTESTW_1;
	OpCodeTable[0xf6] = opGETPSW_0;
	OpCodeTable[0xf7] = opGETPSW_1;
	OpCodeTable[0xf8] = opTRAP_0;
	OpCodeTable[0xf9] = opTRAP_1;
	OpCodeTable[0xfa] = opRETIS_0;
	OpCodeTable[0xfb] = opRETIS_1;
}

#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
