#include <stdlib.h>
#include "simulator.h"

// Initialize the simulator with the given page size and physical memory size
Simulador* inicializar_simulador(int tamanho_pagina, int tamanho_memoria_fisica) {
    // Validate parameters
    if (tamanho_pagina <= 0 || tamanho_memoria_fisica <= 0 || tamanho_memoria_fisica < tamanho_pagina) {
        return NULL; 
    }

    Simulador *sim = (Simulador *)malloc(sizeof(Simulador));
    if (!sim) return NULL;

    sim->tempo_atual = 0;
    sim->tamanho_pagina = tamanho_pagina;
    sim->tamanho_memoria_fisica = tamanho_memoria_fisica;
    sim->num_processos = 0;
    sim->processos = NULL;
    sim->memoria.num_frames = tamanho_memoria_fisica / tamanho_pagina;
    sim->memoria.frames = (int *)calloc(sim->memoria.num_frames, sizeof(int));
    sim->memoria.tempo_carga = (int *)calloc(sim->memoria.num_frames, sizeof(int));
    if (!sim->memoria.frames || !sim->memoria.tempo_carga) {
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return NULL;
    }
    sim->total_acessos = 0;
    sim->page_faults = 0;
    sim->algoritmo = 0; // Default to FIFO
    return sim;
}

Processo* criar_processo(Simulador *sim, int tamanho_processo) {
    if (!sim || tamanho_processo <= 0) return NULL;

    sim->processos = (Processo *)realloc(sim->processos, (sim->num_processos + 1) * sizeof(Processo));
    if (!sim->processos) return NULL;

    Processo *proc = &sim->processos[sim->num_processos];
    proc->pid = sim->num_processos + 1;
    proc->tamanho = tamanho_processo;
    proc->num_paginas = (tamanho_processo + sim->tamanho_pagina - 1) / sim->tamanho_pagina; // Ceiling division
    proc->tabela_paginas = (Pagina *)calloc(proc->num_paginas, sizeof(Pagina));
    if (!proc->tabela_paginas) {
        sim->processos = (Processo *)realloc(sim->processos, sim->num_processos * sizeof(Processo));
        return NULL;
    }
    sim->num_processos++;
    return proc;
}