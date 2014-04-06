;======================================================================
; Include File for the TI-83 Plus
; Last Updated 06/20/2002 
;
; Copyright (c) 1999, 2001, 2002 Texas Instruments: The Licensed Materials are
; copyrighted by TI. LICENSEE agrees that it will 
; not delete the copyright notice, trademarks or
; protective notices from any copy made by LICENSEE.
;
; Warranty: TI does not warrant that the Licensed Materials will
; be free from errors or will meet your specific requirements.
; The Licensed Materials are made available "AS IS" to LICENSEE.
;
; Limitations: TI MAKES NO WARRANTY OR CONDITION, EITHER EXPRESS
; OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
; REGARDING THE LICENSED MATERIALS.  IN NO EVENT SHALL
; TI OR ITS SUPPLIERS BE LIABLE FOR ANY INDIRECT, INCIDENTAL
; OR CONSequENTIAL DAMAGES, LOSS OF PROFITS, LOSS OF USE OR DATA, 
; OR INTERRUPTION OF BUSINESS, WHETHER THE ALLEGED DAMAGES ARE
; LABELED IN TORT, CONTRACT OR INDEMNITY.
;
;
;:::::::::::::::::::::::
;Edited by Brett Wagner
;For use with TASM
;Edited by Michael Kohn
;For use with Java Grinder
;:::::::::::::::::::::::
;
;======================================================================
;           System Variable Equates
;======================================================================
;       Entry Points : RclSysTok, StoSysTok
;
XSCLt		equ  2
YSCLt		equ  3
XMINt		equ  0Ah
XMAXt		equ  0Bh
YMINt		equ  0Ch
YMAXt		equ  0Dh
TMINt		equ  0Eh
TMAXt		equ  0Fh
THETMINt	equ  10h
THETMAXt	equ  11h
TBLMINt		equ  1Ah
PLOTSTARTt	equ  1Bh
NMAXt		equ  1Dh
NMINt		equ  1Fh
TBLSTEPt	equ  21h
TSTEPt		equ  22h
THETSTEPt	equ  23h
DELTAXt		equ  26h
DELTAYt		equ  27h
XFACTt		equ  28h
YFACTt		equ  29h
FINNt		equ  2Bh
FINIt		equ  2Ch
FINPVt		equ  2Dh
FINPMTt		equ  2Eh
FINFVt		equ  2Fh
FINPYt		equ  30h
FINCYt		equ  31h
PLOTSTEPt	equ  34h
XRESt		equ  36h
;

;======================================================================
;           Run indicators
;======================================================================
;
busyPause	equ	10101010b
busyNormal	equ	11110000b
;
;======================================================================
; Macro to call base code from an app,
;======================================================================
;
;rBR_CALL	equ	28h		; NEEDED FOR APPS
;BRT_JUMP0	equ	50h
;
;B_CALL macro   label
;	     RST     rBR_CALL
;	     DW      _&label
;       .endm
;
;B_JUMP macro   label
;	     CALL    BRT_JUMP0	     ; fixed location on page 0
;	     DW      _&label
;       .endm

;======================================================================
;           Common subroutine RST numbers  
;======================================================================
;
rOP1TOOP2	equ  08h
rFINDSYM	equ  10h
rPUSHREALO1	equ  18h
rMOV9TOOP1	equ  20h
rFPADD		equ  30h
;

;SYMBOLIC_DEBUG_START
;======================================================================
;           Entry points  
;======================================================================
;
_AbsO1O2Cp	equ 410Eh
_AbsO1PAbsO2	equ 405Ah
_ACos		equ 40DEh
_ACosH		equ 40F0h
_ACosRad	equ 40D2h
_AdrLEle	equ 462Dh
_AdrMEle	equ 4609h
_AdrMRow	equ 4606h
_AllEq		equ 4876h
_AllocFPS	equ 43A5h
_AllocFPS1	equ 43A8h
_Angle		equ 4102h
_AnsName	equ 4B52h
_ApdSetup	equ 4C93h
_AppGetCalc	equ 4C78h
_AppGetCbl	equ 4C75h
_AppInit	equ 404Bh  ;***
_Arc_Unarc	equ 4FD8h
_ASin		equ 40E4h
_ASinH		equ 40EDh
_ASinRad	equ 40DBh
_ATan		equ 40E1h
_ATan2		equ 40E7h
_ATan2Rad	equ 40D8h
_ATanH		equ 40EAh
_ATanRad	equ 40D5h

_BinOPExec	equ 4663h
_Bit_VertSplit	equ 4FA8h
_BufClr		equ 5074h
_BufCpy		equ 5071h

_CAbs		equ 4E97h
_CAdd		equ 4E88h
_CanAlphIns	equ 4C69h
_CDiv		equ 4E94h
_CDivByReal	equ 4EBBh
_CEtoX		equ 4EA9h
_CFrac		equ 4EC1h
_CheckSplitFlag	equ 49F0h
_Chk_Batt_Low	equ 50B3h  ;***
_ChkFindSym	equ 42F1h
_CIntgr		equ 4EC4h
_CircCmd	equ 47D4h
_CkInt		equ 4234h
_CkOdd		equ 4237h
_CkOP1C0	equ 4225h
_CkOP1Cplx	equ 40FCh
_CkOP1FP0	equ 4228h
_CkOP1Pos	equ 4258h
_CkOP1Real	equ 40FFh
_CkOP2FP0	equ 422Bh
_CkOP2Pos	equ 4255h
_CkOP2Real	equ 42DFh
_CkPosInt	equ 4231h
_CkValidNum	equ 4270h
_CleanAll	equ 4A50h
_ClearRect	equ 4D5Ch
_ClearRow	equ 4CEDh
_CLine		equ 4798h
_CLineS		equ 479Bh
_CLN		equ 4EA0h
_CLog		equ 4EA3h
_CloseEditBuf	equ 48D3h  ;***
_CloseEditBufNoR	equ 476Eh
_CloseEditEqu	equ 496Ch  ;***
_CloseProg	equ 4A35h
_ClrGraphRef	equ 4A38h
_ClrLCD		equ 4543h
_ClrLCDFull	equ 4540h
_ClrLp		equ 41D1h
_ClrOP1S	equ 425Eh
_ClrOP2S	equ 425Bh
_ClrScrn	equ 4549h
_ClrScrnFull	equ 4546h
_ClrTxtShd	equ 454Ch
_CMltByReal	equ 4EB8h
_CmpSyms	equ 4A4Ah
_CMult		equ 4E8Eh
_Conj		equ 4EB5h
_ConvDim	equ 4B43h
_ConvDim00	equ 4B46h
_ConvLcToLr	equ 4A23h
_ConvLrToLc	equ 4A56h
_ConvOP1	equ 4AEFh
_COP1Set0	equ 4105h
_Cos		equ 40C0h
_CosH		equ 40CCh
_CpHLDE		equ 400Ch
_CPoint		equ 4DC8h
_CPointS	equ 47F5h
_CpOP1OP2	equ 4111h
_CpOP4OP3	equ 4108h
_CpyO1ToFPS1	equ 445Ch
_CpyO1ToFPS2	equ 446Bh
_CpyO1ToFPS3	equ 4477h
_CpyO1ToFPS4	equ 4489h
_CpyO1ToFPS5	equ 4483h
_CpyO1ToFPS6	equ 447Dh
_CpyO1ToFPS7	equ 4480h
_CpyO1ToFPST	equ 444Ah
_CpyO2ToFPS1	equ 4459h
_CpyO2ToFPS2	equ 4462h
_CpyO2ToFPS3	equ 4474h
_CpyO2ToFPS4	equ 4486h
_CpyO2ToFPST	equ 4444h
_CpyO3ToFPS1	equ 4453h
_CpyO3ToFPS2	equ 4465h
_CpyO3ToFPST	equ 4441h
_CpyO5ToFPS1	equ 4456h
_CpyO5ToFPS3	equ 4471h
_CpyO6ToFPS2	equ 4468h
_CpyO6ToFPST	equ 4447h
_CpyStack	equ 4429h
_CpyTo1FPS1	equ 4432h
_CpyTo1FPS10	equ 43F3h
_CpyTo1FPS11	equ 43D8h
_CpyTo1FPS2	equ 443Bh
_CpyTo1FPS3	equ 4408h
_CpyTo1FPS4	equ 440Eh
_CpyTo1FPS5	equ 43DEh
_CpyTo1FPS6	equ 43E4h
_CpyTo1FPS7	equ 43EAh
_CpyTo1FPS8	equ 43EDh
_CpyTo1FPS9	equ 43F6h
_CpyTo1FPST	equ 4423h
_CpyTo2FPS1	equ 442Fh
_CpyTo2FPS2	equ 4438h
_CpyTo2FPS3	equ 4402h
_CpyTo2FPS4	equ 43F9h
_CpyTo2FPS5	equ 43DBh
_CpyTo2FPS6	equ 43E1h
_CpyTo2FPS7	equ 43E7h
_CpyTo2FPS8	equ 43F0h
_CpyTo2FPST	equ 4420h
_CpyTo3FPS1	equ 442Ch
_CpyTo3FPS2	equ 4411h
_CpyTo3FPST	equ 441Dh
_CpyTo4FPST	equ 441Ah
_CpyTo5FPST	equ 4414h
_CpyTo6FPS2	equ 43FFh
_CpyTo6FPS3	equ 43FCh
_CpyTo6FPST	equ 4417h
_CpyToFPS1	equ 445Fh
_CpyToFPS2	equ 446Eh
_CpyToFPS3	equ 447Ah
_CpyToFPST	equ 444Dh
_CpyToStack	equ 4450h
_Create0Equ	equ 432Ah
_CreateAppVar	equ 4E6Ah
_CreateCList	equ 431Bh
_CreateCplx	equ 430Ch
_CreateEqu	equ 4330h
_CreatePair	equ 4B0Dh
_CreatePict	equ 4333h
_CreateProg	equ 4339h
_CreateProtProg	equ 4E6Dh
_CreateReal	equ 430Fh
_CreateRList	equ 4315h
_CreateRMat	equ 4321h
_CreateStrng	equ 4327h
_CRecip		equ 4E91h
_CSqRoot	equ 4E9Dh
_CSquare	equ 4E8Bh
_CSub		equ 4E85h
_CTenX		equ 4EA6h
_CTrunc		equ 4EBEh
_Cube		equ 407Bh
_CursorOff	equ 45BEh
_CursorOn	equ 45C4h
_CXrootY	equ 4EACh
_CYtoX		equ 4EB2h

_DarkLine	equ 47DDh
_DarkPnt	equ 47F2h
_DataSize	equ 436Ch
_DataSizeA	equ 4369h
_DeallocFPS	equ 439Fh
_DeallocFPS1	equ 43A2h
_DecO1Exp	equ 4267h
_DelListEl	equ 4A2Fh
_DelMem		equ 4357h
_DelRes		equ 4A20h
_DelVar		equ 4351h
_DelVarArc	equ 4FC6h
_DelVarNoArc	equ 4FC9h
_DisableApd	equ 4C84h
_Disp		equ 4F45h
_DispDone	equ 45B5h
_DispEOL	equ 45A6h
_DispHL		equ 4507h
_DisplayImage	equ 4D9Bh
_DispMenuTitle	equ 5065h
_DispOP1A	equ 4BF7h
_DivHLBy10	equ 400Fh
_DivHLByA	equ 4012h
_DrawCirc2	equ 4C66h
_DrawCmd	equ 48C1h
_DrawRectBorder	equ 4D7Dh
_DrawRectBorderClear	equ 4D8Ch
_DToR		equ 4075h

_EditProg	equ 4A32h
_EnableApd	equ 4C87h
_EnoughMem	equ 42FDh
_EOP1NotReal	equ 4279h
_Equ_or_NewEqu	equ 42C4h
_EraseEOL	equ 4552h
_EraseRectBorder	equ 4D86h
_ErrArgument	equ 44ADh
_ErrBadGuess	equ 44CBh
_ErrBreak	equ 44BFh
_ErrD_OP1_0	equ 42D3h
_ErrD_OP1_LE_0	equ 42D0h
_ErrD_OP1Not_R	equ 42CAh
_ErrD_OP1NotPos	equ 42C7h
_ErrD_OP1NotPosInt	equ 42CDh
_ErrDataType	equ 44AAh
_ErrDimension	equ 44B3h
_ErrDimMismatch	equ 44B0h
_ErrDivBy0	equ 4498h
_ErrDomain	equ 449Eh
_ErrIncrement	equ 44A1h
_ErrInvalid	equ 44BCh
_ErrIterations	equ 44C8h
_ErrLinkXmit	equ 44D4h
_ErrMemory	equ 44B9h
_ErrNon_Real	equ 44A4h
_ErrNonReal	equ 4A8Ch
_ErrNotEnoughMem	equ 448Ch
_ErrOverflow	equ 4495h
_ErrSignChange	equ 44C5h
_ErrSingularMat	equ 449Bh
_ErrStat	equ 44C2h
_ErrStatPlot	equ 44D1h
_ErrSyntax	equ 44A7h
_ErrTolTooSmall	equ 44CEh
_ErrUndefined	equ 44B6h
_EToX		equ 40B4h
_Exch9		equ 43D5h
_ExLp		equ 4222h
_ExpToHex	equ 424Fh

_Factorial	equ 4B85h
_FillRect	equ 4D62h
_FillRectPattern	equ 4D89h
_Find_Parse_Formula	equ 4AF2h
_FindAlphaDn	equ 4A47h
_FindAlphaUp	equ 4A44h
_FindApp		equ 4C4Eh
_FindAppDn		equ 4C4Bh
_FindAppNumPages	equ 509Bh
_FindAppUp		equ 4C48h
_FindSym	equ 42F4h
_FiveExec	equ 467Eh
_FixTempCnt	equ 4A3Bh
_FlashToRam	equ 5017h
_ForceFullScreen	equ 508Fh
_FormBase	equ 50AAh
_FormDCplx	equ 4996h
_FormEReal	equ 4990h
_FormReal	equ 4999h
_FourExec	equ 467Bh
_FPAdd		equ 4072h
_FPDiv		equ 4099h
_FPMult		equ 4084h
_FPRecip	equ 4096h
_FPSquare	equ 4081h
_FPSub		equ 406Fh
_Frac		equ 4093h

_Get_Tok_Strng	equ 4594h  ;***
_GetBaseVer	equ 4C6Fh
_GetCSC		equ 4018h
_GetKey		equ 4972h
_GetLToOP1	equ 4636h
_GetMToOP1	equ 4615h
_GetSysInfo 	equ 50DDh  ; v. 1.13+ only
_GetTokLen	equ 4591h  ;***
_GrBufClr	equ 4BD0h
_GrBufCpy	equ 486Ah
_GrphCirc	equ 47D7h

_HLTimes9	equ 40F9h
_HorizCmd	equ 48A6h
_HTimesL	equ 4276h

_IBounds	equ 4C60h
_IBoundsFull	equ 4D98h
_ILine		equ 47E0h
_IncLstSize	equ 4A29h
_InsertList	equ 4A2Ch
_InsertMem	equ 42F7h
_Int		equ 40A5h
_Intgr		equ 405Dh
_InvCmd		equ 48C7h
_InvertRect	equ 4D5Fh
_InvOP1S	equ 408Dh
_InvOP1SC	equ 408Ah
_InvOP2S	equ 4090h
_InvSub		equ 4063h
_IOffset	equ 4C63h
_IPoint		equ 47E3h
_IsA2ByteTok	equ 42A3h  ;***
_IsEditEmpty	equ 492Dh  ;***

_JError		equ 44D7h
_JErrorNo	equ 4000h
_JForceCmdNoChar	equ 4027h
_JForceGraphKey	equ 5005h
_JForceGraphNoKey	equ 5002h

_KeyToString	equ 45CAh

_LCD_BUSY	equ 4051h  ;***
_LCD_DRIVERON	equ 4978h  ;***
_LdHLind	equ 4009h  ;***
_LineCmd	equ 48ACh
_LnX		equ 40ABh
_Load_SFont	equ 4783h
_LoadCIndPaged	equ 501Dh
_LoadDEIndPaged	equ 501Ah
_LoadPattern	equ 4CB1h
_LogX		equ 40AEh

_Max		equ 4057h
_MemChk		equ 42E5h
_MemClear	equ 4C30h
_MemSet		equ 4C33h
_Min		equ 4054h
_Minus1		equ 406Ch
_Mov10B		equ 415Ch
_Mov18B		equ 47DAh
_Mov7B		equ 4168h
_Mov8B		equ 4165h
_Mov9B		equ 415Fh
_Mov9OP1OP2	equ 417Dh
_Mov9OP2Cp	equ 410Bh
_Mov9ToOP1	equ 417Ah
_Mov9ToOP2	equ 4180h
_MovFrOP1	equ 4183h

_NewLine	equ 452Eh  ;***
_NZIf83Plus	equ 50E0h  ; v. 1.13+ only

_OneVar		equ 4BA3h
_OP1ExOP2	equ 421Fh
_OP1ExOP3	equ 4219h
_OP1ExOP4	equ 421Ch
_OP1ExOP5	equ 420Dh
_OP1ExOP6	equ 4210h
_OP1ExpToDec	equ 4252h
_OP1Set0	equ 41BFh
_OP1Set1	equ 419Bh
_OP1Set2	equ 41A7h
_OP1Set3	equ 41A1h
_OP1Set4	equ 419Eh
_OP1ToOP2	equ 412Fh
_OP1ToOP3	equ 4123h
_OP1ToOP4	equ 4117h
_OP1ToOP5	equ 4153h
_OP1ToOP6	equ 4150h
_OP2ExOP4	equ 4213h
_OP2ExOP5	equ 4216h
_OP2ExOP6	equ 4207h
_OP2Set0	equ 41BCh
_OP2Set1	equ 41ADh
_OP2Set2	equ 41AAh
_OP2Set3	equ 4198h
_OP2Set4	equ 4195h
_OP2Set5	equ 418Fh
_OP2Set60	equ 4AB0h
_OP2Set8	equ 418Ch
_OP2SetA	equ 4192h
_OP2ToOP1	equ 4156h
_OP2ToOP3	equ 416Eh
_OP2ToOP4	equ 411Ah
_OP2ToOP5	equ 414Ah
_OP2ToOP6	equ 414Dh
_OP3Set0	equ 41B9h
_OP3Set1	equ 4189h
_OP3Set2	equ 41A4h
_OP3ToOP1	equ 413Eh
_OP3ToOP2	equ 4120h
_OP3ToOP4	equ 4114h
_OP3ToOP5	equ 4147h
_OP4Set0	equ 41B6h
_OP4Set1	equ 4186h
_OP4ToOP1	equ 4138h
_OP4ToOP2	equ 411Dh
_OP4ToOP3	equ 4171h
_OP4ToOP5	equ 4144h
_OP4ToOP6	equ 4177h
_OP5ExOP6	equ 420Ah
_OP5Set0	equ 41B3h
_OP5ToOP1	equ 413Bh
_OP5ToOP2	equ 4126h
_OP5ToOP3	equ 4174h
_OP5ToOP4	equ 412Ch
_OP5ToOP6	equ 4129h
_OP6ToOP1	equ 4135h
_OP6ToOP2	equ 4132h
_OP6ToOP5	equ 4141h
_OutputExpr	equ 4BB2h

