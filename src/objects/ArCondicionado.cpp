#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"

#include <GL/freeglut.h>

// Ar-condicionado split. Ligado, a aleta oscila (rotação em X na dobradiça).
void desenharArCondicionado()
{
    // Gabinete e painel frontal
    plastico(0.94f, 0.94f, 0.93f);
    bloco(0, 0, 0.11f, 0.92f, 0.3f, 0.22f);
    plastico(0.98f, 0.98f, 0.97f);
    bloco(0, 0.035f, 0.2205f, 0.9f, 0.19f, 0.004f);

    // Saída de ar com defletores verticais
    fosco(0.12f, 0.12f, 0.13f);
    bloco(0, -0.1f, 0.215f, 0.82f, 0.07f, 0.02f);
    fosco(0.3f, 0.3f, 0.32f);
    for (int i = -6; i <= 6; i++)
        bloco(i * 0.06f, -0.1f, 0.222f, 0.006f, 0.06f, 0.006f);

    // Aleta: translada até a dobradiça e gira (ângulo animado em Cena.cpp)
    glPushMatrix();
    glTranslatef(0, -0.135f, 0.225f);
    glRotatef(estado.aleta, 1, 0, 0);
    plastico(0.94f, 0.94f, 0.93f);
    bloco(0, 0, 0.04f, 0.84f, 0.006f, 0.08f);
    glPopMatrix();

    // LED verde de funcionamento
    if (estado.arLigado) {
        material(0.2f, 1, 0.3f, 0, 1);
        emissao(0.2f, 1, 0.3f);
    } else {
        fosco(0.2f, 0.25f, 0.2f);
    }
    bloco(0.38f, -0.04f, 0.223f, 0.015f, 0.015f, 0.003f);
    emissao(0, 0, 0);
}
