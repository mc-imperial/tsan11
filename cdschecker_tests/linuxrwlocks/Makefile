include ../benchmarks.mk

TESTNAME = linuxrwlocks

all: $(TESTNAME)

$(TESTNAME): $(TESTNAME).cc
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
