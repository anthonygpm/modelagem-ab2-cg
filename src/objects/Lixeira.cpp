#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

void desenharLixeira()
{
    plastico(0.2f, 0.22f, 0.24f);
    tubo(0, 0.21f, 0, 0.32f, 0.42f, 0.32f);    // corpo
    tubo(0, 0.42f, 0, 0.34f, 0.03f, 0.34f);    // borda
    fosco(0.03f, 0.03f, 0.03f);
    tubo(0, 0.437f, 0, 0.29f, 0.004f, 0.29f);  // abertura escura
}
