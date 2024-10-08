# Compile code
```bash
gcc -Wall net_api.h -o net_api.o
gcc -Wall server.h -o server.o
gcc -Wall cliente.c server.c net_api.c -o cliente
gcc -Wall servidor.c server.c net_api.c -o servidor
```