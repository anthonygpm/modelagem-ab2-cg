// Sala de Aula 3D — Computação Gráfica (UFAL)
// OpenGL com pipeline fixo + freeglut

#include <GL/freeglut.h>


#include <algorithm>

static int largura = 1280, altura = 720;

// ---------------------------------------------------------------------------
// Callbacks do GLUT
// ---------------------------------------------------------------------------
static void desenhar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

// Chamada continuamente: pede um novo quadro
static void atualizar()
{
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
    switch (tecla) {
    case 27:  glutLeaveMainLoop(); break;   // Esc
    }
}

// ---------------------------------------------------------------------------
static void iniciarOpenGL()
{
    glClearColor(0.1f, 0.15f, 0.25f, 1);
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
    glutMainLoop();
    return 0;
}
