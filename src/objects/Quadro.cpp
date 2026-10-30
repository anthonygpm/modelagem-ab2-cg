#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"
#include "Texturas.h"

#include <GL/freeglut.h>

// Quadro branco com moldura de alumínio, bandeja, pincéis e apagador.
// Origem na parede, virado para +Z.
void desenharQuadro()
{
    const float L = 3.6f, A = 1.2f, cy = 1.5f;   // largura, altura e altura do centro

    // Chapa e superfície com as anotações (textura)
    fosco(0.9f, 0.9f, 0.9f);
    bloco(0, cy, 0.015f, L, A, 0.03f);
    material(1, 1, 1, 0.55f, 80, tex.quadro);
    glPushMatrix();
    glTranslatef(0, cy, 0.032f);
    glScalef(L, A, 1);
    plano();
    glPopMatrix();

    // Moldura
    metal(0.78f, 0.79f, 0.81f);
    bloco(0, cy + A / 2 + 0.02f, 0.025f, L + 0.1f, 0.04f, 0.05f);
    bloco(0, cy - A / 2 - 0.02f, 0.025f, L + 0.1f, 0.04f, 0.05f);
    for (float sx : {-1.0f, 1.0f})
        bloco(sx * (L / 2 + 0.03f), cy, 0.025f, 0.04f, A + 0.08f, 0.05f);

    // Bandeja
    const float yBandeja = cy - A / 2 - 0.05f;
    bloco(0, yBandeja, 0.07f, L * 0.8f, 0.015f, 0.09f);

    // Pincéis: cilindros deitados (rotação de 90° em Z)
    const float cores[3][3] = {{0.1f, 0.2f, 0.75f}, {0.8f, 0.12f, 0.1f}, {0.08f, 0.08f, 0.08f}};
    for (int i = 0; i < 3; i++) {
        plastico(cores[i][0], cores[i][1], cores[i][2]);
        glPushMatrix();
        glTranslatef(0.9f + i * 0.09f, yBandeja + 0.0175f, 0.07f);
        glRotatef(90, 0, 0, 1);
        glScalef(0.02f, 0.13f, 0.02f);
        cilindro();
        glPopMatrix();
    }

    // Apagador
    plastico(0.15f, 0.15f, 0.17f);
    bloco(0.55f, yBandeja + 0.025f, 0.07f, 0.13f, 0.035f, 0.055f);
}
