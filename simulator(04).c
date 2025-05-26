#ifndef SIMULATOR_H
#define SIMULATOR_H


typedef struct {
    int presente;       
    int frame;          // Frame number in physical memory (-1 if not allocated)
    int modificada;    
    int referencia;     // 1 if the page was recently referenced, 0 otherwise
    int tempo_carga;    
    int ultimo_acesso; 
} Pagina;

// Represents a process with its page table
typedef struct {
    int pid;                
    int tamanho;            
    int num_paginas;        
    Pagina *tabela_paginas;
} Processo;


typedef struct {
    int num_frames;    
    int *frames;        // Array of frames (pid << 16 | page, -1 if empty)
    int *tempo_carga;   
} MemoriaFisica;

#endif // SIMULATOR_H