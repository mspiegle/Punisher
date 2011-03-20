INCLUDE_DIRECTORIES(${Network_SOURCE_DIR}/include)
ADD_LIBRARY(Network Host HostAddress HostName HostPort InetSocket LocalSocket Resolve Socket SocketProxy TcpSocket UdpSocket UnixSocket)
