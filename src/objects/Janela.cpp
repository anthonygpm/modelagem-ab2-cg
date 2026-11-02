#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <GL/freeglut.h>

// Janela de alumínio com persiana horizontal. Origem no centro do vão;
// +Z aponta para dentro da sala.
//   - Lâminas giram em X (ROTAÇÃO)                       -> tecla E ou Q
//   - Persiana recolhe: lâminas sobem e se empilham      -> tecla R
//     (TRANSLAÇÃO das lâminas e da régua, ESCALA dos cordões)

const float W = 4.0f;   // largura do vão
const float H = 1.4f;   // altura do vão
const int NUM_LAMINAS = 26;

float luzDaJanela()
{
    float fechada = suavizar(estado.laminas) * (1 - suavizar(estado.recolhimento));
    return 1 - 0.88f * fechada;
}

void desenharJanela()
{
    // Esquadria de alumínio
    metal(0.82f, 0.83f, 0.85f);
    bloco(0, H / 2 + 0.03f, -0.1f, W + 0.06f, 0.06f, 0.2f);
    bloco(0, -H / 2 - 0.03f, -0.1f, W + 0.06f, 0.06f, 0.2f);
    for (float sx : {-1.0f, 1.0f})
        bloco(sx * (W / 2 + 0.03f), 0, -0.1f, 0.06f, H + 0.12f, 0.2f);
    for (float x : {-W / 4, 0.0f, W / 4})
        bloco(x, 0, -0.1f, 0.04f, H, 0.06f);
    bloco(0, H / 2 - 0.35f, -0.1f, W, 0.035f, 0.05f);

    // Peitoril de granito
    material(0.42f, 0.39f, 0.37f, 0.5f, 64);
    bloco(0, -H / 2 - 0.08f, 0.02f, W + 0.3f, 0.04f, 0.26f);

    // ---------------- Persiana ----------------
    material(0.93f, 0.92f, 0.88f, 0.5f, 40);

    const float z = 0.1f;   // a persiana fica à frente do vão
    const float topo = H / 2 + 0.02f;
    const float recolhida = suavizar(estado.recolhimento);
    const float espaco = (1 - recolhida) * (H + 0.06f) / NUM_LAMINAS + recolhida * 0.009f;
    const float angulo = 75 * suavizar(estado.laminas) * (1 - recolhida);

    // Trilho superior
    bloco(0, topo + 0.035f, z, W + 0.1f, 0.05f, 0.07f);

    // Lâminas: translação até a altura de cada uma, rotação e escala
    for (int i = 1; i <= NUM_LAMINAS; i++) {
        glPushMatrix();
        glTranslatef(0, topo - i * espaco, z);
        glRotatef(angulo, 1, 0, 0);
        glScalef(W, 0.004f, 0.066f);
        cubo();
        glPopMatrix();
    }

    // Régua inferior: acompanha a última lâmina (translação)
    const float yBase = topo - NUM_LAMINAS * espaco - 0.025f;
    bloco(0, yBase, z, W, 0.018f, 0.04f);

    // Cordões da escada: escala em Y igual à altura atual da persiana
    fosco(0.85f, 0.85f, 0.8f);
    const float comprimento = topo - yBase;
    for (float sx : {-0.35f * W, 0.35f * W})
        bloco(sx, topo - comprimento / 2, z, 0.003f, comprimento, 0.003f);

    // Cordão de puxar: fica mais comprido quando a persiana está recolhida
    const float puxar = 0.5f + 0.9f * recolhida;
    bloco(W / 2 - 0.05f, topo - puxar / 2, z + 0.04f, 0.004f, puxar, 0.004f);
    material(0.93f, 0.92f, 0.88f, 0.5f, 40);
    tubo(W / 2 - 0.05f, topo - puxar - 0.03f, z + 0.04f, 0.018f, 0.05f, 0.018f);
}

// Vidro transparente: desenhado por último, com blending e sem gravar profundidade
void desenharVidroJanela()
{
    material(0.75f, 0.88f, 0.95f, 1.0f, 128);
    const float vidro[] = {0.75f, 0.88f, 0.95f, 0.18f};   // alfa = 0,18
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vidro);

    glDepthMask(GL_FALSE);
    glPushMatrix();
    glTranslatef(0, 0, -0.1f);
    glScalef(W, H, 1);
    plano();
    glPopMatrix();
    glDepthMask(GL_TRUE);
}
