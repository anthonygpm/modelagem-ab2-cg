#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <GL/freeglut.h>

// Luminária de teto com duas lâmpadas tubulares.
// A luz em si é configurada em Cena.cpp (configurarLuzes).
void desenharLampada(bool acesa)
{
    // Calha e aletas do refletor
    plastico(0.92f, 0.92f, 0.9f);
    bloco(0, -0.03f, 0, 1.25f, 0.06f, 0.32f);
    metal(0.85f, 0.86f, 0.88f);
    for (float sz : {-0.155f, 0.155f})
        bloco(0, -0.07f, sz, 1.25f, 0.04f, 0.01f);

    // Tubos: cilindros deitados (rotação de 90° em Z), brilham quando acesos
    material(0.95f, 0.95f, 0.93f, 0, 1);
    if (acesa) emissao(1, 1, 1);
    for (float sz : {-0.07f, 0.07f}) {
        glPushMatrix();
        glTranslatef(0, -0.085f, sz);
        glRotatef(90, 0, 0, 1);
        glScalef(0.032f, 1.15f, 0.032f);
        cilindro();
        glPopMatrix();
    }
    emissao(0, 0, 0);
}
