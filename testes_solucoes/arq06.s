@ teste06.s - Testa a codificacao de todas as instrucoes do IAS


	LOADMQ
	LOADMQMem M(0)
	STOR M(0)
	LOAD M(0)
	LOADNeg M(0)
	LOADMod M(0)
	JUMP M(rotulo)
rotulo:	JUMPPos M(rotul2)
rotul2: ADD M(0)
	ADDMod M(0)
	SUB M(0)
	SUBMod M(0)
	MUL M(0)
	DIV M(0)
	LSH
	RSH
	STORAddr M(0)
