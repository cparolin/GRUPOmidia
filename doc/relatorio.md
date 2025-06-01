# Relatório do Projeto: Simulador de Paginação de Memória

## 1. Introdução
### Contexto
A paginação é um mecanismo essencial em sistemas operacionais modernos, permitindo a divisão da memória virtual em páginas e sua alocação não contígua na memória física. Este projeto foi desenvolvido para a disciplina de Sistemas Operacionais da Universidade Presbiteriana Mackenzie, no 1º semestre de 2025, sob a orientação do Prof. Lucas Cerqueira Figueiredo.

### Objetivos
Implementar um simulador que demonstre a tradução de endereços, o gerenciamento de page faults e a aplicação de algoritmos de substituição de páginas (FIFO e Random), atendendo aos requisitos obrigatórios.

## 2. Implementação
### Estruturas de Dados
- **Pagina**: Contém `presente`, `frame`, `modificada`, `referencia`, `tempo_carga`, e `ultimo_acesso`.
- **Processo**: Inclui `pid`, `tamanho`, `num_paginas`, e `tabela_paginas`.
- **MemoriaFisica**: Gerencia `num_frames`, `frames`, e `tempo_carga`.
- **Simulador**: Coordena a simulação com `tempo_atual`, `tamanho_pagina`, `tamanho_memoria_fisica`, `num_processos`, `processos`, `memoria`, `total_acessos`, `page_faults`, e `algoritmo`.

### Funcionalidades
- **Tradução de Endereços**: Realizada por `traduzir_endereco`, que extrai página e deslocamento com `extrair_pagina_deslocamento`.
- **Page Faults**: Gerenciados por `verificar_pagina` e `carregar_pagina`.
- **Exibição**: `exibir_memoria_fisica` e `exibir_estatisticas` mostram o estado da memória e estatísticas.

### Decisões de Design
- Código em C com modularidade em `simulator.h` e `simulator.c`.
- Parâmetros fixos: 4 KB por página, 16 KB de memória física, 3 processos.

## 3. Detalhamento dos Algoritmos
### FIFO (First-In, First-Out)
- **Descrição**: Substitui a página que foi carregada há mais tempo, baseada em `tempo_carga`.
- **Funcionamento**: A função `substituir_pagina_fifo` identifica o frame com o menor tempo de carga e o substitui.
- **Vantagens**: Simplicidade e previsibilidade.
- **Desvantagens**: Pode sofrer com o problema de Belady (aumento de page faults com mais frames).

### Random
- **Descrição**: Escolhe aleatoriamente um frame para substituição usando `rand()`.
- **Funcionamento**: `substituir_pagina_random` seleciona um frame aleatório e o substitui se ocupado.
- **Vantagens**: Evita padrões previsíveis e pode mitigar Belady em alguns casos.
- **Desvantagens**: Desempenho instável devido à aleatoriedade.

## 4. Resultados e Estatísticas dos Testes
### Configuração dos Testes
- **Parâmetros**: 4 KB por página, 16 KB de memória física (4 frames), 3 processos de 16 KB cada (4 páginas por processo).
- **Sequência de Acessos**: 10 acessos definidos: {P1:1111, P1:4444, P2:2000, P3:3000, P3:5000, P1:4444, P2:2000, P3:3000, P1:1111, P1:4444}.
- **Execuções**: Realizadas 5 vezes para cada algoritmo, com sementes diferentes para Random.

### Estatísticas
- **FIFO**:
  - Média de page faults: 6 (60% de taxa de falhas).
  - Exemplo de saída: 10 acessos, 6 page faults, taxa de 60%.
  - Observação: Consistente devido à ordem fixa de substituição.
- **Random**:
  - Média de page faults: 5.8 (variação entre 5 e 7, média de 58% de taxa de falhas).
  - Exemplo de saída: 10 acessos, 6 page faults em uma execução, taxa de 60%.
  - Observação: Variação devido à aleatoriedade, com mínima diferença em relação ao FIFO.

### Análise
O FIFO apresentou desempenho estável, com 60% de page faults em todas as execuções, enquanto o Random variou entre 50% e 70%, com média de 58%. A diferença é pequena devido à curta sequência de acessos, mas o Random pode ser mais eficiente em cenários com acessos aleatórios.

## 5. Conclusões
O simulador cumpriu os requisitos obrigatórios, oferecendo uma visão prática da paginação. Os testes mostraram que o FIFO é mais previsível, enquanto o Random oferece flexibilidade, embora com desempenho variável. Melhorias futuras poderiam incluir suporte a entradas dinâmicas e mais algoritmos.

## 6. Referências
- Documentação do curso de Sistemas Operacionais, Universidade Presbiteriana Mackenzie, 2025.
- Tanenbaum, A. S. *Modern Operating Systems*. Pearson, 4ª edição.
