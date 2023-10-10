build:
	gcc -std=c99 ./*.c -ISDL2\include -LSDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -llibSDL2_image -lSDL2_ttf -o main
run: 
	main
macbuild: 
	gcc -std=c99 ./*.c -F/SDL2/maclibrary -I/SDL2/maclibrary/sdl2/Headers -I/SDL2/maclibrary/sdl2image/Headers -I/SDL2/maclibrary/sdl2mixer/Headers -I/SDL2/maclibrary/sdl2ttf/Headers -framework SDL2 -framework SDL2_mixer -framework SDL2_image -framework SDL2_ttf -o main
macb:
	gcc -std=c99 ./*.c -o main -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2_image.framework/Headers -F/Library/Frameworks -framework SDL2_image -I/Library/Frameworks/SDL2_ttf.framework/Headers -F/Library/Frameworks -framework SDL2_ttf