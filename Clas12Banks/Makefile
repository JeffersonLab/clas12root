
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

LIBDIR  = ../lib
BINDIR  = ../bin

LZ4INCLUDES := -I../Lz4/lib/ -I../Hipo

libhipo: checkdirs $(OBJECTS)
	ar cr ../lib/libclas12banks.a $(OBJECTS)
#	$(CXX) -shared -fPIC  -o ../lib/libclas12banks.so $(OBJECTS) -L../Lz4/lib/ -llz4


show:
	@echo $(OBJECTS)

checkdirs: $(LIBDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

clean:
	@echo 'Removing the build files....'
	@rm -rf *~ *.o

%.o: %.cpp
	$(CXX) -fPIC -c $< -O2 -D__LZ4__ $(LZ4INCLUDES)
