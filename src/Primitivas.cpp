#include "Primitivas.h"

#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>

// Retângulo com canto em p e lados u e v. A normal é u x v.
// É dividido em divU x divV quadradinhos; a textura se repete repU x repV vezes.
static void retangulo(const float p[3], const float u[3], const float v[3],
                      float repU, float repV, int divU, int divV)
{
    float n[3] = {u[1] * v[2] - u[2] * v[1],
                  u[2] * v[0] - u[0] * v[2],
                  u[0] * v[1] - u[1] * v[0]};
    float tam = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    glNormal3f(n[0] / tam, n[1] / tam, n[2] / tam);

    const float cantos[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    glBegin(GL_QUADS);
    for (int i = 0; i < divU; i++) {
        for (int j = 0; j < divV; j++) {
            for (const auto& c : cantos) {
                float s = (i + c[0]) / divU;
                float t = (j + c[1]) / divV;
                glTexCoord2f(s * repU, t * repV);
                glVertex3f(p[0] + u[0] * s + v[0] * t,
                           p[1] + u[1] * s + v[1] * t,
                           p[2] + u[2] * s + v[2] * t);
            }
        }
    }
    glEnd();
}

// Desenha as 6 faces de uma caixa alinhada aos eixos.
// tamTextura = 0: textura inteira em cada face, sem subdivisão (objetos pequenos).
static void faces(float x0, float y0, float z0, float x1, float y1, float z1, float tamTextura)
{
    float dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;

    // Para cada face: canto, lado u e lado v (u x v aponta para fora da caixa)
    const float face[6][3][3] = {
        {{x1, y0, z1}, {0, 0, -dz}, {0, dy, 0}},   // +X
        {{x0, y0, z0}, {0, 0, dz},  {0, dy, 0}},   // -X
        {{x0, y1, z1}, {dx, 0, 0},  {0, 0, -dz}},  // +Y (topo)
        {{x0, y0, z0}, {dx, 0, 0},  {0, 0, dz}},   // -Y (base)
        {{x0, y0, z1}, {dx, 0, 0},  {0, dy, 0}},   // +Z
        {{x1, y0, z0}, {-dx, 0, 0}, {0, dy, 0}},   // -Z
    };

    for (const auto& f : face) {
        float compU = std::fabs(f[1][0] + f[1][1] + f[1][2]);
        float compV = std::fabs(f[2][0] + f[2][1] + f[2][2]);
        if (tamTextura <= 0) {
            retangulo(f[0], f[1], f[2], 1, 1, 1, 1);
        } else {
            int divU = std::max(1, (int)std::ceil(compU / 0.5f));
            int divV = std::max(1, (int)std::ceil(compV / 0.5f));
            retangulo(f[0], f[1], f[2], compU / tamTextura, compV / tamTextura, divU, divV);
        }
    }
}

void cubo()
{
    faces(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0);
}

void caixa(float x0, float y0, float z0, float x1, float y1, float z1, float tamTextura)
{
    faces(x0, y0, z0, x1, y1, z1, tamTextura);
}

void plano()
{
    const float p[3] = {-0.5f, -0.5f, 0}, u[3] = {1, 0, 0}, v[3] = {0, 1, 0};
    retangulo(p, u, v, 1, 1, 1, 1);
}

void cilindro()
{
    // GLU desenha cilindros ao longo de +Z a partir da origem;
    // giramos para ficar em pé (eixo Y) e centralizamos.
    static GLUquadric* q = nullptr;
    if (!q) {
        q = gluNewQuadric();
        gluQuadricNormals(q, GLU_SMOOTH);
        gluQuadricTexture(q, GL_TRUE);
    }
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, -0.5f);
    gluCylinder(q, 0.5, 0.5, 1.0, 24, 1);   // lateral
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(q, 0, 0.5, 24, 1);              // tampa de baixo
    glPopMatrix();
    glTranslatef(0, 0, 1);
    gluDisk(q, 0, 0.5, 24, 1);              // tampa de cima
    glPopMatrix();
}

void bloco(float x, float y, float z, float sx, float sy, float sz)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    cubo();
    glPopMatrix();
}

void tubo(float x, float y, float z, float sx, float sy, float sz)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    cilindro();
    glPopMatrix();
}
