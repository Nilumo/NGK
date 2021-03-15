SOURCES = main.cpp
DEPS= $(SOURCES:.cpp=.d)
OBJECTS = $(SOURCES:.cpp=.o)
CXX = g++
EXE = client_prog
CXXFLAGS = -Wall

$(EXE): $(DEPS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.d: %.cpp
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@

-include $(DEPS)


.PHONY: clean
clean:	##Clean up your executeable file and all .o and .d files
	rm -f $(EXE) *.o *.d
	