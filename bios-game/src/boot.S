/* Sektor boot floppy: muat 16 sektor berikutnya ke alamat 0000:8000. */
.code16
.text
.global _start
_start:
    ljmp $0, $entry
entry:
    cli
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movl $0x7c00, %esp
    sti
    movb %dl, boot_drive
    movw $3, %si

read_game:
    xorw %ax, %ax
    movw %ax, %es
    movw $0x8000, %bx
    movw $0x0210, %ax       /* INT 13h AH=02: baca 16 sektor */
    movw $0x0002, %cx       /* silinder 0, sektor 2 */
    xorw %dx, %dx           /* head 0 */
    movb boot_drive, %dl
    int $0x13
    jnc loaded

    xorw %ax, %ax           /* reset drive sebelum mencoba lagi */
    movb boot_drive, %dl
    int $0x13
    decw %si
    jnz read_game

    movw $0x0e45, %ax       /* tampilkan 'E' jika floppy gagal dibaca */
    movw $7, %bx
    int $0x10
halt:
    hlt
    jmp halt

loaded:
    ljmp $0, $0x8000

boot_drive: .byte 0
.org 510
.word 0xaa55
