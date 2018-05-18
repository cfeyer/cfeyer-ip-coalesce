#  The MIT License
#  
#  Copyright (c) 2018 Chris Feyerchak
#  
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#  
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#  
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.

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
