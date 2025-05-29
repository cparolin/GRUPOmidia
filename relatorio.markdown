# Relatório do Projeto: Simulador de Paginação de Memória

## 1. Introdução
### Contexto
A paginação é uma técnica fundamental no gerenciamento de memória de sistemas operacionais, permitindo a divisão de processos em páginas e a alocação dinâmica em memória física. Este trabalho foi realizado para a disciplina de Sistemas Operacionais da Universidade Presbiteriana Mackenzie, no 1º semestre de 2025, sob a supervisão do Prof. Lucas Cerqueira Figueiredo.

### Objetivos
Desenvolver um simulador que demonstre os conceitos de paginação, incluindo tradução de endereços, tratamento de page faults e uso de algoritmos de substituição de páginas (FIFO e Random), atendendo aos requisitos obrigatórios do projeto.

## 2. Implementação
### Estruturas de Dados
- **Pagina**: Estrutura que armazena informações como presença na memória (`presente`), número do frame (`frame`) e tempo de carregamento (`tempo_carga`).
- **Processo**: Representa um processo com sua tabela de páginas e identificador único (`pid`).
- **MemoriaFisica**: Gerencia os frames da memória física.
- **Simulador**: Coordena a simulação, mantendo estatísticas e configurando os parâmetros.

### Funcionalidades Implementadas
- **Tradução de Endereços**: Converte endereços virtuais em físicos usando a função `traduzir_endereco`.
- **Gerenciamento de Page Faults**: Detecta e carrega páginas faltantes com `verificar_pagina` e `carregar_pagina`.
- **Algoritmos de Substituição**: 
  - **FIFO**: Substitui a página mais antiga.
  - **Random**: Escolhe aleatoriamente uma página para substituição.
- **Simulação de Múltiplos Processos**: Suporta até 3 processos com 4 páginas cada.
- **Exibição de Estado**: Mostra o estado da memória física e estatísticas via `exibir_memoria_fisica` e `exibir_estatisticas`.

### Escolhas de Design
- O código foi escrito em C, com modularidade em `simulator.h` e `simulator.c`.
- A simulação utiliza parâmetros fixos (4 KB por página, 16 KB de memória) baseados na seção 7 do documento.

### Restrições
- A sequência de acessos é pré-definida, sem suporte a entrada dinâmica.
- Apenas dois algoritmos obrigatórios foram implementados.

## 3. Resultados e Análise
### Resultados da Simulação
A saída do simulador exibe cada acesso à memória, indicando page faults, frames utilizados e estatísticas finais. Por exemplo, com 4 frames e a sequência de acessos definida, a taxa de page faults varia entre 40% e 60%, dependendo do algoritmo e da aleatoriedade no caso do Random.

### Comparação dos Algoritmos
- **FIFO**: Previsível, mas pode ser menos eficiente em cenários com padrões de acesso anômalos.
- **Random**: Mais imprevisível, mas evita o problema de Belady em alguns casos, embora o desempenho seja instável.

## 4. Conclusões
O simulador atendeu aos requisitos obrigatórios, proporcionando uma visão prática dos conceitos de paginação. A experiência reforçou o entendimento de gerenciamento de memória, e futuras extensões poderiam incluir suporte a entradas dinâmicas ou algoritmos como LRU.

## 5. Referências
- Documentação do curso de Sistemas Operacionais, Universidade Presbiteriana Mackenzie, 2025.
- Silberschatz, A., Galvin, P. B., & Gagne, G. *Operating System Concepts*. Wiley, 10ª edição.