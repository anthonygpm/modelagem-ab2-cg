#pragma once

#include <GL/freeglut.h>

// Identificadores das texturas carregadas de assets/textures/
struct Texturas {
    GLuint piso, parede, forro, paisagem;
};
extern Texturas tex;

void carregarTexturas();
