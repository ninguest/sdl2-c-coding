build:
	gcc -std=c99 ./*.c -ISDL2\include -LSDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -llibSDL2_image -lSDL2_ttf -o main
run: 
	main
macb:
	gcc -std=c99 ./*.c -o main -Wall -ISDL2/maclibrary/SDL2.framework/Headers -FSDL2/maclibrary -framework SDL2 -ISDL2/maclibrary/SDL2_image.framework/Headers -FSDL2/maclibrary -framework SDL2_image -ISDL2/maclibrary/SDL2_ttf.framework/Headers -FSDL2/maclibrary -framework SDL2_ttf -ISDL2/maclibrary/SDL2_mixer.framework/Headers -FSDL2/maclibrary -framework SDL2_mixer