@ teste12.s - Montagem de um programa para soma de vetores

.org 0
Comeco:
		@ Computa endereco de vetor[i]
		LOAD M(0x069)      @ Carrega a base do vetor e
		ADD M(0x067)       @ Soma com i
		STORAddr M(0x002)  @ Atualiza endereço da instrucao SOMA
		@ Realiza a soma -- Soma = soma + vetor[i]
		LOAD M(0x068)      @ Carrega a variavel soma em AC
		ADD M(0x000)       @ Instrucao SOMA: soma AC com Vetor[i]
		STOR M(0x068)      @ Armazena a soma.
		@ Atualiza i -- i = i+1
		LOAD M(0x067)      @ Carrega i
		ADD M(0x065)       @ Soma com a constante 1
		STOR M(0x067)      @ Armazena i.
		@ Enquanto i < N
		LOAD M(0x066)      @ Carrega N
		SUB M(0x067)       @ AC = N-i
		JUMPPos M(Comeco)  @ Salta para comeco se N-i >= 0
		@ Fim

.org 0x65
		@ Constante 1
		.word 1
		@ N-1 (N=40)
		.word 39
		@ i
		.word 0
		@ Soma
		.word 0
		@ Base do vetor
		.word 0x6A

@ Vetor com 40 posicoes
.word 0x0000000001
.word 0x0000000002
.word 0x0000000004
.word 0x0000000008
.word 0x0000000010
.word 0x0000000020
.word 0x0000000040
.word 0x0000000080
.word 0x0000000100
.word 0x0000000200
.word 0x0000000400
.word 0x0000000800
.word 0x0000001000
.word 0x0000002000
.word 0x0000004000
.word 0x0000008000
.word 0x0000010000
.word 0x0000020000
.word 0x0000040000
.word 0x0000080000
.word 0x0000100000
.word 0x0000200000
.word 0x0000400000
.word 0x0000800000
.word 0x0001000000
.word 0x0002000000
.word 0x0004000000
.word 0x0008000000
.word 0x0010000000
.word 0x0020000000
.word 0x0040000000
.word 0x0080000000
.word 0x0100000000
.word 0x0200000000
.word 0x0400000000
.word 0x0800000000
.word 0x1000000000
.word 0x2000000000
.word 0x4000000000
.word 0x8000000000
