@ teste04.s - Testa criacao de rotulos e constantes em outras bases

laco1:	ADD M(_refrigerante)
	ADD M(acompanhamento_)

.align 10
_refrigerante: .word 49354
	.word 0
acompanhamento_:
	.word 45072
	.word 0xDAC
	.word 0o777
	.word 0
	.word 0xFFFFFFFFFF
	.word 0
	.word 0b110
	.word 0b1000
	.word 0b00001111
