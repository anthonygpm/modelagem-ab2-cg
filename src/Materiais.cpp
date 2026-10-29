#include "Materiais.h"

void material(float r, float g, float b, float especular, float brilho, GLuint textura)
{
    const float cor[] = {r, g, b, 1};
    const float reflexo[] = {especular, especular, especular, 1};
    const float nenhuma[] = {0, 0, 0, 1};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, reflexo);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, nenhuma);

    if (textura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
}

void emissao(float r, float g, float b)
{
    const float cor[] = {r, g, b, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cor);
}
