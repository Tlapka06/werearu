all: src/werearu.c src/werearud.c
	mkdir bin
	gcc src/werearu.c -o bin/werearu
	gcc src/werearud.c -o bin/werearud

pack: all
	cp bin/* pkg/usr/bin/
	dpkg-deb --build pkg
	mv pkg.deb werearu.deb

clean:
	rm bin/*
