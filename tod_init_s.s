;; XX docs for these?

;;	.fopt		compiler,"cc65 v 2.18 - Debian 2.19-1"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.macpack	longbranch
;;	.forceimport	__STARTUP__
	.export		_tod_calibrate

.segment	"RODATA"

;; https://codebase64.org/doku.php?id=base:tod_calibration
;; base tod_calibration [Codebase 64 wiki].html

tod_cia		= $dc08			; or $dd08

_tod_calibrate:
		ldy #255
		jsr tod_prescale	; use 50 Hz

		php			; no interrupts
		sei			; while measuring
		jsr tod_measure
		jsr tod_measure
		plp
tod_prescale:   lda tod_cia+6
		asl a
		cpy #204		; threshold
		ror a
		sta tod_cia+6		; set pre-scaler
		rts

tod_measure:    ldy #0
		sty tod_cia
tod_lp2:	lda #64
tod_lp:         cpx $d012
		beq tod_lp
		ldx $d012
		lsr a
		bne tod_lp
		lda tod_cia
		bne tod_done
		iny
		bne tod_lp2
tod_done:       rts
