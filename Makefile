build:
	gcc -std=c99 ./*.c -ISDL2\include -LSDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -llibSDL2_image -lSDL2_ttf -o main
run: 
	main
macbuild: 
	gcc -std=c99 ./*.c -I/SDL2/maclibrary/Headers -F/SDL2/maclibrary -framework SDL2 -framework SDL2_mixer -framework SDL2_image -framework SDL2_ttf -o main