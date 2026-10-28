#pragma once

// Câmera em primeira pessoa: posição + ângulos de Euler (yaw e pitch, em graus).
struct Camera {
    float x = 0.0f, y = 1.65f, z = 3.3f;   // 1,65 m = altura dos olhos
    float yaw = -90.0f;                    // -90° = olhando para o quadro (-Z)
    float pitch = -5.0f;
};
extern Camera camera;

void direcaoCamera(float d[3]);                  // vetor para onde a câmera olha
void aplicarCamera();                            // gluLookAt
void girarCamera(float dx, float dy);            // movimento do mouse
void moverCamera(float frente, float lado, float dt, bool correndo);   // W/A/S/D
