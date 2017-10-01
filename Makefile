CXXFLAGS = -I./msgpack-c/include

.PHONY: all clean

.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: variant variant_msgpack
clean:
	-rm *.o *~


variant: main_variant.o
	$(CXX) -o $@ $^

variant_msgpack: main_variant_msgpack.o
	$(CXX) -o $@ $^
