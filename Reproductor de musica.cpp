//
// Created by gabrielvoid on 11/14/26.
//
#include<iostream>
#include "Graficos.hpp"
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>//esto me permitira ordenas las canciones alfabeticamente
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio-master/miniaudio.h"
#include <termios.h>
#include <unistd.h>
#include <random>

using namespace std;
namespace fs = std::filesystem;

static void limpiarPantalla() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}

void Pidiendo_Ruta_de_Carpeta();
void Menu(const string& carpeta);
void Mostrar_Musica(const string& carpeta);
void Marco_Playlist(
    const vector<string>& canciones,
    size_t inicio,
    size_t seleccionada
);

static ma_engine motor;
static bool motorInicializado = false;
static ma_sound sonidoActual;
static bool sonidoInicializado = false;

static void DetenerSonidoActual() {
    if (sonidoInicializado) {
        ma_sound_stop(&sonidoActual);
        ma_sound_uninit(&sonidoActual);
        sonidoInicializado = false;
    }
}

enum class Tecla {
    ARRIBA,
    ABAJO,
    ENTER,
    SALIR,
    OTRA,
    ALEATORIA
};

class ModoTerminal {
private:
    termios original{};
    bool activo_ = false;

public:
    ModoTerminal() {
        if (!isatty(STDIN_FILENO))
            return;

        if (tcgetattr(STDIN_FILENO, &original) == -1)
            return;

        termios nuevo = original;

        nuevo.c_lflag &= ~(ICANON | ECHO);
        nuevo.c_cc[VMIN] = 1;
        nuevo.c_cc[VTIME] = 0;

        activo_ = tcsetattr(STDIN_FILENO, TCSANOW, &nuevo) == 0;
    }

    ~ModoTerminal() {
        if (activo_) {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
        }
    }

    bool funciona() const {
        return activo_;
    }
};

Tecla LeerTecla() {
    char tecla;

    // Lee bytes directamente del TTY para evitar que el buffer de iostream
    // se adelante y separe las secuencias enviadas por las flechas.
    if (read(STDIN_FILENO, &tecla, 1) != 1)
        return Tecla::SALIR;

    if (tecla == '\x1b') {
        char segundo, tercero;

        if (read(STDIN_FILENO, &segundo, 1) != 1 ||
            read(STDIN_FILENO, &tercero, 1) != 1)
            return Tecla::SALIR;

        // Los terminales suelen usar CSI (ESC [ A) o SS3 (ESC O A).
        if ((segundo == '[' || segundo == 'O') && tercero == 'A')
            return Tecla::ARRIBA;

        if ((segundo == '[' || segundo == 'O') && tercero == 'B')
            return Tecla::ABAJO;

        return Tecla::OTRA;
    }

    if (tecla == '\n' || tecla == '\r')
        return Tecla::ENTER;

    if (tecla == 'q' || tecla == 'Q')
        return Tecla::SALIR;

    if (tecla == 'a' || tecla == 'A')
        return Tecla::ALEATORIA;

    return Tecla::OTRA;
}



//variables globales
size_t seleccionada = 0;
size_t inicio = 0;
random_device rd;
mt19937 gen(rd());

int main()
{
    limpiarPantalla();//#1. Prevenir compilaciones anteriores.
    mostrarInterfaz();//#2. Este solo dice el nombre.
    mostrarBorde();//#3. Por estetica
    Bienvenida();
    Pidiendo_Ruta_de_Carpeta();
    if (motorInicializado) {
        ma_engine_uninit(&motor);
    }
    return 0;
}

