gcc main.c main.h sync.c socket.c config.c msgq.c -o times -lpthread -lrt
gcc main.h command.c msgq.c -o command -lpthread -lrt

