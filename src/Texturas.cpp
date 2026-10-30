#include "Texturas.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdio>
#include <string>

#ifndef PASTA_PROJETO
#define PASTA_PROJETO "."
#endif

Texturas tex;

// Lê uma imagem PNG e envia para a placa de vídeo como textura com mipmaps.
static GLuint carregar(const char* arquivo)
{
    // Procura a pasta assets/ a partir de onde o programa foi aberto
    const char* pastas[] = {"assets/textures/", "../assets/textures/", "../../assets/textures/",
                            PASTA_PROJETO "/assets/textures/"};
    int largura = 0, altura = 0, canais = 0;
    unsigned char* pixels = nullptr;
    for (const char* pasta : pastas) {
        pixels = stbi_load((std::string(pasta) + arquivo).c_str(), &largura, &altura, &canais, 4);
        if (pixels) break;
    }
    if (!pixels) {
        std::printf("Textura nao encontrada: %s\n", arquivo);
        return 0;
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, largura, altura, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(pixels);
    return id;
}

void carregarTexturas()
{
    stbi_set_flip_vertically_on_load(1);   // coordenada (0,0) da textura fica embaixo

    tex.piso     = carregar("piso.png");
    tex.parede   = carregar("parede.png");
    tex.forro    = carregar("forro.png");
    tex.paisagem = carregar("paisagem.png");
    tex.madeira  = carregar("madeira.png");
    tex.quadro   = carregar("quadro.png");

    // A cor da textura é multiplicada pela cor calculada pela iluminação
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
