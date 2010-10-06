C=gcc
CPP=g++
AR=ar
INCLUDE=-Iinclude -I../Network/include -I../Logging/include -I../M/include

CPP_SRC = src/Manager.cpp \
					src/EpollManager.cpp \
          src/Item.cpp
MAIN_SRC = test/main.cpp
C_SRC   = 
CPP_OBJ = $(patsubst %.cpp,%.cpp.o,$(CPP_SRC))
C_OBJ   = $(patsubst %.c,%.c.o,$(C_SRC))
MAIN_OBJ = $(patsubst %.cpp,%.cpp.o,$(MAIN_SRC))
ALL_OBJ = $(CPP_OBJ) $(C_OBJ)
BIN = eventtest
SAR = Event.a
SAR_NETWORK = ../Network/Network.a
SAR_LOGGING = ../Logging/Logging.a
SAR_M = ../M/M.a
SAR_ALL = $(SAR) $(SAR_NETWORK) $(SAR_LOGGING) $(SAR_M)

all: $(BIN) $(SAR_ALL)

%.cpp.o: %.cpp
	@echo "Compiling: " $<
	@$(CPP) -c $(CPPFLAGS) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(BIN): $(ALL_OBJ) $(SAR_ALL) $(MAIN_OBJ)
	@echo "Linking:   " $@
	@$(CPP) $(CPPFLAGS) $(LDFLAGS) $(MAIN_OBJ) $(ALL_OBJ) $(SAR_ALL) -o $@

$(SAR): $(ALL_OBJ)
	@echo "Archiving: " $@
	@$(AR) -r $@ $(ALL_OBJ) 2>/dev/null

$(SAR_NETWORK):
	@$(MAKE) -s -C ../Network

$(SAR_LOGGING):
	@$(MAKE) -s -C ../Logging

$(SAR_M):
	@$(MAKE) -s -C ../M

clean:
	@echo "Removing:  " $(BIN)
	@rm -f $(BIN)
	@echo "Removing:  " $(SAR)
	@rm -f $(SAR)
	@echo "Removing:  " $(ALL_OBJ)
	@rm -f $(ALL_OBJ)
	@echo "Removing:  " $(MAIN_OBJ)
	@rm -f $(MAIN_OBJ)
	@echo "Cleaning:  Network"
	@$(MAKE) -s -C ../Network clean
	@echo "Cleaning:  Logging"
	@$(MAKE) -s -C ../Logging clean
	@echo "Cleaning:  M"
	@$(MAKE) -s -C ../M clean
