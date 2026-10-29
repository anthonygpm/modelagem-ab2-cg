#pragma once

#include <GL/freeglut.h>

// Define como a superfície reage à luz (modelo de iluminação de Phong do OpenGL).
//   r, g, b    cor da superfície (componentes ambiente e difusa)
//   especular  intensidade do reflexo brilhante (ks)
//   brilho     expoente de Phong: quanto maior, menor e mais concentrado o reflexo
//   textura    0 = sem textura
void material(float r, float g, float b, float especular = 0.1f, float brilho = 10.0f, GLuint textura = 0);

// Faz a superfície emitir luz própria (tubos acesos, telas, LEDs).
// Deve ser chamada depois de material(); material() zera a emissão.
void emissao(float r, float g, float b);

// Materiais prontos
inline void metal(float r = 0.62f, float g = 0.64f, float b = 0.67f) { material(r, g, b, 0.9f, 90); }
inline void plastico(float r, float g, float b) { material(r, g, b, 0.35f, 32); }
inline void fosco(float r, float g, float b) { material(r, g, b, 0.03f, 8); }
