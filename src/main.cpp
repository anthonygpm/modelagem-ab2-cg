// Sala de Aula 3D — Computação Gráfica (UFAL)
// OpenGL com pipeline fixo + freeglut

#include <GL/freeglut.h>

#include "Camera.h"
#include "Cena.h"
#include "Texturas.h"

#include <algorithm>
#include <cctype>

// Constantes do OpenGL 1.2/1.3 que o cabeçalho do Windows não declara
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif

static int largura = 1280, altura = 720;
static bool teclas[256];          // teclas pressionadas no momento (W, A, S, D)
static bool correndo = false;     // Shift pressionado
static bool mostrarAjuda = false;
static int tempoAnterior = 0;     // em milissegundos

// ---------------------------------------------------------------------------
// HUD: mira, dica de interação e ajuda, desenhados em 2D por cima da cena
// ---------------------------------------------------------------------------
static void texto(float x, float y, const char* s)
{
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)s);
}

static int larguraTexto(const char* s)
{
    return glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)s);
}

static void fundo(float x0, float y0, float x1, float y1)
{
    glColor4f(0.06f, 0.07f, 0.1f, 0.75f);   // preto semitransparente
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
}

static void caixaDeTexto(float x, float y, const char* s)
{
    fundo(x - 14, y - 12, x + larguraTexto(s) + 14, y + 24);
    glColor3f(1, 1, 1);
    texto(x, y, s);
}

static void desenharAjuda()
{
    const char* linhas[][2] = {
        {"W A S D", "Andar"},
        {"Shift", "Correr"},
        {"Mouse", "Olhar ao redor"},
        {"E", "Interagir com o objeto na mira"},
        {"Q", "Girar laminas da persiana"},
        {"R", "Recolher / baixar persiana"},
        {"L", "Ligar / desligar luzes"},
        {"F", "Tela cheia"},
        {"H", "Mostrar / ocultar ajuda"},
        {"Esc", "Sair"},
    };
    const int n = sizeof(linhas) / sizeof(linhas[0]);
    float cx = largura / 2.0f, topo = altura / 2.0f + n * 15 + 30;

    fundo(cx - 230, topo - n * 30 - 30, cx + 230, topo + 20);
    glColor3f(1, 1, 1);
    texto(cx - larguraTexto("CONTROLES") / 2.0f, topo - 10, "CONTROLES");
    for (int i = 0; i < n; i++) {
        float y = topo - 50 - i * 30;
        glColor3f(1, 0.82f, 0.25f);
        texto(cx - 30 - larguraTexto(linhas[i][0]), y, linhas[i][0]);
        glColor3f(1, 1, 1);
        texto(cx, y, linhas[i][1]);
    }
}

static void desenharHUD()
{
    // Projeção ortográfica em pixels, sem iluminação e sem teste de profundidade
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, largura, 0, altura);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    Alvo alvo = alvoNaMira();
    float cx = largura / 2.0f, cy = altura / 2.0f;

    // Mira: fica amarela e maior quando há algo para interagir
    float t = alvo != NENHUM ? 11 : 7;
    if (alvo != NENHUM) glColor3f(1, 0.82f, 0.25f);
    else glColor3f(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(cx - t, cy);
    glVertex2f(cx + t, cy);
    glVertex2f(cx, cy - t);
    glVertex2f(cx, cy + t);
    glEnd();

    if (alvo != NENHUM)
        caixaDeTexto(cx - larguraTexto(dica(alvo)) / 2.0f, altura * 0.2f, dica(alvo));

    if (mostrarAjuda) desenharAjuda();
    else caixaDeTexto(24, 24, "[H] Ajuda");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
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

    // Câmera, luzes (dependem da câmera) e objetos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    aplicarCamera();
    configurarLuzes();
    desenharCena();

    desenharHUD();
    glutSwapBuffers();
}

// Chamada continuamente: move o jogador, anima a cena e pede um novo quadro
static void atualizar()
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float dt = std::min((agora - tempoAnterior) / 1000.0f, 0.1f);   // segundos
    tempoAnterior = agora;

    float frente = (float)teclas['w'] - (float)teclas['s'];
    float lado = (float)teclas['d'] - (float)teclas['a'];
    moverCamera(frente, lado, dt, correndo);
    atualizarCena(dt);

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
    case 'e': interagir(alvoNaMira()); break;
    case 'q': interagir(JANELA); break;
    case 'r': estado.persianaRecolhida = !estado.persianaRecolhida; break;
    case 'l': interagir(INTERRUPTOR); break;
    case 'h': mostrarAjuda = !mostrarAjuda; break;
    case 'f': glutFullScreenToggle(); break;
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
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);   // reflexo não é escurecido pela textura

    glEnable(GL_BLEND);       // transparência (vidro e HUD)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(largura, altura);
    glutCreateWindow("Sala de Aula 3D - UFAL");

    iniciarOpenGL();
    carregarTexturas();

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
