/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * cpc support by SRS  - https://github.com/deringenieur71
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CPC.h"

CPC::CPC()
{
#if 0
  start_org = 0x0138;
  ram_start = 0x0138;
  ram_end = 0xA100;
#endif
}

CPC::~CPC()
{
  //if (need_plot_lores) { add_plot_lores(); }
}

int CPC::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  fprintf(out, "ram_start equ 0x0138\n");
  fprintf(out, "heap_ptr equ 0xA000\n");
  fprintf(out, "save_iy equ heap_ptr\n");

    fprintf(out,
	    "KL_CHOKE_OFF    EQU 0xBCC8\n"
        "KL_ROM_WALK     EQU 0xBCCB\n"
        "KL_INIT_BACK    EQU 0xBCCE\n"
        "KL_LOG_EXT      EQU 0xBCD1\n"
        "KL_FIND_COMMAND EQU 0xBCD4\n"
        "KL_NEW_FRAMEFLY EQU 0xBCD7\n"
        "KL_ADD_FRAMEFLY EQU 0xBCDA\n"
        "KL_DEL_FRAMEFLY EQU 0xBCDD\n"
        "KL_NEW_FAST_TICKER EQU 0xBCE0\n"
        "KL_ADD_FAST_TICKER EQU 0xBCE3\n"
        "KL_DEL_FAST_TICKER EQU 0xBCE6\n"
        "KL_ADD_TICKER      EQU 0xBCE9\n"
        "KL_DEL_TICKER   EQU 0xBCEC\n"
        "KL_INIT_EVENT   EQU 0xBCEF\n"
        "KL_EVENT        EQU 0xBCF2\n"
        "KL_SYNC_RESET   EQU 0xBCF5\n"
        "KL_DEL_SYNCHRONOUS EQU 0xBCF8\n"
        "KL_NEXT_SYNC    EQU 0xBCFB\n"
        "KL_DO_SYNC      EQU 0xBCFE\n"
        "KL_DONE_SYNC    EQU 0xBD01\n"
        "KL_EVENT_DISABLE EQU 0xBD04\n"
        "KL_EVENT_ENABLE EQU 0xBD07\n"
        "KL_DISARM_EVENT EQU 0xBD0A\n"
        "KL_TIME_PLEASE  EQU 0xBD0D\n"
        "KL_TIME_SET     EQU 0xBD10\n");
  
    fprintf(out,
        "KM_INITIALISE   EQU 0xBB00\n"
        "KM_RESET        EQU 0xBB03\n"
        "KM_WAIT_CHAR    EQU 0xBB06\n"
        "KM_READ_CHAR    EQU 0xBB09\n"
        "KM_CHAR_RETURN  EQU 0xBB0C\n"
        "KM_SET_EXPAND   EQU 0xBB0F\n"
        "KM_GET_EXPAND   EQU 0xBB12\n"
        "KM_EXP_BUFFER   EQU 0xBB15\n"
        "KM_WAIT_KEY     EQU 0xBB18\n"
        "KM_READ_KEY     EQU 0xBB1B\n"
        "KM_TEST_KEY     EQU 0xBB1E\n"
        "KM_GET_STATE    EQU 0xBB21\n"
        "KM_GET_JOYSTICK EQU 0xBB24\n"
        "KM_SET_TRANSLATE EQU 0xBB27\n"
        "KM_GET_TRANSLATE EQU 0xBB2A\n"
        "KM_SET_SHIFT    EQU 0xBB2D\n"
        "KM_GET_SHIFT    EQU 0xBB30\n"
        "KM_SET_CONTROL  EQU 0xBB33\n"
        "KM_GET_CONTROL  EQU 0xBB36\n"
        "KM_SET_REPEAT   EQU 0xBB39\n"
        "KM_GET_REPEAT   EQU 0xBB3C\n"
        "KM_SET_DELAY    EQU 0xBB3F\n"
        "KM_GET_DELAY    EQU 0xBB42\n"
        "KM_ARM_BREAK    EQU 0xBB45\n"
        "KM_DISARM_BREAK EQU 0xBB48\n"
        "KM_BREAK_EVENT  EQU 0xBB4B\n");
		
    fprintf(out,
       "TXT_INITIALISE  EQU 0xBB4E\n"
       "TXT_RESET       EQU 0xBB51\n"
       "TXT_VDU_ENABLE  EQU 0xBB54\n"
       "TXT_VDU_DISABLE EQU 0xBB57\n"
       "TXT_OUTPUT      EQU 0xBB5A\n"
       "TXT_WR_CHAR     EQU 0xBB5D\n"
       "TXT_RD_CHAR     EQU 0xBB60\n"
       "TXT_SET_GRAPHIC EQU 0xBB63\n"
       "TXT_WIN_ENABLE  EQU 0xBB66\n"
       "TXT_GET_WINDOW  EQU 0xBB69\n"
       "TXT_CLEAR_WINDOW EQU 0xBB6C\n"
       "TXT_SET_COLUMN  EQU 0xBB6F\n"
       "TXT_SET_ROW     EQU 0xBB72\n"
       "TXT_SET_CURSOR  EQU 0xBB75\n"
       "TXT_GET_CURSOR  EQU 0xBB78\n"
       "TXT_CUR_ENABLE  EQU 0xBB7B\n"
       "TXT_CUR_DISABLE EQU 0xBB7E\n"
       "TXT_CUR_ON      EQU 0xBB81\n"
       "TXT_CUR_OFF     EQU 0xBB84\n"
       "TXT_VALIDATE    EQU 0xBB87\n"
       "TXT_PLACE_CURSOR EQU 0xBB8A\n"
       "TXT_REMOVE_CURSOR EQU 0xBB8D\n"
       "TXT_SET_PEN     EQU 0xBB90\n"
       "TXT_GET_PEN     EQU 0xBB93\n"
       "TXT_SET_PAPER   EQU 0xBB96\n"
       "TXT_GET_PAPER   EQU 0xBB99\n"
       "TXT_INVERSE     EQU 0xBB9C\n"
       "TXT_SET_BACK    EQU 0xBB9F\n"
       "TXT_GET_BACK    EQU 0xBBA2\n"
       "TXT_GET_MATRIX  EQU 0xBBA5\n"
       "TXT_SET_MATRIX  EQU 0xBBA8\n"
       "TXT_SET_M_TABLE EQU 0xBBAB\n"
       "TXT_GET_M_TABLE EQU 0xBBAE\n"
       "TXT_GET_CONTROLS EQU 0xBBB1\n"
       "TXT_STR_SELECT  EQU 0xBBB4\n"
       "TXT_SWAP_STREAMS EQU 0xBBB7\n");  
	   
  fprintf(out,        
		"GRA_INITIALISE  EQU 0xBBBA\n"
        "GRA_RESET       EQU 0xBBBD\n"
        "GRA_MOVE_ABSOLUTE EQU 0xBBC0\n"
        "GRA_MOVE_RELATIVE EQU 0xBBC3\n"
        "GRA_ASK_CURSOR  EQU 0xBBC6\n"
        "GRA_SET_ORIGIN  EQU 0xBBC9\n"
        "GRA_GET_ORIGIN  EQU 0xBBCC\n"
        "GRA_WIN_WIDTH   EQU 0xBBCF\n"
        "GRA_WIN_HEIGHT  EQU 0xBBD2\n"
        "GRA_GET_W_WIDTH EQU 0xBBD5\n"
        "GRA_GET_W_HEIGHT EQU 0xBBD8\n"
        "GRA_CLEAR_WINDOW EQU 0xBBDB\n"
        "GRA_SET_PEN     EQU 0xBBDE\n"
        "GRA_GET_PEN     EQU 0xBBE1\n"
        "GRA_SET_PAPER   EQU 0xBBE4\n"
        "GRA_GET_PAPER   EQU 0xBBE7\n"
        "GRA_PLOT_ABSOLUTE EQU 0xBBEA\n"
        "GRA_PLOT_RELATIVE EQU 0xBBED\n"
        "GRA_TEST_ABSOLUTE EQU 0xBBF0\n"
        "GRA_TEST_RELATIVE EQU 0xBBF3\n"
        "GRA_LINE_ABSOLUTE EQU 0xBBF6\n"
        "GRA_LINE_RELATIVE EQU 0xBBF9\n"
        "GRA_WR_CHAR     EQU 0xBBFC\n");

