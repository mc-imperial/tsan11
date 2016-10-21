include ../benchmarks.mk

TESTNAME = ms-queue

HEADERS = my_queue.h
OBJECTS = main.o my_queue.o

all: $(TESTNAME)

$(TESTNAME): $(HEADERS) $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(CXXFLAGS) $(LDFLAGS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f $(TESTNAME) *.o
