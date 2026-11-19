/* Penghubung kecil antara C++ 32-bit ABI (kode 16-bit) dan BIOS/VGA. */
.code16gcc
.section .start,"ax"
.global game_start
game_start:
    movw $0x0013, %ax      /* mode VGA 320 x 200, 256 warna */
    int $0x10
    movw $0x0305, %ax      /* BIOS keyboard: ulangi tombol yang ditahan lebih cepat */
    xorw %bx, %bx
    int $0x16
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    cld
    sti
    calll pixel_main
1:
    hlt
    jmp 1b

.text
.global bios_key
bios_key:
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    pushw %ds
    pushw %es
    movb $1, %ah
    int $0x16             /* cek apakah ada tombol tanpa menunggu */
    jz 1f
    xorb %ah, %ah
    int $0x16             /* AH = scan code, AL = ASCII */
    movzwl %ax, %eax
    jmp 2f
1:
    xorl %eax, %eax
2:
    popw %es
    popw %ds
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    cld
    retl

.global canvas_span
canvas_span:
    movzwl 4(%esp), %edx   /* offset awal */
    movzwl 8(%esp), %ecx   /* jumlah pixel */
    movb 12(%esp), %al     /* indeks warna */
    pushl %edi
    pushw %es
    movw $0x1000, %di
    movw %di, %es
    movl %edx, %edi
    rep stosb              /* gambar ke buffer, belum ke layar */
    popw %es
    popl %edi
    retl

.global vga_present
vga_present:
    pushl %esi
    pushl %edi
    pushw %ds
    pushw %es
    movw $0x1000, %ax
    movw %ax, %ds
    movw $0xa000, %ax
    movw %ax, %es
    xorl %esi, %esi
    xorl %edi, %edi
    movl $16000, %ecx
    rep movsl              /* 16.000 x 4 byte = 320 x 200 pixel */
    popw %es
    popw %ds
    popl %edi
    popl %esi
    retl
