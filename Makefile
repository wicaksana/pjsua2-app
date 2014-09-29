#http://trac.pjsip.org/repos/wiki/Getting_Started_Using
all: myapp

myapp: myapp.cpp
	$(CC) -o $@ $< `pkg-config --cflags --libs libpjproject`

clean:
	rm -f myapp.o myapp
