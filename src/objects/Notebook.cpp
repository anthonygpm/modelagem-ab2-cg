#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"
#include "Texturas.h"

#include <GL/freeglut.h>

// Notebook: a tampa gira na dobradiça (rotação em X) quando abre.
// Quem usa fica do lado +Z.
void desenharNotebook()
{
    float abertura = suavizar(estado.notebook);   // 0 fechado, 1 aberto

    // Base, teclado e touchpad
    material(0.32f, 0.33f, 0.36f, 0.6f, 48);
    bloco(0, 0.01f, 0, 0.34f, 0.02f, 0.24f);
    fosco(0.06f, 0.06f, 0.07f);
    bloco(0, 0.0205f, -0.025f, 0.30f, 0.002f, 0.11f);
    plastico(0.25f, 0.26f, 0.28f);
    bloco(0, 0.0205f, 0.075f, 0.10f, 0.0015f, 0.06f);

    // Tampa: translada até a dobradiça (fundo da base) e gira até 110°
    glPushMatrix();
    glTranslatef(0, 0.02f, -0.12f);
    glRotatef(-110 * abertura, 1, 0, 0);

    material(0.32f, 0.33f, 0.36f, 0.6f, 48);
    bloco(0, 0.006f, 0.12f, 0.34f, 0.012f, 0.24f);

    // Tela: apagada quando fechado; acende (emissão) ao abrir
    if (abertura > 0.05f) {
        material(1, 1, 1, 0.4f, 64, tex.notebook);
        emissao(abertura, abertura, abertura);
    } else {
        material(0.02f, 0.02f, 0.02f, 0.4f, 64);
    }
    glTranslatef(0, -0.0008f, 0.125f);
    glRotatef(90, 1, 0, 0);
    glScalef(0.31f, 0.2f, 1);
    plano();
    emissao(0, 0, 0);

    glPopMatrix();
}
