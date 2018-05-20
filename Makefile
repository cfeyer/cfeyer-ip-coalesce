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

INSTALL_BIN_DIR=/usr/bin
INSTALL_LIB_DIR=/usr/lib

.PHONY: src test check clean install unininstall

all: check

src:
	make -C src

test: src
	make -C test

check: test
	cd test; make check

clean:
	make -C src clean
	make -C test clean

INSTALL_TARGETS= \
         $(INSTALL_BIN_DIR)/ip-coalesce \
         $(INSTALL_BIN_DIR)/ip-coalesce-table \
         $(INSTALL_BIN_DIR)/ip-coalesce-table.sh \
         $(INSTALL_LIB_DIR)/libcfeyer_ip_coalesce.so

install: src $(INSTALL_TARGETS)

uninstall:
	rm -rf $(INSTALL_TARGETS)

$(INSTALL_BIN_DIR)/ip-coalesce:
	install --mode=755 ./bin/ip-coalesce $@

$(INSTALL_BIN_DIR)/ip-coalesce-table:
	install --mode=755 ./bin/ip-coalesce-table $@

$(INSTALL_BIN_DIR)/ip-coalesce-table.sh:
	install --mode=755 ./bin/ip-coalesce-table.sh $@

$(INSTALL_LIB_DIR)/libcfeyer_ip_coalesce.so:
	install --mode=644 ./lib/libcfeyer_ip_coalesce.so $@
