make: main.c assemble.h assemble.c semconfig.h semconfig.c paint.h paint.c construction.h construction.c check.h check.c
	gcc main.c assemble.h assemble.c semconfig.h semconfig.c paint.h paint.c construction.h construction.c check.h check.c -lm -o main
