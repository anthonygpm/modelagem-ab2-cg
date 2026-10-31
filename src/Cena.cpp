#include "Cena.h"
#include "Camera.h"

#include <GL/freeglut.h>

#include <cmath>

Estado estado;

// Posição das carteiras dos alunos (3 fileiras x 4 colunas)
static const float FILEIRAS_Z[] = {-1.2f, 0.3f, 1.8f};
static const float COLUNAS_X[] = {-3.3f, -1.1f, 1.1f, 3.3f};

// Posição (x, z) das 4 luminárias da sala
static const float LUMINARIAS[4][2] = {{-2.3f, -1.6f}, {2.3f, -1.6f}, {-2.3f, 1.8f}, {2.3f, 1.8f}};

// ---------------------------------------------------------------------------
// Animação
// ---------------------------------------------------------------------------
float aproximar(float atual, float alvo, float passo)
{
    if (atual < alvo) return std::fmin(atual + passo, alvo);
    return std::fmax(atual - passo, alvo);
}

float suavizar(float t)
{
    return t * t * (3 - 2 * t);
}

void atualizarCena(float dt)
{
    estado.tempo += dt;
}

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

    // GL_LIGHT1 a GL_LIGHT4: luminárias da sala (controladas pelo interruptor)
    for (int i = 0; i < 4; i++)
        luzPontual(GL_LIGHT1 + i, LUMINARIAS[i][0], ALTURA - 0.35f, LUMINARIAS[i][1],
                   estado.luzesLigadas ? 0.42f : 0.0f);

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

    // Luminárias no teto
    for (const auto& l : LUMINARIAS) {
        glPushMatrix();
        posicionar(l[0], ALTURA, l[1]);
        desenharLampada(estado.luzesLigadas);
        glPopMatrix();
    }
    glPushMatrix();
    posicionar(6.4f, ALTURA, -0.6f);   // corredor
    desenharLampada(true);
    glPopMatrix();

    // Objetos presos nas paredes: girados para ficarem de frente para a sala
    glPushMatrix();
    posicionar(X_MAX, 1.3f, -1.9f, -90);
    desenharInterruptor();
    glPopMatrix();
}

// ---------------------------------------------------------------------------
// Colisão: o jogador é um quadrado de 0,5 m visto de cima, do chão até 1,8 m
// ---------------------------------------------------------------------------
static bool bate(const Caixa& c, float x, float z)
{
    const float raio = 0.25f;
    return x + raio > c.x0 && x - raio < c.x1 &&
           z + raio > c.z0 && z - raio < c.z1 &&
           c.y1 > 0.1f && c.y0 < 1.8f;   // ignora o que está acima da cabeça
}

bool colide(float x, float z)
{
    for (int i = 0; i < NUM_PAREDES; i++)
        if (bate(PAREDES[i], x, z)) return true;

    for (float cz : FILEIRAS_Z) {
        for (float cx : COLUNAS_X) {
            Caixa mesa = {cx - 0.45f, 0, cz - 0.28f, cx + 0.45f, 0.75f, cz + 0.28f};
            Caixa cadeira = {cx - 0.23f, 0, cz + 0.22f, cx + 0.23f, 0.9f, cz + 0.68f};
            if (bate(mesa, x, z) || bate(cadeira, x, z)) return true;
        }
    }

    const Caixa outros[] = {
        {-3.3f, 0, -2.75f, -1.9f, 0.76f, -2.05f},   // mesa do professor
        {-2.83f, 0, -3.38f, -2.37f, 0.9f, -2.92f},  // cadeira do professor
        {4.43f, 0, 3.43f, 4.77f, 0.44f, 3.77f},     // lixeira
    };
    for (const Caixa& c : outros)
        if (bate(c, x, z)) return true;
    return false;
}

// ---------------------------------------------------------------------------
// Interação: o alvo é o objeto mais próximo (até 2,6 m) que esteja na mira
// ---------------------------------------------------------------------------
struct PontoInterativo {
    Alvo alvo;
    float x, y, z;
};

static const PontoInterativo PONTOS[] = {
    {INTERRUPTOR, X_MAX, 1.3f, -1.9f},
};

Alvo alvoNaMira()
{
    float olhar[3];
    direcaoCamera(olhar);

    Alvo melhor = NENHUM;
    float melhorNota = -1e9f;
    for (const PontoInterativo& p : PONTOS) {
        float vx = p.x - camera.x, vy = p.y - camera.y, vz = p.z - camera.z;
        float distancia = std::sqrt(vx * vx + vy * vy + vz * vz);
        if (distancia > 2.6f) continue;

        // Produto escalar: 1 = olhando exatamente para o objeto
        float alinhamento = (vx * olhar[0] + vy * olhar[1] + vz * olhar[2]) / distancia;
        if (alinhamento < 0.75f) continue;

        float nota = alinhamento - distancia * 0.05f;
        if (nota > melhorNota) {
            melhorNota = nota;
            melhor = p.alvo;
        }
    }
    return melhor;
}

void interagir(Alvo alvo)
{
    switch (alvo) {
    case INTERRUPTOR:     estado.luzesLigadas = !estado.luzesLigadas; break;
    case NENHUM:          break;
    }
}

const char* dica(Alvo alvo)
{
    switch (alvo) {
    case INTERRUPTOR:     return estado.luzesLigadas ? "[E] Desligar luzes" : "[E] Ligar luzes";
    case NENHUM:          break;
    }
    return "";
}
