        .setcpu		"6502"
        .smart		on
        .autoimport	on
        .case		on
        .debuginfo	on
        .macpack	longbranch
        
        .export		init_break
        .export		break
        .export         _debug_init


;; For triggering the VICE monitor, we need to jump to an address
;; that we tell VICE should trigger the break. That address needs
;; to be at a stable position, so we don't have to update the
;; argument passed to x64 all the time. And it can't be code
;; that's otherwise used as that would trigger in other situations.
;; The Basic ROM contains a $60 byte (RTS instruction) in data at
;; position $1f45, but the Basic ROM seems to be disabled by cc65.

;; Instead, use a location in RAM that should not be in use (todo:
;; make it part of the .prg that is built, but at a stable
;; location, how?)

;;; Run `x64 -initbreak 0x02FF ...` (decimal 767)

break = $02FF

init_break:
        lda #$60                ; RTS opcode
        sta break
        rts

_debug_init:
        jmp init_break

;;; (breakonce idea, putting a nop in front of the break, and then
;;; changing that to rts, so it returns before reaching the break
;;; point: the problem is that to change breakstop, we need to
;;; load a value to a register, which spoils the register and
;;; flags. Again, only solvable via BRK. So, solve that instead.)

