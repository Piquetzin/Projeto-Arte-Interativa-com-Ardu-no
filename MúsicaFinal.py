import pygame.midi
import time
import threading

# Inicialização do MIDI
pygame.midi.init()
port = pygame.midi.Output(2)

# Configuração de canais para os instrumentos
instrumentos_canais = {
    "Piano": 0,
    "Violão": 1,
    "Guitarra": 2,
    "Baixo": 3,
    "Violino": 4,
    "Trompete": 5,
    "Flauta": 6,
    "Tambor": 9  # Canal para percussão
}



# Configuração de instrumentos para os canais MIDI
for instrumento, canal in instrumentos_canais.items():
    port.set_instrument({
        "Piano": 0,
        "Violão": 25,
        "Guitarra": 28,
        "Baixo": 33,
        "Violino": 41,
        "Trompete": 57,
        "Flauta": 74,
        "Tambor": 118
    }[instrumento], canal)

# Notas base (Dó, Mi, Lá, Si)
# Para trocar as notas, tem essa disponibilidade:
# Dó:60
# Ré:62
# Mi:64
# Fa:65
# Sol:67
# La:69
# Si:71
# Em outros oitavos apenas somar ou diminuir 12


notas = [60, 64, 67, 71]
matriz_salva={}

def tocar_nota(nota, duracao, velocidade, canal):
    """Função para tocar uma nota MIDI com temporizador."""
    tempo_inicial = time.perf_counter()
    port.note_on(nota, velocidade, canal)
    while time.perf_counter() - tempo_inicial < duracao:
         pass
    port.note_off(nota, velocidade, canal)

def salvar_matriz(matriz,escala,velocidade,canal):
    matriz_salva[canal] = {"matriz_salva":matriz, "escala":escala, "velocidade":velocidade}

def apagar_matriz():
    global matriz_salva
    matriz_salva = {}


def tocar_linha(linha, canal, escala, velocidade, duracao, indice):
    """Toca as notas de uma única linha e percorre as matrizes salvas."""
    ajuste = 0
    if escala == 0:
        ajuste = -12  # Descer um oitavo
    elif escala == 2:
        ajuste = 12  # Subir um oitavo

    # Tocar a linha recebida
    for idx, celula in enumerate(linha):
        if celula == 'O':  # Tocar nota para células marcadas com 'O'
            nota = notas[idx] + ajuste
            print(f"Tocando nota {nota} no canal {canal}.")
            port.note_on(nota, velocidade, canal)

    # Tocar as linhas correspondentes da matriz_salva
    if isinstance(matriz_salva, dict):
        for canal in instrumentos_canais.values():
            canal_salvo = matriz_salva.get(canal)
            if canal_salvo == None:
                continue
            matriz = canal_salvo.get("matriz_salva")
            escala_salva = canal_salvo.get("escala")
            velocidade_salva = canal_salvo.get("velocidade")

        # Ajuste de escala para a matriz salva
            ajuste_salvo = 0
            if escala_salva == 0:
                ajuste_salvo = -12
            elif escala_salva == 2:
                ajuste_salvo = 12

            # Certifica-se de que o índice é válido na matriz
            if 0 <= indice < len(matriz):
                linha_salva = matriz[indice]
                for idx, celula in enumerate(linha_salva):
                    if celula == 'O':
                        nota = notas[idx] + ajuste_salvo
                        print(f"Tocando nota {nota} na matriz salva, canal {canal}.")
                        port.note_on(nota, velocidade_salva, canal)

    # Timer para silenciar todas as notas após a duração
    timer = threading.Timer(duracao, silenciar_linhas, args=[linha, ajuste, velocidade, indice])
    timer.start()


def silenciar_linhas(linha, ajuste, velocidade, indice):
    """Silencia as notas da linha recebida e das matrizes salvas."""
    # Silenciar a linha recebida
    for idx, celula in enumerate(linha):
        if celula == 'O':
            nota = notas[idx] + ajuste
            print(f"Silenciando nota {nota} no canal principal.")
            port.note_off(nota, velocidade, instrumentos_canais["Piano"])

    # Silenciar as linhas das matrizes salvas
    if isinstance(matriz_salva, dict):
        matriz = matriz_salva.get("matrizsalva", [])
        escala_salva = matriz_salva.get("escala", 0)
        canal_salvo = matriz_salva.get("canal", 0)
        velocidade_salva = matriz_salva.get("velocidade", 100)

        ajuste_salvo = 0
        if escala_salva == 0:
            ajuste_salvo = -12
        elif escala_salva == 2:
            ajuste_salvo = 12

        if 0 <= indice < len(matriz):
            linha_salva = matriz[indice]
            for idx, celula in enumerate(linha_salva):
                if celula == 'O':
                    nota = notas[idx] + ajuste_salvo
                    print(f"Silenciando nota {nota} na matriz salva, canal {canal_salvo}.")
                    port.note_off(nota, velocidade_salva, canal_salvo)


if __name__ == "__main__":
    try:
        print("Meta4 inicializado e aguardando comandos externos.")

    finally:
        del port
        pygame.midi.quit()