_PagedGet	equ 5023h
_ParseInp	equ 4A9Bh
_PDspGrph	equ 48A3h
_PixelTest	equ 48B5h
_Plus1		equ 4069h
_PointCmd	equ 48B2h
_PointOn	equ 4C39h
_PopMCplxO1	equ 436Fh  ;***
_PopOP1		equ 437Eh
_PopOP3		equ 437Bh
_PopOP5		equ 4378h
_PopReal	equ 4393h
_PopRealO1	equ 4390h
_PopRealO2	equ 438Dh
_PopRealO3	equ 438Ah
_PopRealO4	equ 4387h
_PopRealO5	equ 4384h
_PopRealO6	equ 4381h
_PosNo0Int	equ 422Eh
_PToR		equ 40F3h
_PushMCplxO1	equ 43CFh  ;***
_PushMCplxO3	equ 43C6h  ;***
_PushOP1	equ 43C9h
_PushOP3	equ 43C3h
_PushOP5	equ 43C0h
_PushReal	equ 43BDh
_PushRealO1	equ 43BAh
_PushRealO2	equ 43B7h
_PushRealO3	equ 43B4h
_PushRealO4	equ 43B1h
_PushRealO5	equ 43AEh
_PushRealO6	equ 43ABh
_PutAway	equ 403ch  ;***
_PutC		equ 4504h
_PutMap		equ 4501h
_PutPS		equ 4510h
_PutPSB		equ 450dh  ;***
_PutS		equ 450Ah
_PutTokString	equ 4960h
_PutToL		equ 4645h
_PutToMat	equ 461Eh

_RandInit	equ 4B7Fh
_Random		equ 4B79h
_Rcl_StatVar	equ 42DCh
_RclAns		equ 4AD7h
_RclGDB2	equ 47D1h
_RclN		equ 4ADDh
_RclSysTok	equ 4AE6h
_RclVarSym	equ 4AE3h
_RclX		equ 4AE0h
_RclY		equ 4ADAh
_Rec1stByte	equ 4EFAh
_Rec1stByteNC	equ 4EFDh
_RecAByteIO	equ 4F03h
_RedimMat	equ 4A26h
_Regraph	equ 488Eh
_ReleaseBuffer	equ 4771h  ;***
_ReloadAppEntryVecs	equ 4C36h
_RestoreDisp	equ 4870h
_RName		equ 427Fh
_RndGuard	equ 409Fh
_RnFx		equ 40A2h
_Round		equ 40A8h
_RToD		equ 4078h
_RToP		equ 40F6h
_RunIndicOff	equ 4570h
_RunIndicOn	equ 456Dh

_SaveDisp	equ 4C7Bh
_SendAByte	equ 4EE5h
_SendVarCmd	equ 4A14h ;***
_SetAllPlots	equ 4FCCh
_SetExSpeed 	equ 50BFh ; v. 1.13+ only
_SetFuncM	equ 4840h
_SetNorm_Vals	equ 49FCh
_SetParM	equ 4849h
_SetPolM	equ 4846h
_SetSeqM	equ 4843h
_SetTblGraphDraw	equ 4C00h
_SetupPagedPtr	equ 5020h
_SetXXOP1	equ 478Ch
_SetXXOP2	equ 478Fh
_SetXXXXOP2	equ 4792h
_SFont_Len	equ 4786h
_Sin		equ 40BDh
_SinCosRad	equ 40BAh
_SinH		equ 40CFh
_SinHCosH	equ 40C6h
_SqRoot		equ 409Ch
_SrchVLstDn	equ 4F12h
_SrchVLstUp	equ 4F0Fh
_SStringLength	equ 4CB4h
_StMatEl	equ 4AE9h
_StoAns		equ 4ABFh
_StoGDB2	equ 47CEh
_StoN		equ 4ACBh
_StoOther	equ 4AD4h
_StoR		equ 4AC5h
_StoRand	equ 4B7Ch
_StoSysTok	equ 4ABCh
_StoT		equ 4ACEh
_StoTheta	equ 4AC2h
_StoX		equ 4AD1h
_StoY		equ 4AC8h
_StrCopy	equ 44E3h
_StrLength	equ 4C3Fh   

_Tan		equ 40C3h
_TanH		equ 40C9h
_TanLnF		equ 48BBh
_TenX		equ 40B7h
_ThetaName	equ 427Ch
_ThreeExec	equ 4675h
_Times2		equ 4066h
_TimesPt5	equ 407Eh
_TName		equ 428Eh
_ToFrac		equ 4657h
_Trunc		equ 4060h
_TwoVarSet	equ 50D4h  ; v. 1.13+ only

_UCLineS	equ 4795h
_UnLineCmd	equ 48AFh
_UnOPExec	equ 4672h

_VertCmd	equ 48A9h
_VPutMap	equ 455Eh
_VPutS		equ 4561h
_VPutSN		equ 4564h
_VtoWHLDE	equ 47FBh


_XftoI		equ 4804h
_Xitof		equ 47FEh
_XName		equ 4288h
_XRootY		equ 479Eh

_YftoI		equ 4801h
_YName		equ 428Bh
_YToX		equ 47A1h

_Zero16D	equ 41B0h
_ZeroOP		equ 41CEh
_ZeroOP1	equ 41C5h
_ZeroOP2	equ 41C8h
_ZeroOP3	equ 41CBh
_ZmDecml	equ 484Fh
_ZmFit		equ 485Bh
_ZmInt		equ 484Ch
_ZmPrev		equ 4852h
_ZmSquare	equ 485Eh
_ZmStats	equ 47A4h
_ZmTrig		equ 4861h
_ZmUsr		equ 4855h
_ZooDefault	equ 4867h
;
;======================================================================
;           System-defined RAM Variable Address Equates
;======================================================================
;

ramStart             equ  8000h
appData              equ  8000h
ramCode              equ  8100h
ramCodeEnd           equ  822Fh
baseAppBrTab         equ  8230h
bootTemp             equ  8251h
appSearchPage        equ  82A3h
tempSwapArea         equ  82A5h
appID                equ  838Dh
ramReturnData        equ  83EDh
arcInfo              equ  83EEh
savedArcInfo         equ  8406h
appInfo              equ  8432h
appBank_jump         equ  843Ch
appPage              equ  843Eh
kbdScanCode          equ  843Fh
kbdKey               equ  8444h
kbdGetKy             equ  8445h
keyExtend            equ  8446h
contrast             equ  8447h
apdSubTimer          equ  8448h
apdTimer             equ  8449h
curTime              equ  844Ah
curRow               equ  844Bh
curCol               equ  844Ch
curOffset            equ  844Dh
curUnder             equ  844Eh
curY                 equ  844Fh
curType              equ  8450h
curXRow              equ  8451h
prevDData            equ  8452h
lFont_record         equ  845Ah
sFont_record         equ  8462h
tokVarPtr            equ  846Ah
tokLen               equ  846Ch
indicMem             equ  846Eh
indicCounter         equ  8476h
indicBusy            equ  8477h
OP1                  equ  8478h
OP1M                 equ  847Ah
OP2                  equ  8483h
OP2M                 equ  8485h
OP2EXT               equ  848Ch
OP3                  equ  848Eh
OP3M                 equ  8490h
OP4                  equ  8499h
OP4M                 equ  849Bh
OP5                  equ  84A4h
OP5M                 equ  84A6h
OP6                  equ  84AFh
OP6M                 equ  84B1h
OP6EXT               equ  84B8h
progToEdit           equ  84BFh
nameBuff             equ  84C7h
equ_edit_save        equ  84D2h
iMathPtr1            equ  84D3h
iMathPtr2            equ  84D5h
iMathPtr3            equ  84D7h
iMathPtr4            equ  84D9h
iMathPtr5            equ  84DBh
chkDelPtr1           equ  84DDh
chkDelPtr2           equ  84DFh
insDelPtr            equ  84E1h
upDownPtr            equ  84E3h
fOutDat              equ  84E5h
asm_data_ptr1        equ  84EBh
asm_data_ptr2        equ  84EDh
asm_sym_ptr1         equ  84EFh
asm_sym_ptr2         equ  84F1h
asm_ram              equ  84F3h
asm_ind_call         equ  8507h
textShadow           equ  8508h
textShadCur          equ  8588h
textShadTop          equ  858Ah
textShadAlph         equ  858Bh
textShadIns          equ  858Ch
cxMain               equ  858Dh
cxPPutAway           equ  858Fh
cxPutAway            equ  8591h
cxRedisp             equ  8593h
cxErrorEP            equ  8595h
cxSizeWind           equ  8597h
cxPage               equ  8599h
cxCurApp             equ  859Ah
cxPrev               equ  859Bh
monQH                equ  85AAh
monQT                equ  85ABh
monQueue             equ  85ACh
onSP                 equ  85BCh
promptRow            equ  85C0h
promptCol            equ  85C1h
promptIns            equ  85C2h
promptShift          equ  85C3h
promptRet            equ  85C4h
promptValid          equ  85C6h
promptTop            equ  85C8h
promptCursor         equ  85CAh
promptTail           equ  85CCh
promptBtm            equ  85CEh
varType              equ  85D0h
varCurrent           equ  85D1h
varClass             equ  85D9h
menuActive           equ  85DCh
menuAppDepth         equ  85DDh
MenuCurrent          equ  85DEh
ProgCurrent          equ  85E8h
userMenuSA           equ  85FEh
ioPrompt             equ  865Fh
dImageWidth          equ  8660h
RectFillPHeight      equ  8660h
RectFillPWidth       equ  8661h
RectFillPattern      equ  8662h
ioFlag               equ  8670h
sndRecState          equ  8672h
ioErrState           equ  8673h
header               equ  8674h
ioData               equ  867Dh
ioNewData            equ  8689h
bakHeader            equ  868Bh
penCol               equ  86D7h
penRow               equ  86D8h
rclQueue             equ  86D9h
rclQueueEnd          equ  86DBh
errNo                equ  86DDh
errSP                equ  86DEh
errOffset            equ  86E0h
saveSScreen          equ  86ECh
bstCounter           equ  89EEh
flags                equ  89F0h
statVars             equ  8A3Ah
anovaf_vars          equ  8C17h
infVars              equ  8C4Dh
curGStyle            equ  8D17h
curGY                equ  8D18h
curGX                equ  8D19h
curGY2               equ  8D1Ah
curGX2               equ  8D1Bh
freeSaveY            equ  8D1Ch
freeSaveX            equ  8D1Dh
XOffset              equ  8DA1h
YOffset              equ  8DA2h
lcdTallP             equ  8DA3h
pixWideP             equ  8DA4h
pixWide_m_1          equ  8DA5h
pixWide_m_2          equ  8DA6h
lastEntryPTR         equ  8DA7h
lastEntryStk         equ  8DA9h
numLastEntries       equ  8E29h
currLastEntry        equ  8E2Ah
curInc               equ  8E67h
uXmin                equ  8E7Eh
uXmax                equ  8E87h
uXscl                equ  8E90h
uYmin                equ  8E99h
uYmax                equ  8EA2h
uYscl                equ  8EABh
uThetMin             equ  8EB4h
uThetMax             equ  8EBDh
uThetStep            equ  8EC6h
uTmin                equ  8ECFh
uTmax                equ  8ED8h
uTStep               equ  8EE1h
uPlotStart           equ  8EEAh
unMax                equ  8EF3h
uu0                  equ  8EFCh
uv0                  equ  8F05h
unMin                equ  8F0Eh
uu02                 equ  8F17h
uv02                 equ  8F20h
uw0                  equ  8F29h
uPlotStep            equ  8F32h
uXres                equ  8F3Bh
uw02                 equ  8F44h
Xmin                 equ  8F50h
Xmax                 equ  8F59h
Xscl                 equ  8F62h
Ymin                 equ  8F6Bh
Ymax                 equ  8F74h
Yscl                 equ  8F7Dh
ThetaMin             equ  8F86h
ThetaMax             equ  8F8Fh
ThetaStep            equ  8F98h
TminPar              equ  8FA1h
TmaxPar              equ  8FAAh
Tstep                equ  8FB3h
PlotStart            equ  8FBCh
nMax                 equ  8FC5h
u0                   equ  8FCEh
v0                   equ  8FD7h
nMin                 equ  8FE0h
u02                  equ  8FE9h
v02                  equ  8FF2h
w0                   equ  8FFBh
PlotStep             equ  9004h
XresO                equ  900Dh
w02                  equ  9016h
un1                  equ  901Fh
un2                  equ  9028h
vn1                  equ  9031h
vn2                  equ  903Ah
wn1                  equ  9043h
wn2                  equ  904Ch
fin_N                equ  9055h
fin_I                equ  905Eh
fin_PV               equ  9067h
fin_PMT              equ  9070h
fin_FV               equ  9079h
fin_PY               equ  9082h
fin_CY               equ  908Bh
cal_N                equ  9094h
cal_I                equ  909Dh
cal_PV               equ  90A6h
cal_PMT              equ  90AFh
cal_FV               equ  90B8h
cal_PY               equ  90C1h
smallEditRAM         equ  90D3h
XFact                equ  913Fh
YFact                equ  9148h
Xres_int             equ  9151h
deltaX               equ  9152h
deltaY               equ  915Bh
shortX               equ  9164h
shortY               equ  916Dh
lower                equ  9176h
upper                equ  917Fh
XOutSym              equ  918Ch
XOutDat              equ  918Eh
YOutSym              equ  9190h
YOutDat              equ  9192h
inputSym             equ  9194h
inputDat             equ  9196h
prevData             equ  9198h
TblMin               equ  92B3h
TblStep              equ  92BCh
P1Type		     equ  92C9h
SavX1List	     equ  92CAh
SavY1List	     equ  92CFh
SavF1List	     equ  92D4h
P1FrqOnOff	     equ  92D9h
P2Type		     equ  92DAh
SavX2List	     equ  92DBh
SavY2List	     equ  92E0h
SavF2List	     equ  92E5h
P2FrqOnOff	     equ  92EAh
P3Type		     equ  92EBh	
SavX3List	     equ  92ECh
SavY3List	     equ  92F1h
SavF3List	     equ  92F6h
P3FrqOnOff	     equ  92FBh
plotSScreen          equ  9340h
seed1                equ  9640h
seed2                equ  9649h
cmdShadow            equ  966Eh
cmdShadCur           equ  96EEh
cmdShadAlph          equ  96F0h
cmdShadIns           equ  96F1h
cmdCursor            equ  96F2h
editTop              equ  96F4h
editCursor           equ  96F6h
editTail             equ  96F8h
editBtm              equ  96FAh
editSym              equ  9706h
editDat              equ  9708h
winTop               equ  97A5h
winBtm               equ  97A6h
winLeftEdge          equ  97A7h
winLeft              equ  97A8h
winAbove             equ  97AAh
winRow               equ  97ACh
winCol               equ  97AEh
fmtDigits            equ  97B0h
fmtString            equ  97B1h
fmtConv              equ  97F2h
fmtLeft              equ  9804h
fmtIndex             equ  9806h
fmtMatSym            equ  9808h
fmtMatMem            equ  980Ah
EQS                  equ  980Ch
tSymPtr1             equ  9818h
tSymPtr2             equ  981Ah
chkDelPtr3           equ  981Ch
chkDelPtr4           equ  981Eh
tempMem              equ  9820h
fpBase               equ  9822h
FPS                  equ  9824h
OPBase               equ  9826h
OPS                  equ  9828h
pTempCnt             equ  982Ah
cleanTmp             equ  982Ch
pTemp                equ  982Eh
progPtr              equ  9830h
newDataPtr           equ  9832h
pagedCount           equ  9834h
pagedPN              equ  9835h
pagedGetPtr          equ  9836h
pagedPutPtr          equ  9838h
pagedBuf             equ  983Ah
appErr1              equ  984Dh
appErr2              equ  985Ah
flashByte1           equ  9867h
flashByte2           equ  9868h
freeArcBlock         equ  9869h
arcPage              equ  986Bh
arcPtr               equ  986Ch
appRawKeyHandle      equ  9870h
appBackUpScreen      equ  9872h
customHeight         equ  9B72h
localLanguage        equ  9B73h
cursorHookPtr        equ  9B7Ch
rawKeyHookPtr        equ  9B84h
getKeyHookPtr        equ  9B88h
fontHookPtr          equ  9B9Ch
restartClr           equ  9BD0h
localTokStr          equ  9D65h
keyForStr            equ  9D76h
keyToStrRam          equ  9D77h
sedMonSp             equ  9D88h
bpSave               equ  9D8Ah
userMem              equ  9D95h
symTable             equ  0FE66h

;SYMBOLIC_DEBUG_END
;
;======================================================================
;		Language localization equates
;======================================================================
LANG_NEUTRAL                       equ   00h
LANG_DANISH                        equ   06h
LANG_DUTCH                         equ   13h
LANG_ENGLISH                       equ   09h
LANG_FINNISH                       equ   0bh
LANG_FRENCH                        equ   0ch
LANG_GERMAN                        equ   07h
LANG_HUNGARIAN                     equ   0eh
LANG_ITALIAN                       equ   10h
LANG_NORWEGIAN                     equ   14h
LANG_POLISH                        equ   15h
LANG_PORTUGUESE                    equ   16h
LANG_SPANISH                       equ   0ah
LANG_SWEDISH                       equ   1dh

