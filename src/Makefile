.PHONY: default

SYSNAME=$(shell uname -s)
ifeq ($(SYSNAME),Darwin)
		ECHO=echo
else
		ECHO=echo -e
endif

CXX = g++
CFLAGS = -pg -Wall -std=c++0x
KDS_MOBILE_STOCK_HOME=~/build-env/build-system
LDFLAGS = -L $(KDS_MOBILE_STOCK_HOME)/3rd-soft/lib/
CLINC = -I $(KDS_MOBILE_STOCK_HOME)/3rd-soft/include/
LDLIBS = -lsybdb -lgcrypt -ljsoncpp

SOURCES = $(wildcard *.cpp ./common/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

TARGETS=batch-database-demo

# TODO: 这里加入所有默认需要编译的目标
default: version.h $(TARGETS)
	@echo Generation default ...

# 链接过程
${TARGETS}: $(OBJECTS)
	@echo Generation targets ...
	$(CXX) $(OBJECTS) $(CLINC) $(LDFLAGS) $(LDLIBS) -o $(TARGETS)
	cp -f $(TARGETS) ../bin/$(TARGETS)
	rm -f $(TARGETS)
# 编译过程
%.o : %.cpp
	@echo Generation cpp ...
	$(CXX) -c $(CFLAGS) $(CLINC) $(LDFLAGS) $(LDLIBS) $< -o $@

# version.h 文件的生成过程
KDS_PROGRAM_NAME=AS
KDS_PROGRAM_NAME_CN=测试批量处理数据库Demo
MAIN_VERSION_STRING="1.0.0"
VERSION_STRING:=$(shell LANG=C git log --oneline -1 2>/dev/null |cut -c1-7)
version.h: force
	@echo Generation version.h ...
	@$(ECHO) "# ifndef  __KDS_MOBILE_STOCK_SERVICE_VERSION_H__\n\
    # define __KDS_MOBILE_STOCK_SERVICE_VERSION_H__\n\
    # define SERVICE_VERSION_STRING \"$(MAIN_VERSION_STRING).$(VERSION_STRING)\"\n\
    # define KDS_PROGRAM_NAME \"$(KDS_PROGRAM_NAME)\"\n\
    # define KDS_PROGRAM_NAME_CN \"$(KDS_PROGRAM_NAME_CN)\"\n\
    # endif" > version.h
    
force:
	@echo Generation force ...
# 程序的生成过程 {{{

# }}}

install: 
	@echo Generation install ...
	cp -f $(TARGETS) ../bin/$(TARGETS)
	rm -f $(TARGETS)
clean:
	@echo Generation clean ...
	rm -f *.o
	rm -f ./common/*.o
	rm -f $(TARGETS)
	rm -f version.h
