#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"
#include "Texturas.h"

#include <GL/freeglut.h>

// Projetor de teto, aponta para -Z (para o quadro)
void desenharProjetor()
{
    // Suporte e corpo
    metal(0.3f, 0.3f, 0.32f);
    bloco(0, -0.02f, 0, 0.2f, 0.04f, 0.2f);
    tubo(0, -0.19f, 0, 0.04f, 0.3f, 0.04f);
    plastico(0.9f, 0.9f, 0.88f);
    bloco(0, -0.39f, 0, 0.34f, 0.11f, 0.27f);

    // Lente: cilindro girado 90° em X para apontar para a tela
    metal(0.15f, 0.15f, 0.17f);
    glPushMatrix();
    glTranslatef(0.09f, -0.39f, -0.14f);
    glRotatef(90, 1, 0, 0);
    glScalef(0.075f, 0.03f, 0.075f);
    cilindro();
    glPopMatrix();

    // Vidro da lente: acende quando o projetor está ligado
    if (estado.projetorLigado) {
        material(0.9f, 0.95f, 1, 0, 1);
        emissao(0.9f, 0.95f, 1);
    } else {
        fosco(0.05f, 0.05f, 0.08f);
    }
    glPushMatrix();
    glTranslatef(0.09f, -0.39f, -0.156f);
    glRotatef(90, 1, 0, 0);
    glScalef(0.055f, 0.004f, 0.055f);
    cilindro();
    glPopMatrix();
    emissao(0, 0, 0);

    // Grade de ventilação lateral
    fosco(0.15f, 0.15f, 0.16f);
    for (int i = 0; i < 6; i++)
        bloco(0.171f, -0.39f, -0.075f + i * 0.03f, 0.002f, 0.07f, 0.012f);
}

// Tela de projeção retrátil. Origem na caixa da tela, presa no teto.
// A tela desce por ESCALA em Y com o pivô no topo; a régua de baixo
// acompanha por TRANSLAÇÃO.
void desenharTelaProjecao()
{
    // Caixa onde a tela fica enrolada
    plastico(0.9f, 0.9f, 0.88f);
    bloco(0, 0.05f, 0, 2.6f, 0.09f, 0.09f);

    const float comprimento = 1.7f * suavizar(estado.tela);
    if (comprimento > 0.005f) {
        // Mostra o slide quando o projetor está ligado e a tela terminou de descer
        if (estado.projetorLigado && estado.tela > 0.99f) {
            material(1, 1, 1, 0, 1, tex.slide);
            emissao(1, 1, 1);
        } else {
            fosco(0.97f, 0.97f, 0.97f);
        }
        glPushMatrix();
        glScalef(2.4f, comprimento, 1);   // escala em Y = quanto já desceu
        glTranslatef(0, -0.5f, 0);        // pivô no topo: o plano fica todo abaixo da origem
        plano();
        glPopMatrix();
        emissao(0, 0, 0);
    }

    // Régua inferior
    metal(0.3f, 0.3f, 0.32f);
    bloco(0, -comprimento, 0, 2.45f, 0.025f, 0.025f);
}

// Painel de controle da tela, na parede ao lado do quadro (é o ponto de interação)
void desenharPainelProjetor()
{
    plastico(0.92f, 0.92f, 0.9f);
    bloco(0, 0, 0.01f, 0.1f, 0.16f, 0.02f);
    plastico(0.25f, 0.25f, 0.27f);
    for (float y : {0.035f, -0.035f})
        bloco(0, y, 0.022f, 0.04f, 0.03f, 0.006f);

    if (estado.projetorLigado) {
        material(0.3f, 0.6f, 1, 0, 1);
        emissao(0.3f, 0.6f, 1);
    } else {
        fosco(0.2f, 0.2f, 0.25f);
    }
    bloco(0, 0.068f, 0.021f, 0.01f, 0.01f, 0.002f);
    emissao(0, 0, 0);
}
