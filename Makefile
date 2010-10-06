CPP=g++
C=gcc
#DEBUG_FLAGS=-fstack-check -fstack-protector -DDEBUG
DEBUG_FLAGS=-DDEBUG
BASEFLAGS=-O0 -g3 -Wall -Wextra -Werror -Wfatal-errors $(DEBUG_FLAGS)
CFLAGS=$(BASEFLAGS)
CPPFLAGS=$(BASEFLAGS)
LDFLAGS=-lpthread -lyaml -rdynamic -lrt -lssl
INCLUDE=-Iinclude \
        -Iexternals/Library/Logging/include \
        -Iexternals/Library/Network/include \
        -Iexternals/Library/Event/include \
        -Iexternals/Library/M/include

CPP_SRC = src/main.cpp \
          src/Client.cpp \
          src/Config.cpp \
          src/Thread.cpp \
          src/Request.cpp \
          src/HttpRequest.cpp \
          src/Validator.cpp \
          src/Md5Validator.cpp \
          src/Worker.cpp \
          src/State.cpp \
          src/HttpState.cpp \
          src/HttpMethod.cpp \
          src/HttpVersion.cpp \
          src/Statistics.cpp \
          src/HttpStatus.cpp
C_SRC   = 
CPP_OBJ = $(patsubst %.cpp,%.cpp.o,$(CPP_SRC))
C_OBJ   = $(patsubst %.c,%.c.o,$(C_SRC))
ALL_OBJ = $(C_OBJ) $(CPP_OBJ)
AR_LIBRARY = externals/Library/Library.a
ALL_AR  = $(AR_LIBRARY)
BIN = punisher

.EXPORT_ALL_VARIABLES:

all: $(BIN)

%.cpp.o: %.cpp
	@echo "Compiling: " $<
	@$(CPP) -c $(CPPFLAGS) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(BIN): $(ALL_OBJ) $(ALL_AR)
	@echo "Linking:   " $@
	@$(CPP) $(LDFLAGS) $(ALL_OBJ) $(ALL_AR) -o $@

$(AR_LIBRARY):
	@$(MAKE) -s -C externals/Library

clean:
	@echo "Removing:  " $(BIN)
	@rm -f $(BIN)
	@echo "Removing:  " $(ALL_OBJ)
	@rm -f $(ALL_OBJ)
	@echo "Removing:   Callgrind files"
	@rm -f callgrind.*
	@echo "Cleaning:   Library"
	@$(MAKE) -s -C externals/Library clean

debug:
	@$(MAKE) -s clean
	@$(MAKE) -s
	#@gdb --args ./punisher -T 1 -c 1 -s scripts/vm02.yaml
	@gdb --args ./punisher -T 1 -c 1 -s scripts/localhost.yaml

run:
	@$(MAKE) -s clean
	@$(MAKE) -s -j5
	@./punisher -v -T 1 -c 1 -s scripts/localhost.yaml

profile:
	@$(MAKE) -s clean
	@$(MAKE) -s -j5
	@valgrind --tool=callgrind ./punisher -T 1 -c 1 -s scripts/localhost.yaml
