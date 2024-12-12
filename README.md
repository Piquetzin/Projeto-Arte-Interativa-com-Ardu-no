# Projeto Arte Interativa com Arduino

# Mesa de Música Interativa

Este projeto permite aos usuários criar arte interativa através de notas musicais ativadas por bolas colocadas em locais designados na mesa. Ele combina tecnologia e criatividade para deixar a imaginação fluir.

## Como Funciona

### Detecção das Bolas
- **Matriz de Detecção**: Uma matriz de detecção 4x4 utiliza fios equivalentes à soma das linhas e colunas.
- **Bolas Condutoras**: Quando uma corrente elétrica (5 volts) é enviada pelas linhas, identifica onde a corrente passa devido ao material condutor das bolas.
- **Identificação do Ponto**: Localizando tanto a linha quanto a coluna que permitem a passagem de corrente, o sistema identifica a posição da bola.
- **Sequenciador MIDI**: Ao ser identificado pelo código arduino "Codigo_Final_IDE", o ponto é enviado ao código python "ComunicaçãoFinal.py" via Serial e é conectado a uma nota musical correspondente por meio de um sequenciador MIDI, que está no código "MúsicaFinal.py" do computador.

### Feedback Visual
- **Indicadores de LED**: Uma fita de LED fornece feedback visual, acendendo para indicar os pontos que estão sendo escaneados e lidos.

## Funcionalidades
- **Interface Interativa**:
  - Instrumentos (Change Instrument).
  - Velocidade da varredura (Change Speed).
  - Escala (Change Scale).
  - Salvar Loop (Save Loop).
  - Apagar Loop (Delete Loop).
      
## Possibilidades de Uso
- Com a opção do Loop, o usuário pode formar diferentes beats com quatro tempos, podendo formar um fundo de diferentes instrumentos com diferentes escalas.
- Além disso, a velocidade da varredura permite que o usuário aumente o bpm do beat.

## Pré-Requisitos
- Para rodar o projeto é necessário baixar o [IDE do Arduino](https://www.arduino.cc/en/software), um editor de código que suporte Python e a biblioteca `pygame.midi`.
- É necessário baixar os seguintes aplicativos:
  - [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html).
  - [VirtualMIDISynth](https://coolsoft.altervista.org/en/virtualmidisynth).

![matriz_bb](https://github.com/user-attachments/assets/5ec819c5-38e0-45e9-b5c5-4b3ce51b3bdf)
