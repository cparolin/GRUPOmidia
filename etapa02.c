#include <stdio.h>
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

    return 0;
}