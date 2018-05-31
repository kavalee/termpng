termpng: render.c
	gcc render.c -lpng -o termpng
run: termpng
	./termpng 
