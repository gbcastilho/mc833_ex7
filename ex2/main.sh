#!/bin/bash

# Defina a porta do servidor
PORTA=4263
IP=0.0.0.0

# Lista de valores de backlog para testar
BACKLOGS=12

NUM_CLIENTES=15

# Diretório onde se encontra o executável do servidor
DIRETORIO_EXECUTAVEL="./"

rm ./logs.txt

# Loop para iterar sobre os valores de backlog
for ((backlog=0; backlog<=BACKLOGS; backlog++)); do
PORTA=$((PORTA + 1))
    # Iniciar o servidor em segundo plano
    ./$DIRETORIO_EXECUTAVEL/servidor $PORTA $backlog &
    echo "Servidor iniciado com backlog $backlog em segundo plano"

    # Iniciar os clientes em segundo plano
    for ((i=1; i<=NUM_CLIENTES; i++)); do
        ./$DIRETORIO_EXECUTAVEL/cliente $IP $PORTA &
        echo "Cliente $i iniciado para backlog $backlog"
    done
    echo "Backlog = $backlog" >> ./logs.txt
    netstat -taulpn | grep $PORTA | grep ESTABLISHED | grep cliente >> ./logs.txt
    echo "Quantidade de conexoes estabelecidas:" >> ./logs.txt
    netstat -taulpn | grep $PORTA | grep ESTABLISHED | grep -c cliente >> ./logs.txt
    echo "" >> ./logs.txt
    sleep 1
    pkill servidor
    pkill cliente
    # Aguardar a conclusão dos clientes (ajuste o tempo de espera conforme necessário)

    # Matar os processos do servidor e clientes (opcional)
    # pkill seu_servidor
    # pkill seu_cliente

    echo "Fim do teste para backlog $backlog"
done