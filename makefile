a.out: render.c
	gcc render.c -lpng
run: a.out
	./a.out
