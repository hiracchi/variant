.PHONY: all clean

.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: variant
clean:
	-rm *.o *~


variant: main_variant.o
	$(CXX) -o $@ $^

msgpack-test: msgpack-test.o
	$(CXX) -o $@ $^
