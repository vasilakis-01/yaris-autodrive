#simple make file to compile all the header files and the main prog as well
#type 'make' to compile and 'make clean' before any compilation to remove the object files


CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.
TARGET = oopproj
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run