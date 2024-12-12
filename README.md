# Projeto-Arte-Interativa-com-Arduino

# Mesa de Música Interativa

Este projeto permite aos usuários criar arte interativa através de notas musicais ativadas por bolas colocadas em locais designados na mesa. Ele combina tecnologia e criatividade para deixar a imaginação fluir.

## Como Funciona

### Detecção das Bolas
- **Matriz de Detecção**: Uma matriz de detecção utiliza fios equivalentes à soma das linhas e colunas.
- **Bolas Condutoras**: Quando uma corrente elétrica (5 volts) é enviada pelas linhas, identifica onde a corrente passa devido ao material condutor das bolas.
- **Identificação do Ponto**: Localizando tanto a linha quanto a coluna que permitem a passagem de corrente, o sistema identifica a posição da bola.
- **Sequenciador MIDI**: Cada ponto identificado é conectado a uma nota musical correspondente por meio de um sequenciador MIDI para o PC.

### Feedback Visual
- **Indicadores de LED**: Uma fita de LED fornece feedback visual, acendendo para indicar os pontos que estão sendo escaneados e lidos.

## Funcionalidades
- **Interface Interativa**:
  - Funcionalidade de Play/Pause.
  - Salvar preferências do usuário, como:
    - Velocidade da varredura.
    - Duração das notas.
    - Instrumentos.
    - Escala.
    - Salvar Loop.
    - Apagar Loop.

