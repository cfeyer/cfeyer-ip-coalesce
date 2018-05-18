LIB_CC_FILES = \
   IP_Range.cpp \
   Subnet.cpp \
   Format.cpp \
   Interval.cpp \
   Coalescing_IP_Range_Set.cpp

LIB_H_FILES = \
   ../include/cfeyer/ip_coalesce/IP_Range.h \
   Subnet.h \
   Format.h \
   Interval.h \
   ../include/cfeyer/ip_coalesce/Coalescing_IP_Range_Set.h

LIB_BASE_NAME = cfeyer_ip_coalesce
LIB_PATH = ../lib/lib$(LIB_BASE_NAME).so
IP_COALESCE_EXE_PATH = ../bin/ip-coalesce
IP_COALESCE_TABLE_EXE_PATH = ../bin/ip-coalesce-table

CPP_FLAGS += -I../include
CXX_FLAGS += -std=c++14

.PHONY: all clean

all: $(IP_COALESCE_EXE_PATH) $(IP_COALESCE_TABLE_EXE_PATH)

$(IP_COALESCE_EXE_PATH): main_ip_coalesce.cpp $(LIB_PATH)
	g++ $(CPP_FLAGS) $(CXX_FLAGS) $< -L$(dir $(LIB_PATH)) -l$(LIB_BASE_NAME) -o $@

$(IP_COALESCE_TABLE_EXE_PATH): main_ip_coalesce_table.cpp $(LIB_PATH)
	g++ $(CPP_FLAGS) $(CXX_FLAGS) $< -L$(dir $(LIB_PATH)) -l$(LIB_BASE_NAME) -o $@

$(LIB_PATH): $(LIB_CC_FILES) $(LIB_H_FILES)
	g++ $(CPP_FLAGS) $(CXX_FLAGS) -fPIC -shared $(LIB_CC_FILES) -o $@

clean:
	rm -f *.o $(LIB_PATH) $(IP_COALESCE_EXE_PATH) $(IP_COALESCE_TABLE_EXE_PATH)
