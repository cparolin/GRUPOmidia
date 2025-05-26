#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

// Memory Paging Simulator: Simulates paging in an operating system with virtual memory management.
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
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return 1;
    }
    printf("Process created:\n");
    printf("- PID: %d\n", proc->pid);
    printf("- Number of Pages: %d\n", proc->num_paginas);

    // Test extracting page number and offset
    int virtual_addr = 4444; // Example virtual address
    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, virtual_addr, &pagina, &deslocamento);
    if (pagina == -1 || deslocamento == -1) {
        printf("Failed to extract page and offset for Virtual Address: %d\n", virtual_addr);
        // Clean up
        for (int i = 0; i < sim->num_processos; i++) {
            free(sim->processos[i].tabela_paginas);
        }
        free(sim->processos);
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return 1;
    }
    printf("Virtual Address: %d -> Page: %d, Offset: %d\n", virtual_addr, pagina, deslocamento);

    // Test verifying page fault
    int page_present = verificar_pagina(sim, proc->pid, pagina);
    if (page_present == -1) {
        printf("Invalid page or process: PID %d, Page %d\n", proc->pid, pagina);
        // Clean up
        for (int i = 0; i < sim->num_processos; i++) {
            free(sim->processos[i].tabela_paginas);
        }
        free(sim->processos);
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return 1;
    }
    printf("Page %d of Process %d - Present: %d\n", pagina, proc->pid, page_present);

    // Test loading a page into memory
    sim->tempo_atual = 0;
    int frame = carregar_pagina(sim, proc->pid, pagina);
    if (frame == -1) {
        printf("Failed to load Page %d of Process %d into memory\n", pagina, proc->pid);
        // Clean up
        for (int i = 0; i < sim->num_processos; i++) {
            free(sim->processos[i].tabela_paginas);
        }
        free(sim->processos);
        free(sim->memoria.frames);
        free(sim->memoria.tempo_carga);
        free(sim);
        return 1;
    }
    printf("Page %d of Process %d loaded into Frame %d\n", pagina, proc->pid, frame);

    // Verify the page is now present
    page_present = verificar_pagina(sim, proc->pid, pagina);
    if (page_present == -1) {
        printf("Invalid page or process: PID %d, Page %d\n", proc->pid, pagina);
    } else {
        printf("Page %d of Process %d - Present: %d\n", pagina, proc->pid, page_present);
    }

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