SUBLANG_NEUTRAL                    equ   00h   ; language neutral
SUBLANG_DUTCH                    equ   01h   ; Dutch
SUBLANG_ENGLISH_US               equ   01h   ; English (USA)
SUBLANG_FRENCH                   equ   01h   ; French
SUBLANG_GERMAN                   equ   01h   ; German
SUBLANG_ITALIAN                  equ   01h   ; Italian
SUBLANG_NORWEGIAN_BOKMAL         equ   01h   ; Norwegian (Bokmal)
SUBLANG_PORTUGUESE               equ   02h   ; Portuguese
SUBLANG_SPANISH                  equ   01h   ; Spanish (Castilian)

;======================================================================
;           System and State Flags
;======================================================================
;

trigFlags	equ  0		;Trigonometry mode settings
trigDeg 	equ    2	; 1=degrees, 0=radians

kbdFlags	equ  0		;Keyboard scan
kbdSCR		equ    3	; 1=scan code ready
kbdKeyPress	equ    4	; 1=key has been pressed

doneFlags	equ  0		;display "Done"
donePrgm	equ    5	; 1=display "Done" after prgm

ioDelFlag	equ  0
inDelete	equ    0	;1 = DELETE SCREEN 

;----------------------------------------------------------------------
editFlags	equ  1
editOpen	equ    2	; 1=edit buffer is open

