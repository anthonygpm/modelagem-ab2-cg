#include "Cena.h"
#include "Materiais.h"
#include "Primitivas.h"
#include "Texturas.h"

// Mesa: tampo de madeira, 4 pernas cilíndricas de aço e travessas.
// A frente fica para -Z (quem senta fica do lado +Z).
void desenharMesa(float largura, float profundidade, float altura, bool painelFrontal)
{
    // Tampo
    material(1, 1, 1, 0.3f, 32, tex.madeira);
    bloco(0, altura - 0.015f, 0, largura, 0.03f, profundidade);

    // Painel frontal (só na mesa do professor)
    float hx = largura / 2 - 0.05f, hz = profundidade / 2 - 0.05f;
    if (painelFrontal)
        bloco(0, altura * 0.55f, -hz, 2 * hx, altura * 0.6f, 0.015f);

    // Estrutura de aço
    material(0.22f, 0.23f, 0.25f, 0.6f, 48);
    float hp = altura - 0.03f;   // altura das pernas
    for (float sx : {-1.0f, 1.0f})
        for (float sz : {-1.0f, 1.0f})
            tubo(sx * hx, hp / 2, sz * hz, 0.04f, hp, 0.04f);

    for (float sx : {-1.0f, 1.0f})
        bloco(sx * hx, 0.12f, 0, 0.025f, 0.025f, 2 * hz);   // travessas laterais
    for (float sz : {-1.0f, 1.0f})
        bloco(0, altura - 0.06f, sz * hz, 2 * hx, 0.05f, 0.02f);   // travessas sob o tampo
}
