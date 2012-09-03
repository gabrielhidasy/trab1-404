@ teste05.s - Soma 1 + 2 utilizando rotulos para o enderecamento

LOAD M(rotulo1)
ADD M(rotulo2)

.org 10
rotulo1:
.word 1
rotulo2:
.word 2
