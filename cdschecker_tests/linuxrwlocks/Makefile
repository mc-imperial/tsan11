include ../benchmarks.mk

TESTNAME = linuxrwlocks

all: $(TESTNAME)

$(TESTNAME): $(TESTNAME).cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
