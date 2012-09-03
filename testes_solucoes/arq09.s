@ teste09.s - Montagem do programa balistico do laboratorio 1
@
@ Programa para calculo balistico
@
@ Entrada: O dado de entrada x esta armazenada no endereco 0x00100
@
@ O programa deve multiplicar x por 10 e computar a raiz quadrada
@ utilizando o metodo de aproximacao descrito. O numero de iteracoes
@ do metodo deve ser 10.
@
@ Saida: O resultado sera armazenado em AC


@ Carregar o dado de entrada em MQ (para multiplicacao)
LOADMQMem M(0x100)
@ Multiplicar o valor por 10
MUL M(0x101)  @ M(x0101) contém a constante 10.

@ Salvar o resultado em Y
LOADMQ        @ AC <= MQ
STOR M(0x103) @ M(0x102) <= AC

@ ---------------------------
@ Computar o primeiro X e salvar.
@ X = Y/2
@ Y ja esta em AC
RSH
STOR M(0x102)

@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
@ AC <= Y
LOAD M(0x103)
@ MQ <= AC / M(X)
DIV M(0x102)
@ AC <= MQ
LOADMQ
@ AC <= AC + M(X)
ADD M(0x102)
@ AC <= AC >> 1
RSH
@ M(X) <= AC
STOR M(0x102)
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------
@ Computar X' e salvar em X
@ X'= (X + (Y/X)) / 2
LOAD M(0x103)
DIV M(0x102)
LOADMQ
ADD M(0x102)
RSH
STOR M(0x102)
@ ---------------------------
@ ---------------------------


@ ---------------------------
.org 0x100
@ Dado 3500 decimal
.word 3500
@ Constante 10
.word 0xA
@ Temporario X (X' fica em AC)
.word 0
@ Temporario Y (entrada * 10)
.word 0


