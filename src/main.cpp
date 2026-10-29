// Sala de Aula 3D — Computação Gráfica (UFAL)
// OpenGL com pipeline fixo + freeglut

#include <GL/freeglut.h>

#include "Camera.h"
#include "Cena.h"

#include <algorithm>
#include <cctype>

static int largura = 1280, altura = 720;
static bool teclas[256];          // teclas pressionadas no momento (W, A, S, D)
static bool correndo = false;     // Shift pressionado
static int tempoAnterior = 0;     // em milissegundos

// ---------------------------------------------------------------------------
// Callbacks do GLUT
// ---------------------------------------------------------------------------
static void desenhar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projeção perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (double)largura / std::max(altura, 1), 0.05, 100);

    // Câmera, luzes (dependem da câmera) e objetos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    aplicarCamera();
    configurarLuzes();
    desenharCena();
    glutSwapBuffers();
}

// Chamada continuamente: move o jogador e pede um novo quadro
static void atualizar()
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float dt = std::min((agora - tempoAnterior) / 1000.0f, 0.1f);   // segundos
    tempoAnterior = agora;

    float frente = (float)teclas['w'] - (float)teclas['s'];
    float lado = (float)teclas['d'] - (float)teclas['a'];
    moverCamera(frente, lado, dt, correndo);

    glutPostRedisplay();
}

static void redimensionar(int w, int h)
{
    largura = w;
    altura = h;
    glViewport(0, 0, w, h);
}

static void teclaPressionada(unsigned char tecla, int, int)
{
    tecla = (unsigned char)std::tolower(tecla);   // com Shift as letras chegam maiúsculas
    teclas[tecla] = true;

    switch (tecla) {
    case 27:  glutLeaveMainLoop(); break;   // Esc
    }
}

static void teclaSolta(unsigned char tecla, int, int)
{
    teclas[std::tolower(tecla)] = false;
}

static void especialPressionada(int tecla, int, int)
{
    if (tecla == GLUT_KEY_SHIFT_L || tecla == GLUT_KEY_SHIFT_R) correndo = true;
}

static void especialSolta(int tecla, int, int)
{
    if (tecla == GLUT_KEY_SHIFT_L || tecla == GLUT_KEY_SHIFT_R) correndo = false;
}

// Mouse: gira a câmera e devolve o cursor para o centro da janela
static void moverMouse(int x, int y)
{
    int cx = largura / 2, cy = altura / 2;
    if (x == cx && y == cy) return;   // evento gerado pelo próprio glutWarpPointer
    girarCamera((float)(x - cx), (float)(y - cy));
    glutWarpPointer(cx, cy);
}

// ---------------------------------------------------------------------------
static void iniciarOpenGL()
{
    glClearColor(0.02f, 0.02f, 0.03f, 1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);   // corrige as normais depois de glScalef
    glShadeModel(GL_SMOOTH);  // Gouraud: cor interpolada entre os vértices
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);   // reflexo especular calculado com a posição real do olho
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(largura, altura);
    glutCreateWindow("Sala de Aula 3D - UFAL");

    iniciarOpenGL();

    glutDisplayFunc(desenhar);
    glutIdleFunc(atualizar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclaPressionada);
    glutKeyboardUpFunc(teclaSolta);
    glutSpecialFunc(especialPressionada);
    glutSpecialUpFunc(especialSolta);
    glutPassiveMotionFunc(moverMouse);
    glutMotionFunc(moverMouse);

    glutIgnoreKeyRepeat(1);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(largura / 2, altura / 2);

    tempoAnterior = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
    return 0;
}