fprintf(out,  
        "SCR_INITIALISE  EQU 0xBBFF\n"
        "SCR_RESET       EQU 0xBC02\n"
        "SCR_SET_OFFSET  EQU 0xBC05\n"
        "SCR_SET_BASE    EQU 0xBC08\n"
        "SCR_GET_LOCATION EQU 0xBC0B\n"
        "SCR_SET_MODE    EQU 0xBC0E\n"
        "SCR_GET_MODE    EQU 0xBC11\n"
        "SCR_CLEAR       EQU 0xBC14\n"
        "SCR_CHAR_LIMITS EQU 0xBC17\n"
        "SCR_CHAR_POSITION EQU 0xBC1A\n"
        "SCR_DOT_POSITION EQU 0xBC1D\n"
        "SCR_NEXT_BYTE   EQU 0xBC20\n"
        "SCR_PREV_BYTE   EQU 0xBC23\n"
        "SCR_NEXT_LINE   EQU 0xBC26\n"
        "SCR_PREV_LINE   EQU 0xBC29\n"
        "SCR_INK_ENCODE  EQU 0xBC2C\n"
        "SCR_INK_DECODE  EQU 0xBC2F\n"
        "SCR_SET_INK     EQU 0xBC32\n"
        "SCR_GET_INK     EQU 0xBC35\n"
        "SCR_SET_BORDER  EQU 0xBC38\n"
        "SCR_GET_BORDER  EQU 0xBC3B\n"
        "SCR_SET_FLASHING EQU 0xBC3E\n"
        "SCR_GET_FLASHING EQU 0xBC41\n"
        "SCR_FILL_BOX    EQU 0xBC44\n"
        "SCR_FLOOD_BOX   EQU 0xBC17\n"
        "SCR_CHAR_INVERT EQU 0xBC4A\n"
        "SCR_HW_ROLL     EQU 0xBC4D\n"
        "SCR_SW_ROLL     EQU 0xBC50\n"
        "SCR_UNPACK      EQU 0xBC53\n"
        "SCR_REPACK      EQU 0xBC56\n"
        "SCR_ACCESS      EQU 0xBC59\n"
        "SCR_PIXELS      EQU 0xBC5C\n"
        "SCR_HORIZONTAL  EQU 0xBC5F\n"
        "SCR_VERTICAL    EQU 0xBC62\n");
