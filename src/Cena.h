#pragma once

#include <initializer_list>

// ---------------------------------------------------------------------------
// Dimensões da sala, em metros. Origem no centro do piso.
//   X: de -5 (parede da janela) a +5 (parede da porta)
//   Z: de -4 (parede do quadro) a +4 (parede do fundo)
//   Y: de 0 (piso) a 3 (teto)
// ---------------------------------------------------------------------------
const float X_MIN = -5.0f, X_MAX = 5.0f;
const float Z_MIN = -4.0f, Z_MAX = 4.0f;
const float ALTURA = 3.0f;
const float ESPESSURA = 0.2f;

const float JANELA_Z0 = -2.0f, JANELA_Z1 = 2.0f;   // vão da janela
const float JANELA_Y0 = 0.9f, JANELA_Y1 = 2.3f;
const float PORTA_Z0 = -3.4f, PORTA_Z1 = -2.3f;    // vão da porta
const float PORTA_ALTURA = 2.15f;
const float CORREDOR_X1 = 7.6f;                    // corredor do lado de fora
const float CORREDOR_Z0 = -4.2f, CORREDOR_Z1 = 3.0f;

// Caixa alinhada aos eixos (usada para paredes e colisão)
struct Caixa {
    float x0, y0, z0, x1, y1, z1;
};
extern const Caixa PAREDES[];
extern const int NUM_PAREDES;

// ---------------------------------------------------------------------------
// Cena
// ---------------------------------------------------------------------------
void configurarLuzes();   // chamar depois de posicionar a câmera
void desenharCena();

// ---------------------------------------------------------------------------
// Objetos (pasta objects/). Cada função desenha o objeto na origem;
// Cena.cpp posiciona cada um com glTranslatef e glRotatef.
// ---------------------------------------------------------------------------
void desenharSala();
void desenharMesa(float largura, float profundidade, float altura, bool painelFrontal);
void desenharCadeira(float r, float g, float b);
void desenharQuadro();
void desenharLixeira();
