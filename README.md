# CVI — Capítulo 9: Rasterizador manual

Demo interactivo en **openFrameworks (C++)** para la materia *Computación Visual
Interactiva* — Capítulo 9, "The Graphics Pipeline" (Marschner & Shirley, *Fundamentals
of Computer Graphics*, 5th ed.).

En vez de usar `ofDrawTriangle()` (que delega todo el trabajo a la GPU), esta app
implementa el algoritmo clásico de rasterización de triángulos **a mano, píxel por
píxel**, usando funciones de arista (edge functions) y coordenadas baricéntricas —
para que se vea exactamente lo que describe el libro en las secciones 9.1 a 9.4.

## Qué cubre

| Sección | Tema | Dónde está en el código |
|---|---|---|
| 9.1 | Rasterización (bounding box + edge functions) | `edgeFunction()`, inicio de `rasterize()` |
| 9.2 | Operaciones antes/después de rasterizar (interpolación de atributos, escritura de fragmento) | dentro del loop de `rasterize()` |
| 9.3 | Antialiasing simple (supersampling 2×2) | `sampleOffsets`, cálculo de `coverage` |
| 9.4 | Culling de primitivas (orientación por área con signo) | `signedArea`, `wasCulled` |

## Controles

- **Arrastrar** cualquiera de los 3 vértices con el mouse — re-rasteriza en vivo
- **`a`** — prende/apaga antialiasing (supersampling 4 muestras por píxel)
- **`c`** — prende/apaga backface culling
- **`w`** — invierte el orden de los vértices (winding); combinado con culling activo,
  hace que el triángulo desaparezca — demuestra 9.4 en vivo

El HUD en pantalla muestra el área con signo (resultado de la edge function), la
orientación del triángulo, y cuántos píxeles se rasterizaron — la matemática de la
teoría, en vivo.

## Cómo correrlo

Requiere [openFrameworks](https://openframeworks.cc/download/) (probado en 0.12.1).

1. Genera un proyecto nuevo con el **Project Generator** de openFrameworks
   (plataforma Xcode y/o VS Code según tu IDE).
2. Reemplaza la carpeta `src/` generada por la de este repo.
3. Compila y corre (`make RunRelease` desde terminal, o Run Build Task en VS Code).

> **Nota (macOS):** en algunos Mac con chip Apple Silicon puede aparecer un artefacto
> visual de "rastro" al arrastrar los vértices muy rápido — es un efecto de cómo
> macOS compone los frames en pantalla, no un error del algoritmo (el HUD confirma
> que el área con signo y el conteo de píxeles son correctos en todo momento). Mover
> los vértices en pasos cortos evita el efecto.

## Contexto

Trabajo para la materia Computación Visual Interactiva, Universidad de los Andes.
Acompaña una presentación (teoría + matemática + código) sobre el Capítulo 9 del
libro de texto del curso.