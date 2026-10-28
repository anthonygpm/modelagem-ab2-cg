#pragma once

// Formas básicas. Todos os objetos da cena são montados com elas,
// usando glTranslatef, glRotatef e glScalef.

void cubo();      // cubo de lado 1 centrado na origem
void cilindro();  // cilindro de raio 0,5 e altura 1, em pé no eixo Y, centrado na origem
void plano();     // quadrado de lado 1 no plano XY, virado para +Z

// Atalhos: desenham a forma depois de uma translação até (x, y, z)
// e de uma escala (sx, sy, sz).
void bloco(float x, float y, float z, float sx, float sy, float sz);
void tubo(float x, float y, float z, float sx, float sy, float sz);

// Caixa grande (paredes, piso, teto) dividida em pedaços de 0,5 m.
// A iluminação do OpenGL é calculada nos vértices, então superfícies grandes
// precisam de vários vértices para a luz das lâmpadas aparecer.
// tamTextura = quantos metros a textura ocupa antes de se repetir.
void caixa(float x0, float y0, float z0, float x1, float y1, float z1, float tamTextura);
