#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <algorithm>

// Paredes da sala e do corredor, com os vãos da janela e da porta
const float E = ESPESSURA;
const Caixa PAREDES[] = {
    // Parede do quadro e parede do fundo
    {X_MIN - E, 0, Z_MIN - E, X_MAX + E, ALTURA, Z_MIN},
    {X_MIN - E, 0, Z_MAX, X_MAX + E, ALTURA, Z_MAX + E},
    // Parede da janela: dois lados, abaixo e acima do vão
    {X_MIN - E, 0, Z_MIN, X_MIN, ALTURA, JANELA_Z0},
    {X_MIN - E, 0, JANELA_Z1, X_MIN, ALTURA, Z_MAX},
    {X_MIN - E, 0, JANELA_Z0, X_MIN, JANELA_Y0, JANELA_Z1},
    {X_MIN - E, JANELA_Y1, JANELA_Z0, X_MIN, ALTURA, JANELA_Z1},
    // Parede da porta: dois lados e acima do vão
    {X_MAX, 0, Z_MIN, X_MAX + E, ALTURA, PORTA_Z0},
    {X_MAX, 0, PORTA_Z1, X_MAX + E, ALTURA, Z_MAX},
    {X_MAX, PORTA_ALTURA, PORTA_Z0, X_MAX + E, ALTURA, PORTA_Z1},
    // Corredor
    {CORREDOR_X1, 0, CORREDOR_Z0, CORREDOR_X1 + E, ALTURA, CORREDOR_Z1},
    {X_MAX, 0, CORREDOR_Z0 - E, CORREDOR_X1 + E, ALTURA, CORREDOR_Z0},
    {X_MAX + E, 0, CORREDOR_Z1, CORREDOR_X1 + E, ALTURA, CORREDOR_Z1 + E},
};
const int NUM_PAREDES = sizeof(PAREDES) / sizeof(PAREDES[0]);

void desenharSala()
{
    // Paredes: barrado pintado de azul até 1 m e reboco branco acima
    const float BARRADO = 1.0f;
    for (int i = 0; i < NUM_PAREDES; i++) {
        const Caixa& p = PAREDES[i];
        if (p.y0 < BARRADO) {
            material(0.60f, 0.70f, 0.78f, 0.25f, 24);
            caixa(p.x0, p.y0, p.z0, p.x1, std::min(p.y1, BARRADO), p.z1, 2.0f);
        }
        if (p.y1 > BARRADO) {
            material(0.97f, 0.96f, 0.93f, 0.05f, 8);
            caixa(p.x0, std::max(p.y0, BARRADO), p.z0, p.x1, p.y1, p.z1, 2.0f);
        }
    }

    // Piso
    material(0.78f, 0.76f, 0.72f, 0.35f, 48);
    caixa(X_MIN, -0.1f, Z_MIN, X_MAX, 0, Z_MAX, 1.2f);
    caixa(X_MAX, -0.1f, CORREDOR_Z0, CORREDOR_X1, 0, CORREDOR_Z1, 1.2f);

    // Forro do teto
    material(0.92f, 0.92f, 0.92f, 0.03f, 4);
    caixa(X_MIN, ALTURA, Z_MIN, X_MAX, ALTURA + 0.1f, Z_MAX, 1.25f);
    caixa(X_MAX, ALTURA, CORREDOR_Z0, CORREDOR_X1, ALTURA + 0.1f, CORREDOR_Z1, 1.25f);

    // Rodapé (faixas finas encostadas nas paredes)
    plastico(0.28f, 0.29f, 0.31f);
    bloco(0, 0.04f, Z_MIN + 0.008f, X_MAX - X_MIN, 0.08f, 0.015f);
    bloco(0, 0.04f, Z_MAX - 0.008f, X_MAX - X_MIN, 0.08f, 0.015f);
    bloco(X_MIN + 0.008f, 0.04f, 0, 0.015f, 0.08f, Z_MAX - Z_MIN);
    bloco(X_MAX - 0.008f, 0.04f, (Z_MIN + PORTA_Z0) / 2, 0.015f, 0.08f, PORTA_Z0 - Z_MIN);
    bloco(X_MAX - 0.008f, 0.04f, (PORTA_Z1 + Z_MAX) / 2, 0.015f, 0.08f, Z_MAX - PORTA_Z1);

    // Lado de fora da janela: céu e grama, sem iluminação
    glDisable(GL_LIGHTING);
    glColor3f(0.55f, 0.75f, 0.95f);
    glPushMatrix();
    glTranslatef(-14, 2, 0);
    glRotatef(90, 0, 1, 0);   // plano virado para a janela
    glScalef(40, 16, 1);
    plano();
    glPopMatrix();

    glColor3f(0.33f, 0.45f, 0.24f);
    bloco(-9.6f, -0.55f, 0, 9.2f, 0.1f, 40);
    glEnable(GL_LIGHTING);
}
