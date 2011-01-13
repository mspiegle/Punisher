C=gcc
CPP=g++
AR=ar
INCLUDE=-Iinclude -I../Logging/include -I../M/include

CPP_SRC  = src/Socket.cpp \
           src/SocketProxy.cpp \
					 src/InetSocket.cpp \
					 src/LocalSocket.cpp \
           src/TcpSocket.cpp \
           src/UdpSocket.cpp \
           src/UnixSocket.cpp \
					 src/HostPort.cpp \
					 src/HostAddress.cpp \
					 src/HostName.cpp \
					 src/Host.cpp \
					 src/HostPort.cpp \
					 src/HostAddress.cpp \
					 src/HostName.cpp \
					 src/Resolve.cpp
MAIN_SRC = test/main.cpp
C_SRC    = 
CPP_OBJ  = $(patsubst %.cpp,%.cpp.o,$(CPP_SRC))
MAIN_OBJ = $(patsubst %.cpp,%.cpp.o,$(MAIN_SRC))
C_OBJ    = $(patsubst %.c,%.c.o,$(C_SRC))
ALL_OBJ  = $(CPP_OBJ) $(C_OBJ)
BIN = nettest
SAR = Network.a
SAR_LOGGING = ../Logging/Logging.a
SAR_M = ../M/M.a
SAR_ALL = $(SAR) $(SAR_LOGGING) $(SAR_M)

all: $(BIN) $(SAR) $(SAR_LOGGING) $(SAR_M)

%.cpp.o: %.cpp
	@echo "Compiling: " $<
	@$(CPP) -c $(CPPFLAGS) $(INCLUDE) $< -o $@

%.c.o: %.c
	@echo "Compiling: " $<
	@$(C) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(BIN): $(SAR) $(MAIN_OBJ) $(SAR_LOGGING) $(SAR_M)
	@echo "Linking:   " $@
	@$(CPP) $(CPPFLAGS) $(LDFLAGS) $(MAIN_OBJ) $(SAR_ALL) -o $@

$(SAR): $(ALL_OBJ)
	@echo "Archiving: " $@
	@$(AR) -r $@ $(ALL_OBJ) 2>/dev/null

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
	@echo "Cleaning:   Logging"
	@$(MAKE) -s -C ../Logging clean
	@echo "Cleaning:   M"
	@$(MAKE) -s -C ../M clean