void Pidiendo_Ruta_de_Carpeta()
{
    string carpeta;

    cout << "Introduce la ruta aqui ->" << endl;
    getline(cin, carpeta);

    std::error_code ec;

    if (fs::exists(carpeta, ec))
    {
        if (!fs::is_directory(carpeta, ec)) {
            cout << "La ruta no es una carpeta." << endl;
            return;
        }
        cout << "La ruta existe" << endl;
        Menu(carpeta);
    }
    else if (ec)
    {
        cout << "Error: " << ec.message() << endl;
    }
}
void Mostrar_Musica(const string& carpeta)
{
    vector<string> canciones;

    for (const auto& archivo : fs::directory_iterator(carpeta))
    {
        if (archivo.path().extension() == ".mp3")
        {
            canciones.push_back(
                archivo.path().filename().string()
            );
        }
    }

    sort(canciones.begin(), canciones.end());

    if (canciones.empty()) {
        cout << "No se encontraron archivos .mp3." << endl;
        return;
    }

    // Inicializar miniaudio.
    ma_result resultado = ma_engine_init(nullptr, &motor);

    if (resultado != MA_SUCCESS) {
        cerr << "No se pudo inicializar miniaudio: "
             << resultado << endl;
        return;
    }

    motorInicializado = true;

    // Variables de navegación.
    const size_t FILAS = 10;

    size_t seleccionada = 0;
    size_t inicio = 0;

    string estado = "Flechas: navegar | ENTER: reproducir | Q: salir del programa | A: musica aleatoria";

    auto reproducirCancion = [&](size_t indice) {
        const fs::path ruta = fs::path(carpeta) / canciones[indice];

        // Detener y liberar la canción anterior antes de iniciar la nueva.
        DetenerSonidoActual();

        resultado = ma_sound_init_from_file(
            &motor,
            ruta.string().c_str(),
            MA_SOUND_FLAG_STREAM,
            nullptr,
            nullptr,
            &sonidoActual
        );

        if (resultado == MA_SUCCESS) {
            sonidoInicializado = true;
            resultado = ma_sound_start(&sonidoActual);
        }

        if (resultado == MA_SUCCESS) {
            estado = "Reproduciendo: " + canciones[indice];
        } else {
            DetenerSonidoActual();
            estado = "Error al reproducir: " + canciones[indice]
                   + " (codigo " + to_string(resultado) + ")";
        }
    };

    ModoTerminal terminal;

    if (!terminal.funciona()) {
        cerr << "No se pudo configurar el terminal." << endl;
        return;
    }

    while (true)
    {
        limpiarPantalla();

        Disco_de_Vinilo();

        cout << endl;

        Marco_Playlist(canciones, inicio, seleccionada);

        cout << "\nCanciones "
             << inicio + 1 << "-"
             << min(inicio + FILAS, canciones.size())
             << " de " << canciones.size() << endl;

        cout << estado << endl;

        cout.flush();

        Tecla tecla = LeerTecla();

        switch (tecla)
        {
            case Tecla::ARRIBA:

                if (seleccionada > 0) {
                    seleccionada--;
                }

                break;

            case Tecla::ABAJO:

                if (seleccionada + 1 < canciones.size()) {
                    seleccionada++;
                }

                break;

            case Tecla::ENTER:
            {
                reproducirCancion(seleccionada);
                break;
            }

            case Tecla::ALEATORIA:
            {
                uniform_int_distribution<size_t> elegir(0, canciones.size() - 1);
                seleccionada = elegir(gen);
                reproducirCancion(seleccionada);
                break;
            }

            case Tecla::SALIR:

                DetenerSonidoActual();
                limpiarPantalla();
                return;

            default:
                break;
        }

        // Controlar el desplazamiento de la ventana.

        if (seleccionada < inicio) {
            inicio = seleccionada;
        }
        else if (seleccionada >= inicio + FILAS) {
            inicio = seleccionada - FILAS + 1;
        }
    }
}

void Menu(const string& carpeta)
{
    limpiarPantalla();
    Disco_de_Vinilo();
    Mostrar_Musica(carpeta);
}

void Marco_Playlist(
    const vector<string>& canciones,
    size_t inicio,
    size_t seleccionada
)
{
    const size_t FILAS = 10;

    string borde(78, '=');

    cout << "╔" << borde << "╗" << endl;

    cout << "║"
         << "                                 MY PLAYLIST                                  "
         << "║" << endl;

    for (size_t fila = 0; fila < FILAS; ++fila)
    {
        size_t indice = inicio + fila;

        string nombre = "";

        if (indice < canciones.size()) {
            nombre = canciones[indice];
        }

        if (nombre.size() > 76) {
            nombre = nombre.substr(0, 73) + "...";
        }

        string espacios(76 - nombre.size(), ' ');

        // Resaltar la canción seleccionada.
        if (indice < canciones.size() && indice == seleccionada) {
            cout << "\033[7m";
        }

        cout << "║ " << nombre << espacios << " ║";

        if (indice < canciones.size() && indice == seleccionada) {
            cout << "\033[0m";
        }

        cout << endl;
    }

    cout << "╚" << borde << "╝" << endl;
}
