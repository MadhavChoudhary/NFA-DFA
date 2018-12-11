# NFA-DFA-Converter + Simulator

## Features:

State table input can be given through files. ex:in.txt  
If the Automata is Non-Deterministic, the program can still check the string for language and then also converts the NFA to th equivalent DFA.  

In the output of state table, '( )' represents end state and '[ ]' represents start state.  

## Steps:
``
g++ FA.cpp  
./a.out 			//to give input of each transition manually  
./a.out <in.txt 	//to give input from in.txt  
``

3 Example input files have been attached for DFA, NFA, and Complex Models.  