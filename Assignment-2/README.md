# Compilation and Execution 

1)  Compile server code and client code 
    - clang server.c -o server 
    - clang client.c -o client

2) Run the server code in terminal
   - sudo ./server

3) Open another terminal and run the client code
   - ./client

# Deviation

Changed if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
to setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
to resolve compilation errors in macOS

