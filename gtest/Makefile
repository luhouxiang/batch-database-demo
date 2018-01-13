.PHONY: default force

SYSNAME=$(shell uname -s)
ifeq ($(SYSNAME),Darwin)
		ECHO=echo
else
		ECHO=echo -e
endif

CXX = g++
CFLAGS = -pg -Wall -std=c++0x

KDS_PROGRAM_NAME=AS
KDS_PROGRAM_NAME_CN=测试批量处理数据库Demo

# TODO: 这里加入所有默认需要编译的目标
LINK_TARGETS=batch-database-demo.a
TEST_TARGETS=start
default: version.h $(LINK_TARGETS) $(TEST_TARGETS)

# 全局的编译和链接选项都放在这儿
KDS_MOBILE_STOCK_HOME=~/build-env/build-system
LDFLAGS = -L $(KDS_MOBILE_STOCK_HOME)/3rd-soft/lib/
CLINC = -I $(KDS_MOBILE_STOCK_HOME)/3rd-soft/include/ -I $(KDS_MOBILE_STOCK_HOME)/3rd-soft/include/googletest1.8.0/include/  -I ../src/ -I ../src/common/
LDLIBS = -ljsoncpp -lsybdb -lgcrypt -lcurl -luuid -lglog -ljsoncpp -lgtest -lgtest_main

# 下面是每个目标的生成过程
server.o: version.h
MAIN_VERSION_STRING="1.0.0"
VERSION_STRING:=$(shell LANG=C git log --oneline -1 2>/dev/null |cut -c1-7)
version.h: force
	@echo Generation version.h ...
	$(ECHO) "# ifndef  __KDS_MOBILE_STOCK_SERVICE_VERSION_H__\n\
    # define __KDS_MOBILE_STOCK_SERVICE_VERSION_H__\n\
    # define SERVICE_VERSION_STRING \"$(MAIN_VERSION_STRING).$(VERSION_STRING)\"\n\
    # define KDS_PROGRAM_NAME \"$(KDS_PROGRAM_NAME)\"\n\
    # define KDS_PROGRAM_NAME_CN \"$(KDS_PROGRAM_NAME_CN)\"\n\
    # endif" > ../src/version.h
    


force:

# 程序的生成过程 {{{
LINK_SOURCES = $(wildcard ../src/*.cpp ../src/common/*.cpp)
LINK_OBJECTS = $(patsubst %.cpp, %.o,$(LINK_SOURCES))
$(LINK_TARGETS): $(LINK_OBJECTS)
	ar rcs $(LINK_TARGETS) $(LINK_OBJECTS)


TEST_SOURCES = $(wildcard *.cpp)
TEST_OBJECTS = $(patsubst %.cpp,%.o,$(TEST_SOURCES))
$(TEST_TARGETS):$(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) $(CLINC)  $(LDFLAGS) $(LDLIBS) $(LINK_TARGETS) -o ${TEST_TARGETS}


# }}}

# 编译过程
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CLINC) $(LDFLAGS) $(LDLIBS) $< -o $@

install:


clean:
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/common/*.o
	rm -f ../src/version.h
	rm -f ${LINK_TARGETS}
	rm -f ${TEST_TARGETS}


