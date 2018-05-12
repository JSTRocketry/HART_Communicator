TARGET =parser
IDIR =include
ODIR =build
SRC_DIR =src
CXX=g++-5
CXXFLAGS=-std=c++11 -Wall -I$(IDIR)
LIBS= -lglut -lGL -lGLU -lGLEW -lm
_DEPS = OpenGlGraph.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = OpenGlGraph.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LIBS) $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ exec
