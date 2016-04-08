; By jostein.topland@gmail.com
; Hearts ripped from Mikie by Ocean.

          *= $1000

          ; init random
          lda #$ff
          sta $d40e

          sta $d40f
          lda #$80
          sta $d412

          ; clear text
          jsr $e544

          ; background color
          lda #$a
          sta $d020
          sta $d021


          ; write some text
          ldx #0
text      lda message,x
          sec
          sbc #$40 ; some dirty stuff to convert ASCII to PETSCII
          sta $0574,x
          sta $d96e,x
          inx

          cpx #$0e
          bne text

          ; sprite color
          lda #6
          sta $d025
          lda #1
          sta $d026

          ; enable multicolor
          lda #$ff

          sta $d01c

          ; enable sprites
          lda #$ff
          sta $d015

          ; sprite pointers
          ldx #0
          lda #$30
init_ptr  sta $07f8,x
          clc

          adc #1
          inx
          cmp #8
          bne init_ptr

          ; load bitmaps
          ldx #0
init_spr  lda heart1,x
          sta $0c00,x
          lda heart2,x
          sta $0c40,x

          lda heart3,x
          sta $0c80,x
          lda heart4,x
          sta $0cc0,x
          lda heart5,x
          sta $0d00,x
          lda heart4,x
          sta $0d40,x
          lda heart3,x

          sta $0d80,x
          lda heart2,x
          sta $0dc0,x
          lda heart1,x
          sta $0e00,x
          inx
          cpx #$40
          bne init_spr

          ; update for each frame

anim_spr  ldx #0
          ldy #0
_anim_spr lda $07f8,x
          cmp #$38
          bne nextframe

          ; reset sprite
          lda $d41b 
          sta $d027,x ; color
          lda $d41b

          sta $d000,y ; x location
          lda $d41b
          sta $d001,y ; y location
          lda #$30
          sta $07f8,x ; first frame

          nextframe

          ; move hearts up

          lda $d001,y
          sec
          sbc #2
          sta $d001,y

          inc $07f8,x ; next frame
          inx
          iny
          iny
          cpx #8
          bne _anim_spr


          ldx #0
anim_txt  inc $d96e,x
          inx
          cpx #$1B
          bne anim_txt
        
          ; sleep
wait      ldx #$50
wait1     ldy #$ff
wait2     dey
          bne wait2

          dex
          bne wait1

          ; animate
mainloop  jmp anim_spr
          jmp anim_txt
          jmp wait
          jmp mainloop

          rts

message   .text "I`LOVE`MY`Cmvt"


heart1    .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$40,$00
          .byte $01,$90,$00

          .byte $00,$40,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

heart2    .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$40,$00
          .byte $01,$d0,$00
          .byte $01,$90,$00
          .byte $01,$90,$00
          .byte $00,$40,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

heart3    .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

          .byte $01,$10,$00
          .byte $07,$64,$00
          .byte $07,$a4,$00
          .byte $07,$a4,$00
          .byte $06,$a4,$00
          .byte $01,$90,$00
          .byte $00,$40,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00

heart4    .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $05,$14,$00
          .byte $1b,$69,$00
          .byte $1e,$69,$00

          .byte $1e,$69,$00
          .byte $1e,$a9,$00
          .byte $1e,$a9,$00
          .byte $07,$a4,$00
          .byte $06,$a4,$00
          .byte $01,$90,$00
          .byte $00,$40,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00


heart5    .byte $05,$14,$00
          .byte $1a,$69,$00
          .byte $6e,$6a,$40
          .byte $7a,$aa,$40
          .byte $7a,$aa,$40
          .byte $7a,$aa,$40
          .byte $7a,$aa,$40

          .byte $1e,$a9,$00
          .byte $1e,$a9,$00
          .byte $07,$a4,$00
          .byte $07,$a4,$00
          .byte $01,$90,$00
          .byte $01,$90,$00
          .byte $00,$40,$00
          .byte $00,$00,$00

          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00
          .byte $00,$00,$00