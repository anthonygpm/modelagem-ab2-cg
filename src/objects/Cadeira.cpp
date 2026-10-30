#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <GL/freeglut.h>

// Cadeira de plástico com estrutura cromada. Quem senta olha para -Z.
void desenharCadeira(float r, float g, float b)
{
    // Pernas
    metal();
    for (float sx : {-0.19f, 0.19f})
        for (float sz : {-0.17f, 0.17f})
            tubo(sx, 0.215f, sz, 0.03f, 0.43f, 0.03f);

    // Assento
    plastico(r, g, b);
    bloco(0, 0.45f, 0, 0.44f, 0.035f, 0.42f);

    // Encosto: inclinado 10° para trás a partir da borda traseira do assento
    glPushMatrix();
    glTranslatef(0, 0.47f, 0.2f);
    glRotatef(10, 1, 0, 0);
    bloco(0, 0.27f, 0.01f, 0.42f, 0.26f, 0.03f);
    metal();
    for (float sx : {-0.16f, 0.16f})
        bloco(sx, 0.13f, -0.01f, 0.02f, 0.26f, 0.02f);
    glPopMatrix();
}