fprintf(out,
        "CAS_INITIALISE  EQU 0xBC65\n"
        "CAS_SET_SPEED   EQU 0xBC68\n"
        "CAS_NOISY       EQU 0xBC6B\n"
        "CAS_START_MOTOR EQU 0xBC6E\n"
        "CAS_STOP_MOTOR  EQU 0xBC71\n"
        "CAS_RESTORE_MOTOR EQU 0xBC74\n"
		"CAS_IN_OPEN	    EQU 0xBC77\n"
		"CAS_IN_CLOSE	EQU 0xBC7A\n"
        "CAS_IN_ABANDON  EQU 0xBC7D\n"
		"CAS_IN_CHAR	    EQU 0xBC80\n"
        "CAS_IN_DIRECT   EQU 0xBC83\n"
        "CAS_RETURN      EQU 0xBC86\n"
        "CAS_TEST_EOF    EQU 0xBC89\n"
		"CAS_OUT_OPEN	EQU 0xBC8C\n"
		"CAS_OUT_CLOSE	EQU 0xBC8F\n"
        "CAS_OUT_ABANDON EQU 0xBC92\n"
		"CAS_OUT_CHAR	EQU 0xBC95\n"
        "CAS_OUT_DIRECT  EQU 0xBC98\n"
        "CAS_CATALOG     EQU 0xBC9B\n"
        "CAS_WRITE       EQU 0xBC9E\n"
        "CAS_READ        EQU 0xBCA1\n"
        "CAS_CHECK       EQU 0xBCA4\n");

