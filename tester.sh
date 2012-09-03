#!/bin/bash
for i in $(ls testes_solucoes/*.s)
 do
 echo $(echo $i | cut -d '.' -f 1-1 )
 $(./montador-ias $i $(echo $i | cut -d '.' -f 1-1 ).cand)
 diff ./$(echo $i | cut -d '.' -f 1-1 ).res ./$(echo $i | cut -d '.' -f 1-1 ).cand
 done
