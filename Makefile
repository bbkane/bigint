all: bigint

.PHONY: test

test: bigint
	python test/test_add.py

clean:
	rm bigint

bigint: main.c
	clang -o bigint -Wall -Wextra -pedantic -Wno-gnu-statement-expression main.c
