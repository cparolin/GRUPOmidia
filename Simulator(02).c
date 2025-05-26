#ifndef SIMULATOR_H
#define SIMULATOR_H

// Represents a page in the virtual memory system
typedef struct {
    int presente;       // 1 if the page is in physical memory, 0 otherwise
    int frame;          // Frame number in physical memory (-1 if not allocated)
    int modificada;     // 1 if the page was modified, 0 otherwise
    int referencia;     // 1 if the page was recently referenced, 0 otherwise
    int tempo_carga;    // Time when the page was loaded into memory
    int ultimo_acesso;  // Time of the last access to the page
} Pagina;

#endif // SIMULATOR_H