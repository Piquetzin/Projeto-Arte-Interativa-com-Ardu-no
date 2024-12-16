# Projeto Arte Interativa com Arduino

Projeto realizado por Gabriel Piquet, Luciana Freitas e Ian Moura

# Mesa de Música Interativa

Este projeto é uma iniciativa inovadora que permite aos usuários criar arte interativa através de notas musicais ativadas por bolas colocadas em locais designados na mesa. A proposta combina elementos de tecnologia avançada com a liberdade criativa, oferecendo uma experiência única e envolvente. Os participantes podem explorar sons e ritmos de maneira dinâmica, enquanto interagem diretamente com o sistema. Essa abordagem estimula a criatividade, proporcionando uma nova forma de expressão artística onde a imaginação é o único limite.

Ao transformar uma interface física em um instrumento musical interativo, o projeto une o mundo real e o digital, criando um ambiente propício para a experimentação e o aprendizado. Seja para fins educacionais, recreativos ou artísticos, esta plataforma demonstra como a tecnologia pode ser usada para ampliar as possibilidades criativas.

## Como Funciona

### Detecção das Bolas
- **Matriz de Detecção**: Uma matriz de detecção 4x4 utiliza fios equivalentes à soma das linhas e colunas.
- **Bolas Condutoras**: Quando uma corrente elétrica (5 volts) é enviada pelas linhas, identifica onde a corrente passa devido ao material condutor das bolas.
- **Identificação do Ponto**: Localizando tanto a linha quanto a coluna que permitem a passagem de corrente, o sistema identifica a posição da bola.
- **Sequenciador MIDI**: Ao ser identificado pelo código arduino `Codigo_Final_IDE`, o ponto é enviado ao código python `ComunicaçãoFinal.py` via Serial e é conectado a uma nota musical correspondente por meio de um sequenciador MIDI, que está no código `MúsicaFinal.py` do computador.

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
  - Além disso, é necessário montar a matriz de detecção, para a qual existe um esquemático no arquivo `matriz.fzz`.

![Esquemático da matriz](https://github.com/user-attachments/assets/5ec819c5-38e0-45e9-b5c5-4b3ce51b3bdf)
