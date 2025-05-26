#include <stdlib.h>
#include <stdio.h>
#include "simulator.h"

// Log a message with the current simulation time
static void log_message(Simulador *sim, const char *message) {
    printf("Time t=%d: %s\n", sim->tempo_atual, message);
}

// Initialize the simulator with the given page size and physical memory size
Simulador* inicializar_simulador(int tamanho_pagina, int tamanho_memoria_fisica) {
    if (tamanho_pagina <= 0 || tamanho_memoria_fisica <= 0 || tamanho_memoria_fisica < tamanho_pagina) {
        return NULL; // Invalid parameters
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

// Create a new process with the given size and add it to the simulator
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

// Extract the page number and offset from a virtual address
void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento) {
    if (!sim || !pagina || !deslocamento || endereco_virtual < 0) {
        *pagina = -1;
        *deslocamento = -1;
        return;
    }
    *pagina = endereco_virtual / sim->tamanho_pagina;
    *deslocamento = endereco_virtual % sim->tamanho_pagina;
}

// Check if a page is present in physical memory
// Returns 1 if present, 0 if not present, -1 if invalid
int verificar_pagina(Simulador *sim, int pid, int pagina) {
    if (!sim) return -1;

    // Find the process
    Processo *proc = NULL;
    for (int i = 0; i < sim->num_processos; i++) {
        if (sim->processos[i].pid == pid) {
            proc = &sim->processos[i];
            break;
        }
    }
    if (!proc || pagina < 0 || pagina >= proc->num_paginas) {
        return -1; // Invalid PID or page number
    }
    return proc->tabela_paginas[pagina].presente;
}

// Select a frame to replace using FIFO policy
// Returns the frame number to use
int substituir_pagina_fifo(Simulador *sim) {
    if (!sim || sim->memoria.num_frames <= 0) return -1;

    // Find the oldest frame
    int oldest_frame = 0;
    int min_tempo = sim->memoria.tempo_carga[0];
    for (int i = 1; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.tempo_carga[i] < min_tempo) {
            min_tempo = sim->memoria.tempo_carga[i];
            oldest_frame = i;
        }
    }

    // If the frame is occupied, remove the current page
    if (sim->memoria.frames[oldest_frame] != -1) {
        int old_pid = sim->memoria.frames[oldest_frame] >> 16;
        int old_page = sim->memoria.frames[oldest_frame] & 0xFFFF;
        for (int i = 0; i < sim->num_processos; i++) {
            if (sim->processos[i].pid == old_pid) {
                sim->processos[i].tabela_paginas[old_page].presente = 0;
                sim->processos[i].tabela_paginas[old_page].frame = -1;
                break;
            }
        }
    }

    char msg[100];
    snprintf(msg, sizeof(msg), "[PAGE FAULT] Replacing page in Frame %d using FIFO", oldest_frame);
    log_message(sim, msg);
    return oldest_frame;
}

// Load a page into physical memory
int carregar_pagina(Simulador *sim, int pid, int pagina) {
    if (!sim) return -1;

    // Find the process
    Processo *proc = NULL;
    for (int i = 0; i < sim->num_processos; i++) {
        if (sim->processos[i].pid == pid) {
            proc = &sim->processos[i];
            break;
        }
    }
    if (!proc || pagina < 0 || pagina >= proc->num_paginas) {
        return -1; // Invalid PID or page number
    }

    // Check for a free frame
    int frame = -1;
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.frames[i] == -1) {
            frame = i;
            break;
        }
    }

    // If no free frame, use FIFO replacement
    if (frame == -1) {
        frame = substituir_pagina_fifo(sim);
        if (frame == -1) return -1; 
    } else {
        char msg[100];
        snprintf(msg, sizeof(msg), "[PAGE FAULT] Loading Page %d of Process %d into Frame %d", 
                 pagina, pid, frame);
        log_message(sim, msg);
    }

    // Update the frame
    sim->memoria.frames[frame] = (pid << 16) | pagina;
    sim->memoria.tempo_carga[frame] = sim->tempo_atual;

    // Update the page table
    proc->tabela_paginas[pagina].presente = 1;
    proc->tabela_paginas[pagina].frame = frame;
    proc->tabela_paginas[pagina].tempo_carga = sim->tempo_atual;

    sim->page_faults++;
    return frame;
}