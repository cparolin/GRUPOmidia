#ifndef SIMULATOR_H
#define SIMULATOR_H

// Represents a page in the virtual memory system
typedef struct {
    int presente;       
    int frame;          // Frame number in physical memory (-1 if not allocated)
    int modificada;     
    int referencia;     // 1 if the page was recently referenced, 0 otherwise
    int tempo_carga;   
    int ultimo_acesso;  
} Pagina;


typedef struct {
    int pid;                
    int tamanho;            /
    int num_paginas;       
    Pagina *tabela_paginas; 
} Processo;

#endif // SIMULATOR_H