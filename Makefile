CC?=gcc

all: src/werearu.c src/werearud.c
	mkdir -p bin
	$(CC) src/werearu.c -o bin/werearu
	$(CC) src/werearud.c -o bin/werearud

pack: all
	if ! type dpkg-deb > /dev/nul then
		$(error dpkg-deb is not installed (Maybe not under Debian?))
	fi
	chmod 755 pkg/DEBIAN/p*
	cp bin/* pkg/usr/bin/
	dpkg-deb --build pkg
	mv pkg.deb werearu.deb

clean:
	rm bin/*
