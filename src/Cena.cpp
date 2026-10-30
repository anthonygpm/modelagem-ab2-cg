#include "Cena.h"

#include <GL/freeglut.h>

#include <cmath>


// Posição das carteiras dos alunos (3 fileiras x 4 colunas)
static const float FILEIRAS_Z[] = {-1.2f, 0.3f, 1.8f};
static const float COLUNAS_X[] = {-3.3f, -1.1f, 1.1f, 3.3f};

// Posição (x, z) das 4 luminárias da sala
static const float LUMINARIAS[4][2] = {{-2.3f, -1.6f}, {2.3f, -1.6f}, {-2.3f, 1.8f}, {2.3f, 1.8f}};

// ---------------------------------------------------------------------------
// Iluminação (pipeline fixo: GL_LIGHT0 a GL_LIGHT7)
// ---------------------------------------------------------------------------
static void luzPontual(GLenum luz, float x, float y, float z, float intensidade)
{
    const float posicao[] = {x, y, z, 1};   // w = 1: luz pontual
    const float cor[] = {intensidade, intensidade * 0.97f, intensidade * 0.92f, 1};
    glLightfv(luz, GL_POSITION, posicao);
    glLightfv(luz, GL_DIFFUSE, cor);
    glLightfv(luz, GL_SPECULAR, cor);
    // Atenuação: 1 / (constante + linear*d + quadrática*d²)
    glLightf(luz, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(luz, GL_LINEAR_ATTENUATION, 0.09f);
    glLightf(luz, GL_QUADRATIC_ATTENUATION, 0.032f);

    if (intensidade > 0) glEnable(luz);
    else glDisable(luz);
}

void configurarLuzes()
{
    float sol = 1;

    // Luz ambiente: ilumina tudo por igual, para nada ficar totalmente preto
    const float ambiente[] = {0.10f + 0.12f * sol, 0.10f + 0.13f * sol, 0.11f + 0.14f * sol, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiente);

    // GL_LIGHT0: sol, luz direcional (w = 0) vindo da janela
    const float direcao[] = {-1.0f, 0.6f, -0.3f, 0};
    const float corSol[] = {0.6f * sol, 0.57f * sol, 0.5f * sol, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, direcao);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corSol);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corSol);
    glEnable(GL_LIGHT0);

    // GL_LIGHT1 a GL_LIGHT4: luminárias da sala
    for (int i = 0; i < 4; i++)
        luzPontual(GL_LIGHT1 + i, LUMINARIAS[i][0], ALTURA - 0.35f, LUMINARIAS[i][1], 0.42f);

    // GL_LIGHT5: luminária do corredor (sempre acesa)
    luzPontual(GL_LIGHT5, 6.4f, ALTURA - 0.35f, -0.6f, 0.35f);
}

// ---------------------------------------------------------------------------
// Desenho: cada objeto é posicionado com translação + rotação em Y
// ---------------------------------------------------------------------------
static void posicionar(float x, float y, float z, float rotacaoY = 0)
{
    glTranslatef(x, y, z);
    glRotatef(rotacaoY, 0, 1, 0);
}

void desenharCena()
{
    desenharSala();

    // Carteiras: o mesmo modelo desenhado 12 vezes em posições diferentes
    for (float z : FILEIRAS_Z) {
        for (float x : COLUNAS_X) {
            glPushMatrix();
            posicionar(x, 0, z);
            desenharMesa(0.9f, 0.55f, 0.75f, false);
            glPopMatrix();

            glPushMatrix();
            posicionar(x, 0, z + 0.45f);
            desenharCadeira(0.13f, 0.30f, 0.62f);
            glPopMatrix();
        }
    }

    // Mesa e cadeira do professor: giradas 180° para ficarem de frente para a turma
    glPushMatrix();
    posicionar(-2.6f, 0, -2.4f, 180);
    desenharMesa(1.4f, 0.7f, 0.76f, true);
    glPopMatrix();

    glPushMatrix();
    posicionar(-2.6f, 0, -3.15f, 180);
    desenharCadeira(0.12f, 0.12f, 0.13f);
    glPopMatrix();

    glPushMatrix();
    posicionar(0, 0, Z_MIN);
    desenharQuadro();
    glPopMatrix();

    glPushMatrix();
    posicionar(4.6f, 0, 3.6f);
    desenharLixeira();
    glPopMatrix();
}
