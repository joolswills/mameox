#pragma code_seg("CC8")
#pragma data_seg("CD8")
#pragma bss_seg("CB8")
#pragma const_seg("CK8")
#pragma comment(linker, "/merge:CD8=CPU8")
#pragma comment(linker, "/merge:CC8=CPU8")
#pragma comment(linker, "/merge:CB8=CPU8")
#pragma comment(linker, "/merge:CK8=CPU8")
/*
 *  HALT: must add log
 */

UINT32 opBRK(void)
{
/*
	UPDATEPSW();

	SP -=4;
	MemWrite32(SP, 0x0d00);
	SP -=4;
	MemWrite32(SP, PSW);
	SP -=4;
	MemWrite32(SP, PC + 1);
	PC = GETINTVECT(13);
	ChangePC(PC);
*/
	logerror("Skipping BRK opcode! PC=%x", PC);

	return 1;
}

UINT32 opBRKV(void)
{
	UPDATEPSW();

	SP -=4;
	MemWrite32(SP, PC);
	SP -=4;
	MemWrite32(SP, 0x0d00);
	SP -=4;
	MemWrite32(SP, PSW);
	SP -=4;
	MemWrite32(SP, PC + 1);
	PC = GETINTVECT(21);
	ChangePC(PC);

	return 0;
}

UINT32 opCLRTLBA(void)
{
	// @@@ TLB not yet supported
	logerror("Skipping CLRTLBA opcode! PC=%x\n", PC);
	return 1;
}

UINT32 opDISPOSE(void)
{
	SP = FP;
	FP = MemRead32(SP);
	SP +=4;

	return 1;
}

UINT32 opHALT(void)
{
	// @@@ It should wait for an interrupt to occur
	//logerror("HALT found: skipping");
	return 1;
}

UINT32 opNOP(void) /* TRUSTED */
{
	return 1;
}

UINT32 opRSR(void)
{
	PC = MemRead32(SP);
	SP +=4;
	ChangePC(PC);

	return 0;
}

UINT32 opTRAPFL(void)
{
	UPDATEPSW();

	if ((TKCW & 0x1F0) & ((PSW & 0x1F00) >> 4))
	{
		// @@@ FPU exception
		logerror("Hit TRAPFL! PC=%x\n", PC);
		abort();
	}

	return 1;
}







#pragma code_seg()
#pragma data_seg()
#pragma bss_seg()
#pragma const_seg()
