/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Playstation2.h"

void Playstation2::add_vu0_code()
{
  // Upload VU0 code from EE to VU0
  fprintf(out,
    "  ;; _vu0_upload_code()\n"
    "_vu0_upload_code:\n"
    "  li $v0, VU0_MICRO_MEM\n"
    "  lw $a2, -4($a0)\n"
    "_vu0_upload_code_loop:\n"
    "  lq $v1, ($a0)\n"
    "  sq $v1, ($v0)\n"
    "  addiu $a0, $a0, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a2, $a2, -16\n"
    "  bgtz $a2, _vu0_upload_code_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");

  // Upload VU0 data from EE to VU0 as words (int or float)
  fprintf(out,
    "  ;; _vu0_upload_data_words()\n"
    "_vu0_upload_data_words:\n"
    "  li $v0, VU0_VU_MEM\n"
    "  lw $a2, -4($a1)\n"
    "  sll $a0, $a0, 4\n"
    "  addu $v0, $v0, $a0\n"
    "_vu0_upload_data_words_loop:\n"
    "  lq $v1, ($a1)\n"
    "  sq $v1, ($v0)\n"
    "  addiu $a1, $a1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a2, $a2, -4\n"
    "  bgtz $a2, _vu0_upload_data_words_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");

  // Upload VU0 data from EE to VU0 as bytes
  fprintf(out,
    "  ;; _vu0_upload_data_bytes()\n"
    "_vu0_upload_data_bytes:\n"
    "  li $v0, VU0_VU_MEM\n"
    "  lw $a2, -4($a1)\n"
    "  sll $a0, $a0, 4\n"
    "  addu $v0, $v0, $a0\n"
    "_vu0_upload_data_bytes_loop:\n"
    "  lq $v1, ($a1)\n"
    "  sq $v1, ($v0)\n"
    "  addiu $a1, $a1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a2, $a2, -16\n"
    "  bgtz $a2, _vu0_upload_data_bytes_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");

  // Download VU0 data to EE from VU0 as words (int or float)
  fprintf(out,
    "  ;; _vu0_download_data_words()\n"
    "_vu0_download_data_words:\n"
    "  li $v0, VU0_VU_MEM\n"
    "  lw $a2, -4($a1)\n"
    "  sll $a0, $a0, 4\n"
    "  addu $v0, $v0, $a0\n"
    "_vu0_download_data_words_loop:\n"
    "  lq $v1, ($v0)\n"
    "  sq $v1, ($a1)\n"
    "  addiu $a1, $a1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a2, $a2, -4\n"
    "  bgtz $a2, _vu0_download_data_words_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");

  // Download VU0 data to EE from VU0 as bytes
  fprintf(out,
    "  ;; _vu0_download_data_bytes()\n"
    "_vu0_download_data_bytes:\n"
    "  li $v0, VU0_VU_MEM\n"
    "  lw $a2, -4($a1)\n"
    "  sll $a0, $a0, 4\n"
    "  addu $v0, $v0, $a0\n"
    "_vu0_download_data_bytes_loop:\n"
    "  lq $v1, ($v0)\n"
    "  sq $v1, ($a1)\n"
    "  addiu $a1, $a1, 16\n"
    "  addiu $v0, $v0, 16\n"
    "  addiu $a2, $a2, -16\n"
    "  bgtz $a2, _vu0_download_data_bytes_loop\n"
    "  nop\n"
    "  jr $ra\n"
    "  nop\n");
}

