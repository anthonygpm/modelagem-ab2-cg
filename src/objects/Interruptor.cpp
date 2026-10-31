#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <GL/freeglut.h>

// Interruptor de parede: a tecla gira 12° para cima ou para baixo.
void desenharInterruptor()
{
    // Espelho
    plastico(0.95f, 0.95f, 0.93f);
    bloco(0, 0, 0.006f, 0.08f, 0.12f, 0.012f);

    // Tecla (rotação em X)
    glPushMatrix();
    glTranslatef(0, 0, 0.013f);
    glRotatef(estado.luzesLigadas ? 12 : -12, 1, 0, 0);
    plastico(0.88f, 0.88f, 0.86f);
    bloco(0, 0, 0.004f, 0.028f, 0.05f, 0.012f);
    glPopMatrix();

    // Indicador luminoso: acende quando as luzes estão apagadas
    if (estado.luzesLigadas) {
        fosco(0.4f, 0.3f, 0.2f);
    } else {
        material(1, 0.55f, 0.1f, 0, 1);
        emissao(1, 0.55f, 0.1f);
    }
    bloco(0, -0.042f, 0.0125f, 0.008f, 0.008f, 0.002f);
    emissao(0, 0, 0);
}
