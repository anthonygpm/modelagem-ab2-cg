// Sala de Aula 3D — Computação Gráfica (UFAL)
// OpenGL com pipeline fixo + freeglut

#include <GL/freeglut.h>

#include "Camera.h"
#include "Primitivas.h"

#include <algorithm>
#include <cctype>

static int largura = 1280, altura = 720;
static bool teclas[256];          // teclas pressionadas no momento (W, A, S, D)
static bool correndo = false;     // Shift pressionado
static int tempoAnterior = 0;     // em milissegundos

// Cena de teste: um piso quadriculado de cubos coloridos para testar a câmera
static void desenharCubosDeTeste()
{
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            if ((i + j) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
            else glColor3f(0.3f, 0.5f, 0.8f);
            glPushMatrix();
            glTranslatef((float)i, -0.05f, (float)j);
            glScalef(1, 0.1f, 1);
            cubo();
            glPopMatrix();
        }
    }
    glColor3f(0.9f, 0.3f, 0.2f);
    bloco(0, 0.5f, -3, 1, 1, 1);
    glColor3f(0.2f, 0.8f, 0.3f);
    bloco(2, 1, -2, 0.5f, 2, 0.5f);
}

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

    // Câmera e objetos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    aplicarCamera();
    desenharCubosDeTeste();
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
