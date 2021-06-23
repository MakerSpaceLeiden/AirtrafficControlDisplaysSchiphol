BOARDS=50228 50229

all:
	for dir in $(BOARDS); do make -C $$dir; done
