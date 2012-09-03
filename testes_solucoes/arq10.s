@ teste10.s - Montagem de um programa para calculo de fatorial
@ usando instrucoes do IAS

Aqui:
	LOADMQMem M(0x102)
	MUL M(0x103)
	LOADMQ
	STOR M(0x102)
	LOAD M(0x103)
	ADD M(0x101)
	STOR M(0x103)
	LOAD M(0x100)
	SUB M(0x103)
	JUMPPos M(Aqui)

.org 0x100
@ N – Para N=10 (0xA), o resultado e 3628800 (375F00)
.word 10
@ Constante 1
.word 1
@ Resultado do Fatorial
.word 1
@ i
.word 1
