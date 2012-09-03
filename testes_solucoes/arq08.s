@ teste08.s - Testa a codificacao de opcodes que podem mudar dependendo do
@ destino do rotulo.

laco:		LOAD M(soma)
acesso_vetor:	ADD M(vetor)
acesso_vetor2:	ADD M(vetor)
		STOR M(soma)
		LOAD M(i)
		ADD M(incr)
		STOR M(i)
		ADD M(vetor_ptr)
		STORAddr M(acesso_vetor)
		STORAddr M(acesso_vetor2)
		LOAD M(N)
		SUB M(i)
		JUMPPos M(laco)

@ Dados
.align 1
soma:		.word 0
i:		.word 0
incr:		.word 1
N:		.word 9
vetor_ptr:	.word vetor
vetor:		.wfill 10, 1

