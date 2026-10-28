#include "Camera.h"

#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>

Camera camera;

static float radianos(float graus) { return graus * 3.14159265f / 180.0f; }

void direcaoCamera(float d[3])
{
    float yaw = radianos(camera.yaw), pitch = radianos(camera.pitch);
    d[0] = std::cos(yaw) * std::cos(pitch);
    d[1] = std::sin(pitch);
    d[2] = std::sin(yaw) * std::cos(pitch);
}

void aplicarCamera()
{
    float d[3];
    direcaoCamera(d);
    gluLookAt(camera.x, camera.y, camera.z,                        // olho
              camera.x + d[0], camera.y + d[1], camera.z + d[2],   // ponto observado
              0, 1, 0);                                            // "para cima"
}

void girarCamera(float dx, float dy)
{
    const float sensibilidade = 0.1f;
    camera.yaw += dx * sensibilidade;
    camera.pitch -= dy * sensibilidade;
    camera.pitch = std::clamp(camera.pitch, -89.0f, 89.0f);
}

void moverCamera(float frente, float lado, float dt, bool correndo)
{
    if (frente == 0 && lado == 0) return;

    // Frente no plano do chão (ignora o pitch para não "voar") e vetor para a direita
    float yaw = radianos(camera.yaw);
    float fx = std::cos(yaw), fz = std::sin(yaw);
    float dx = fx * frente - fz * lado;
    float dz = fz * frente + fx * lado;

    float tam = std::sqrt(dx * dx + dz * dz);
    float velocidade = correndo ? 4.2f : 2.4f;   // metros por segundo
    dx = dx / tam * velocidade * dt;
    dz = dz / tam * velocidade * dt;

    camera.x += dx;
    camera.z += dz;
}
