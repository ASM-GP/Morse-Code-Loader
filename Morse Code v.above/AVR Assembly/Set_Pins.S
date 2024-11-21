;call "function"
;CBi DDRB, 3          ; CBI -> set pin as i/p
;CPI R24, 0x00        ; compare somthing
;BREQ input_output_inputPullup ; branch equal
;RJMP "function name"  ; calls function
;SUBI                  ; - increments a varaible
;in                    ; used to copy the data from a port to a register

; Define the I/O register addresses
#define __SFR_OFFSET 0x00
#include <avr/io.h>

.global Set_Pin

.section .text
Set_Pin:
    CP r24, 2
    BRGE check_if_less_than_max ; branches if equal to or greater than 2
    RET                         ; return from the function

check_if_less_than_max:       ; checks if it is less than or equal tor12
    CP 12, R24
    BRGE pin_is_in_scope      ; is inside pin scope for MEGA 2560

pin_is_in_scope:
    ; case for 2 and 3
    CP 3, R24
    BRGE handle_DDRE          ; less than or equal to 
    ; Case for pin 4
    CP R24, 4
    BREQ handle_DDRG
    ; Case for pin 5
    CP R24, 5
    BREQ handle_DDRE_5
    ; Cases for pins 6 to 9
    CP 9, R24                ; less than or equal to
    BRGE handle_DDRH
    ; Cases for pinsr10 tor12
    CP 12, R24
    BRGE handle_DDRB         ; less than or equal to
    ; Case for pin 44
    CPI R24, 44
    BREQ handle_DDL_5
    ; Case for pin 45
    CPI R24, 45
    BREQ handle_DDL_4
    ; Case for pin 46
    CPI R24, 46
    BREQ handle_DDL_3

handle_DDRE:
    MOV R18, R24             ; load 0 into general purpose register 18
    ADD R18, 2
    LDI R30, lo8(DDRE)       ; gets lower 8 bits of the address
    LDI R31, hi8(DDRE)       ; gets higher 8 bits of the address
    RCALL check_pin_type

handle_DDRG:
    LDI R18, DDG5
    LDI R30, lo8(DDRG)
    LDI R31, hi8(DDRG)
    RCALL check_pin_type

handle_DDRE_5:
    LDI R18, DDE3
    LDI R30, lo8(DDRE)
    LDI R31, hi8(DDRE)
    RCALL check_pin_type

handle_DDRH:
    MOV R18, R24
    SUB R18, 3                  ; subtract 3
    LDI R30, lo8(DDRH)
    LDI R31, hi8(DDRH)
    RCALL check_pin_type

handle_DDRB:
    MOV R18, R24
    SUB R18, 6                  ; subtract 6
    LDI R30, lo8(DDRB)
    LDI R31, hi8(DDRB)
    RCALL check_pin_type

handle_DDL_5:
    LDI R18, DDL5
    LDI R30, lo8(DDRL)
    LDI R31, hi8(DDRL)    
    RCALL check_pin_type

handle_DDL_4:
    LDI R18, DDL4
    LDI R30, lo8(DDRL)
    LDI R31, hi8(DDRL)
    RCALL check_pin_type

handle_DDL_3:
    LDI R18, DDL3
    LDI R30, lo8(DDRL)
    LDI R31, hi8(DDRL)
    RCALL check_pin_type

check_pin_type:
    CPi R22, 0
    BREQ setting_pin_input
    CPI R22, 1
    BREQ setting_pin_output
    CPI R22, 2
    BREQ setting_pin_input_pullup
    RJMP end_function                ; guaranteed to stop program

setting_pin_input:
    LD R20, Z
    AND R20, R18
    ST Z, R20

setting_pin_output:
    LD R20, Z
    OR R20, R18
    ST Z, R20
    
setting_pin_input_pullup:
    LD R20, Z
    AND R20, R18
    ST Z, R20

handle_PINE:

handle_PING:

handle_PINE_5:

handle_PINH:

handle_PINB:

handle_PINL_5:

handlePINL_4:

handle_PINL_3:


end_function:
    RET