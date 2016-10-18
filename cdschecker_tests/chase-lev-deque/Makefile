include ../benchmarks.mk

TESTNAME = chase-lev-deque

HEADERS = deque.h
OBJECTS = main.o deque.o

all: $(TESTNAME)

$(TESTNAME): $(HEADERS) $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS) $(LDFLAGS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(TESTNAME) *.o
