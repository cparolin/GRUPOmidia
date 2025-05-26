#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

// Memory Paging Simulator: Simulates paging in an operating system with virtual memory management.
int main() {
    printf("Memory Paging Simulator\n");

    // Test the Pagina structure
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

    // Clean up
    free(proc.tabela_paginas);

    return 0;
}