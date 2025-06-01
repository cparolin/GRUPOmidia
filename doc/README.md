### Simulador de Paginação de Memória

## Integrantes do Grupo

- Camila Parolin (RA: 10425480)
- Rodrigo Lucas Mascarenhas (RA: 10427925)
- Andre Ward (RA: 10425684)

## Descrição do Projeto

Este projeto foi desenvolvido para a disciplina de Sistemas Operacionais da Universidade Presbiteriana Mackenzie, sob a orientação do Prof. Lucas Cerqueira Figueiredo, no 1º semestre de 2025. O simulador implementa o mecanismo de paginação de memória, demonstrando a tradução de endereços virtuais para físicos, o tratamento de page faults e a aplicação de dois algoritmos de substituição de páginas: FIFO e Random.

## Instruções de Compilação e Execução

1. Certifique-se de ter um compilador C (como `gcc`) instalado. Verifique com:

   ```
   gcc --version
   ```

   Se não estiver instalado, instale-o com (no Ubuntu, por exemplo):

   ```
   sudo apt update && sudo apt install build-essential
   ```

2. Clone o repositório 1. 2. Navegue até o diretório raiz do projeto:

   ```
   cd GRUPOmidia
   ```

   Verifique se a estrutura está correta com:

   ```
   ls -R
   ```

3. Compile o código:

   ```
    gcc -o simulador GRUPOmidia/src/main.c GRUPOmidia/src/simulator.c
   ```

   **Nota**: Se a compilação for bem-sucedida, nenhuma mensagem será exibida, mas o executável `simulador` será criado. Verifique com `ls simulador`.

4. Execute o simulador:

   ```
   ./simulador
   ```

## Documentação

O relatório completo do projeto está disponível em `doc/relatorio.md`, incluído neste repositório.

## Estrutura do Projeto

- `src/`: Contém os arquivos de código-fonte (`main.c`, `simulator.c`, `simulator.h`).
- `doc/`: Contém o arquivo de documentação (`relatorio.md`).
