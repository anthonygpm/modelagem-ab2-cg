#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"
#include "Texturas.h"

#include <GL/freeglut.h>

// Porta de madeira que gira em torno da dobradiça.
// Para girar no pivô (e não no centro da folha):
//   1. glTranslatef até a dobradiça
//   2. glRotatef em Y
//   3. desenha a folha deslocada meia largura a partir do pivô
// Origem no centro inferior do vão; +Z aponta para dentro da sala.
void desenharPorta()
{
    const float LARGURA = 0.94f, ALTURA_FOLHA = 2.06f;

    // Batentes
    material(0.75f, 0.6f, 0.45f, 0.2f, 16, tex.madeira);
    for (float sx : {-1.0f, 1.0f})
        bloco(sx * 0.52f, 1.075f, -0.1f, 0.06f, 2.15f, 0.24f);
    bloco(0, 2.12f, -0.1f, 1.1f, 0.06f, 0.24f);

    // Folha
    glPushMatrix();
    glTranslatef(-LARGURA / 2, 0, -0.02f);                // 1. vai até a dobradiça
    glRotatef(-95 * suavizar(estado.porta), 0, 1, 0);     // 2. gira até 95°

    material(1, 1, 1, 0.25f, 24, tex.porta);
    bloco(LARGURA / 2, ALTURA_FOLHA / 2 + 0.01f, 0, LARGURA, ALTURA_FOLHA, 0.04f);   // 3.

    // Dobradiças
    metal(0.8f, 0.8f, 0.82f);
    for (float y : {0.25f, 1.05f, 1.85f})
        tubo(0, y, 0, 0.025f, 0.1f, 0.025f);

    // Maçanetas dos dois lados
    for (float lado : {-1.0f, 1.0f}) {
        glPushMatrix();
        glTranslatef(0.85f, 1.0f, lado * 0.025f);
        glRotatef(90, 1, 0, 0);
        glScalef(0.055f, 0.012f, 0.055f);
        cilindro();                                               // espelho
        glPopMatrix();
        bloco(0.85f, 1.0f, lado * 0.04f, 0.018f, 0.018f, 0.03f);   // haste
        bloco(0.80f, 1.0f, lado * 0.058f, 0.12f, 0.02f, 0.02f);    // alavanca
    }
    glPopMatrix();
}