fprintf(out,
        "SOUND_RESET     EQU 0xBCA7\n"
        "SOUND_QUEUE     EQU 0xBCAA\n"
        "SOUND_CHECK     EQU 0xBCAD\n"
        "SOUND_ARM_EVENT EQU 0xBCB0\n"
        "SOUND_RELEASE   EQU 0xBCB3\n"
        "SOUND_HOLD      EQU 0xBCB6\n"
        "SOUND_CONTINUE  EQU 0xBCB9\n"
        "SOUND_AMPL_ENVELOPE EQU 0xBCBC\n"
        "SOUND_TONE_ENVELOPE EQU 0xBCBF\n"
        "SOUND_A_ADDRESS EQU 0xBCC2\n"
        "SOUND_T_ADDRESS EQU 0xBCC5\n");

fprintf(out,
        "MC_BOOT_PROGRAM EQU 0xBD13\n"
        "MC_START_PROGRAM EQU 0xBD16\n"
        "MC_WAIT_FLYBACK EQU 0xBD19\n"
        "MC_SET_MODE     EQU 0xBD1C\n"
        "MC_SCREEN_OFFSET EQU 0xBD1F\n"
        "MC_CLEAR_INKS   EQU 0xBD22\n"
        "MC_SET_INKS     EQU 0xBD25\n"
        "MC_RESET_PRINTER EQU 0xBD28\n"
        "MC_PRINT_CHAR   EQU 0xBD2B\n"
        "MC_BUSY_PRINTER EQU 0xBD2E\n"
        "MC_SEND_PRINTER EQU 0xBD31\n"
        "MC_SOUND_REGISTER EQU 0xBD34\n"
        "MC_JUMP_RESTORE EQU 0xBD37\n");
fprintf(out,
         "BIOS_SET_MESSAGE EQU 0xC033\n"
         "BIOS_SETUP_DISC EQU 0xC036\n"
         "BIOS_SELECT_FORMAT EQU 0xC039\n"
         "BIOS_READ_SECTOR EQU 0xC03C\n"
         "BIOS_WRITE_SECTOR EQU 0xC03F\n"
         "BIOS_FORMAT_TRACK EQU 0xC042\n"
         "BIOS_MOVE_TRACK EQU 0xC045\n"
         "BIOS_GET_STATUS EQU 0xC048\n"
         "BIOS_SET_RETRY_COUNT EQU 0xC04B\n"
         "BIOS_GET_SECTOR_DATA EQU 0xC56C\n");

fprintf(out,
     "; 664 + 6128 ONLY\n"
     "KM_SET_LOCKS    EQU 0xBD3A\n"
     "KM_FLUSH        EQU 0xBD3D\n"
     "TXT_ASK_STATE   EQU 0xBD40\n"
     "GRA_DEFAULT     EQU 0xBD43\n"
     "GRA_SET_BACK    EQU 0xBD46\n"
     "GRA_SET_FIRST   EQU 0xBD49\n"
     "GRA_SET_LINE_MASK EQU 0xBD4C\n"
     "GRA_FROM_USER   EQU 0xBD4F\n"
     "GRA_FILL        EQU 0xBD52\n"
     "SCR_SET_POSITION EQU 0xBD55\n"
     "MC_PRINT_TRANSLATION EQU 0xBD58\n"
	"; 6128 ONLY\n"	
     "KL_BANK_SWITCH  EQU 0xBD5B\n");		


  fprintf(out, "\n");
  fprintf(out, ".org 0x0138\n");

  return 0;
}

