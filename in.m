ADD M(3) 
@ teste02.s - testa diretivas basicas do montador
@ teste vai dar pau
ADD M(3)
labelx:
ADD M(10)
.align 1
.word 64202
jump m(labelc)
.align 1
.word -10
DIV M(4)
.align 11
labeljucara: 
.word 0x9
.set CAO 1010
.word 0x9
.word 0x10000
.org 0o200
@.word 0x100000
.word 0x99090
.word 0o377777
@Comment inutil
.word 0o17777768777777
.align 20
.word 0b1101110110101011101010101101001011101011
.word 0b1010
.word CAO
.org 0x150
labelc:
.wfill 10,100
add M(4)
addmod M(93)
add M(labelc)
sub M(96)
sub M(54)
.org 0x14E
.wfill 5,50
jump M(labelx)
caca:
