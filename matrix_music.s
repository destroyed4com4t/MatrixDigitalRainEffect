
FT_DPCM_PTR=$c000
	.export _music_data
;this file for FamiTone2 library generated by text2data tool
_music_data:
	.byte 1
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,307,256

@instruments:
	.byte $70 ;instrument $02
	.word @env1,@env3,@env0
	.byte $00
	.byte $b0 ;instrument $03
	.word @env2,@env0,@env0
	.byte $00
	.byte $70 ;instrument $04
	.word @env1,@env4,@env0
	.byte $00

@samples:
@env0:
	.byte $c0,$00,$00
@env1:
	.byte $cc,$c9,$c8,$c5,$c4,$c2,$c3,$02,$c2,$03,$c2,$00,$0a
@env2:
	.byte $cc,$ca,$c8,$c6,$c4,$c2,$c0,$00,$06
@env3:
	.byte $c0,$c1,$c0,$00,$02
@env4:
	.byte $c0,$c0,$c3,$c3,$c7,$c7,$c0,$c0,$00,$00


@song0ch0:
	.byte $fb,$07
@song0ch0loop:
@ref0:
	.byte $80,$32,$34,$32,$34,$32,$34,$32,$34,$32,$34,$32,$34,$32,$34,$32
	.byte $34,$3c,$32,$3c,$32,$3c,$32,$3c,$32,$3c,$32,$3c,$32,$3c,$32,$3c
	.byte $32
@ref1:
	.byte $40,$3c,$40,$3c,$40,$3c,$40,$3c,$40,$3c,$40,$3c,$40,$3c,$40,$3c
	.byte $84,$40,$9d
	.byte $ff,$20
	.word @ref0
	.byte $ff,$12
	.word @ref1
@ref4:
	.byte $82,$33,$41,$3f,$39,$33,$34,$85,$32,$85,$28,$85,$29,$2b,$29,$27
	.byte $20,$81
@ref5:
	.byte $33,$41,$3f,$39,$33,$34,$85,$32,$89,$1a,$28,$1a,$28,$1c,$26,$1c
	.byte $26,$1a,$28,$1a,$28
@ref6:
	.byte $2e,$38,$2e,$38,$28,$36,$28,$36,$2e,$38,$2e,$38,$28,$36,$28,$36
	.byte $36,$38,$36,$38,$32,$36,$32,$36,$36,$38,$36,$38,$32,$36,$32,$84
	.byte $36
@ref7:
	.byte $bf
@ref8:
	.byte $82,$4b,$59,$57,$51,$4b,$4c,$85,$4a,$85,$40,$85,$41,$43,$41,$3f
	.byte $38,$81
@ref9:
	.byte $4b,$59,$57,$51,$4b,$4c,$85,$4a,$89,$33,$33,$37,$37,$39,$38,$81
	.byte $ff,$20
	.word @ref6
@ref11:
	.byte $bf
	.byte $fd
	.word @song0ch0loop

@song0ch1:
@song0ch1loop:
@ref12:
	.byte $82,$33,$32,$32,$35,$33,$2f,$33,$2b,$2f,$1b,$1a,$1a,$1d,$1b,$17
	.byte $1b,$13,$16,$81
@ref13:
	.byte $33,$32,$32,$35,$33,$2f,$33,$2b,$2f,$1b,$1b,$1a,$85,$1b,$1b,$1a
	.byte $85
@ref14:
	.byte $4b,$4a,$4a,$4d,$4b,$47,$4b,$43,$47,$33,$32,$32,$35,$33,$2f,$33
	.byte $2b,$2e,$81
@ref15:
	.byte $4b,$4a,$4a,$4d,$4b,$47,$4b,$43,$47,$33,$33,$32,$85,$33,$33,$32
	.byte $85
@ref16:
	.byte $4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50
	.byte $4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50
@ref17:
	.byte $4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50,$4a,$50,$58,$50
	.byte $4a,$50,$58,$50,$70,$50,$58,$50,$66,$50,$58,$50,$68,$50,$58,$50
@ref18:
	.byte $4a,$8d,$50,$58,$51,$4e,$85,$4e,$8d,$38,$40,$39,$36,$85
@ref19:
	.byte $bf
	.byte $ff,$20
	.word @ref16
	.byte $ff,$20
	.word @ref17
	.byte $ff,$0e
	.word @ref18
@ref23:
	.byte $bf
	.byte $fd
	.word @song0ch1loop

@song0ch2:
@song0ch2loop:
@ref24:
	.byte $80,$33,$32,$32,$35,$33,$2f,$33,$2b,$2f,$1b,$1a,$1a,$1d,$1b,$17
	.byte $1b,$13,$16,$81
	.byte $ff,$11
	.word @ref13
@ref26:
	.byte $33,$32,$32,$35,$33,$2f,$33,$2b,$2f,$1b,$1a,$1a,$1d,$1b,$17,$1b
	.byte $13,$16,$81
	.byte $ff,$11
	.word @ref13
@ref28:
	.byte $32,$89,$32,$85,$32,$85,$33,$32,$89,$32,$85,$32,$85,$32,$81
@ref29:
	.byte $32,$89,$32,$85,$32,$85,$33,$32,$89,$33,$32,$8d
@ref30:
	.byte $43,$43,$47,$43,$4b,$42,$85,$42,$46,$5b,$5b,$5f,$5b,$63,$5a,$85
	.byte $42,$84,$2e
@ref31:
	.byte $bf
@ref32:
	.byte $80,$32,$89,$32,$85,$32,$85,$33,$32,$89,$32,$85,$32,$85,$32,$81
	.byte $ff,$0c
	.word @ref29
	.byte $ff,$12
	.word @ref30
@ref35:
	.byte $bf
	.byte $fd
	.word @song0ch2loop

@song0ch3:
@song0ch3loop:
@ref36:
	.byte $82,$05,$1e,$1e,$13,$1e,$1e,$05,$1e,$1e,$13,$1e,$1e,$05,$1e,$1e
	.byte $13,$1e,$1e,$05,$1e,$1e,$13,$1e,$1e
@ref37:
	.byte $05,$1e,$1e,$13,$1e,$1e,$05,$1e,$1e,$13,$1e,$1e,$05,$1e,$1e,$13
	.byte $1e,$1e,$05,$1e,$1e,$13,$1e,$1e
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $ff,$18
	.word @ref37
	.byte $fd
	.word @song0ch3loop

@song0ch4:
@song0ch4loop:
@ref48:
	.byte $bf
@ref49:
	.byte $bf
@ref50:
	.byte $bf
@ref51:
	.byte $bf
@ref52:
	.byte $bf
@ref53:
	.byte $bf
@ref54:
	.byte $bf
@ref55:
	.byte $bf
@ref56:
	.byte $bf
@ref57:
	.byte $bf
@ref58:
	.byte $bf
@ref59:
	.byte $bf
	.byte $fd
	.word @song0ch4loop