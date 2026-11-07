# Sala de Aula 3D — OpenGL

Aplicação em OpenGL que reproduz uma sala de aula da UFAL em 3D. O usuário explora a cena em primeira pessoa e interage com objetos: abre a porta, liga a luz, ajusta a persiana da janela, liga o ar-condicionado, abre o notebook e desce a tela do projetor.

Projeto da disciplina de **Computação Gráfica** — Universidade Federal de Alagoas (UFAL).

## Equipe

| Nome | GitHub |
|---|---|
| Anthony | [@usuario](https://github.com/usuario) |
| Integrante 2 | [@usuario](https://github.com/usuario) |
| Integrante 3 | [@usuario](https://github.com/usuario) |

## Funcionalidades

- Navegação em primeira pessoa (W/A/S/D + mouse), com colisão
- 6 objetos interativos: porta, luzes, persiana, ar-condicionado, notebook e projetor
- Iluminação com 1 luz direcional (sol pela janela) e 5 luzes pontuais (luminárias)
- Materiais diferentes por objeto (madeira, metal, plástico, vidro, granito)
- Mapeamento de textura (piso, paredes, teto, mesas, porta, quadro, paisagem, telas)
- Transformações animadas de translação, rotação e escala
- Corredor do lado de fora da porta

## Controles

| Tecla | Função |
|---|---|
| `W` `A` `S` `D` | Andar |
| `Shift` | Correr |
| Mouse | Olhar ao redor |
| `E` | Interagir com o objeto na mira (a mira fica amarela e aparece a dica) |
| `Q` | Girar as lâminas da persiana |
| `R` | Recolher / baixar a persiana |
| `L` | Ligar / desligar as luzes |
| `H` | Mostrar / ocultar a ajuda |
| `F` | Tela cheia |
| `Esc` | Sair |

Objetos que respondem ao `E`: porta, interruptor (ao lado da porta), ar-condicionado (parede do fundo), janela, notebook (mesa do professor) e painel da tela de projeção (à direita do quadro).

## Bibliotecas

| Biblioteca | Uso |
|---|---|
| OpenGL + GLU | Desenho (pipeline fixo) |
| [freeglut](https://freeglut.sourceforge.net/) | Janela, teclado, mouse e texto na tela |
| [stb_image](https://github.com/nothings/stb) | Leitura das imagens de textura (já incluída em `external/`) |

Nenhum motor gráfico ou framework (Unity, Godot, three.js etc.) é utilizado.

## Como compilar e executar

Precisa de um compilador C++17 e do [CMake](https://cmake.org/) 3.16 ou superior.

### Windows (MinGW)

```powershell
cmake -B build -G "MinGW Makefiles"
cmake --build build -j
.\build\sala-de-aula.exe
```

Na primeira compilação o CMake baixa e compila o freeglut automaticamente (precisa de internet).

### Linux

```bash
sudo apt install build-essential cmake freeglut3-dev
cmake -B build
cmake --build build -j
./build/sala-de-aula
```

O programa encontra a pasta `assets/` sozinho, seja aberto da raiz do projeto ou de dentro de `build/`.

## Estrutura do código

```
src/
├── main.cpp          Janela GLUT, teclado, mouse, laço principal e HUD (mira, dicas, ajuda)
├── Camera.cpp/.h     Câmera em primeira pessoa (yaw/pitch + gluLookAt) e movimento
├── Cena.cpp/.h       Estado dos objetos, animações, luzes, posição de cada objeto,
│                     colisão e interação
├── Primitivas.cpp/.h Formas básicas: cubo, cilindro e plano
├── Materiais.cpp/.h  Material de cada superfície (glMaterial)
├── Texturas.cpp/.h   Carregamento das texturas
└── objects/          Um arquivo por objeto modelado
    ├── Sala.cpp            Paredes, piso, teto, rodapé, corredor e paisagem
    ├── Mesa.cpp            ├── Cadeira.cpp        ├── Quadro.cpp
    ├── Notebook.cpp        ├── Lampada.cpp        ├── Interruptor.cpp
    ├── ArCondicionado.cpp  ├── Janela.cpp         ├── Porta.cpp
    ├── Projetor.cpp        └── Lixeira.cpp
assets/textures/      Imagens das texturas
tools/                Script que gera as texturas (python tools/gerar_texturas.py)
```

**Como um quadro é desenhado** (`main.cpp → desenhar`):

1. `gluPerspective` define a projeção.
2. `aplicarCamera()` posiciona a câmera com `gluLookAt`.
3. `configurarLuzes()` posiciona o sol e as lâmpadas (depois da câmera, para ficarem fixas no mundo).
4. `desenharCena()` posiciona cada objeto com `glTranslatef`/`glRotatef` e chama sua função de desenho.
5. `desenharHUD()` desenha a mira e os textos em 2D por cima.

## Objetos modelados

Todos modelados pela equipe a partir de cubos, cilindros e planos:

| # | Objeto | Interação / Animação |
|---|---|---|
| 1 | Cadeira | — (12 de alunos + 1 do professor) |
| 2 | Mesa | — (12 de alunos + mesa do professor com painel frontal) |
| 3 | Quadro branco | — (moldura, bandeja, pincéis e apagador) |
| 4 | Notebook | Tampa abre/fecha (rotação na dobradiça) e a tela acende |
| 5 | Luminária | Liga/desliga a luz pontual |
| 6 | Ar-condicionado | Liga/desliga; aleta oscila (rotação) |
| 7 | Janela + persiana | Lâminas giram (rotação); persiana recolhe (translação + escala) |
| 8 | Porta | Abre/fecha na dobradiça (rotação em torno de pivô) |
| 9 | Interruptor | Tecla gira e liga/desliga as luminárias |
| 10 | Projetor e tela | Tela desce/sobe (escala) e exibe o slide |
| 11 | Lixeira | — |

Modelos importados: nenhum.

## Técnicas implementadas

- **Transformações:** cada peça é um cubo ou cilindro unitário posicionado com `glTranslatef` e dimensionado com `glScalef` (função `bloco`). Os objetos são posicionados na sala com `glTranslatef` + `glRotatef`, e `glPushMatrix`/`glPopMatrix` isolam cada transformação.
  - **Rotação em torno de pivô:** porta, tampa do notebook, aleta do ar e tecla do interruptor: translada até a dobradiça, rotaciona e desenha a peça deslocada a partir dali.
  - **Translação animada:** lâminas e régua da persiana subindo; régua da tela de projeção.
  - **Escala animada:** tela de projeção desenrolando (`glScalef` em Y com pivô no topo); cordões da persiana.
- **Animação:** cada objeto tem um valor de 0 a 1 que avança aos poucos em direção ao estado pedido (`aproximar`) e é suavizado (`suavizar`).
- **Câmera:** primeira pessoa com ângulos de Euler (yaw/pitch) e `gluLookAt`.
- **Iluminação:** modelo de Phong do OpenGL (ambiente + difusa + especular) com sombreamento de Gouraud. `GL_LIGHT0` é o sol (direcional, `w = 0`), `GL_LIGHT1`–`GL_LIGHT5` são as luminárias (pontuais, `w = 1`, com atenuação). A luz do sol diminui quando a persiana fecha. Paredes, piso e teto são divididos em pedaços de 0,5 m para a iluminação por vértice ficar suave.
- **Materiais:** `glMaterialfv` com cor, coeficiente especular, expoente de brilho e emissão (lâmpadas acesas, telas, LEDs) próprios de cada objeto.
- **Texturas:** imagens lidas com stb_image e enviadas com `gluBuild2DMipmaps`, em modo `GL_MODULATE` (a textura é multiplicada pela iluminação).
- **Transparência:** vidro da janela com `GL_BLEND`, desenhado por último sem gravar no depth buffer.
- **Colisão:** caixas alinhadas aos eixos (AABB); o movimento é testado em X e Z separadamente para o jogador deslizar nas paredes.
- **Interação:** o alvo é o objeto mais próximo (até 2,6 m) cuja direção faz um ângulo pequeno com a direção da câmera (produto escalar ≥ 0,75).

## Entregáveis

- [ ] Código-fonte neste repositório
- [ ] Relatório técnico
- [ ] Vídeo de demonstração (2 a 4 min) — *link aqui*