int CPC::cpc_beep()
{
  fprintf(out, "ld a, 7\n");
  fprintf(out, "call TXT_OUTPUT\n");

  return 0;
}

int CPC::cpc_setTxtPen_I(int c)
{

 if (c < 0 || c > 15)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  call TXT_SET_PEN\n");

  return 0;
}

int CPC::cpc_setTxtPen_I()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call TXT_SET_PEN\n");

  return 0;
}

int CPC::cpc_setTxtPaper_I(int c)
{
 if (c < 0 || c > 15)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  call TXT_SET_PAPER\n");

  return 0;
}

int CPC::cpc_setTxtPaper_I()
{
  fprintf(out, "  ; setTxtPaper()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call TXT_SET_PAPER\n");

  return 0;
}

int CPC::cpc_setGraPen_I(int c)
{
  if (c < 0 || c > 15)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PEN\n");

  return 0;
}

int CPC::cpc_setGraPen_I()
{
  fprintf(out, "  ; setGraPen()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PEN\n");

  return 0;
}

int CPC::cpc_setGraPaper_I(int c)
{
   if (c < 0 || c > 15)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PAPER\n");

  return 0;
}

int CPC::cpc_setGraPaper_I()
{
  fprintf(out, "  ; setGraPaper()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PAPER\n");

  return 0;
}

int CPC::cpc_setBorderColor_I()
{
  fprintf(out, "  ; setBorderColor()\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  call SCR_SET_BORDER\n");

  return 0;
}

int CPC::cpc_setBorderColor_I(int c)
{
   if (c < 0 || c > 15)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld c, 0x%02x\n", c);
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  call SCR_SET_BORDER\n");

  return 0;
}

int CPC::cpc_screen_I()
{
  fprintf(out, "  ; screen()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "call SCR_SET_MODE\n");

  return 0;
}

int CPC::cpc_screen_I(int c)
{
  if (c < 0 || c > 2)
  {
    printf("Error: Color index out of range\n");
    return -1;
  }
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  ld a,l\n");
  fprintf(out, "call SCR_SET_MODE\n");

  return 0;
}

int CPC::cpc_cls()
{
  fprintf(out, "  ; cls()\n");
  fprintf(out, "  call SCR_CLEAR\n");

  return 0;
}

int CPC::cpc_setCursor_II()
{
  fprintf(out, "  ; setCursor_II()\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  ld l,e\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  ld h,e\n");
  fprintf(out, "  call TXT_SET_CURSOR\n");
  
  return 0;
}

int CPC::cpc_putChar_C(char c)
{
  fprintf(out, "  ld a, 0x%02x\n", c);
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call TXT_OUTPUT\n");

  return 0;
}

int CPC::cpc_putChar_C()
{
  fprintf(out, "  ; putChar_C()\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call TXT_OUTPUT\n");

  return 0;
}

char CPC::cpc_readChar()
{
  fprintf(out, "  ; readChar()\n");
  fprintf(out, "  call KM_WAIT_CHAR\n");
  fprintf(out, "  ld l,a\n");
  fprintf(out, "  ld h,0\n");  
  fprintf(out, "  push hl\n");  
  
  return 0;
}



int CPC::cpc_plot_III()
{
  fprintf(out, "  ; plot_III()\n");
  fprintf(out, "  pop hl\n"); // get 
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PEN\n");
  fprintf(out, "  pop hl\n"); // get y
  fprintf(out, "  pop de\n"); // get x
  fprintf(out, "  call GRA_PLOT_ABSOLUTE\n");

  return 0;
}

int CPC::cpc_draw_III()
{
  fprintf(out, "  ; draw_III()\n");
  fprintf(out, "  pop hl\n"); // get 
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call GRA_SET_PEN\n");
  fprintf(out, "  pop hl\n"); // get y
  fprintf(out, "  pop de\n"); // get x
  fprintf(out, "  call GRA_LINE_ABSOLUTE\n");

  return 0;
}

