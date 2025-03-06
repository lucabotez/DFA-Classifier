# Copyright @lucabotez

build:
	g++ classifier.cpp -o classifier

run:
	./classifier

.PHONY: clean

clean:
	rm classifier


