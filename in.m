@ teste02.s - testa diretivas basicas do montador
labelx:
ADD M(10)
.align 1
.word 64202
.word -1
.word -10
.word 0x9
.word 0x10000
.org 0o200
.word 0x100000
.word 0x99090
.word 0o377777
.word 0o17777768777777
.align 20
.word 0b1101110110101011101010101101001011101011
.word 0b1010
.word 0xFFFFFFFFFF
.org 0x150
.wfill 10,100
add M(4)
add M(5)
sub M(96)
sub M(54)
