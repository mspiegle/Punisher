CXX=g++
C=gcc
AR=ar
INCLUDE=-Iinclude
LDFLAGS=-lcrypto

CXX_SRC = src/String.cxx \
          src/Md5Hash.cxx
MAIN_SRC = test/main.cxx
C_SRC   = 
CXX_OBJ = $(patsubst %.cxx,%.cxx.o,$(CXX_SRC))
C_OBJ   = $(patsubst %.c,%.c.o,$(C_SRC))
MAIN_OBJ = $(patsubst %.cxx,%.cxx.o,$(MAIN_SRC))
ALL_OBJ = $(CXX_OBJ) $(C_OBJ)
BIN = teststring
SAR = M.a

all: $(BIN) $(SAR)

%.cxx.o: %.cxx
	@echo "Compiling: " $<
	@$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(BIN): $(ALL_OBJ) $(MAIN_OBJ)
	@echo "Linking:   " $@
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ALL_OBJ) $(MAIN_OBJ) -o $@

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
