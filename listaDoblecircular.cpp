#include <iostream>
#include <string>
#include <conio.h>   
#include <windows.h> 

using namespace std;

struct Cancion {
    string titulo;
    string artista;
    string duracion;

    Cancion* siguiente;
    Cancion* anterior;
};

string leerCadenaObligatoria(string mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);

        if (entrada.empty()) {
            cout << "[Error] Este campo no puede quedar vacio. Intente de nuevo.\n";
            continue;
        }

        bool soloEspacios = true;
        for (char c : entrada) {
            if (c != ' ') {
                soloEspacios = false;
                break;
            }
        }

        if (soloEspacios) {
            cout << "[Error] El campo no puede contener solo espacios. Escriba algo real.\n";
        }
        else {
            return entrada;
        }
    }
}

string leerDuracionValida(string mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);

        size_t posicionDosPuntos = entrada.find(':');

        if (posicionDosPuntos == string::npos || posicionDosPuntos == 0 || posicionDosPuntos == entrada.length() - 1) {
            cout << "[Error] Formato incorrecto. Debe usar el formato MINUTOS:SEGUNDOS (ej: 3:45 o 04:12).\n";
            continue;
        }

        bool esValido = true;
        for (size_t i = 0; i < entrada.length(); i++) {
            if (i == posicionDosPuntos) continue;

            if (entrada[i] < '0' || entrada[i] > '9') {
                esValido = false;
                break;
            }
        }

        if (!esValido) {
            cout << "[Error] La duracion solo debe contener numeros y el caracter ':'. Intente de nuevo.\n";
            continue;
        }

        return entrada;
    }
}

void agregarCancion(Cancion*& lista, string titulo, string artista, string duracion) {
    Cancion* nueva = new Cancion();
    nueva->titulo = titulo;
    nueva->artista = artista;
    nueva->duracion = duracion;

    if (lista == nullptr) {
        lista = nueva;
        nueva->siguiente = lista;
        nueva->anterior = lista;
    }
    else {
        Cancion* ultimo = lista->anterior;

        ultimo->siguiente = nueva;
        nueva->anterior = ultimo;
        nueva->siguiente = lista;
        lista->anterior = nueva;
    }
    cout << "\n[OK] Cancion '" << titulo << "' agregada exitosamente a la lista circular.\n";
}

void mostrarListaCompleta(Cancion* lista) {
    if (lista == nullptr) {
        cout << "\n==================================================\n";
        cout << " [ALERTA] La playlist esta vacia.                 \n";
        cout << " No hay canciones registradas para mostrar.       \n";
        cout << "==================================================\n";
        return;
    }

    Cancion* actual = lista;
    cout << "\n=== ESQUEMA DE LA PLAYLIST CIRCULAR ===\n\n";
    cout << "   (INICIO)\n";
    cout << "      v\n";

    do {
        cout << "  [ " << actual->titulo << " | " << actual->artista << " (" << actual->duracion << ") ]\n";

        if (actual->siguiente != lista) {
            cout << "     | ^\n";
            cout << "     v |\n";
        }

        actual = actual->siguiente;
    } while (actual != lista);

    cout << "     | ^\n";
    cout << "     v |\n";
    cout << "   (Vuelve al INICIO para cerrar el circulo)\n";
    cout << "=======================================\n";
}

void reproducirPlaylist(Cancion* lista) {
    if (lista == nullptr) {
        cout << "\n==================================================\n";
        cout << " [ERROR] No se puede iniciar el reproductor.      \n";
        cout << " La playlist esta vacia. Agrega canciones primero.\n";
        cout << "==================================================\n";
        return;
    }

    Cancion* actual = lista;
    bool enReproduccion = true;

    while (enReproduccion) {
        system("cls");

        cout << "==================================================\n";
        cout << "               REPRODUCIENDO PLAYLIST             \n";
        cout << "==================================================\n\n";
        cout << " >> Cancion:  " << actual->titulo << "\n";
        cout << " >> Artista:  " << actual->artista << "\n";
        cout << " >> Duracion: " << actual->duracion << "\n\n";
        cout << "==================================================\n";
        cout << " [INSTRUCCIONES DE NAVEGACION]\n";
        cout << " -> Flecha Derecha   : Siguiente cancion\n";
        cout << " -> Flecha Izquierda : Cancion anterior\n";
        cout << " -> Tecla ESC        : Regresar al menu principal\n";
        cout << "==================================================\n";

        int tecla = _getch();

        if (tecla == 0 || tecla == 224) {
            tecla = _getch();

            if (tecla == 77) {
                actual = actual->siguiente;
            }
            else if (tecla == 75) {
                actual = actual->anterior;
            }
        }
        else if (tecla == 27) {
            enReproduccion = false;
        }
    }
}

int main() {
    Cancion* playlist = nullptr;
    int opcion;

    do {
        cout << "\n===================================\n";
        cout << "          MENU REPRODUCTOR         \n";
        cout << "===================================\n";
        cout << "1. Agregar cancion\n";
        cout << "2. Mostrar lista completa\n";
        cout << "3. Reproducir playlist\n";
        cout << "4. Salir\n";
        cout << "===================================\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            system("cls");
            cout << "\n[Error] Por favor, ingrese un numero valido (1, 2, 3 o 4).\n";
            opcion = 0;
            continue;
        }

        cin.ignore();

        switch (opcion) {
        case 1: {
            string titulo, artista, duracion;
            cout << "\n--- REGISTRAR NUEVA CANCION ---\n";
            titulo = leerCadenaObligatoria("Ingrese el nombre de la cancion: ");
            artista = leerCadenaObligatoria("Ingrese el artista: ");
            duracion = leerDuracionValida("Ingrese la duracion (ej. 3:40): ");

            agregarCancion(playlist, titulo, artista, duracion);
            break;
        }
        case 2:
            mostrarListaCompleta(playlist);
            break;
        case 3:
            reproducirPlaylist(playlist);
            break;
        case 4:
            cout << "\nSaliendo del programa de forma segura. ¡Hasta luego!\n";
            break;
        default:
            cout << "\n[Error] Opcion fuera de rango (1-4). Intente de nuevo.\n";
        }
    } while (opcion != 4);

    return 0;
}