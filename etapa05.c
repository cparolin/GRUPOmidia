#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

int main() {
    printf("Memory Paging Simulator\n");

    // Test the Simulador structure
    Simulador *sim = inicializar_simulador(4096, 16384); // 4 KB pages, 16 KB physical memory
    if (!sim) {
        printf("Failed to initialize simulator\n");
        return 1;
    }
    printf("Simulator initialized:\n");
    printf("- Page Size: %d bytes\n", sim->tamanho_pagina);
    printf("- Physical Memory: %d bytes\n", sim->tamanho_memoria_fisica);
    printf("- Number of Frames: %d\n", sim->memoria.num_frames);
    printf("- Algorithm: %d (FIFO)\n", sim->algoritmo);

    // Create a process
    Processo *proc = criar_processo(sim, 16384); // 16 KB process
    if (!proc) {
        printf("Failed to create process\n");
        // Clean up simulator
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return 1;
    }
    printf("Process created:\n");
    printf("- PID: %d\n", proc->pid);
    printf("- Number of Pages: %d\n", proc->num_paginas);

    // Clean up
    for (int i = 0; i < sim->num_processos; i++) {
        free(sim->processos[i].tabela_paginas);
    }
    free(sim->processos);
    free(sim->memoria.frames);
    free(sim->memoria.tempo_carga);
    free(sim);

    return 0;
}