CXX = g++
CXXFLAGS = -std=c++17 -O3
LDFLAGS = -lpng

SRC = main.cc
OBJ = ${SRC:.cc=.o}

all: image_match_palette

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

image_match_palette: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) image_match_palette

.PHONY: all clean
