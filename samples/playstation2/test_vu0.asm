.ps2_ee_vu0

  ;; vi01 = count (how many 16 byte chunks to write)
  ;; vi02 = data (what to put in each 32 bit part of the result)
  ;; vi03 = pointer to next to data buffer
  nop                         ilw.x vi01, 0(vi00)
  nop                         lq.y vf02, 0(vi00)
  nop                         mfir.xyzw vf03, vi00
  addy.xyzw vf01, vf03, vf02y iadd vi03, vi00, vi00

loop:
  nop                         isubiu vi01, vi01, 1
  nop                         sqi.xyzw vf01, (vi03++)
  nop                         ibne vi01, vi00, loop
  nop                         nop
  nop[E]                      nop
  nop                         nop

