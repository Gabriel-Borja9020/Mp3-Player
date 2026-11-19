# ♫ Reproductor de Música

**Un reproductor de archivos MP3 desde la terminal, desarrollado en C++17.**

Selecciona una carpeta con tu música, navega por tu colección y reproduce tus canciones favoritas directamente desde la terminal.

---

## ✨ Características

- 🎵 Reproducción de archivos MP3.
- 📂 Selección de una carpeta para cargar tu música.
- ⌨️ Navegación mediante teclado.
- 🔀 Reproducción aleatoria.
- 🔤 Organización alfabética de las canciones.
- 🖥️ Interfaz interactiva en la terminal.

## 🎮 Controles

| Tecla | Acción |
|:---:|---|
| `↑` / `↓` | Navegar por la lista de canciones |
| `Enter` | Reproducir la canción seleccionada |
| `A` | Elegir y reproducir una canción aleatoria |
| `Q` | Salir del reproductor |

---

## 🛠️ Tecnologías

- **C++17:** lenguaje principal del proyecto.
- **CMake:** sistema de construcción.
- **miniaudio:** biblioteca de audio utilizada para la reproducción.
- **POSIX:** gestión de la entrada del teclado y la terminal.

Un agradecimiento especial al equipo de [miniaudio](https://github.com/mackron/miniaudio) por desarrollar y compartir esta biblioteca, que hace posible la reproducción de audio en este proyecto.

---

## 📋 Requisitos

- Linux o macOS.
- Compilador de C++ con soporte para C++17.
- **CMake 4.4 o superior**, requerido por el `CMakeLists.txt` actual.
- Un dispositivo de salida de audio configurado.

> **Nota:** El programa utiliza interfaces POSIX como `termios` y `unistd`, por lo que no está preparado para compilar directamente en Windows.

---

## 🚀 Instalación y compilación

### Linux

**Void Linux**

```bash
sudo xbps-install -S base-devel cmake
```

**Debian / Ubuntu**

```bash
sudo apt update
sudo apt install build-essential cmake
```

### macOS

Instala las herramientas de línea de comandos de Apple y CMake:

```bash
xcode-select --install
brew install cmake
```

### Compilar el proyecto

Desde la raíz del repositorio, ejecuta:

```bash
cmake -S . -B build
cmake --build build
```

### Ejecutar

**Linux:**

```bash
./build/Musica_CPP
```

**macOS:**

```bash
./build/Musica_CPP
```

Cuando el programa lo solicite, introduce la ruta de la carpeta que contiene tus archivos MP3.

Por ejemplo:

```text
/home/tu_usuario/Música
```

En macOS también puedes arrastrar la carpeta directamente a la terminal para insertar su ruta.

---

## 📝 Notas importantes

- La búsqueda incluye archivos cuya extensión sea exactamente `.mp3`.
- No se realiza una búsqueda recursiva en subcarpetas.
- La interfaz necesita ejecutarse en una terminal interactiva real.
- La reproducción depende del dispositivo de salida y del backend de audio disponible en el sistema.

---

## 📜 Licencia

Este proyecto está distribuido bajo la licencia **MIT**.

Consulta el archivo [LICENSE](LICENSE) para obtener más información.

---

<p align="center">
  Hecho con C++17 ♫
</p>
