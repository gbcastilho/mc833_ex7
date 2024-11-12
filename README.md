Compilacao
```bash
gcc -Wall net_api.h -o net_api.o
gcc -Wall server.h -o server.o
gcc -Wall cliente.c server.c net_api.c -o cliente
gcc -Wall servidor.c server.c net_api.c -o servidor
```

Execucao
```bash
./servidor 1234
./servidor 1235
./cliente 0.0.0.0 1234 1235 entrada.txt saida.txt
```