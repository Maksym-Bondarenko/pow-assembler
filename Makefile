make_project:
	@gcc -O3 Rahmenprogramm.c tests.c main.c f.s pow_int.s exponent.s ln.s ./Alternativer_Ansatz/f_alternativ.s ./Alternativer_Ansatz/ln_alternativ.s ./Alternativer_Ansatz/simpson.s -o main -lm 
	make -C ../Ausarbeitung/
clean:
	make clean -C ../Ausarbeitung/