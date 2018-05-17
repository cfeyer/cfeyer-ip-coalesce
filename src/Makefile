CC_FILES= \
   IP_Range.cpp \
   Subnet.cpp \
   Format.cpp \
   Interval.cpp \
   Coalescing_IP_Range_Set.cpp

H_FILES= \
   ../include/IP_Range.h \
   ../include/Subnet.h \
   ../include/Format.h \
   ../include/Interval.h \
   ../include/Coalescing_IP_Range_Set.h

LIB_NAME=../lib/libip-coalesce.so

$(LIB_NAME): $(CC_FILES) $(H_FILES)
	g++ -o $@ -I../include -std=c++14 -fPIC -shared $(CC_FILES)

.PHONY: clean

clean:
	rm -f *.o $(LIB_NAME)
