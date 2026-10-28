# Sala de Aula 3D — OpenGL

Aplicação em OpenGL que reproduz uma sala de aula da UFAL em 3D, explorável em primeira pessoa.

Projeto da disciplina de **Computação Gráfica** — Universidade Federal de Alagoas (UFAL).

> Em desenvolvimento.

## Como compilar e executar

### Windows (MinGW)

```powershell
cmake -B build -G "MinGW Makefiles"
cmake --build build -j
.\build\sala-de-aula.exe
```

### Linux

```bash
sudo apt install build-essential cmake freeglut3-dev
cmake -B build
cmake --build build -j
./build/sala-de-aula
```
