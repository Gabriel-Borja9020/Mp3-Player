//
// Created by gabrielvoid on 11/14/26.
//

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Graficos.hpp"
#include <fcntl.h>

int obtenerAnchoTerminal() {
    struct winsize w{};

    int terminal = open("/dev/tty", O_RDONLY);

    if (terminal == -1) {
        return 80;
    }

    if (ioctl(terminal, TIOCGWINSZ, &w) == -1) {
        close(terminal);
        return 80;
    }

    close(terminal);

    if (w.ws_col == 0) {
        return 80;
    }

    return w.ws_col;
}

void mostrarBorde() {
    int ancho = obtenerAnchoTerminal();
    std::cout << std::string(ancho, '_') << std::endl;
}

void mostrarInterfaz() {

mostrarBorde();
    std::cout << R"(
▄▖         ▌    ▗       ▖  ▖▄▖▄▖
▙▘█▌▛▌▛▘▛▌▛▌▌▌▛▘▜▘▛▌▛▘  ▛▖▞▌▙▌▄▌
▌▌▙▖▙▌▌ ▙▌▙▌▙▌▙▖▐▖▙▌▌   ▌▝ ▌▌ ▄▌
    ▌
    )"<<std::endl;;
}

void mostrarCancion(const std::string& nombre) {
    std::cout << "\nReproduciendo: " << nombre << '\n';
}

void Bienvenida()
{
std::cout<<R"(
  ╔══════════════════════════════════════════════════════════════════════════════════════╗
  ║                                                                                      ║
  ║     ♫  MUSIC PLAYER                                           WELCOME TO YOUR MUSIC  ║
  ║                                                                                      ║
  ╠══════════════════════════════════════════════════════════════════════════════════════╣
  ║                                                                                      ║
  ║     ¡Bienvenido, usuario! Antes de escuchar tu música favorita, lee lo siguiente:    ║
  ║                                                                                      ║
  ║       [1] Ten preparada una carpeta con tus archivos de audio .mp3                   ║
  ║       [2] Ingresa a continuación la ruta exacta de tu carpeta                        ║
  ║                                                                                      ║
  ║                                                                                      ║
  ║     Gabriel Borja agradece que utilices su programa.                                 ║
  ║                                                                                      ║
  ║     ♫  ¡Disfruta de tu música!                                                       ║
  ║                                                                                      ║
  ╚══════════════════════════════════════════════════════════════════════════════════════╝
)"<<std::endl;
}

void Disco_de_Vinilo()
{
    std::cout<<R"(
             .@@@@@@@@@@@@@@@@
          =@@@@@@@@@@@@@@@@@@@@@@.
        %@@@@@@@@@@@@@@@@@@@@@@@@@@+        .::
      :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    *@@@@%+
     +@@@@@@@@@@@@@@@##@@@@@@@@@@@@@@@+ +%@@@@@%-
    #@@@@@@@@@@@=:*######+:*@@@@@@@@@@@@.%@@@@##.
   -@@@@@@@@@@-#############++@@@@@@@@@@  -:=*+
   @@@@@@@@@#:################:@@@@@@@@@@-
  +@@@@@@@@@.##################-@@@@@@@@*:
  @@@@@@@@@@-#######   .#######.@@@@@@@@ -
  @@@@@@@@@@-#######    #######.@@@@@@@.@*
  %@@@@@@@@@ ...................@@@@@@:+@
   @@@@@@@@@@..................%@@= :#%@@
   *@@@@@@@@@@:..............=@@:+%@@@@@
    @@@@@@@@@@@@- ........ +@@@-+@@@@@@+
     @@@@@@@@@@@@@@@@##@@@@@@@**@@@@@@-
      +@@@@@@@@@@@@@@@@@@@@@=#%%@@@@@
        @@@@@@@@@@@@@@@@@@@*%%%@@@@*
          *@@@@@@@@@@@@@@@@%%%@@@:
             :@@@@@@@@@@@@@@@@
                     ..
)"<<std::endl;
}
