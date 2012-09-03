@ teste11.s - Montagem de um programa para computar a expressao
@   (234h + 3h) * (899h + 23h)

	LOADMQMem M(op1)
	MUL M(op2)
	LOADMQ
	STOR M(temp)
	LOADMQMem M(op3)
	MUL M(op4)
	LOADMQ
	ADD M(temp)

@ Fim -- Resultado esta em AC

@ Dados

temp:	.word 0
op1:	.word 0x234
op2:	.word 3
op3:	.word 0x899
op4:	.word 0x23
