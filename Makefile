all: 
	./make.sh

test:
	./make.sh
	bin/main

clean:
	rm -rf bin
	rm -rf build