monFlags	equ  1		;monitor flags
monAbandon	equ    4	; 1=don't start any long process
				; in put away (#715)
;----------------------------------------------------------------------
plotFlags	equ  2		;plot generation flags
plotLoc 	equ    1	; 0=bkup & display, 1=display only
plotDisp	equ    2	; 1=plot is in display, 0=text in display


grfModeFlags	equ  2		;graph mode settings
grfFuncM	equ    4	; 1=function graph
grfPolarM	equ    5	; 1=polar graph
grfParamM	equ    6	; 1=parametric graph
grfRecurM       equ    7        ; 1=RECURSION graph

graphFlags	equ  3
graphDraw	equ    0	; 0=graph is valid, 1=redraw graph

grfDBFlags	equ  4
grfDot		equ    0	; 0=line, 1=dot
grfSimul	equ    1	; 0=sequential, 1=simultaneous
grfGrid 	equ    2	; 0=no grid, 1=grid
grfPolar	equ    3	; 0=rectangular, 1=polar coordinates
grfNoCoord	equ    4	; 0=display coordinates, 1=off
grfNoAxis	equ    5	; 0=axis, 1=no axis
grfLabel	equ    6	; 0=off, 1=axis label

textFlags	equ  5		;Text output flags
textEraseBelow	equ    1	; 1=erase line below small char
textScrolled	equ    2	; 1=screen scrolled
textInverse	equ    3	; 1=display inverse bit-map
textInsMode	equ    4	; 0=overstrike, 1=insert mode

ParsFlag2	equ  7		;PARSER flags
numOP1		equ    0	; 1=RESULT IN OP1, 0=NO RESULT

newDispF        equ  8		;Derivative mode flags
preClrForMode   equ    0	; 1=HELP BLINK ON MODE SCREEN

apdFlags	equ  8		;Automatic power-down
apdAble 	equ    2	; 1=APD enabled
apdRunning	equ    3	; 1=APD clock running


web_err_mask    equ  60h

onFlags 	equ  9		;on key flags
onRunning	equ  3		; 1 = calculator is running
onInterrupt	equ    4	; 1=on key interrupt request

statFlags	equ  9		;statistics flags
statsValid	equ    6	; 1=stats are valid
statANSDISP	equ  7		; 1=display stat results


fmtFlags	equ  10		;numeric format flags
fmtExponent	equ    0	; 1=show exponent, 0=no exponent
fmtEng		equ    1	; 1=engineering notion, 0=scientific
fmtHex		equ    2	; 1=hexadecimal
fmtOct		equ    3	; 1=octal
fmtBin		equ    4	; 1=binary
;
numMode         equ  10
fmtReal         equ    5
fmtRect         equ    6
fmtPolar        equ    7

realMode        equ    5
rectMode        equ    6
polarMode       equ    7
;					;   if Hex and Oct both = 1
;					;   then Bin=0 means >Frac
;					;	 Bin=1 means >DMS
fmtBaseMask     equ  00011100B		; mask to base flags
fmtBaseShift    equ  2			; offset to base flags
;
;       CHECK IF THESE ARE USED BY NUMFORM,
;
;               equ  6
;               equ  7

fmtOverride	equ  11		;copy of fmtFlags with conversion override

fmtEditFlags	equ  12		;numeric editing flags
fmtEdit 	equ    0	; 1=format number for editing

curFlags	equ  12		;Cursor
curAble 	equ    2	; 1=cursor flash is enabled
curOn		equ    3	; 1=cursor is showing
curLock 	equ    4	; 1=cursor is locked off

appFlags	equ  13		;application flags
appWantIntrpt	equ    0		; 1=want ON key interrupts
appTextSave	equ    1		; 1=save characters in textShadow
appAutoScroll	equ    2		; 1=auto-scroll text on last line
appMenus	equ    3		; 1=process keys that bring up menus
					; 0=check Lock menu flag
appLockMenus	equ    4		; 1=ignore menu keys
					; 0=switch to home screen and bring up menu
appCurGraphic	equ    5		; 1=graphic cursor
appCurWord	equ    6		; 1=text cursor covers entire word
appExit 	equ    7		; 1=application handles [EXIT] key itself

appWantIntrptF	equ	1<<appWantIntrpt
appTextSaveF	equ	1<<appTextSave
appAutoScrollF	equ	1<<appAutoScroll
appMenusF	equ	1<<appMenus
appLockMenusF	equ	1<<appLockMenus
appCurGraphicF	equ	1<<appCurGraphic
appCurWordF	equ	1<<appCurWord
appExitF	equ	1<<appExit


seqFlags        equ  15              ; Sequential Graph flags
webMode         equ    0             ; 0 = NORMAL SEQ MODE, 1 = WEB MODE
webVert         equ    1             ;
sequv           equ    2             ; U vs V
seqvw           equ    3             ; V vs W
sequw           equ    4             ; U vs W


promptFlags	equ  17		;prompt line flags
promptEdit	equ    0	; 1=editing in prompt buffer

indicFlags	equ  18		;Indicator flags
indicRun	equ    0	; 1=run indicator ON
indicInUse	equ    1	; indicator save area in use=1, free=0
indicOnly	equ    2	; interrupt handler only checks run indicator

shiftFlags	equ  18		;[2nd] and [ALPHA] flags
shift2nd	equ    3	; 1=[2nd] has been pressed
shiftAlpha	equ    4	; 1=[ALPHA] has been pressed
shiftLwrAlph	equ    5	; 1=lower case, 0=upper case
shiftALock	equ    6	; 1=alpha lock has been pressed
shiftKeepAlph	equ    7	; 1=cannot cancel alpha shift


tblFlags        equ  19		;table flags.
autoFill        equ    4	; 1=prompt, 0=fillAuto
autoCalc        equ    5	; 1=prompt, 0=CalcAuto
reTable         equ    6	; 0=table is ok, 1=must recompute table.

sGrFlags	equ  20
grfSplit        equ    0	; 1=Split Graph, 0=Normal
vertSplit       equ    1	; 1=Vertical (left-right) Split
grfSChanged     equ    2	; 1=Graph just changed Split <-> normal
grfSplitOverride equ   3	; 1 = ignore graph split flag if set
write_on_graph  equ    4	; 1 = TEXT OR EQU WRITING TO GRAPH SCREEN
g_style_active  equ    5	; 1 = GRAPH STYLES ARE ENABLED, USE THEM
cmp_mod_box     equ    6	; 1 = DOING MOD BOX PLOT COMPUTATION
textWrite       equ    7	; 1 = Small font writes to buffer
							; 0 = Small font writes to display
;

newIndicFlags   equ  21
extraIndic      equ    0
saIndic         equ    1

newFlags2	    equ  22
noRestores	    equ  5

smartFlags      equ  23
;
;----------------------------------------------------------------------
; Note: Fix these Equates if smartFlags are moved                            
;----------------------------------------------------------------------
;
smarter_mask    equ  3
smarter_test    equ  1
smartGraph      equ  0
smartGraph_inv  equ  1                                                 

more_Flags	equ 26

No_Del_Stat	equ 2
;----------------------------------------------------------------------
;           Available for ASM programming
;----------------------------------------------------------------------

asm_Flag1       equ  33          ; ASM CODING
asm_Flag2       equ  34          ; ASM CODING
asm_Flag3       equ  35          ; ASM CODING

;----------------------------------------------------------------------
;
getSendFlg	equ  36
comFailed	equ  1	      ; 1 = Get/Send Communication Failed
;
appLwrCaseFlag  equ  36
lwrCaseActive   equ  3
;
apiFlg3		equ  42 
;
apiFlg4		equ  43
fullScrnDraw	equ    2	; DRAW INTO LAST ROW/COL OF SCREEN

groupFlags	equ  38
inGroup		equ  1		;1 = IN GROUP CONTEXT


xapFlag0        equ  46      ; external app flags
xapFlag1        equ  47
xapFlag2        equ  48
xapFlag3        equ  49

fontFlags	equ  50
fracDrawLFont	equ    2
fracTallLFont	equ    3
customFont	equ    7  

plotFlag3	equ  60  
bufferOnly	equ    0
useFastCirc	equ    4

;
varTypeMask	equ  1FH             
varGraphRef	equ    6

;
;======================================================================
;           Character Font equates  
;======================================================================
;
; Large font equates
;
LrecurN         equ     001h
LrecurU 	equ	002h
LrecurV 	equ	003h
LrecurW		equ	004h
Lconvert	equ	005h
LsqUp		equ	006h
LsqDown		equ	007h
Lintegral	equ	008h
Lcross		equ	009h
LboxIcon 	equ     00Ah
LcrossIcon 	equ     00Bh
LdotIcon 	equ     00Ch
LsubT   	equ     00Dh		;small capital T for parametric mode.
LcubeR  	equ     00Eh		;slightly different 3 for cubed root.
LhexF		equ	00Fh
Lroot		equ	010h
Linverse	equ	011h
Lsquare		equ	012h
Langle		equ	013h
Ldegree		equ	014h
Lradian		equ	015h
Ltranspose	equ	016h
LLE		equ	017h
LNE		equ	018h
LGE		equ	019h
Lneg		equ	01Ah
Lexponent	equ	01Bh
Lstore		equ	01Ch
Lten		equ	01Dh
LupArrow	equ	01Eh
LdownArrow	equ	01Fh
Lspace		equ	020h
Lexclam		equ	021h
Lquote		equ	022h
Lpound		equ	023h
Lfourth		equ     024h
Lpercent	equ	025h
Lampersand	equ	026h
Lapostrophe	equ	027h
LlParen		equ	028h
LrParen		equ	029h
Lasterisk	equ	02Ah
LplusSign	equ	02Bh
Lcomma		equ	02Ch
Ldash		equ	02Dh
Lperiod		equ	02Eh
Lslash		equ	02Fh
L0		equ	030h
L1		equ	031h
L2		equ	032h
L3		equ	033h
L4		equ	034h
L5		equ	035h
L6		equ	036h
L7		equ	037h
L8		equ	038h
L9		equ	039h
Lcolon		equ	03Ah
Lsemicolon	equ	03Bh
LLT		equ	03Ch
LEQ		equ	03Dh
LGT		equ	03Eh
Lquestion	equ	03Fh
LatSign		equ	040h
LcapA	equ	041h
LcapB	equ	042h
LcapC	equ	043h
LcapD	equ	044h
LcapE	equ	045h
LcapF	equ	046h
LcapG	equ	047h
LcapH	equ	048h
LcapI	equ	049h
LcapJ	equ	04Ah
LcapK	equ	04Bh
LcapL	equ	04Ch
LcapM	equ	04Dh
LcapN	equ	04Eh
LcapO	equ	04Fh
LcapP	equ	050h
LcapQ	equ	051h
LcapR	equ	052h
LcapS	equ	053h
LcapT	equ	054h
LcapU	equ	055h
LcapV	equ	056h
LcapW	equ	057h
LcapX	equ	058h
LcapY	equ	059h
LcapZ	equ	05Ah
Ltheta  equ     05Bh
Lbackslash	equ	05Ch
LrBrack		equ	05Dh
Lcaret		equ	05Eh
Lunderscore	equ	05Fh
Lbackquote	equ	060h
La	equ	061h
Lb	equ	062h
Lc	equ	063h
Ld	equ	064h
Le	equ	065h
Lf	equ	066h
Lg	equ	067h
Lh	equ	068h
Li	equ	069h
Lj	equ	06Ah
Lk	equ	06Bh
Ll	equ	06Ch
Lm	equ	06Dh
Ln	equ	06Eh
Lo	equ	06Fh
Lp	equ	070h
Lq	equ	071h
Lr	equ	072h
Ls	equ	073h
Lt	equ	074h
Lu	equ	075h
Lv	equ	076h
Lw	equ	077h
Lx	equ	078h
Ly	equ	079h
Lz	equ	07Ah
LlBrace	equ	07Bh
Lbar	equ	07Ch
LrBrace	equ	07Dh
Ltilde	equ	07Eh
LinvEQ	equ	07Fh
Lsub0	equ	080h
Lsub1	equ	081h
Lsub2	equ	082h
Lsub3	equ	083h
Lsub4	equ	084h
Lsub5	equ	085h
Lsub6	equ	086h
Lsub7	equ	087h
Lsub8	equ	088h
Lsub9	equ	089h
LcapAAcute	equ	08Ah
LcapAGrave	equ	08Bh
LcapACaret	equ	08Ch
LcapADier	equ	08Dh
LaAcute		equ	08Eh
LaGrave		equ	08Fh
LaCaret		equ	090h
LaDier		equ	091h
LcapEAcute	equ	092h
LcapEGrave	equ	093h
LcapECaret	equ	094h
LcapEDier	equ	095h
LeAcute		equ	096h
LeGrave		equ	097h
LeCaret		equ	098h
LeDier		equ	099h
LcapIAcute	equ	09Ah
LcapIGrave	equ	09Bh
LcapICaret	equ	09Ch
LcapIDier	equ	09Dh
LiAcute		equ	09Eh
LiGrave		equ	09Fh
LiCaret		equ	0A0h
LiDier		equ	0A1h
LcapOAcute	equ	0A2h
LcapOGrave	equ	0A3h
LcapOCaret	equ	0A4h
LcapODier	equ	0A5h
LoAcute		equ	0A6h
LoGrave		equ	0A7h
LoCaret		equ	0A8h
LoDier		equ	0A9h
LcapUAcute	equ	0AAh
LcapUGrave	equ	0ABh
LcapUCaret	equ	0ACh
LcapUDier	equ	0ADh
LuAcute		equ	0AEh
LuGrave		equ	0AFh
LuCaret		equ	0B0h
LuDier		equ	0B1h
LcapCCed	equ	0B2h
LcCed		equ	0B3h
LcapNTilde	equ	0B4h
LnTilde		equ	0B5h
Laccent		equ	0B6h
Lgrave		equ	0B7h
Ldieresis	equ	0B8h
LquesDown	equ	0B9h
LexclamDown	equ	0BAh
Lalpha		equ	0BBh
Lbeta		equ	0BCh
Lgamma		equ	0BDh
LcapDelta	equ	0BEh
Ldelta		equ	0BFh
Lepsilon	equ	0C0h
LlBrack 	equ     0C1h
Llambda		equ	0C2h
Lmu		equ	0C3h
Lpi		equ	0C4h
Lrho		equ	0C5h
LcapSigma	equ	0C6h
Lsigma		equ	0C7h
Ltau		equ	0C8h
Lphi		equ	0C9h
LcapOmega	equ	0CAh
LxMean		equ	0CBh
LyMean		equ	0CCh
LsupX		equ	0CDh
Lellipsis	equ	0CEh
Lleft		equ	0CFh
Lblock		equ	0D0h
Lper		equ	0D1h
Lhyphen		equ	0D2h
Larea		equ	0D3h
Ltemp		equ	0D4h
Lcube		equ	0D5h
Lenter		equ	0D6h
LimagI		equ	0D7h
Lphat		equ	0D8h
Lchi		equ	0D9h
LstatF		equ     0DAh
Llne		equ	0DBh
LlistL		equ	0DCh
LfinanN 	equ	0DDh
L2_r_paren 	equ	0DEh
LblockArrow     equ     0DFh
LcurO   	equ     0E0h
LcurO2  	equ     0E1h
LcurOcapA       equ     0E2h
LcurOa  	equ     0E3h
LcurI   	equ     0E4h
LcurI2  	equ     0E5h
LcurIcapA       equ     0E6h
LcurIa  	equ     0E7h
LGline          equ     0E8h    ; = 0
LGthick         equ     0E9h    ; = 1
LGabove         equ     0EAh    ; = 2
LGbelow         equ     0EBh    ; = 3
LGpath          equ     0ECh    ; = 4
LGanimate       equ     0EDh    ; = 5
LGdot           equ     0EEh    ; = 6
LUpBlk          equ     0EFh    ;Up arrow and Block in solver
LDnBlk          equ     0F0h    ;Down arrow and Block in solver
LcurFull        equ     0F1h    ;note: must be last char (PutMap checks)

;Small font equates
;
SrecurN         equ     001h
SrecurU		equ	002h
SrecurV		equ	003h
SrecurW		equ	004h
Sconvert	equ	005h
SFourSpaces 	equ 	006h
SsqDown		equ	007h
Sintegral	equ	008h
Scross		equ	009h
SboxIcon 	equ	00Ah
ScrossIcon 	equ	00Bh
SdotIcon 	equ    	00Ch
SsubT   	equ     00Dh
ScubeR  	equ     00Eh
ShexF		equ	00Fh
Sroot		equ	010h
Sinverse	equ	011h
Ssquare		equ	012h
Sangle		equ	013h
Sdegree		equ	014h
Sradian		equ	015h
Stranspose	equ	016h
SLE		equ	017h
SNE		equ	018h
SGE		equ	019h
Sneg		equ	01Ah
Sexponent	equ	01Bh
Sstore		equ	01Ch
Sten		equ	01Dh
SupArrow	equ	01Eh
SdownArrow	equ	01Fh
Sspace		equ	020h
Sexclam		equ	021h
Squote		equ	022h
Spound		equ	023h
Sdollar		equ	024h
Spercent	equ	025h
Sampersand	equ	026h
Sapostrophe	equ	027h
SlParen		equ	028h
SrParen		equ	029h
Sasterisk	equ	02Ah
SplusSign	equ	02Bh
Scomma		equ	02Ch
Sdash		equ	02Dh
Speriod		equ	02Eh
Sslash		equ	02Fh
S0		equ	030h
S1		equ	031h
S2		equ	032h
S3		equ	033h
S4		equ	034h
S5		equ	035h
S6		equ	036h
S7		equ	037h
S8		equ	038h
S9		equ	039h
Scolon		equ	03Ah
Ssemicolon	equ	03Bh
SLT		equ	03Ch
SEQ		equ	03Dh
SGT		equ	03Eh
Squestion	equ	03Fh
SatSign		equ	040h
ScapA		equ	041h
ScapB		equ	042h
ScapC		equ	043h
ScapD		equ	044h
ScapE		equ	045h
ScapF		equ	046h
ScapG		equ	047h
ScapH		equ	048h
ScapI		equ	049h
ScapJ		equ	04Ah
ScapK		equ	04Bh
ScapL		equ	04Ch
ScapM		equ	04Dh
ScapN		equ	04Eh
ScapO		equ	04Fh
ScapP		equ	050h
ScapQ		equ	051h
ScapR		equ	052h
ScapS		equ	053h
ScapT		equ	054h
ScapU		equ	055h
ScapV		equ	056h
ScapW		equ	057h
ScapX		equ	058h
ScapY		equ	059h
ScapZ		equ	05Ah
Stheta  	equ     05Bh
Sbackslash	equ	05Ch
SrBrack		equ	05Dh
Scaret		equ	05Eh
Sunderscore	equ	05Fh
Sbackquote	equ	060h
SmallA		equ	061h
SmallB		equ	062h
SmallC		equ	063h
SmallD		equ	064h
SmallE		equ	065h
SmallF		equ	066h
SmallG		equ	067h
SmallH		equ	068h
SmallI		equ	069h
SmallJ		equ	06Ah
SmallK		equ	06Bh
SmallL		equ	06Ch
SmallM		equ	06Dh
SmallN		equ	06Eh
SmallO		equ	06Fh
SmallP		equ	070h
SmallQ		equ	071h
SmallR		equ	072h
SmallS		equ	073h
SmallT		equ	074h
SmallU		equ	075h
SmallV		equ	076h
SmallW		equ	077h
SmallX		equ	078h
SmallY		equ	079h
SmallZ		equ	07Ah
SlBrace		equ	07Bh
Sbar		equ	07Ch
SrBrace		equ	07Dh
Stilde		equ	07Eh
SinvEQ		equ	07Fh
Ssub0		equ	080h
Ssub1		equ	081h
Ssub2		equ	082h
Ssub3		equ	083h
Ssub4		equ	084h
Ssub5		equ	085h
Ssub6		equ	086h
Ssub7		equ	087h
Ssub8		equ	088h
Ssub9		equ	089h
ScapAAcute	equ	08Ah
ScapAGrave	equ	08Bh
ScapACaret	equ	08Ch
ScapADier	equ	08Dh
SaAcute		equ	08Eh
SaGrave		equ	08Fh
SaCaret		equ	090h
SaDier		equ	091h
ScapEGrave	equ	092h
ScapEAcute	equ	093h
ScapECaret	equ	094h
ScapEDier	equ	095h
SeAcute		equ	096h
SeGrave		equ	097h
SeCaret		equ	098h
SeDier		equ	099h
ScapIAcute	equ	09Ah
ScapIGrave	equ	09Bh
ScapICaret	equ	09Ch
ScapIDier	equ	09Dh
SiAcute		equ	09Eh
SiGrave		equ	09Fh
SiCaret		equ	0A0h
SiDier		equ	0A1h
ScapOAcute	equ	0A2h
ScapOGrave	equ	0A3h
ScapOCaret	equ	0A4h
ScapODier	equ	0A5h
SoAcute		equ	0A6h
SoGrave		equ	0A7h
SoCaret		equ	0A8h
SoDier		equ	0A9h
ScapUAcute	equ	0AAh
ScapUGrave	equ	0ABh
ScapUCaret	equ	0ACh
ScapUDier	equ	0ADh
SuAcute		equ	0AEh
SuGrave		equ	0AFh
SuCaret		equ	0B0h
SuDier		equ	0B1h
ScapCCed	equ	0B2h
ScCed		equ	0B3h
ScapNTilde	equ	0B4h
SnTilde		equ	0B5h
Saccent		equ	0B6h
Sgrave		equ	0B7h
Sdieresis	equ	0B8h
SquesDown	equ	0B9h
SexclamDown	equ	0BAh
Salpha		equ	0BBh
Sbeta		equ	0BCh
Sgamma		equ	0BDh
ScapDelta	equ	0BEh
Sdelta		equ	0BFh
Sepsilon	equ	0C0h
SlBrack 	equ     0C1h
Slambda		equ	0C2h
Smu		equ	0C3h
Spi		equ	0C4h
Srho		equ	0C5h
ScapSigma	equ	0C6h
Ssigma		equ	0C7h
Stau		equ	0C8h
Sphi		equ	0C9h
ScapOmega	equ	0CAh
SxMean		equ	0CBh
SyMean		equ	0CCh
SsupX		equ	0CDh
Sellipsis	equ	0CEh
Sleft		equ	0CFh
Sblock		equ	0D0h
Sper		equ	0D1h
Shyphen		equ	0D2h
Sarea		equ	0D3h
Stemp		equ	0D4h
Scube		equ	0D5h
Senter		equ	0D6h
SimagI		equ	0D7h
Sphat		equ	0D8h
Schi		equ	0D9h
SstatF  	equ     0DAh
Slne		equ	0DBh
SlistL		equ	0DCh
SfinanN 	equ	0DDh
S2_r_paren 	equ	0DEh
SnarrowCapE     equ     0DFh
SListLock 	equ	0E0h
Sscatter1 	equ	0E1h                    
Sscatter2 	equ	0E2h                    
Sxyline1  	equ	0E3h                    
Sxyline2  	equ	0E4h
Sboxplot1 	equ	0E5h
Sboxplot2 	equ	0E6h
Shist1    	equ	0E7h
Shist2    	equ	0E8h
SmodBox1  	equ	0E9h
SmodBox2  	equ	0EAh
Snormal1  	equ	0EBh
Snormal2  	equ	0ECh
;
;======================================================================
;           Keypress Equates
;======================================================================
;           Keyboard key names
;
;
kRight          equ       001h
kLeft           equ       002h
kUp             equ       003h
kDown           equ       004h
kEnter          equ       005h
kAlphaEnter     equ       006h
kAlphaUp        equ       007h
kAlphaDown      equ       008h
kClear          equ       009h
kDel            equ       00Ah
kIns            equ       00Bh
kRecall         equ       00Ch
kLastEnt        equ       00Dh
kBOL            equ       00Eh
kEOL            equ       00Fh
;
kSelAll         equ       010h
kUnselAll       equ       011h
kLtoTI82        equ       012h
kBackup         equ       013h
kRecieve        equ       014h
kLnkQuit        equ       015h
kTrans          equ       016h
kRename         equ       017h
kOverw          equ       018h
kOmit           equ       019h
kCont           equ       01Ah
kSendID         equ       01Bh
kSendSW         equ       01Ch
kYes            equ       01Dh
kNoWay          equ       01Eh
kvSendType      equ       01Fh
kOverWAll       equ       020h
;
kNo             equ       025h
kKReset         equ       026h
kApp            equ       027h
;
kDoug           equ       028h
kListflag       equ       029h
menuStart       equ       02Bh
;
kAreYouSure     equ       02Bh
kAppsMenu       equ       02Ch
kPrgm           equ       02Dh
kZoom           equ       02Eh
kDraw           equ       02Fh
kSPlot          equ       030h
kStat           equ       031h
kMath           equ       032h
kTest           equ       033h
kChar           equ       034h
kVars           equ       035h
kMem            equ       036h
kMatrix         equ       037h
kDist           equ       038h
kAngle          equ       039h
kList           equ       03Ah
kCalc           equ       03Bh
kFin            equ       03Ch
;
menuEnd         equ       kFin
;
kCatalog        equ       03Eh
kInputDone      equ       03Fh
kOff            equ       kInputDone
;
kQuit           equ       040h
appStart        equ       kQuit

;
kLinkIO         equ       041h
kMatrixEd       equ       042h
kStatEd         equ       043h
kGraph          equ       044h
kMode           equ       045h
kPrgmEd         equ       046h        ; PROGRAM EDIT
kPrgmCr         equ       047h        ; PROGRAM CREATE
kWindow         equ       048h        ; RANGE EDITOR
kYequ           equ       049h        ; EQUATION EDITOR
kTable          equ       04Ah        ; TABLE EDITOR
kTblSet         equ       04Bh        ; TABLE SET
kChkRAM         equ       04Ch        ; CHECK RAM
kDelMem         equ       04Dh        ; DELETE MEM
kResetMem       equ       04Eh        ; RESET MEM
kResetDef	equ	  04Fh        ; RESET DEFAULT
kPrgmInput      equ       050h        ; PROGRAM INPUT
kZFactEd        equ       051h        ; ZOOM FACTOR EDITOR
kError          equ       052h        ; ERROR
kSolveTVM       equ       053h        ; TVM SOLVER
kSolveRoot	equ	  054h        ; SOLVE EDITOR
kStatP          equ       055h        ; stat plot
kInfStat 	equ	  056h	      ; Inferential Statistic
kFormat         equ       057h        ; FORMAT
kExtApps        equ       058h        ; External Applications.     NEW
kNewApps        equ       059h        ; New Apps for Cerberus.
;
append          equ       kNewApps
;
echoStart1      equ       05Ah
;
kTrace          equ       05Ah
kZFit           equ       05Bh
kZIn            equ       05Ch
kZOut           equ       05Dh
kZPrev          equ       05Eh
kBox            equ       05Fh
kDecml          equ       060h
kSetZm          equ       061h
kSquar          equ       062h
kStd            equ       063h
kTrig           equ       064h
kUsrZm          equ       065h
kZSto           equ       066h
kZInt           equ       067h
kZStat          equ       068h
;
echoStart2      equ       069h
;
kSelect         equ       069h
kCircl          equ       06Ah
kClDrw          equ       06Bh
kLine           equ       06Ch
kPen            equ       06Dh
kPtChg          equ       06Eh
kPtOff          equ       06Fh
kPtOn           equ       070h
kVert           equ       071h
kHoriz          equ       072h
kText           equ       073h
kTanLn          equ       074h
;
kEval           equ       075h
kInters         equ       076h
kDYDX           equ       077h
kFnIntg         equ       078h
kRootG          equ       079h
kDYDT           equ       07Ah
kDXDT           equ       07Bh
kDRDo           equ       07Ch
KGFMin          equ       07Dh
KGFMax          equ       07Eh
;
;
EchoStart       equ       07Fh
;
kListName       equ       07Fh
kAdd            equ       080h
kSub            equ       081h
kMul            equ       082h
kDiv            equ       083h
kExpon          equ       084h
kLParen         equ       085h
kRParen         equ       086h
kLBrack         equ       087h
kRBrack         equ       088h
kShade          equ       089h
kStore          equ       08Ah
kComma          equ       08Bh
kChs            equ       08Ch
kDecPnt         equ       08Dh
k0              equ       08Eh
k1              equ       08Fh
k2              equ       090h
k3              equ       091h
k4              equ       092h
k5              equ       093h
k6              equ       094h
k7              equ       095h
k8              equ       096h
k9              equ       097h
kEE             equ       098h
kSpace          equ       099h
kCapA           equ       09Ah
kCapB           equ       09Bh
kCapC           equ       09Ch
kCapD           equ       09Dh
kCapE           equ       09Eh
kCapF           equ       09Fh
kCapG           equ       0A0h
kCapH           equ       0A1h
kCapI           equ       0A2h
kCapJ           equ       0A3h
kCapK           equ       0A4h
kCapL           equ       0A5h
kCapM           equ       0A6h
kCapN           equ       0A7h
kCapO           equ       0A8h
kCapP           equ       0A9h
kCapQ           equ       0AAh
kCapR           equ       0ABh
kCapS           equ       0ACh
kCapT           equ       0ADh
kCapU           equ       0AEh
kCapV           equ       0AFh
kCapW           equ       0B0h
kCapX           equ       0B1h
kCapY           equ       0B2h
kCapZ           equ       0B3h
kVarx           equ       0B4h
kPi             equ       0B5h
kInv            equ       0B6h
kSin            equ       0B7h
kASin           equ       0B8h
kCos            equ       0B9h
kACos           equ       0BAh
kTan            equ       0BBh
kATan           equ       0BCh
kSquare         equ       0BDh
kSqrt           equ       0BEh
kLn             equ       0BFh
kExp            equ       0C0h
kLog            equ       0C1h
kALog           equ       0C2h
kToABC          equ       0C3h
;
kClrTbl         equ       0C4h
;
kAns            equ       0C5h
kColon          equ       0C6h
;
kNDeriv         equ       0C7h
kFnInt          equ       0C8h
kRoot           equ       0C9h
;
kQuest          equ       0CAh
kQuote          equ       0CBh
kTheta          equ       0CCh
kIf             equ       0CDh
kThen           equ       0CEh
kElse           equ       0CFh
kFor            equ       0D0h
kWhile          equ       0D1h
kRepeat         equ       0D2h
kEnd            equ       0D3h
kPause          equ       0D4h
kLbl            equ       0D5h
kGoto           equ       0D6h
kISG            equ       0D7h
kDSL            equ       0D8h
kMenu           equ       0D9h
kExec           equ       0DAh
kReturn         equ       0DBh
kStop           equ       0DCh
kInput          equ       0DDh
kPrompt         equ       0DEh
kDisp           equ       0DFh
kDispG          equ       0E0h
kDispT          equ       0E1h
kOutput         equ       0E2h
kGetKey         equ       0E3h
kClrHome        equ       0E4h
kPrtScr         equ       0E5h
kSinH           equ       0E6h
kCosH           equ       0E7h
kTanH           equ       0E8h
kASinH          equ       0E9h
kACosH          equ       0EAh
kATanH          equ       0EBh
kLBrace         equ       0ECh
kRBrace         equ       0EDh
kI              equ       0EEh
kCONSTeA        equ       0EFh
kPlot3          equ       0F0h
kFMin           equ       0F1h
kFMax           equ       0F2h
kL1A            equ       0F3h
kL2A            equ       0F4h
kL3A            equ       0F5h
kL4A            equ       0F6h
kL5A            equ       0F7h
kL6A            equ       0F8h
kunA            equ       0F9h
kvnA            equ       0FAh
kwnA            equ       0FBh
;
;======================================================================
;       THIS KEY MEANS THAT IT IS A 2 BYTE KEYCODE
;       THERE ARE 2 OF THESE KEYS; BE CAREFUL WITH USAGE
;======================================================================
;
kExtendEcho2    equ       0FCh
;
;======================================================================;
;       THIS KEY MEANS THAT THE KEY PRESS IS ONE THAT ECHOS
;       INTO A BUFFER, AND IT IS A 2 BYTE KEY CODE, GO LOOK AT
;       (EXTECHO) FOR THE KEY VALUE 
;======================================================================
;
kExtendEcho     equ       0FEh
;

kE1BT           equ       0

kDrawInv        equ       kE1BT
kDrawF          equ       kE1BT+1
kPixelOn        equ       kE1BT+2
kPixelOff       equ       kE1BT+3
kPxlTest        equ       kE1BT+4
kRCGDB          equ       kE1BT+5
kRCPic          equ       kE1BT+6
kSTGDB          equ       kE1BT+7
kSTPic          equ       kE1BT+8
kAbs            equ       kE1BT+9
kTEqu           equ       kE1BT+10    ; ==
kTNoteQ         equ       kE1BT+11    ; <>
kTGT            equ       kE1BT+12    ; >
kTGTE           equ       kE1BT+13    ; > =
kTLT            equ       kE1BT+14    ; <
kTLTE           equ       kE1BT+15    ; < =

kAnd            equ       kE1BT+16
kOr             equ       kE1BT+17
kXor            equ       kE1BT+18
kNot            equ       kE1BT+19

kLR1            equ       kE1BT+20

kXRoot          equ       kE1BT+21
kCube           equ       kE1BT+22
kCbRt           equ       kE1BT+23    ; Cube ROOT
kToDec          equ       kE1BT+24
;
kCubicR         equ       kE1BT+25
kQuartR         equ       kE1BT+26
;
kPlot1          equ       kE1BT+27
kPlot2          equ       kE1BT+28
;

kRound          equ       kE1BT+29
kIPart          equ       kE1BT+30
kFPart          equ       kE1BT+31
kInt            equ       kE1BT+32

kRand           equ       kE1BT+33
kNPR            equ       kE1BT+34
kNCR            equ       kE1BT+35
kXFactorial     equ       kE1BT+36

kRad            equ       kE1BT+37
kDegr           equ       kE1BT+38    ; DEGREES CONV
kAPost          equ       kE1BT+39
kToDMS          equ       kE1BT+40
kRToPo          equ       kE1BT+41    ; R
kRToPr          equ       kE1BT+42
kPToRx          equ       kE1BT+43
kPToRy          equ       kE1BT+44

kRowSwap        equ       kE1BT+45
kRowPlus        equ       kE1BT+46
kTimRow         equ       kE1BT+47
kTRowP          equ       kE1BT+48

kSortA          equ       kE1BT+49
kSortD          equ       kE1BT+50
kSeq            equ       kE1BT+51

kMin            equ       kE1BT+52
kMax            equ       kE1BT+53
kMean           equ       kE1BT+54
kMedian         equ       kE1BT+55
kSum            equ       kE1BT+56
kProd           equ       kE1BT+57

kDet            equ       kE1BT+58
kTransp         equ       kE1BT+59
kDim            equ       kE1BT+60
kFill           equ       kE1BT+61
kIdent          equ       kE1BT+62
kRandm          equ       kE1BT+63
kAug            equ       kE1BT+64

kOneVar         equ       kE1BT+65
kTwoVar         equ       kE1BT+66
kLR             equ       kE1BT+67
kLRExp          equ       kE1BT+68
kLRLn           equ       kE1BT+69
kLRPwr          equ       kE1BT+70
kMedMed         equ       kE1BT+71
kQuad           equ       kE1BT+72
kClrLst         equ       kE1BT+73
kHist           equ       kE1BT+74
kxyLine         equ       kE1BT+75
kScatter        equ       kE1BT+76

kmRad           equ       kE1BT+77
kmDeg           equ       kE1BT+78
kmNormF         equ       kE1BT+79
kmSci           equ       kE1BT+80
kmEng           equ       kE1BT+81
kmFloat         equ       kE1BT+82

kFix            equ       kE1BT+83
kSplitOn        equ       kE1BT+84
kFullScreen     equ       kE1BT+85
kStndrd         equ       kE1BT+86
kParam          equ       kE1BT+87
kPolar          equ       kE1BT+88
kSeqG           equ       kE1BT+89
kAFillOn        equ       kE1BT+90
kAFillOff       equ       kE1BT+91
kACalcOn        equ       kE1BT+92
kACalcOff       equ       kE1BT+93
kFNOn           equ       kE1BT+94
kFNOff          equ       kE1BT+95

kPlotsOn        equ       kE1BT+96
kPlotsOff       equ       kE1BT+97

kPixelChg       equ       kE1BT+98

kSendMBL        equ       kE1BT+99
kRecvMBL        equ       kE1BT+100

kBoxPlot        equ       kE1BT+101
kBoxIcon        equ       kE1BT+102
kCrossIcon      equ       kE1BT+103
kDotIcon        equ       kE1BT+104

kE2BT           equ       kE1BT+105

kSeqential      equ       kE2BT
kSimulG         equ       kE2BT+1
kPolarG         equ       kE2BT+2
kRectG          equ       kE2BT+3
kCoordOn        equ       kE2BT+4
kCoordOff       equ       kE2BT+5
kDrawLine       equ       kE2BT+6
kDrawDot        equ       kE2BT+7
kAxisOn         equ       kE2BT+8
kAxisOff        equ       kE2BT+9
kGridOn         equ       kE2BT+10
kGridOff        equ       kE2BT+11
kLblOn          equ       kE2BT+12
kLblOff         equ       kE2BT+13

kL1             equ       kE2BT+14
kL2             equ       kE2BT+15
kL3             equ       kE2BT+16
kL4             equ       kE2BT+17
kL5             equ       kE2BT+18
kL6             equ       kE2BT+19

;
;======================================================================
;           These keys are layed on top of existing keys to
;           enable localization in the inferential stats editor
;======================================================================
;
kinfData        equ       kL1
kinfStats       equ       kL1+1
kinfYes         equ       kL1+2
kinfNo          equ       kL1+3
kinfCalc        equ       kL1+4
kinfDraw        equ       kL1+5
kinfAlt1ne      equ       kL1+6
kinfAlt1lt      equ       kL1+7
kinfAlt1gt      equ       kL1+8
kinfAlt2ne      equ       kL1+9
kinfAlt2lt      equ       kL1+10
kinfAlt2gt      equ       kL1+11
kinfAlt3ne      equ       kL1+12
kinfAlt3lt      equ       kL1+13
kinfAlt3gt      equ       kL1+14
kinfAlt4ne      equ       kL1+15
kinfAlt4lt      equ       kL1+16
kinfAlt4gt      equ       kL1+17
kinfAlt5ne      equ       kL1+18
kinfAlt5lt      equ       kL1+19
kinfAlt5gt      equ       kL1+20
kinfAlt6ne      equ       kL1+21
kinfAlt6lt      equ       kL1+22
kinfAlt6gt      equ       kL1+23
;
;
kMatA           equ       kE2BT+20
kMatB           equ       kE2BT+21
kMatC           equ       kE2BT+22
kMatD           equ       kE2BT+23
kMatE           equ       kE2BT+24

kXmin           equ       kE2BT+25
kXmax           equ       kE2BT+26
kXscl           equ       kE2BT+27
kYmin           equ       kE2BT+28
kYmax           equ       kE2BT+29
kYscl           equ       kE2BT+30
kTmin           equ       kE2BT+31
kTmax           equ       kE2BT+32
kTStep          equ       kE2BT+33
kOmin           equ       kE2BT+34
kOmax           equ       kE2BT+35
kOStep          equ       kE2BT+36
ku0             equ       kE2BT+37
kv0             equ       kE2BT+38
knMin           equ       kE2BT+39
knMax           equ       kE2BT+40
kDeltaY         equ       kE2BT+41
kDeltaX         equ       kE2BT+42

kZXmin          equ       kE2BT+43
kZXmax          equ       kE2BT+44
kZXscl          equ       kE2BT+45
kZYmin          equ       kE2BT+46
kZYmax          equ       kE2BT+47
kZYscl          equ       kE2BT+48
kZTmin          equ       kE2BT+49
kZTmax          equ       kE2BT+50
kZTStep         equ       kE2BT+51
kZOmin          equ       kE2BT+52
kZOmax          equ       kE2BT+53
kZOStep         equ       kE2BT+54
kZu0            equ       kE2BT+55
kZv0            equ       kE2BT+56
kZnMin          equ       kE2BT+57
kZnMax          equ       kE2BT+58

kDelLast        equ       kE2BT+59
kSinReg         equ       kE2BT+60
kConstE         equ       kE2BT+61

kPic1           equ       kE2BT+62
kPic2           equ       kE2BT+63
kPic3           equ       kE2BT+64

kDelVar         equ       kE2BT+65
kGetCalc        equ       kE2BT+66
kRealM          equ       kE2BT+67
kPolarM         equ       kE2BT+68
kRectM          equ       kE2BT+69
kuv             equ       kE2BT+70    ; U vs V
kvw             equ       kE2BT+71    ; V vs W
kuw             equ       kE2BT+72    ; U vs W
kFinPMTend      equ       kE2BT+73
kFinPMTbeg      equ       kE2BT+74
;
kGraphStyle     equ       kE2BT+75
;
kExprOn         equ       kE2BT+76
kExprOff        equ       kE2BT+77
kStatA          equ       kE2BT+78
kStatB          equ       kE2BT+79
kStatC          equ       kE2BT+80
kCorr           equ       kE2BT+81
kStatD          equ       kE2BT+82
kStatE          equ       kE2BT+83
kRegEq          equ       kE2BT+84
kMinX           equ       kE2BT+85
kQ1             equ       kE2BT+86
kMD             equ       kE2BT+87
kQ3             equ       kE2BT+88
kMaxX           equ       kE2BT+89
kStatX1         equ       kE2BT+90
kStatY1         equ       kE2BT+91
kStatX2         equ       kE2BT+92
kStatY2         equ       kE2BT+93
kStatX3         equ       kE2BT+94
kStatY3         equ       kE2BT+95
kTblMin         equ       kE2BT+96
kTblStep        equ       kE2BT+97
kSetupLst       equ       kE2BT+98
kClrAllLst      equ       kE2BT+99
kLogistic       equ       kE2BT+100
kZTest          equ       kE2BT+101
kTTest          equ       kE2BT+102
k2SampZTest     equ       kE2BT+103
k2SampTTest     equ       kE2BT+104
k1PropZTest     equ       kE2BT+105
k2PropZTest     equ       kE2BT+106
kChiTest        equ       kE2BT+107
k2SampFTest     equ       kE2BT+108
kZIntVal        equ       kE2BT+109
kTIntVal        equ       kE2BT+110
k2SampTInt      equ       kE2BT+111
k2SampZInt      equ       kE2BT+112
k1PropZInt      equ       kE2BT+113
k2PropZInt      equ       kE2BT+114
kDNormal        equ       kE2BT+115
kInvNorm        equ       kE2BT+116
kDT             equ       kE2BT+117
kChi            equ       kE2BT+118
kDF             equ       kE2BT+119
kBinPDF         equ       kE2BT+120
kBinCDF         equ       kE2BT+121
kPoiPDF         equ       kE2BT+122
kPoiCDF         equ       kE2BT+123
kun             equ       kE2BT+124
kvn             equ       kE2BT+125
kwn             equ       kE2BT+126
kRecn           equ       kE2BT+127
kPlotStart      equ       kE2BT+128
kZPlotStart     equ       kE2BT+129   ; recursion n
kXFact          equ       kE2BT+130   ; PlotStart
kYFact          equ       kE2BT+131   ; ZPlotStart
kANOVA          equ       kE2BT+132   ; XFact
kMaxY           equ       kE2BT+133   ; YFact
kWebOn          equ       kE2BT+134   ; MinY
kWebOff         equ       kE2BT+135   ; MaxY
kTblInput       equ       kE2BT+136   ; WEB ON
kGeoPDF         equ       kE2BT+137   ; WEB OFF
kGeoCDF         equ       kE2BT+138   ; WEB OFF
kShadeNorm      equ       kE2BT+139
kShadeT         equ       kE2BT+140
kShadeChi       equ       kE2BT+141
kShadeF         equ       kE2BT+142
kPlotStep       equ       kE2BT+143
kZPlotStep      equ       kE2BT+144
kLinRegtTest    equ       kE2BT+145
KMGT		equ	  kE2BT+146   ; VERT SPLIT
kSelectA        equ       kE2BT+147
kZFitA          equ       kE2BT+148
kE2BT_End       equ       kZFitA
;
;
;======================================================================
;           More 2 Byte Keys 
;======================================================================

kE2BT2		equ       0

kGDB1		equ       kE2BT2
kGDB2		equ       kE2BT2+1
kGDB3		equ       kE2BT2+2
kY1		equ       kE2BT2+3
kY2		equ       kE2BT2+4
kY3		equ       kE2BT2+5
kY4		equ       kE2BT2+6
kY5		equ       kE2BT2+7
kY6		equ       kE2BT2+8
kY7		equ       kE2BT2+9
kY8		equ       kE2BT2+10
kY9		equ       kE2BT2+11
kY0		equ       kE2BT2+12
kX1T		equ       kE2BT2+13
kY1T		equ       kE2BT2+14
kX2T		equ       kE2BT2+15
kY2T		equ       kE2BT2+16
kX3T		equ       kE2BT2+17
kY3T		equ       kE2BT2+18
kX4T		equ       kE2BT2+19
kY4T		equ       kE2BT2+20
kX5T		equ       kE2BT2+21
kY5T		equ       kE2BT2+22
kX6T		equ       kE2BT2+23
kY6T		equ       kE2BT2+24
kR1		equ       kE2BT2+25
kR2		equ       kE2BT2+26
kR3		equ       kE2BT2+27
kR4		equ       kE2BT2+28
kR5		equ       kE2BT2+29
kR6		equ       kE2BT2+30
kGDB4		equ       kE2BT2+31
kGDB5		equ       kE2BT2+32
kGDB6		equ       kE2BT2+33
kPic4		equ       kE2BT2+34
kPic5		equ       kE2BT2+35
kPic6		equ       kE2BT2+36
kGDB7		equ       kE2BT2+37
kGDB8		equ       kE2BT2+38
kGDB9		equ       kE2BT2+39
kGDB0		equ       kE2BT2+40
kPic7		equ       kE2BT2+41
kPic8		equ       kE2BT2+42
kPic9		equ       kE2BT2+43
kPic0		equ       kE2BT2+44
kStatN		equ       kE2BT2+45
kXMean		equ       kE2BT2+46
kConj		equ       kE2BT2+47
kReal		equ       kE2BT2+48
kFAngle		equ       kE2BT2+49
kLCM		equ       kE2BT2+50
kGCD		equ       kE2BT2+51
kRandInt	equ       kE2BT2+52
kRandNorm	equ       kE2BT2+53
kToPolar	equ       kE2BT2+54
kToRect		equ       kE2BT2+55
kYMean		equ       kE2BT2+56
kStdX		equ       kE2BT2+57
kStdX1		equ       kE2BT2+58
kw0		equ       kE2BT2+59
kMatF		equ       kE2BT2+60
kMatG		equ       kE2BT2+61
kMatRH		equ       kE2BT2+62
kMatI		equ       kE2BT2+63
kMatJ		equ       kE2BT2+64
kYMean1		equ       kE2BT2+65
kStdY		equ       kE2BT2+66
kStdY1		equ       kE2BT2+67
kMatToLst	equ       kE2BT2+68
kLstToMat	equ       kE2BT2+69
kCumSum		equ       kE2BT2+70
kDeltaLst	equ       kE2BT2+71
kStdDev		equ       kE2BT2+72
kVariance	equ       kE2BT2+73
kLength		equ       kE2BT2+74
kEquToStrng	equ       kE2BT2+75
kStrngToEqu	equ       kE2BT2+76
kExpr		equ       kE2BT2+77
kSubStrng	equ       kE2BT2+78
kInStrng	equ       kE2BT2+79
kStr1		equ       kE2BT2+80
kStr2		equ       kE2BT2+81
kStr3		equ       kE2BT2+82
kStr4           equ       kE2BT2+83
kStr5           equ       kE2BT2+84
kStr6           equ       kE2BT2+85
kStr7           equ       kE2BT2+86
kStr8           equ       kE2BT2+87
kStr9           equ       kE2BT2+88
kStr0           equ       kE2BT2+89
kFinN           equ       kE2BT2+90
kFinI           equ       kE2BT2+91
kFinPV          equ       kE2BT2+92
kFinPMT         equ       kE2BT2+93
kFinFV          equ       kE2BT2+94
kFinPY          equ       kE2BT2+95
kFinCY          equ       kE2BT2+96
kFinFPMT        equ       kE2BT2+97
kFinFI          equ       kE2BT2+98
kFinFPV         equ       kE2BT2+99
kFinFN          equ       kE2BT2+100
kFinFFV         equ       kE2BT2+101
kFinNPV         equ       kE2BT2+102
kFinIRR         equ       kE2BT2+103
kFinBAL         equ       kE2BT2+104
kFinPRN         equ       kE2BT2+105
kFinINT         equ       kE2BT2+106
kSumX           equ       kE2BT2+107
kSumX2          equ       kE2BT2+108
kFinToNom       equ       kE2BT2+109
kFinToEff       equ       kE2BT2+110
kFinDBD         equ       kE2BT2+111
kStatVP         equ       kE2BT2+112
kStatZ          equ       kE2BT2+113
kStatT          equ       kE2BT2+114
kStatChi        equ       kE2BT2+115
kStatF          equ       kE2BT2+116
kStatDF         equ       kE2BT2+117
kStatPhat       equ       kE2BT2+118
kStatPhat1      equ       kE2BT2+119
kStatPhat2      equ       kE2BT2+120
kStatMeanX1     equ       kE2BT2+121
kStatMeanX2     equ       kE2BT2+122
kStatStdX1      equ       kE2BT2+123
kStatStdX2      equ       kE2BT2+124
kStatStdXP      equ       kE2BT2+125
kStatN1         equ       kE2BT2+126
kStatN2         equ       kE2BT2+127
kStatLower      equ       kE2BT2+128
kStatUpper      equ       kE2BT2+129
kuw0            equ       kE2BT2+130
kImag           equ       kE2BT2+131
kSumY           equ       kE2BT2+132
kXres           equ       kE2BT2+133
kStat_s         equ       kE2BT2+134
kSumY2          equ       kE2BT2+135
kSumXY          equ       kE2BT2+136
kuXres          equ       kE2BT2+137
kModBox		equ       kE2BT2+138
kNormProb	equ       kE2BT2+139
kNormalPDF      equ       kE2BT2+140
kTPDF           equ       kE2BT2+141
kChiPDF         equ       kE2BT2+142
kFPDF           equ       kE2BT2+143
kMinY           equ       kE2BT2+144   ; MinY
kRandBin        equ       kE2BT2+145
kRef            equ       kE2BT2+146
kRRef           equ       kE2BT2+147
kLRSqr          equ       kE2BT2+148
kBRSqr          equ       kE2BT2+149
kDiagOn         equ       kE2BT2+150
kDiagOff        equ       kE2BT2+151
kun1            equ       kE2BT2+152   ; FOR RCL USE WHEN GOTTEN FROM 82
kvn1            equ       kE2BT2+153   ; FOR RCL USE WHEN GOTTEN FROM 82
;
k83_00End       equ	kvn1            ;end of original keys...
kArchive        equ     k83_00End + 1
kUnarchive      equ     k83_00End + 2
kAsm            equ     k83_00End + 3   ; Asm(
kAsmPrgm        equ     k83_00End + 4   ; AsmPrgm
kAsmComp        equ     k83_00End + 5   ; AsmComp(
;
kcapAAcute	equ       k83_00End + 6
kcapAGrave	equ       k83_00End + 7
kcapACaret	equ       k83_00End + 8
kcapADier	equ       k83_00End + 9
kaAcute		equ       k83_00End + 10
kaGrave		equ       k83_00End + 11
kaCaret		equ       k83_00End + 12
kaDier		equ       k83_00End + 13
kcapEAcute	equ       k83_00End + 14
kcapEGrave	equ       k83_00End + 15
kcapECaret	equ       k83_00End + 16
kcapEDier	equ       k83_00End + 17
keAcute		equ       k83_00End + 18
keGrave		equ       k83_00End + 19
keCaret		equ       k83_00End + 20
keDier		equ       k83_00End + 21
kcapIAcute	equ       k83_00End + 22
kcapIGrave	equ       k83_00End + 23
kcapICaret	equ       k83_00End + 24
kcapIDier	equ       k83_00End + 25
kiAcute		equ       k83_00End + 26
kiGrave		equ       k83_00End + 27
kiCaret		equ       k83_00End + 28
kiDier		equ       k83_00End + 29
kcapOAcute	equ       k83_00End + 30
kcapOGrave	equ       k83_00End + 31
kcapOCaret	equ       k83_00End + 32
kcapODier	equ       k83_00End + 33
koAcute		equ       k83_00End + 34
koGrave		equ       k83_00End + 35
koCaret		equ       k83_00End + 36
koDier		equ       k83_00End + 37
kcapUAcute	equ       k83_00End + 38
kcapUGrave	equ       k83_00End + 39
kcapUCaret	equ       k83_00End + 40
kcapUDier	equ       k83_00End + 41
kuAcute		equ       k83_00End + 42
kuGrave		equ       k83_00End + 43
kuCaret		equ       k83_00End + 44
kuDier		equ       k83_00End + 45
kcapCCed	equ       k83_00End + 46
kcCed		equ       k83_00End + 47
kcapNTilde	equ       k83_00End + 48
knTilde		equ       k83_00End + 49
kaccent		equ       k83_00End + 50
kgrave		equ       k83_00End + 51
kdieresis	equ       k83_00End + 52
kquesDown	equ       k83_00End + 53
kexclamDown     equ       k83_00End + 54
kalpha          equ       k83_00End + 55
kbeta           equ      k83_00End +  56
kgamma          equ      k83_00End +  57
kcapDelta       equ      k83_00End +  58
kdelta          equ      k83_00End +  59
kepsilon        equ      k83_00End +  60
klambda         equ      k83_00End +  61
kmu             equ      k83_00End +  62
kpi2            equ      k83_00End +  63
krho            equ      k83_00End +  64
kcapSigma       equ      k83_00End +  65
ksigma          equ      k83_00End +  66
ktau            equ      k83_00End +  67
kphi            equ      k83_00End +  68
kcapOmega       equ      k83_00End +  69
kphat           equ      k83_00End +  70
kchi2           equ      k83_00End +  71
kstatF2         equ      k83_00End +  72
kLa		equ     k83_00End + 73
kLb		equ     k83_00End + 74
kLc		equ     k83_00End + 75
kLd		equ     k83_00End + 76
kLe		equ     k83_00End + 77
kLf		equ     k83_00End + 78
kLg		equ     k83_00End + 79
kLh		equ     k83_00End + 80
kLi		equ     k83_00End + 81
kLj		equ     k83_00End + 82
kLk		equ     k83_00End + 83
kLl		equ     k83_00End + 84
kLm		equ     k83_00End + 85
kLsmalln	equ    k83_00End +  86
kLo		equ    k83_00End +  87
kLp		equ    k83_00End +  88
kLq		equ    k83_00End +  89
kLsmallr	equ    k83_00End +  90
kLs		equ    k83_00End +  91
kLt		equ    k83_00End +  92
kLu		equ    k83_00End +  93
kLv		equ    k83_00End +  94
kLw		equ    k83_00End +  95
kLx		equ    k83_00End +  96
kLy		equ    k83_00End +  97
kLz		equ    k83_00End +  98
kGarbageC	equ  k83_00End +  99 ; GarbageCollect
;
kE2BT2_End	equ     kGarbageC
;
;======================================================================
;           TI-83 Plus Context Equates
;======================================================================
;
cxCmd           equ     kQuit                   ;home screen
cxMatEdit       equ     kMatrixEd               ;matrix editor
cxPrgmEdit      equ     kPrgmEd                 ;program editor
cxEquEdit       equ     kYequ                   ;equation editor
cxGrRange       equ     kWindow                 ;graph range editor
cxGrZfact       equ     kZFactEd                ;graph zoom factors editor
cxGraph         equ     kGraph                  ;graph mode
cxStatEdit      equ     kStatEd                 ;statistics list editor
cxPrgmInput     equ     kPrgmInput              ;programmed input
cxError         equ     kError                  ;error handler
cxLinkIO        equ     kLinkIO                 ;LINK I/O interface
cxMem           equ     kResetMem               ;reset memory
cxDefMem        equ     kResetDef               ;reset default
cxRAMApp        equ     kChkRAM                 ;RAM usage screen
cxMode          equ     kMode                   ;mode settings screen
cxErase         equ     kDelMem                 ;memory erase
cxPrgmCreate    equ     kPrgmCr                 ;PROGRAM CREATE
cxTableEditor   equ     kTable                  ;TABLE EDITOR
cxTableSet      equ     kTblSet                 ;TABLE SET UP
cxStatPlot      equ     kStatP                  ;STAT PLOTS
cxInfStat	equ	kInfStat		;Inferential Statistic
cxFormat        equ     kFormat                 ;FORMAT CONTEXT
cxSolveTVM      equ     kSolveTVM               ;Solve TVM
cxSolveRoot     equ     kSolveRoot              ;Solve Root
lastOldApp      equ     kExtApps                ;external applications
cxextapps       equ     kExtApps
cxNewApps       equ     kNewApps                ;new cerberus applications
cxGroup         equ     cxNewApps+0             ;1st new app.
cxUnGroup       equ     cxNewApps+1             ;2nd new app.
lastNewApp      equ     cxUnGroup               ;last new app for this ver

;
;======================================================================
;           Scan Code Equates
;======================================================================;
skDown          equ	01h
skLeft          equ	02h
skRight         equ	03h
skUp            equ	04h
skEnter         equ	09h
skAdd           equ	0Ah
skSub           equ	0Bh
skMul           equ	0Ch
skDiv           equ	0Dh
skPower         equ	0Eh
skClear         equ	0Fh
skChs           equ	11h
sk3             equ	12h
sk6             equ	13h
sk9             equ	14h
skRParen        equ	15h
skTan           equ	16h
skVars          equ	17h
skDecPnt        equ	19h
sk2             equ	1Ah
sk5             equ	1Bh
sk8             equ	1Ch
skLParen        equ	1Dh
skCos           equ	1Eh
skPrgm          equ	1Fh
skStat          equ	20h
sk0             equ	21h
sk1             equ	22h
sk4             equ	23h
sk7             equ	24h
skComma         equ	25h
skSin           equ	26h
skMatrix        equ	27h
skGraphvar      equ	28h
skStore         equ	2Ah
skLn            equ	2Bh
skLog           equ	2Ch
skSquare        equ	2Dh
skRecip         equ	2Eh
skMath          equ	2Fh
skAlpha         equ	30h
skGraph         equ	31h
skTrace         equ	32h
skZoom          equ	33h
skWindow        equ	34h
skYEqu          equ	35h
sk2nd           equ	36h
skMode          equ	37h
skDel           equ	38h
;
;======================================================================
;           Tokens
;======================================================================
;
EOSSTART        equ       0
;
;
;   DISPLAY CONVERSIONS COME IMMEDIATELY BEFORE 'TSTORE'
;
DCONV   equ     01h
;
tToDMS          equ       DCONV       ; 01h
tToDEC          equ       DCONV+1     ; 02h
tToAbc          equ       DCONV+2     ; 03h    > A b/c
;
tStore          equ       DCONV+3     ; 04h Lstore                01
;
tBoxPlot        equ       05h
;
BRACKS          equ       06h
;
tLBrack         equ       BRACKS      ; 06h '['
tRBrack         equ       BRACKS+1    ; 07h ']'
tLBrace         equ       BRACKS+2    ; 08h '{'
tRBrace         equ       BRACKS+3    ; 09h '}'
;
tPOST1          equ       BRACKS+4
;
tFromRad        equ       tPOST1       ; 0Ah Lradian
tFromDeg        equ       tPOST1+1     ; 0Bh Ldegree
tRecip          equ       tPOST1+2     ; 0Ch Linverse
tSqr            equ       tPOST1+3     ; 0Dh Lsquare
tTrnspos        equ       tPOST1+4     ; 0Eh Ltranspose
tCube           equ       tPOST1+5     ; 0Fh '^3'
;
tLParen         equ       10h         ; 10h '('
tRParen         equ       11h         ; 11h ')'
;
;
IMUN            equ       12h
;
tRound          equ       IMUN        ; 12h 'round'
tPxTst          equ       IMUN+1      ; 13h 'PXL-TEST'
tAug            equ       IMUN+2      ; 14h 'aug'
tRowSwap        equ       IMUN+3      ; 15h 'rSwap'
tRowPlus        equ       IMUN+4      ; 16h 'rAdd'
tmRow           equ       IMUN+5      ; 17h 'multR'
tmRowPlus       equ       IMUN+6      ; 18h 'mRAdd'
tMax            equ       IMUN+7      ; 19h 'max'
tMin            equ       IMUN+8      ; 1Ah 'min'
tRToPr          equ       IMUN+9      ; 1Bh 'R>Pr
tRToPo          equ       IMUN+10     ; 1Ch 'R>Po
tPToRx          equ       IMUN+11     ; 1Dh 'P>Rx
tPToRy          equ       IMUN+12     ; 1Eh 'P>Ry
tMedian         equ       IMUN+13     ; 1Fh 'MEDIAN
tRandM          equ       IMUN+14     ; 20h 'randM'
tMean           equ       IMUN+15     ; 21h
tRoot           equ       IMUN+16     ; 22h 'ROOT'
tSeries         equ       IMUN+17     ; 23h 'seq'
tFnInt          equ       IMUN+18     ; 24h 'fnInt'
tNDeriv         equ       IMUN+19     ; 25h 'fnIr'
tEvalF          equ       IMUN+20     ; 26h
tFmin           equ       IMUN+21     ; 27h
tFmax           equ       IMUN+22     ; 28h
;
tEOSEL          equ       IMUN+23
;
tSpace          equ       tEOSEL       ; 29h   ' '
tString         equ       tEOSEL+1     ; 2Ah  '"'
tComma          equ       tEOSEL+2     ; 2Bh  ','
;
;
tii             equ       2Ch         ; i
;
;======================================================================
;           Postfix Functions
;======================================================================
;
tPost           equ       2Dh
;
tFact           equ       tPost       ; 2Dh '!'
;
tCubicR         equ       2Eh
tQuartR         equ       2Fh

;======================================================================
;           Number Tokens 
;======================================================================
;
NUMS            equ       30h
;
t0              equ       NUMS        ; 30h
t1              equ       NUMS+1      ; 31h
t2              equ       NUMS+2      ; 32h
t3              equ       NUMS+3      ; 33h
t4              equ       NUMS+4      ; 34h
t5              equ       NUMS+5      ; 35h
t6              equ       NUMS+6      ; 36h
t7              equ       NUMS+7      ; 37h
t8              equ       NUMS+8      ; 38h
t9              equ       NUMS+9      ; 39h
tDecPt          equ       NUMS+10     ; 3Ah
tee             equ       NUMS+11     ; 3Bh
;
;======================================================================
;            BINARY OP
;======================================================================
;
tOr             equ       3Ch         ; 3Ch  '_or_'
tXor            equ       3Dh         ; 3Dh
;
tColon          equ       3Eh         ; 3Eh ':'
tEnter          equ       3Fh         ; 3Fh  Lenter
;
tAnd            equ       40h         ; 40h  '_and_'       
;
;======================================================================
;            LETTER TOKENS   
;======================================================================

;
LET             equ       41h
;
tA              equ       LET         ; 41h
tB              equ       LET+1       ; 42h
tC              equ       LET+2       ; 43h
tD              equ       LET+3       ; 44h
tE              equ       LET+4       ; 45h
tF              equ       LET+5       ; 46h
tG              equ       LET+6       ; 47h
tH              equ       LET+7       ; 48h
tI              equ       LET+8       ; 49h
tJ              equ       LET+9       ; 4Ah
tK              equ       LET+10      ; 4Bh
tL              equ       LET+11      ; 4Ch
tM              equ       LET+12      ; 4Dh
tN              equ       LET+13      ; 4Eh
tO              equ       LET+14      ; 4Fh
tP              equ       LET+15      ; 50h
tQ              equ       LET+16      ; 51h
tR              equ       LET+17      ; 52h
tS              equ       LET+18      ; 53h
tT              equ       LET+19      ; 54h
tU              equ       LET+20      ; 55h
tV              equ       LET+21      ; 56h
tW              equ       LET+22      ; 57h
tX              equ       LET+23      ; 58h
tY              equ       LET+24      ; 59h
tZ              equ       LET+25      ; 5Ah
tTheta          equ       LET+26      ; 5Bh
;
;======================================================================
;   THESE VAR TOKENS ARE 1ST OF A DOUBLE TOKEN 
;======================================================================

;
vToks   equ     LET+27    ;
;
;       USER MATRIX TOKEN, 2ND TOKEN NEEDED FOR NAME
;
tVarMat         equ       vToks       ; 5Ch
;
;       USER LIST TOKEN, 2ND TOKEN NEEDED FOR NAME
;
tVarLst         equ       vToks+1     ; 5Dh
;
;       USER equATION TOKEN, 2ND TOKEN NEEDED FOR NAME
;
tVarEqu         equ       vToks+2     ; 5Eh
tProg           equ       vToks+3     ; 5Fh
;
;       USER PICT TOKEN, 2ND TOKEN NEEDED FOR NAME
;
tVarPict        equ       vToks+4     ; 60h
;
;       USER GDB TOKEN, 2ND TOKEN NEEDED FOR NAME
;
tVarGDB         equ       vToks+5     ; 61h
tVarOut         equ       vToks+6     ; 62h
tVarSys         equ       vToks+7     ; 63h
;
;
;======================================================================
;            Mode Setting Commands
;======================================================================

;
MODESA          equ       vToks+8     ; 64h
;
tRad            equ       MODESA      ; 64h 'Radian'
tDeg            equ       MODESA+1    ; 65h 'Degree'
tNormF          equ       MODESA+2    ; 66h 'Normal'
tSci            equ       MODESA+3    ; 67h 'Sci'
tEng            equ       MODESA+4    ; 68h 'Eng'
tFloat          equ       MODESA+5    ; 69h 'Float'
;
CMPS            equ       6Ah
;
tEQ             equ       CMPS        ; 6Ah '=='
tLT             equ       CMPS+1      ; 6Bh '<'
tGT             equ       CMPS+2      ; 6Ch '>'
tLE             equ       CMPS+3      ; 6Dh LLE
tGE             equ       CMPS+4      ; 6Eh LGE
tNE             equ       CMPS+5      ; 6Fh LNE
;
;======================================================================
;            BINARY OP
;======================================================================
;
tAdd            equ       70h         ; 70h '+'
tSub            equ       71h         ; 71h '-'
tAns            equ       72h         ; 72h
;
;======================================================================
;            Mode Setting Commands
;======================================================================
;
MODES           equ       73h
;
tFix            equ       MODES       ; 73h 'Fix_'
tSplitOn        equ       MODES+1     ; 74h
tFullScreen     equ       MODES+2     ; 75h
tStndrd         equ       MODES+3     ; 76h 'Func'
tParam          equ       MODES+4     ; 77h 'Param'
tPolar          equ       MODES+5     ; 78h 'Pol'
tSeqG           equ       MODES+6     ; 79h
tAFillOn        equ       MODES+7     ; 7Ah 'AUTO FILL ON
tAFillOff       equ       MODES+8     ; 7Bh
tACalcOn        equ       MODES+9     ; 7Ch
tACalcOff       equ       MODES+10    ; 7Dh 'AutoFill OFF
;
;       GRAPH FORMAT TOKENS ARE 2 BYTE TOKENS
;
tGFormat        equ       MODES+11    ; 7Eh
;
tBoxIcon        equ       7Fh
tCrossIcon      equ       80h
tDotIcon        equ       81h
;
;======================================================================
;            (More) BINARY OP
;======================================================================
;
tMul            equ       82h         ; 82h  '*'
tDiv            equ       83h         ; 83h  '/'
;
;======================================================================
;            SOME GRAPH COMMANDS
;======================================================================
;
GRCMDS          equ       84h
;
tTrace          equ       GRCMDS      ; 84h 'Trace'
tClDrw          equ       GRCMDS+1    ; 85h 'ClDrw'
tZoomStd        equ       GRCMDS+2    ; 86h 'ZStd'
tZoomtrg        equ       GRCMDS+3    ; 87h 'Ztrg'
tZoomBox        equ       GRCMDS+4    ; 88h 'ZBOX'
tZoomIn         equ       GRCMDS+5    ; 89h 'ZIn'
tZoomOut        equ       GRCMDS+6    ; 8Ah 'ZOut'
tZoomSqr        equ       GRCMDS+7    ; 8Bh 'ZSqr'
tZoomInt        equ       GRCMDS+8    ; 8Ch 'ZInt'
tZoomPrev       equ       GRCMDS+9    ; 8Dh 'ZPrev'
tZoomDec        equ       GRCMDS+10   ; 8Eh 'ZDecm'
tZoomStat       equ       GRCMDS+11   ; 8Fh 'ZStat
tUsrZm          equ       GRCMDS+12   ; 90h 'ZRcl'
tPrtScrn        equ       GRCMDS+13   ; 91h 'PrtScrn'
tZoomSto        equ       GRCMDS+14   ; 92h  ZOOM STORE
tText           equ       GRCMDS+15   ; 93h
;
;======================================================================
;           BINARY OP  (Combination & Permutation)
;======================================================================
;
tnPr            equ       GRCMDS+16   ; 94h '_nPr_'
tnCr            equ       GRCMDS+17   ; 95h '_nCr_'
;
;======================================================================
;           MORE GRAPH COMMANDS
;======================================================================
;
tYOn            equ       GRCMDS+18   ; 96h 'FnOn_'
tYOff           equ       GRCMDS+19   ; 97h 'FnOff_'
tStPic          equ       GRCMDS+20   ; 98h 'StPic_'
tRcPic          equ       GRCMDS+21   ; 99h 'RcPic_'
tStoDB          equ       GRCMDS+22   ; 9Ah 'StGDB_'
tRclDB          equ       GRCMDS+23   ; 9Bh 'RcGDB_'
tLine           equ       GRCMDS+24   ; 9Ch 'Line'
tVert           equ       GRCMDS+25   ; 9Dh 'Vert_'
tPtOn           equ       GRCMDS+26   ; 9Eh 'PtOn'
tPtOff          equ       GRCMDS+27   ; 9Fh 'PtOff'
;
;======================================================================
;           TOKEN A0 CANNOT BE AN EOS FUNCTION
;           SINCE LOW MULT=A0 ALREADY
;======================================================================
;
tPtChg          equ       GRCMDS+28   ; A0h 'PtChg'
tPXOn           equ       GRCMDS+29   ; A1h
tPXOff          equ       GRCMDS+30   ; A2h
tPXChg          equ       GRCMDS+31   ; A3h
tShade          equ       GRCMDS+32   ; A4h 'Shade'
tCircl          equ       GRCMDS+33   ; A5h 'Circl'
tHorz           equ       GRCMDS+34   ; A6h 'HORIZONTAL'
tTanLn          equ       GRCMDS+35   ; A7h 'TanLn'
tDrInv          equ       GRCMDS+36   ; A8h 'DrInv_'
tDrawF          equ       GRCMDS+37   ; A9h 'DrawF_'
;
tVarStrng       equ       0AAh
;
;======================================================================
;           Functions with No Arguments                                    
;======================================================================
;
NOARG   equ     0ABh
;
tRand           equ       NOARG       ; ABh 'rand'
tPi             equ       NOARG+1     ; ACh  Lpi
tGetKey         equ       NOARG+2     ; ADh 'getKy'
;
;
tAPost          equ       tGetKey+1   ; APOSTROPHY
tQuest          equ       tAPost+1    ; QUESTION MARK
;
;
UNARY   equ     tQuest+1              ; B0h
;
tChs            equ       UNARY       ; B0h
tInt            equ       UNARY+1     ; B1h
tAbs            equ       UNARY+2     ; B2h
tDet            equ       UNARY+3     ; B3h
tIdent          equ       UNARY+4     ; B4h
tDim            equ       UNARY+5     ; B5h
tSum            equ       UNARY+6     ; B6h
tProd           equ       UNARY+7     ; B7h
tNot            equ       UNARY+8     ; B8h
tIPart          equ       UNARY+9     ; B9h
tFPart          equ       UNARY+10    ; BAh
;
;
;======================================================================
;           NEW 2 BYTE TOKENS
;======================================================================
;
t2ByteTok       equ       0BBh         ;
;
;
UNARYLR equ     UNARY+12
;
tSqrt           equ       UNARYLR     ; BCh
tCubRt          equ       UNARYLR+1   ; BDh
tLn             equ       UNARYLR+2   ; BEh
tExp            equ       UNARYLR+3   ; BFh
tLog            equ       UNARYLR+4   ; C0h
tALog           equ       UNARYLR+5   ; C1h
tSin            equ       UNARYLR+6   ; C2h
tASin           equ       UNARYLR+7   ; C3h
tCos            equ       UNARYLR+8   ; C4h
tACos           equ       UNARYLR+9   ; C5h
tTan            equ       UNARYLR+10  ; C6h
tATan           equ       UNARYLR+11  ; C7h
tSinH           equ       UNARYLR+12  ; C8h
tASinH          equ       UNARYLR+13  ; C9h
tCosH           equ       UNARYLR+14  ; CAh
tACosH          equ       UNARYLR+15  ; CBh
tTanH           equ       UNARYLR+16  ; CCh
tATanH          equ       UNARYLR+17  ; CDh
; 
;======================================================================
;           SOME PROGRAMMING COMMANDS                                      
;======================================================================
;
PROGTOK equ     UNARYLR+18
;
tIf             equ       PROGTOK     ; CEh
tThen           equ       PROGTOK+1   ; CFh
tElse           equ       PROGTOK+2   ; D0h
tWhile          equ       PROGTOK+3   ; D1h
tRepeat         equ       PROGTOK+4   ; D2h
tFor            equ       PROGTOK+5   ; D3h
tEnd            equ       PROGTOK+6   ; D4h
tReturn         equ       PROGTOK+7   ; D5h
tLbl            equ       PROGTOK+8   ; D6h   'Lbl_'
tGoto           equ       PROGTOK+9   ; D7h   'Goto_'
tPause          equ       PROGTOK+10  ; D8h   'Pause_'
tStop           equ       PROGTOK+11  ; D9h   'Stop'
tISG            equ       PROGTOK+12  ; DAh   'IS>'
tDSL            equ       PROGTOK+13  ; DBh   'DS<'
tInput          equ       PROGTOK+14  ; DCh   'Input_'
tPrompt         equ       PROGTOK+15  ; DDh   'Prompt_'
tDisp           equ       PROGTOK+16  ; DEh   'Disp_'
tDispG          equ       PROGTOK+17  ; DFh   'DispG'
;
tOutput         equ       PROGTOK+18  ; E0h    'Outpt'
tClLCD          equ       PROGTOK+19  ; E1h    'ClLCD'
tConst          equ       PROGTOK+20  ; E2h    'Fill'
tSortA          equ       PROGTOK+21  ; E3h    'sortA_'
tSortD          equ       PROGTOK+22  ; E4h    'sortD_'
tDispTab        equ       PROGTOK+23  ; E5h    'Disp Table
tMenu           equ       PROGTOK+24  ; E6h    'Menu'
tSendMBL        equ       PROGTOK+25  ; E7h    'SEND'
tGetMBL         equ       PROGTOK+26  ; E8h    'GET'
;
;======================================================================
;           STAT PLOT COMMANDS                                             
;======================================================================
;
statPCmd        equ       PROGTOK+27
;
tPlotOn         equ       statPCmd    ; E9h     ' PLOTSON'
tPlotOff        equ       statPCmd+1   ; EAh     ' PLOTSOFF
;
tListName       equ       0EBh        ; LIST DESIGNATOR
;
tPlot1          equ       0ECh
tPlot2          equ       0EDh
tPlot3          equ       0EEh
;
tUnused01       equ       0EFh        ;        available?
;
tPower          equ       0F0h        ;        '^'
tXRoot          equ       0F1h        ;        LsupX,Lroot

STATCMD         equ       0F2h
;
tOneVar         equ       STATCMD     ; F2h    'OneVar_'
tTwoVar         equ       STATCMD+1   ; F3h
tLR             equ       STATCMD+2   ; F4h    'LinR(A+BX
tLRExp          equ       STATCMD+3   ; F5h    'ExpR_'
tLRLn           equ       STATCMD+4   ; F6h    'LnR_'
tLRPwr          equ       STATCMD+5   ; F7h    'PwrR_'
tMedMed         equ       STATCMD+6   ; F8h
tQuad           equ       STATCMD+7   ; F9h
tClrLst         equ       STATCMD+8   ; FAh    'CLEAR LIST
tClrTbl         equ       STATCMD+9   ; FBh    CLEAR TABLE
tHist           equ       STATCMD+10  ; FCh    'Hist_'
txyLine         equ       STATCMD+11  ; FDh    'xyline_'
tScatter        equ       STATCMD+12  ; FEh    'Scatter_'
tLR1            equ       STATCMD+13  ; FFh    'LINR(AX+B

;       
;======================================================================
;           2ND HALF OF GRAPH FORMAT TOKENS
;======================================================================
;           Format settings commands
;
GFMT            equ       0
;
tSeq            equ       GFMT        ; 'SeqG'
tSimulG         equ       GFMT+1      ; 'SimulG'
tPolarG         equ       GFMT+2      ; 'PolarGC'
tRectG          equ       GFMT+3      ; 'RectGC'
tCoordOn        equ       GFMT+4      ; 'CoordOn'
tCoordOff       equ       GFMT+5      ; 'CoordOff'
tDrawLine       equ       GFMT+6      ; 'DrawLine'
tDrawDot        equ       GFMT+7      ; 'DrawDot'
tAxisOn         equ       GFMT+8      ; 'AxesOn'
tAxisOff        equ       GFMT+9      ; 'AxesOff'
tGridOn         equ       GFMT+10     ; 'GridOn'
tGridOff        equ       GFMT+11     ; 'GridOff'
tLblOn          equ       GFMT+12     ; 'LabelOn'
tLblOff         equ       GFMT+13     ; 'LabelOff'
tWebOn          equ       GFMT+14     ; 'WebOn'
tWebOff         equ       GFMT+15     ; 'WebOFF'
tuv             equ       GFMT+16     ; U vs V
tvw             equ       GFMT+17     ; V vs W
tuw             equ       GFMT+18     ; U vs W
;
;======================================================================
;           2ND HALF OF USER MATRIX TOKENS                                 
;======================================================================
;
tMatA           equ       00h         ; MAT A
tMatB           equ       01h         ; MAT B
tMatC           equ       02h         ; MAT C
tMatD           equ       03h         ; MAT D
tMatE           equ       04h         ; MAT E
tMatF           equ       05h         ; MAT F
tMatG           equ       06h         ; MAT G
tMatH           equ       07h         ; MAT H
tMatI           equ       08h         ; MAT I
tMatJ           equ       09h         ; MAT J
;      
;======================================================================
;           2ND HALF OF USER LIST TOKENS
;======================================================================
;
tL1             equ       00h         ; LIST 1
tL2             equ       01h         ; LIST 2
tL3             equ       02h         ; LIST 3
tL4             equ       03h         ; LIST 4
tL5             equ       04h         ; LIST 5
tL6             equ       05h         ; LIST 6

;
;======================================================================
;           2ND HALF OF USER equATION TOKENS
;======================================================================
;
;       "Y" equATIONS HAVE BIT 4 SET
;
tY1             equ       10h         ; Y1
tY2             equ       11h         ; Y2
tY3             equ       12h         ; Y3
tY4             equ       13h         ; Y4
tY5             equ       14h         ; Y5
tY6             equ       15h         ; Y6
tY7             equ       16h         ; Y7
tY8             equ       17h         ; Y8
tY9             equ       18h         ; Y9
tY0             equ       19h         ; Y0
;
;       PARAM equATIONS HAVE BIT 5 SET
;
tX1T            equ       20h         ; X1t
tY1T            equ       21h         ; Y1t
tX2T            equ       22h         ; X2t
tY2T            equ       23h         ; Y2t
tX3T            equ       24h         ; X3t
tY3T            equ       25h         ; Y3t
tX4T            equ       26h         ; X4t
tY4T            equ       27h         ; Y4t
tX5T            equ       28h         ; X5t
tY5T            equ       29h         ; Y5t
tX6T            equ       2Ah         ; X6t
tY6T            equ       2Bh         ; Y6t
;
;       POLAR equATIONS HAVE BIT 6 SET
;
tR1             equ       40h         ; R1
tR2             equ       41h         ; R2
tR3             equ       42h         ; R3
tR4             equ       43h         ; R4
tR5             equ       44h         ; R5
tR6             equ       45h         ; R6
;
;       RECURSION equATIONS HAVE BIT 7 SET
;
tun             equ       80h         ; Un
tvn             equ       81h         ; Vn
twn             equ       82h         ; Wn
;
;======================================================================
;           2ND HALF OF USER PICTURE TOKENS                                
;======================================================================
;
tPic1           equ       00h         ; PIC1
tPic2           equ       01h         ; PIC2
tPic3           equ       02h         ; PIC3
tPic4           equ       03h         ; PIC4
tPic5           equ       04h         ; PIC5
tPic6           equ       05h         ; PIC6
tPic7           equ       06h         ; PIC7
tPic8           equ       07h         ; PIC8
tPic9           equ       08h         ; PIC9
tPic0           equ       09h         ; PIC0
; 
;======================================================================
;           2ND HALF OF USER GRAPH DATABASE TOKENS                         
;======================================================================
;
tGDB1           equ       00h         ; GDB1
tGDB2           equ       01h         ; GDB2
tGDB3           equ       02h         ; GDB3
tGDB4           equ       03h         ; GDB4
tGDB5           equ       04h         ; GDB5
tGDB6           equ       05h         ; GDB6
tGDB7           equ       06h         ; GDB7
tGDB8           equ       07h         ; GDB8
tGDB9           equ       08h         ; GDB9
tGDB0           equ       09h         ; GDB0
;  
;======================================================================
;           2ND HALF OF STRING VARS                                        
;======================================================================
;
tStr1           equ       00h
tStr2           equ       01h
tStr3           equ       02h
tStr4           equ       03h
tStr5           equ       04h
tStr6           equ       05h
tStr7           equ       06h
tStr8           equ       07h
tStr9           equ       08h
tStr0           equ       09h

; 
;======================================================================
;           2ND HALF OF SYSTEM OUTPUT ONLY VARIABLES                       
;======================================================================
;
;
;   OPEN        equ       00h
;
tRegEq          equ       01h         ; REGRESSION EQUATION
;
tStatN          equ       02h         ; STATISTICS N
;
tXMean          equ       03h         ; X MEAN
tSumX           equ       04h         ; SUM(X)
tSumXSqr        equ       05h         ; SUM(X^2)
tStdX           equ       06h         ; STANDARD DEV X
tStdPX          equ       07h         ; STANDARD DEV POP X
tMinX           equ       08h         ; Min X VALUE
tMaxX           equ       09h         ; Max X VALUE
;
tMinY           equ       0Ah         ; Min Y VALUE
tMaxY           equ       0Bh         ; Max Y VALUE
tYmean          equ       0Ch         ; Y MEAN
tSumY           equ       0Dh         ; SUM(Y)
tSumYSqr        equ       0Eh         ; SUM(Y^2)
tStdY           equ       0Fh         ; STANDARD DEV Y
tStdPY          equ       10h         ; STANDARD DEV POP Y
;
tSumXY          equ       11h         ; SUM(XY)
tCorr           equ       12h         ; CORRELATION
tMedX           equ       13h         ; MED(X)
tQ1             equ       14h         ; 1ST QUADRANT OF X
tQ3             equ       15h         ; 3RD QUADRANT OF X
tQuadA          equ       16h         ; 1ST TERM OF QUAD POLY REG/ Y-INT
tQuadB          equ       17h         ; 2ND TERM OF QUAD POLY REG/ SLOPE
tQuadC          equ       18h         ; 3RD TERM OF QUAD POLY REG
tCubeD          equ       19h         ; 4TH TERM OF CUBIC POLY REG
tQuartE         equ       1Ah         ; 5TH TERM OF QUART POLY REG
tMedX1          equ       1Bh         ; x1 FOR MED-MED
tMedX2          equ       1Ch         ; x2 FOR MED-MED
tMedX3          equ       1Dh         ; x3 FOR MED-MED
tMedY1          equ       1Eh         ; y1 FOR MED-MED
tMedY2          equ       1Fh         ; y2 FOR MED-MED
tMedY3          equ       20h         ; y3 FOR MED-MED
;
tRecurn		equ       21h		; RECURSION N
tStatP          equ       22h
tStatZ          equ       23h
tStatT          equ       24h
tStatChi        equ       25h
tStatF          equ       26h
tStatDF         equ       27h
tStatPhat       equ       28h
tStatPhat1      equ       29h
tStatPhat2      equ       2Ah
tStatMeanX1     equ       2Bh
tStatStdX1      equ       2Ch
tStatN1         equ       2Dh
tStatMeanX2     equ       2Eh
tStatStdX2      equ       2Fh
tStatN2         equ       30h
tStatStdXP      equ       31h
tStatLower      equ       32h
tStatUpper      equ       33h
tStat_s         equ       34h
tLRSqr          equ       35h         ; r^2
tBRSqr          equ       36h         ; R^2
;
;======================================================================
;       These next tokens are only used to access the data
;       they are display only and the user cannot access them at all
;======================================================================
;
tF_DF    equ     37h       ; ANOFAV FACTOR DF
tF_SS    equ     38h       ; ANOFAV FACTOR SS
tF_MS    equ     39h       ; ANOFAV FACTOR MS
tE_DF    equ     3Ah       ; ANOFAV ERROR DF
tE_SS    equ     3Bh       ; ANOFAV ERROR SS
tE_MS    equ     3Ch       ; ANOFAV ERROR MS
;
;
;======================================================================
;           2ND HALF OF SYSTEM INPUT/OUTPUT VARIABLES
;======================================================================
;      SYSTEM VARIABLE equATES
;
tuXscl       equ  0
tuYscl       equ  1
tXscl        equ  2
tYscl        equ  3
tRecuru0     equ  4        ; U 1ST INITIAL COND
tRecurv0     equ  5        ; V 1ST INITIAL COND
tun1         equ  6        ; U(N-1)    ; NOT USED
tvn1         equ  7        ; V(N-1)    ; NOT USED
tuRecuru0    equ  8        ;
tuRecurv0    equ  9        ;
;
tXmin        equ  0Ah
tXmax        equ  0Bh
tYmin        equ  0Ch
tYmax        equ  0Dh
tTmin        equ  0Eh
tTmax        equ  0Fh
tThetaMin    equ  10h
tThetaMax    equ  11h
tuXmin       equ  12h
tuXmax       equ  13h
tuYmin       equ  14h
tuYmax       equ  15h
tuThetMin    equ  16h
tuThetMax    equ  17h
tuTmin       equ  18h
tuTmax       equ  19h
tTblMin      equ  1Ah
tPlotStart   equ  1Bh
tuPlotStart  equ  1Ch
tnMax        equ  1Dh
tunMax       equ  1Eh
tnMin        equ  1Fh
tunMin       equ  20h
;
tTblStep	equ  21h
tTStep		equ  22h
tThetaStep	equ  23h
tuTStep		equ  24h
tuThetStep	equ  25h
;
tDeltaX		equ  26h
tDeltaY		equ  27h
;
tXFact		equ  28h
tYFact		equ  29h
;
tTblInput       equ       2Ah
;
tFinN           equ       2Bh
tFinI           equ       2Ch
tFinPV          equ       2Dh
tFinPMT         equ       2Eh
tFinFV          equ       2Fh
tFinPY          equ       30h
tFinCY          equ       31h
;
tRecurw0        equ       32h         ; w0(1)
tuRecurw0       equ       33h
;
tPlotStep       equ       34h
tuPlotStep      equ       35h
;
tXres           equ       36h
tuXres          equ       37h
;
tRecuru02       equ       38h         ; u0(2)
tuRecuru02      equ       39h
tRecurv02       equ       3Ch         ; v0(2)
tuRecurv02      equ       3Dh
tRecurw02       equ       3Eh         ; w0(2)
tuRecurw02      equ       3Fh
;
;======================================================================
;           2nd byte of t2ByteTok tokens
;======================================================================
;
tFinNPV         equ       00h         ;
tFinIRR         equ       01h         ;
tFinBAL         equ       02h         ;
tFinPRN         equ       03h         ;
tFinINT         equ       04h         ;
tFinToNom       equ       05h         ;
tFinToEff       equ       06h         ;
tFinDBD         equ       07h         ;
tLCM            equ       08h         ;
tGCD            equ       09h         ;
tRandInt        equ       0Ah         ;
tRandBin        equ       0Bh         ;
tSubStrng       equ       0Ch         ;
tStdDev         equ       0Dh         ;
tVariance       equ       0Eh         ;
tInStrng        equ       0Fh         ;
tDNormal        equ       10h         ;
tInvNorm        equ       11h         ;
tDT             equ       12h         ;
tChI            equ       13h         ;
tDF             equ       14h         ;
tBINPDF         equ       15h         ;
tBINCDF         equ       16h         ;
tPOIPDF         equ       17h         ;
tPOICDF         equ       18h         ;
tGEOPDF         equ       19h         ;
tGEOCDF         equ       1Ah         ;
tNormalPDF      equ       1Bh         ;
tTPDF           equ       1Ch         ;
tChiPDF         equ       1Dh         ;
tFPDF           equ       1Eh         ;
tRandNorm       equ       1Fh         ;
tFinFPMT        equ       20h         ;
tFinFI          equ       21h         ;
tFinFPV         equ       22h         ;
tFinFN          equ       23h         ;
tFinFFV         equ       24h         ;
tConj           equ       25h         ;
tReal           equ       26h         ;
tImag           equ       27h         ;
tAngle          equ       28h         ;
tCumSum         equ       29h         ;
tExpr           equ       2Ah         ;
tLength         equ       2Bh         ;
tDeltaLst       equ       2Ch         ;
tRef            equ       2Dh         ;
tRRef           equ       2Eh         ;
tToRect         equ       2Fh         ;
tToPolar        equ       30h         ;
tConste         equ       31h         ;
tSinReg         equ       32h         ;
tLogistic       equ       33h         ;
tLinRegTTest    equ       34h         ;
tShadeNorm      equ       35h         ;
tShadeT         equ       36h         ;
tShadeChi       equ       37h         ;
tShadeF         equ       38h         ;
tMatToLst       equ       39h         ;
tLstToMat       equ       3Ah         ;
tZTest          equ       3Bh         ;
tTTest          equ       3Ch         ;
t2SampZTest     equ       3Dh         ;
t1PropZTest     equ       3Eh         ;
t2PropZTest     equ       3Fh         ;
tChiTest        equ       40h         ;
tZIntVal        equ       41h         ;
t2SampZInt      equ       42h         ;
t1PropZInt      equ       43h         ;
t2PropZInt      equ       44h         ;
tGraphStyle     equ       45h         ;
t2SampTTest     equ       46h         ;
t2SampFTest     equ       47h         ;
tTIntVal        equ       48h         ;
t2SampTInt      equ       49h         ;
tSetupLst       equ       4Ah         ;
tFinPMTend      equ       4Bh         ;
tFinPMTbeg      equ       4Ch         ;
tRealM          equ       4Dh         ;
tPolarM         equ       4Eh         ;
tRectM          equ       4Fh         ;
tExprOn         equ       50h         ;
tExprOff        equ       51h         ;
tClrAllLst      equ       52h         ;
tGetCalc        equ       53h         ;
tDelVar         equ       54h         ;
tEquToStrng     equ       55h         ;
tStrngToEqu     equ       56h         ;
tDelLast        equ       57h         ;
tSelect         equ       58h         ;
tANOVA          equ       59h         ;
tModBox         equ       5Ah         ;
tNormProb       equ       5Bh         ;
;
;
tMGT            equ       64h         ; VERTICAL SPLIT
tZFit           equ       65h         ; ZOOM FIT
tDiag_on        equ       66h         ; DIANOSTIC DISPLAY ON
tDiag_off       equ       67h         ; DIANOSTIC DISPLAY OFF
tOkEnd2v0       equ       67h         ;end of 2byte tokens for version 0.
tArchive        equ       68h         ;archive
tUnarchive      equ       69h         ;unarchive
tasm		equ	  6Ah
tasmComp	equ	  6Bh	      ; asm compile
tasmPrgm	equ	  6Ch	      ; signifies a program is asm
tasmCmp 	equ	  6Dh	      ; asm program is compiled
;
tLcapAAcute     equ       6Eh
tLcapAGrave     equ       6Fh
tLcapACaret     equ       70h
tLcapADier      equ       71h
tLaAcute        equ       72h
tLaGrave        equ       73h
tLaCaret        equ       74h
tLaDier         equ       75h
tLcapEAcute     equ       76h
tLcapEGrave     equ       77h
tLcapECaret     equ       78h
tLcapEDier      equ       79h
tLeAcute        equ       7Ah
tLeGrave        equ       7Bh
tLeCaret        equ       7Ch
tLeDier         equ       7Dh
;
tLcapIGrave     equ       7Fh
tLcapICaret     equ       80h
tLcapIDier      equ       81h
tLiAcute        equ       82h
tLiGrave        equ       83h
tLiCaret        equ       84h
tLiDier         equ       85h
tLcapOAcute     equ       86h
tLcapOGrave     equ       87h
tLcapOCaret     equ       88h
tLcapODier      equ       89h
tLoAcute        equ       8Ah
tLoGrave        equ       8Bh
tLoCaret        equ       8Ch
tLoDier         equ       8Dh
tLcapUAcute     equ       8Eh
tLcapUGrave     equ       8Fh
tLcapUCaret     equ       90h
tLcapUDier      equ       91h
tLuAcute        equ       92h
tLuGrave        equ       93h
tLuCaret        equ       94h
tLuDier         equ       95h
tLcapCCed       equ       96h
tLcCed          equ       97h
tLcapNTilde     equ       98h
tLnTilde        equ       99h
tLaccent        equ       9Ah
tLgrave         equ       9Bh
tLdieresis      equ       9Ch
tLquesDown      equ       9Dh
tLexclamDown    equ       9Eh
tLalpha         equ       9Fh
tLbeta          equ      0a0h
tLgamma         equ      0a1h
tLcapDelta      equ      0a2h
tLdelta         equ      0a3h
tLepsilon       equ      0a4h
tLlambda        equ      0a5h
tLmu            equ      0a6h
tLpi            equ      0a7h
tLrho           equ      0a8h
tLcapSigma      equ      0a9h
;
;
tLphi           equ      0aBh
tLcapOmega      equ      0aCh
tLphat          equ      0aDh
tLchi           equ      0aEh
tLstatF         equ      0aFh
;

tLa      equ     0b0h
tLb      equ     0b1h
tLc      equ     0b2h
tLd      equ     0b3h
tLsmalle equ     0b4h
tLf      equ     0b5h
tLsmallg equ     0b6h
tLh      equ     0b7h
tLi      equ     0b8h
tLj      equ     0b9h
tLk      equ     0bAh
;

tLl      equ     0bCh
tLm      equ     0bDh
;tLn      equ     0bEh
tLo      equ     0bFh
tLp      equ     0c0h
tLq      equ     0c1h
tLsmallr equ     0c2h
tLs      equ     0c3h
tLsmallt equ     0c4h
tLu      equ     0c5h
tLv      equ     0c6h
tLw      equ     0c7h
tLx      equ     0c8h
tLy      equ     0c9h
tLz      equ     0cAh
tLsigma         equ      0cbh
tLtau           equ      0cch
tLcapIAcute     equ      0cdh
;
;
tGarbagec equ    0Ceh
LastToken       equ      0Ceh         ;tLAST TOKEN IN THIS VERSION...
;
;======================================================================
;           Data Type Equates                                                    
;======================================================================
;
RealObj         equ       0
ListObj         equ       1
MatObj          equ       2
EquObj          equ       3
StrngObj        equ       4
ProgObj         equ       5
ProtProgObj     equ       6
PictObj         equ       7
GDBObj          equ       8
UnknownObj      equ       9
UnknownEquObj   equ       0Ah
NewEquObj       equ       0Bh
CplxObj         equ       0Ch
CListObj        equ       0Dh
UndefObj        equ       0Eh
WindowObj       equ       0Fh
ZStoObj         equ       10h
TblRngObj       equ       11h
LCDObj          equ       12h
BackupObj       equ       13h
AppObj		equ	  14h		;application, only used in menus/link
AppVarObj	equ	  15h		;application variable
TempProgObj     equ       16h           ;program, home deletes when finished
GroupObj        equ       17h           ;group.
; 
;======================================================================
;	    Parser Equates
;======================================================================
OPRand      equ              tRand
OPProd      equ              tProd
OPSum       equ              tSum
OPAdd       equ              tAdd
OPSub       equ              tSub
OPMult      equ              tMul
OPDiv       equ              tDiv
OPPower     equ              tPower
OPXRoot     equ              tXRoot
OPEq        equ              tEQ
OPNeq       equ              tNE
OPRound2    equ              tRound
OPAug       equ              tAug
OPMax       equ              tMax
OPMin       equ              tMin
OPAnd       equ              tAnd
OPOr        equ              tOr
OPXor       equ              tXor
OPNcr       equ              tnCr
OPNpr       equ              tnPr
OPLt        equ              tLT
OPLe        equ              tLE
OPGt        equ              tGT
OPGe        equ              tGE
;
OPLog       equ              tLog
OPTenX      equ              tALog
OPLn        equ              tLn
OPEtoX      equ              tExp
OPNot       equ              tNot
OPSin       equ              tSin
OPASin      equ              tASin
OPCos       equ              tCos
OPACos      equ              tACos
OPTan       equ              tTan
OPATan      equ              tATan
OPSinH      equ              tSinH
OPASinH     equ              tASinH
OPCosH      equ              tCosH
OPACosH     equ              tACosH
OPTanH      equ              tTanH
OPATanH     equ              tATanH
OPInverse   equ              tRecip
OPDet       equ              tDet
OPSqroot    equ              tSqrt
OPSquare    equ              tSqr
OPNegate    equ              tChs
OPIpart     equ              tIPart
OPFpart     equ              tFPart
OPIntgr     equ              tInt
OPFact      equ              tFact
OPAbs       equ              tAbs
OPIdent     equ              tIdent
OPTranspose equ              tTrnspos
OPSortA     equ              tSortA
OPSortD     equ              tSortD
OPToDec     equ              tToDEC
OPToFrac    equ              tToAbc
OPRadd      equ              tRowPlus
OPRmlt      equ              tmRow
OPRswap     equ              tRowSwap
OPMltRadd   equ              tmRowPlus
OPRand1     equ              tRandM
OPSolve     equ              tRoot
OPDeriv81   equ              tNDeriv
OPQuad      equ              tFnInt
OPSeq       equ              tSeries
OPDim       equ              tDim
OPRad       equ              tFromRad
OPDeg       equ              tFromDeg
OPRtopr     equ              tRToPr
OPRtopo     equ              tRToPo
OPPtorx     equ              tPToRx
OPPtory     equ              tPToRy
OPCube      equ              tCube
OPCbrt      equ              tCubRt
OPFmin      equ              tFmin
OPFmax      equ              tFmax
OPNpv           equ       tIf         ; immediate unary
OPIrr           equ       tIf+1       ; immediate unary
OPBal           equ       tIf+2       ; immediate unary
OPPrn           equ       tIf+3       ; immediate unary
OPIntr          equ       tIf+4       ; immediate unary
OPToNom         equ       tIf+5       ; immediate unary
OPToEff         equ       tIf+6       ; immediate unary
OPFinDbd           equ       tIf+7       ; immediate unary
OPLcm           equ       tIf+8       ; immediate unary
OPGcd           equ       tIf+9       ; immediate unary
OPRandInt       equ       tIf+10      ; immediate unary
OPRandBin        equ      tIf+11      ; immediate unary
OPSubstr        equ       tIf+12      ; immediate unary
OPstDev         equ       tIf+13      ; immediate unary
OPVariance      equ       tIf+14      ; immediate unary
OPInstr         equ       tIf+15      ; immediate unary
OPNormal        equ       tIf+16      ; immediate unary
OPINormal       equ       tIf+17      ; immediate unary
OPDt            equ       tIf+18      ; immediate unary
OPChi           equ       tIf+19      ; immediate unary
OPDf            equ       tIf+20      ; immediate unary
OPBinpdf        equ       tIf+21      ; immediate unary
OPBincdf        equ       tIf+22      ; immediate unary
OPPoipdf        equ       tIf+23      ; immediate unary
OPPoicdf        equ       tIf+24      ; immediate unary
OPGeopdf        equ       tIf+25      ; immediate unary
OPGeocdf        equ       tIf+26      ; immediate unary
OPNormalpdf     equ       tIf+27      ; immediate unary
OPTpdf          equ       tIf+28      ; immediate unary
OPChipdf        equ       tIf+29      ; immediate unary
OPFpdf          equ       tIf+30      ; immediate unary
OPRandNrm       equ       tIf+31      ; immediate unary
OPConj           equ       tZoomIn
OPReal           equ       tZoomIn	   +1
OPImag           equ       tZoomIn	   +2
OPAngle          equ       tZoomIn	   +3
OPCumSum         equ       tZoomIn	   +4
OPExpr           equ       tZoomIn	   +5
OPLength         equ       tZoomIn	   +6
OPDeltaLst       equ       tZoomIn	   +7
OPRef            equ       tZoomIn	   +8
OPRref           equ       tZoomIn	   +9
OPConst          equ       tZoomIn	   +10

;======================================================================
;           I/O equates                                                          
;======================================================================
;
D0D1_bits	equ  03h
D0LD1L		equ  03h
D0LD1H		equ  01h
D0HD1L		equ  02h
D0HD1H		equ  00h
bport		equ  0			; 4-BIT LINK PORT (I/O) 
;
;======================================================================
;	Interrupt equates
;======================================================================
;
iAll	equ 1011b
intrptEnPort	equ 3 ; interrupt enable (in/out) 
;
;======================================================================
;	Memory paging equates
;======================================================================
;
memPageAPort	equ 6 ; memory page A (in/out)
;
;======================================================================
;	LCD Driver equates
;======================================================================
LCDinstPort	equ 10h
LCDdataPort equ 11h
;
;======================================================================
;           DEVICE CODES                                                       
;======================================================================
;
;
TI82DEV		equ  82h
PC82DEV		equ  02h
MAC82DEV	equ  12h
;
LINK83FDEV	equ  23h
;
TI83DEV		equ  83h
PC83DEV		equ  03h
MAC83DEV	equ  13h
;
TI85DEV		equ  95h      ; DIFF THEN REAL 85 SO ME TALK
PC85DEV		equ  05h
MAC85DEV	equ  15h
;
TI73DEV		equ  74h      ; Device x3 is always an 83
PC73DEV		equ  07h
MAC73DEV	equ  17h
;
TI83FDEV	equ  73h
LINK73FDEV	equ  23h
PC83FDEV	equ  23h
;
;======================================================================
;           System Error Codes                                                  
;======================================================================
;
E_EDITF		equ	7		;allow re-entering application
E_EDIT		equ	1<<E_EDITF
E_Mask		equ	7Fh

E_Overflow	equ	1+E_EDIT
E_DivBy0	equ	2+E_EDIT
E_SingularMat	equ	3+E_EDIT
E_Domain	equ	4+E_EDIT
E_Increment	equ	5+E_EDIT
E_Break		equ	6+E_EDIT
E_NonReal       equ     7+E_EDIT
E_Syntax        equ     8+E_EDIT
E_DataType      equ     9+E_EDIT
E_Argument      equ     10+E_EDIT
E_DimMismatch   equ     11+E_EDIT
E_Dimension     equ     12+E_EDIT
E_Undefined     equ     13+E_EDIT
E_Memory        equ     14+E_EDIT
E_Invalid       equ     15+E_EDIT
E_IllegalNest   equ     16+E_EDIT
E_Bound         equ     17+E_EDIT
E_GraphRange    equ     18+E_EDIT
E_Zoom          equ     19+E_EDIT
E_Label         equ     20
E_Stat          equ     21
E_Solver        equ     22+E_EDIT
E_Singularity   equ     23+E_EDIT
E_SignChange    equ     24+E_EDIT
E_Iterations    equ     25+E_EDIT
E_BadGuess      equ     26+E_EDIT
E_StatPlot      equ     27
E_TolTooSmall   equ     28+E_EDIT
E_Reserved      equ     29+E_EDIT
E_Mode          equ     30+E_EDIT
E_LnkErr        equ     31+E_EDIT
E_LnkMemErr     equ     32+E_EDIT
E_LnkTransErr   equ     33+E_EDIT
E_LnkDupErr     equ     34+E_EDIT
E_LnkMemFull    equ     35+E_EDIT
E_Unknown       equ     36+E_EDIT

E_Scale         equ     37+E_EDIT
E_IdNotFound    equ     38
E_NoMode        equ     39+E_EDIT
E_Validation    equ     40
E_Length        equ     41+E_EDIT
E_Application   equ     42+E_EDIT
E_AppErr1       equ     43+E_EDIT
E_AppErr2       equ     44+E_EDIT
E_ExpiredApp	equ	45
E_BadAddr	equ	46
E_Archived      equ     47+E_EDIT
E_Version       equ     48
E_ArchFull      equ     49
E_Variable      equ     50+E_EDIT
E_Duplicate     equ     51+E_EDIT
HigErrNum       equ     51


; obsolete error numbers34              ;first LINK error
E_LinkIOChkSum	equ	34
E_LinkIOTimeOut	equ	35
E_LinkIOBusy	equ	36
E_LinkIOVer	equ	37

;
;======================================================================
;           equATES TO RAM LOCATIONS FOR STAT VARS 
;======================================================================
;
;
FPLEN	equ	9		;Length of a floating-point number.
StatN   equ     statVars
XMean   equ     StatN      + FPLEN
SumX    equ     XMean      + FPLEN
SumXSqr equ     SumX       + FPLEN
StdX    equ     SumXSqr    + FPLEN
StdPX   equ     StdX       + FPLEN
MinX    equ     StdPX      + FPLEN
MaxX    equ     MinX       + FPLEN
MinY    equ     MaxX       + FPLEN
MaxY    equ     MinY       + FPLEN
YMean   equ     MaxY       + FPLEN
SumY    equ     YMean      + FPLEN
SumYSqr equ     SumY       + FPLEN
StdY    equ     SumYSqr    + FPLEN
StdPY   equ     StdY       + FPLEN
SumXY   equ     StdPY      + FPLEN
Corr    equ     SumXY      + FPLEN
MedX    equ     Corr       + FPLEN
Q1      equ     MedX       + FPLEN
Q3      equ     Q1         + FPLEN
QuadA   equ     Q3         + FPLEN
QuadB   equ     QuadA      + FPLEN
QuadC   equ     QuadB      + FPLEN
CubeD   equ     QuadC      + FPLEN
QuartE  equ     CubeD      + FPLEN
MedX1   equ     QuartE     + FPLEN
MedX2   equ     MedX1      + FPLEN
MedX3   equ     MedX2      + FPLEN
MedY1   equ     MedX3      + FPLEN
MedY2   equ     MedY1      + FPLEN
MedY3   equ     MedY2      + FPLEN
PStat   equ     MedY3      + 2*FPLEN
ZStat   equ     PStat      + FPLEN
TStat   equ     ZStat      + FPLEN
ChiStat equ     TStat      + FPLEN
FStat   equ     ChiStat    + FPLEN
DF      equ     FStat      + FPLEN
Phat    equ     DF         + FPLEN
Phat1   equ     Phat       + FPLEN
Phat2   equ     Phat1      + FPLEN
MeanX1  equ     Phat2      + FPLEN
StdX1   equ     MeanX1     + FPLEN
StatN1  equ     StdX1      + FPLEN
MeanX2  equ     StatN1     + FPLEN
StdX2   equ     MeanX2     + FPLEN
StatN2  equ     StdX2      + FPLEN
StdXP2  equ     StatN2     + FPLEN
SLower  equ     StdXP2     + FPLEN
SUpper  equ     SLower     + FPLEN
SStat   equ     SUpper     + FPLEN

F_DF	equ     anovaf_vars
F_SS	equ     F_DF + FPLEN
F_MS	equ     F_SS + FPLEN
E_DF	equ     F_MS + FPLEN
E_SS	equ     E_DF + FPLEN
E_MS	equ     E_SS + FPLEN


