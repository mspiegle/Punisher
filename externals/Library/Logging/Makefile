CPP=g++
C=gcc
AR=ar
INCLUDE=-Iinclude -I../M/include

CPP_SRC = src/Logging.cpp
MAIN_SRC = test/main.cpp
C_SRC   = 
CPP_OBJ = $(patsubst %.cpp,%.cpp.o,$(CPP_SRC))
MAIN_OBJ = $(patsubst %.cpp,%.cpp.o,$(MAIN_SRC))
C_OBJ   = $(patsubst %.c,%.c.o,$(C_SRC))
ALL_OBJ = $(CPP_OBJ) $(C_OBJ)
BIN = testlog
SAR = Logging.a
SAR_M = ../M/M.a

all: $(BIN) $(SAR) $(SAR_M)

%.cpp.o: %.cpp
	@echo "Compiling: " $<
	@$(CPP) -c $(CPPFLAGS) $(VERBOSE) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(VERBOSE) $(INCLUDE) $< -o $@

$(BIN): $(ALL_OBJ) $(MAIN_OBJ) $(SAR_M)
	@echo "Linking:   " $@
	@$(CPP) $(CPPFLAGS) $(LDFLAGS) $(MAIN_OBJ) $(ALL_OBJ) $(SAR_M) -o $@

$(SAR): $(ALL_OBJ)
	@echo "Archiving: " $@
	@$(AR) -r $@ $(ALL_OBJ) 2>/dev/null

$(SAR_M):
	@$(MAKE) -s -C ../M

clean:
	@echo "Removing:  " $(BIN)
	@rm -f $(BIN)
	@echo "Removing:  " $(SAR)
	@rm -f $(SAR)
	@echo "Removing:  " $(ALL_OBJ)
	@rm -f $(ALL_OBJ)
	@echo "Cleaning:   M"
	@$(MAKE) -s -C ../M clean
