.ps2_ee_vu0

  ;; vi01 = count (how many 16 byte chunks to write)
  ;; vf03 = data (what to put in each 32 bit part of the result)
  ;; vi03 = pointer to next to data buffer

  ;; Load vi01 with vu0_params[0] (x component of data mem location 0)
  nop                         ilw.x vi01, 0(vi00)

  ;; Load vf02 [ 0, 0, vu0_params[1], 0 ]
  nop                         lq.y vf02, 0(vi00)

  ;; Set vf03 to [ 0, 0, 0, 0 ] (copy vi00, which is always 0 to vf03)
  nop                         mfir.xyzw vf03, vi00

  ;; Set vf01 to [ vu0_param[1], vu0_param[1], vu0_param[1], vu0_param[1] ]
  ;; Set vi03 to 0
  addy.xyzw vf01, vf03, vf02y iadd vi03, vi00, vi00

  ;; while(vi01 != 0)
  ;; {
  ;;   vi01--;
  ;;   Copy vf01 [ w, z, y, x ] into location in data mem that vio03 points to
  ;;   vi03++;
  ;; }
loop:
  nop                         isubiu vi01, vi01, 1
  nop                         sqi.xyzw vf01, (vi03++)
  nop                         ibne vi01, vi00, loop
  nop                         nop

  ;; nop[E] will stop the execution of VU0
  nop[E]                      nop
  nop                         nop

