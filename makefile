CC = gcc
CFLAGS = -Wall -O2 -pthread

all: mainprocess

mainprocess: hw3_q8_page_reclamation.c
	$(CC) $(CFLAGS) -o mainprocess hw3_q8_page_reclamation.c

run:
	@if [ -z "$(word 2, $(MAKECMDGOALS))" ]; then \
		echo "Usage: make run <N> <M>"; \
		echo "<N>: number of pages"; \
		echo "<M>: checker sleep in microseconds"; \
	elif [ -z "$(word 3, $(MAKECMDGOALS))" ]; then \
		echo "You must specify <M> (checker sleep)"; \
	elif ! echo "$(word 2, $(MAKECMDGOALS))" | grep -Eq '^[0-9]+$$'; then \
		echo "First argument (N) must be an integer"; \
	elif ! echo "$(word 3, $(MAKECMDGOALS))" | grep -Eq '^[0-9]+$$'; then \
		echo "Second argument (M) must be an integer"; \
	else \
		./mainprocess $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS)); \
	fi

%:# prevents "no rule to make target" errors
	@:

clean:
	rm -f mainprocess