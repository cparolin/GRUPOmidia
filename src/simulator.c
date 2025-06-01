#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "simulator.h"

// Initialize the simulator with the given parameters
Simulador* inicializar_simulador(int tamanho_pagina, int tamanho_memoria_fisica) {
    Simulador *sim = (Simulador *)malloc(sizeof(Simulador));
    sim->tempo_atual = 0;
    sim->tamanho_pagina = tamanho_pagina;
    sim->tamanho_memoria_fisica = tamanho_memoria_fisica;
    sim->num_processos = 0;
    sim->processos = NULL;
    sim->memoria.num_frames = tamanho_memoria_fisica / tamanho_pagina;
    sim->memoria.frames = (int *)calloc(sim->memoria.num_frames, sizeof(int));
    sim->memoria.tempo_carga = (int *)calloc(sim->memoria.num_frames, sizeof(int));
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        sim->memoria.frames[i] = -1;
        sim->memoria.tempo_carga[i] = 0;
    }
    sim->total_acessos = 0;
    sim->page_faults = 0;
    sim->algoritmo = 0;
    srand(time(NULL));
    return sim;
}

// Create a new process and add it to the simulator
Processo* criar_processo(Simulador *sim, int tamanho_processo) {
    sim->processos = (Processo *)realloc(sim->processos, (sim->num_processos + 1) * sizeof(Processo));
    Processo *proc = &sim->processos[sim->num_processos];
    proc->pid = sim->num_processos + 1;
    proc->tamanho = tamanho_processo;
    proc->num_paginas = (tamanho_processo + sim->tamanho_pagina - 1) / sim->tamanho_pagina;
    proc->tabela_paginas = (Pagina *)calloc(proc->num_paginas, sizeof(Pagina));
    for (int i = 0; i < proc->num_paginas; i++) {
        proc->tabela_paginas[i].presente = 0;
        proc->tabela_paginas[i].frame = -1;
        proc->tabela_paginas[i].modificada = 0;
        proc->tabela_paginas[i].referencia = 0;
        proc->tabela_paginas[i].tempo_carga = 0;
        proc->tabela_paginas[i].ultimo_acesso = 0;
    }
    sim->num_processos++;
    return proc;
}

// Extract the page number and offset from a virtual address
void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento) {
    *pagina = endereco_virtual / sim->tamanho_pagina;
    *deslocamento = endereco_virtual % sim->tamanho_pagina;
}

// Verify if a page is present in physical memory
int verificar_pagina(Simulador *sim, int pid, int pagina) {
    Processo *proc = NULL;
    for (int i = 0; i < sim->num_processos; i++) {
        if (sim->processos[i].pid == pid) {
            proc = &sim->processos[i];
            break;
        }
    }
    if (!proc || pagina >= proc->num_paginas) return -1;
    return proc->tabela_paginas[pagina].presente;
}

// Implement FIFO page replacement
int substituir_pagina_fifo(Simulador *sim) {
    int oldest_frame = 0, min_tempo = sim->memoria.tempo_carga[0];
    for (int i = 1; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.tempo_carga[i] < min_tempo) {
            min_tempo = sim->memoria.tempo_carga[i];
            oldest_frame = i;
        }
    }
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
    return oldest_frame;
}

// Implement Random page replacement
int substituir_pagina_random(Simulador *sim) {
    int frame = rand() % sim->memoria.num_frames;
    if (sim->memoria.frames[frame] != -1) {
        int old_pid = sim->memoria.frames[frame] >> 16;
        int old_page = sim->memoria.frames[frame] & 0xFFFF;
        for (int i = 0; i < sim->num_processos; i++) {
            if (sim->processos[i].pid == old_pid) {
                sim->processos[i].tabela_paginas[old_page].presente = 0;
                sim->processos[i].tabela_paginas[old_page].frame = -1;
                break;
            }
        }
    }
    return frame;
}

