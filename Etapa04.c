#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

// Memory Paging Simulator: Simulates paging in an operating system with virtual memory management.
int main() {
    printf("Memory Paging Simulator\n");

    
    Pagina page = {0, -1, 0, 0, 0, 0};
    printf("Page initialized:\n");
    printf("- Presente: %d\n", page.presente);
    printf("- Frame: %d\n", page.frame);
    printf("- Modificada: %d\n", page.modificada);
    printf("- Referencia: %d\n", page.referencia);
    printf("- Tempo de Carga: %d\n", page.tempo_carga);
    printf("- Ultimo Acesso: %d\n", page.ultimo_acesso);

    
    Processo proc;
    proc.pid = 1;
    proc.tamanho = 16384; // 16 KB
    proc.num_paginas = 4; // Assuming 4 KB pages (16384 / 4096 = 4 pages)
    proc.tabela_paginas = (Pagina *)calloc(proc.num_paginas, sizeof(Pagina));
    printf("Process initialized:\n");
    printf("- PID: %d\n", proc.pid);
    printf("- Number of Pages: %d\n", proc.num_paginas);
    printf("- Page Table Entry 0 - Presente: %d, Frame: %d\n", 
           proc.tabela_paginas[0].presente, proc.tabela_paginas[0].frame);

    // Test the MemoriaFisica structure
    MemoriaFisica mem;
    mem.num_frames = 4; // 16 KB physical memory / 4 KB page size = 4 frames
    mem.frames = (int *)calloc(mem.num_frames, sizeof(int));
    mem.tempo_carga = (int *)calloc(mem.num_frames, sizeof(int));
    printf("Physical Memory initialized:\n");
    printf("- Number of Frames: %d\n", mem.num_frames);
    printf("- Frame 0 - Content: %d, Load Time: %d\n", mem.frames[0], mem.tempo_carga[0]);

    free(proc.tabela_paginas);
    free(mem.frames);
    free(mem.tempo_carga);

    return 0;
}