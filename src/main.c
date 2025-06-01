#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

int main() {
    printf("Memory Paging Simulator\n");

    Simulador *sim = inicializar_simulador(4096, 16384); // 4 KB pages, 16 KB physical memory

    for (int i = 0; i < 3; i++) {
        criar_processo(sim, 16384); // 3 processes, 16 KB each
    }
    printf("Created %d processes, each with 4 pages\n", sim->num_processos);

    printf("\nRunning simulation with FIFO...\n");
    sim->tempo_atual = 0;
    sim->total_acessos = 0;
    sim->page_faults = 0;
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        sim->memoria.frames[i] = -1;
        sim->memoria.tempo_carga[i] = 0;
    }
    for (int i = 0; i < sim->num_processos; i++) {
        for (int j = 0; j < sim->processos[i].num_paginas; j++) {
            sim->processos[i].tabela_paginas[j].presente = 0;
            sim->processos[i].tabela_paginas[j].frame = -1;
        }
    }
    executar_simulacao(sim, 0);

    printf("\nRunning simulation with Random...\n");
    sim->tempo_atual = 0;
    sim->total_acessos = 0;
    sim->page_faults = 0;
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        sim->memoria.frames[i] = -1;
        sim->memoria.tempo_carga[i] = 0;
    }
    for (int i = 0; i < sim->num_processos; i++) {
        for (int j = 0; j < sim->processos[i].num_paginas; j++) {
            sim->processos[i].tabela_paginas[j].presente = 0;
            sim->processos[i].tabela_paginas[j].frame = -1;
        }
    }
    executar_simulacao(sim, 3);

    for (int i = 0; i < sim->num_processos; i++) {
        free(sim->processos[i].tabela_paginas);
    }
    free(sim->processos);
    free(sim->memoria.frames);
    free(sim->memoria.tempo_carga);
    free(sim);

    return 0;
}