// Load a page into physical memory
int carregar_pagina(Simulador *sim, int pid, int pagina) {
    Processo *proc = NULL;
    for (int i = 0; i < sim->num_processos; i++) {
        if (sim->processos[i].pid == pid) {
            proc = &sim->processos[i];
            break;
        }
    }
    if (!proc || pagina >= proc->num_paginas) return -1;

    int frame = -1;
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.frames[i] == -1) {
            frame = i;
            break;
        }
    }
    if (frame == -1) {
        if (sim->algoritmo == 0) {
            frame = substituir_pagina_fifo(sim);
            printf("Time t=%d: [PAGE FAULT] Replacing page in Frame %d using FIFO\n", sim->tempo_atual, frame);
        } else if (sim->algoritmo == 3) {
            frame = substituir_pagina_random(sim);
            printf("Time t=%d: [PAGE FAULT] Replacing page in Frame %d using Random\n", sim->tempo_atual, frame);
        }
    } else {
        printf("Time t=%d: [PAGE FAULT] Loading Page %d of Process %d into Frame %d\n", sim->tempo_atual, pagina, pid, frame);
    }

    sim->memoria.frames[frame] = (pid << 16) | pagina;
    sim->memoria.tempo_carga[frame] = sim->tempo_atual;
    proc->tabela_paginas[pagina].presente = 1;
    proc->tabela_paginas[pagina].frame = frame;
    proc->tabela_paginas[pagina].tempo_carga = sim->tempo_atual;
    sim->page_faults++;
    return frame;
}

// Translate a virtual address to a physical address
int traduzir_endereco(Simulador *sim, int pid, int endereco_virtual) {
    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, endereco_virtual, &pagina, &deslocamento);

    int presente = verificar_pagina(sim, pid, pagina);
    if (presente == -1) return -1;

    int frame;
    if (!presente) {
        frame = carregar_pagina(sim, pid, pagina);
        if (frame == -1) return -1;
    } else {
        Processo *proc = NULL;
        for (int i = 0; i < sim->num_processos; i++) {
            if (sim->processos[i].pid == pid) {
                proc = &sim->processos[i];
                break;
            }
        }
        frame = proc->tabela_paginas[pagina].frame;
    }

    int endereco_fisico = frame * sim->tamanho_pagina + deslocamento;
    sim->total_acessos++;
    sim->tempo_atual++;
    printf("Time t=%d: Virtual Address (P%d): %d -> Page: %d -> Frame: %d -> Physical Address: %d\n",
           sim->tempo_atual, pid, endereco_virtual, pagina, frame, endereco_fisico);
    return endereco_fisico;
}

// Display the current state of physical memory
void exibir_memoria_fisica(Simulador *sim) {
    printf("Time t=%d\n", sim->tempo_atual);
    printf("State of Physical Memory:\n");
    for (int i = 0; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.frames[i] == -1) {
            printf("| ----- |\n");
        } else {
            int pid = sim->memoria.frames[i] >> 16;
            int page = sim->memoria.frames[i] & 0xFFFF;
            printf("| P%d-%d |\n", pid, page);
        }
    }
}

// Display simulation statistics
void exibir_estatisticas(Simulador *sim) {
    printf("\nTotal memory accesses: %d\n", sim->total_acessos);
    printf("Total page faults: %d\n", sim->page_faults);
    float fault_rate = sim->total_acessos > 0 ? (float)sim->page_faults / sim->total_acessos * 100 : 0;
    printf("Page fault rate: %.2f%%\n", fault_rate);
    printf("Algorithm: %s\n", sim->algoritmo == 0 ? "FIFO" : "Random");
}

// Simulate a memory access
int acessar_memoria(Simulador *sim, int pid, int endereco_virtual) {
    int endereco_fisico = traduzir_endereco(sim, pid, endereco_virtual);
    if (endereco_fisico != -1) exibir_memoria_fisica(sim);
    return endereco_fisico;
}

// Execute the simulation with a sequence of memory accesses
void executar_simulacao(Simulador *sim, int algoritmo) {
    sim->algoritmo = algoritmo;
    printf("Page size: %d bytes (4 KB)\n", sim->tamanho_pagina);
    printf("Physical memory size: %d bytes (16 KB)\n", sim->tamanho_memoria_fisica);
    printf("Number of frames: %d\n", sim->memoria.num_frames);
    printf("Page replacement algorithm: %s\n\n", sim->algoritmo == 0 ? "FIFO" : "Random");

    int acessos[][2] = {
        {1, 1111}, {1, 4444}, {2, 2000}, {3, 3000}, {3, 5000},
        {1, 4444}, {2, 2000}, {3, 3000}, {1, 1111}, {1, 4444}
    };
    int num_acessos = sizeof(acessos) / sizeof(acessos[0]);

    for (int i = 0; i < num_acessos; i++) {
        int pid = acessos[i][0];
        int endereco_virtual = acessos[i][1];
        acessar_memoria(sim, pid, endereco_virtual);
    }
    exibir_estatisticas(sim);
}
