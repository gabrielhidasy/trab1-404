@ teste07.s - Testa a codificacao de opcodes que podem mudar dependendo do
@ destino do rotulo.


laco1:		LOAD M(var_x)
		ADD M(incremento)
		STOR M(var_x)
		LOAD M(referencia)
		SUB M(var_x)
		JUMPPos M(laco1)
		JUMP M(fim_laco_1)
fim_laco_1:	ADD M(var_x)

@ Dados

var_x:		.word 0
incremento:	.word 1
referencia:	.word 9

