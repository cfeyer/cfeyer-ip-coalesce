LIB_CC_FILES = \
   IP_Range.cpp \
   Subnet.cpp \
   Format.cpp \
   Interval.cpp \
   Coalescing_IP_Range_Set.cpp

LIB_H_FILES = \
   ../include/IP_Range.h \
   ../include/Subnet.h \
   ../include/Format.h \
   ../include/Interval.h \
   ../include/Coalescing_IP_Range_Set.h

LIB_BASE_NAME = ip-coalesce
LIB_PATH = ../lib/lib$(LIB_BASE_NAME).so
EXE_PATH = ../bin/ip-coalesce

CPP_FLAGS += -I../include
CXX_FLAGS += -std=c++14

$(EXE_PATH): main.cpp $(LIB_PATH)
	g++ $(CPP_FLAGS) $(CXX_FLAGS) main.cpp -L$(dir $(LIB_PATH)) -l$(LIB_BASE_NAME) -o $@

$(LIB_PATH): $(LIB_CC_FILES) $(LIB_H_FILES)
	g++ $(CPP_FLAGS) $(CXX_FLAGS) -fPIC -shared $(LIB_CC_FILES) -o $@

.PHONY: clean

clean:
	rm -f *.o $(LIB_PATH) $(EXE_PATH)
