"""Código para a comunicação do arduíno com o Python via Serial"""

import serial
import time
from MúsicaFinal import tocar_linha, instrumentos_canais,salvar_matriz,apagar_matriz

# Definição dos instrumentos e escalas
instrumentos = ["Piano", "Violão", "Guitarra", "Baixo", "Tambor"]
escalas = ["Maior", "Média", "Menor"]

# Configuração do Arduino
arduino = serial.Serial(port='COM8', baudrate=115200, timeout=1)
time.sleep(2)
while True:
        novoloop = arduino.readline().decode('utf-8').strip()  
        if novoloop == "Novo loop":

            indice_instrumento = arduino.readline().decode('utf-8').strip()
            indice_escala = arduino.readline().decode('utf-8').strip()
            tempo_espera = arduino.readline().decode('utf-8').strip()
            indice_salvar = arduino.readline().decode('utf-8').strip()
            indice_apagar = arduino.readline().decode('utf-8').strip()
            indice_colunaAtual = arduino.readline().decode('utf-8').strip()

            try:
                indice_instrumento = int(indice_instrumento)
                indice_escala = int(indice_escala)
                indice_colunaAtual = int(indice_colunaAtual)
                tempo_espera = float(tempo_espera)
                indice_salvar = int(indice_salvar)
                indice_apagar = int(indice_apagar)
                indice_colunaAtual+=1
                if indice_colunaAtual>3:
                     indice_colunaAtual=0
                if not (0 <= indice_instrumento < len(instrumentos)):
                    raise ValueError(f"Índice de instrumento inválido: {indice_instrumento}\n")
                if not (0 <= indice_escala < len(escalas)):
                    raise ValueError(f"Índice de escala inválido: {indice_escala}")
                if indice_salvar == 1:
                    salvar_matriz(matriz_nova,indice_escala,100,instrumentos_canais[instrumentos[indice_instrumento]])
                if indice_apagar ==1:
                    apagar_matriz()
            except ValueError as ve:
                print(f"Erro de índice: {ve}")
                continue
            
            matriz_nova = []

            for i in range(4):
                coluna = arduino.readline().decode('utf-8').strip()
                matriz_nova.append(coluna)

            tocar_linha(matriz_nova[indice_colunaAtual], instrumentos_canais[instrumentos[indice_instrumento]], indice_escala, 100,tempo_espera/1000,indice_colunaAtual)

