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
// Estado dos objetos interativos.
// Os booleanos dizem o que o usuário pediu; os floats de 0 a 1 são o
// progresso da animação, que anda aos poucos até o valor pedido.
// ---------------------------------------------------------------------------
struct Estado {
    bool luzesLigadas = true;
    bool portaAberta = false;

    float porta = 0;          // 0 fechada, 1 aberta
    float tempo = 0;          // segundos desde o início
};
extern Estado estado;

// Avança "atual" em direção a "alvo" no máximo "passo" por quadro
float aproximar(float atual, float alvo, float passo);
// Suaviza uma animação de 0 a 1 (começa e termina devagar)
float suavizar(float t);

// ---------------------------------------------------------------------------
// Cena
// ---------------------------------------------------------------------------
void atualizarCena(float dt);
void configurarLuzes();   // chamar depois de posicionar a câmera
void desenharCena();
bool colide(float x, float z);   // o jogador em (x, z) bate em algo?

// Interação com a tecla E
enum Alvo {
    NENHUM,
    INTERRUPTOR,
    PORTA,
};
Alvo alvoNaMira();
void interagir(Alvo alvo);
const char* dica(Alvo alvo);

// ---------------------------------------------------------------------------
// Objetos (pasta objects/). Cada função desenha o objeto na origem;
// Cena.cpp posiciona cada um com glTranslatef e glRotatef.
// ---------------------------------------------------------------------------
void desenharSala();
void desenharMesa(float largura, float profundidade, float altura, bool painelFrontal);
void desenharCadeira(float r, float g, float b);
void desenharQuadro();
void desenharLixeira();
void desenharLampada(bool acesa);
void desenharInterruptor();
void desenharPorta();
