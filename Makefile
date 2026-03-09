CXX      := g++
CXXFLAGS := -O2
LDFLAGS  :=
# If you use OpenMP in your codes, keep this:
CXXFLAGS += -fopenmp
LDFLAGS  += -fopenmp

TARGETS := pp1 pp2 pp3 sp

all: $(TARGETS)

pp1: pp1.cpp config.hpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

pp2: pp2.cpp config.hpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

pp3: pp3.cpp config.hpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sp: sp.cpp config.hpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean