CPP=g++
C=gcc
AR=ar
INCLUDE=-Iinclude
LDFLAGS=-lssl

CPP_SRC = src/String.cpp \
          src/Md5Hash.cpp
MAIN_SRC = test/main.cpp
C_SRC   = 
CPP_OBJ = $(patsubst %.cpp,%.cpp.o,$(CPP_SRC))
C_OBJ   = $(patsubst %.c,%.c.o,$(C_SRC))
MAIN_OBJ = $(patsubst %.cpp,%.cpp.o,$(MAIN_SRC))
ALL_OBJ = $(CPP_OBJ) $(C_OBJ)
BIN = teststring
SAR = M.a

all: $(BIN) $(SAR)

%.cpp.o: %.cpp
	@echo "Compiling: " $<
	@$(CPP) -c $(CPPFLAGS) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(BIN): $(ALL_OBJ) $(MAIN_OBJ)
	@echo "Linking:   " $@
	@$(CPP) $(CPPFLAGS) $(LDFLAGS) $(ALL_OBJ) $(MAIN_OBJ) -o $@

$(SAR): $(ALL_OBJ)
	@echo "Archiving: " $@
	@$(AR) -r $@ $(ALL_OBJ) 2>/dev/null

clean:
	@echo "Removing:  " $(BIN)
	@rm -f $(BIN)
	@echo "Removing:  " $(SAR)
	@rm -f $(SAR)
	@echo "Removing:  " $(ALL_OBJ)
	@rm -f $(ALL_OBJ)
	@echo "Removing:  " $(MAIN_OBJ)
	@rm -f $(MAIN_OBJ)
