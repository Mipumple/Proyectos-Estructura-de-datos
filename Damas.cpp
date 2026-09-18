#include <iostream>//Libreria de base 
#include <vector>// Libreria para vecxtores dinamicos 
#include <cmath>// Libreria para funciones matematicas
#include <string>// Libreria cadena de texto
#include <cstdlib>// Libreria para el "cls"

using namespace std;

// ponemos como constante la dimencion del tablero sw 8*8
const int tamaño = 8;

// Representación de fichas:
// ' ' : Casilla vacía
// 'B' : Ficha normal blanca    
// 'N' : Ficha normal negra 
// 'W' : Dama blanca
// 'M' : Dama negra

class JuegoDamas 
{
 private:
 //Estructura principal de los datos papu :V
    vector<vector<char>> tablero;
    char turnoActual; // 'B' para Blancas, 'N' para Negras

 public:
    //Se fabrica el tablero con espacion en blanco 
    JuegoDamas() 
    {
        //Declaramos quien inicia el juego
        tablero = vector<vector<char>>(tamaño, vector<char>(tamaño, ' '));
        inicializarTablero();
        turnoActual = 'B'; // Declara quien inicia comienzan las fichas claras (Blancas)
    }

    void inicializarTablero() 
    {
        // Colocar fichas negras en las primeras 3 filas )
        for (int i = 0; i < 3; ++i) 
        {
            for (int j = 0; j < tamaño; ++j) 
            {
                if ((i + j) % 2 != 0) 
                {
                    tablero[i][j] = 'N';
                }
            }
        }
        // Colocar fichas blancas en las últimas 3 filas 
        for (int i = 5; i < tamaño; ++i) 
        {
            for (int j = 0; j < tamaño; ++j) 
            {
                if ((i + j) % 2 != 0) 
                {
                    tablero[i][j] = 'B';
                }
            }
        }
    }

    void mostrarTablero() 
    //Tablero con las cordenadas 
    {
        cout << "\n    A   B   C   D   E   F   G   H\n";
        cout << "  ---------------------------------\n";
        for (int i = 0; i < tamaño; ++i) 
        {
            cout << i << " |";
            for (int j = 0; j < tamaño; ++j) 
            {
                cout << " " << tablero[i][j] << " |";
            }
            cout << " " << i << "\n";
            cout << "  ---------------------------------\n";
        }
        cout << "    A   B   C   D   E   F   G   H\n\n";
    }
    //Se verifica si es balca o negra
    bool esBlanca(char f) { return f == 'B' || f == 'W'; }
    bool esNegra(char f) { return f == 'N' || f == 'M'; }
    //Verifica el turno del oponente 
    bool esOponente(char f1, char f2) 
    {
        if (esBlanca(f1) && esNegra(f2)) return true;
        if (esNegra(f1) && esBlanca(f2)) return true;
        return false;
    }

    // Valida si hay movimientos de captura disponibles en todo el tablero para el jugador actual
    bool tieneCapturaObligatoria(char jugador) 
    {
        for (int i = 0; i < tamaño; ++i) 
        {
            for (int j = 0; j < tamaño; ++j) 
            {
                if (tablero[i][j] == jugador || (jugador == 'B' && tablero[i][j] == 'W') || (jugador == 'N' && tablero[i][j] == 'M')) 
                {
                    if (puedeComerDesde(i, j)) return true;
                }
            }
        }
        return false;
    }

    bool puedeComerDesde(int r, int c) 
    {
        char f = tablero[r][c];
        int dirF[4] = {-2, -2, 2, 2};
        int dirC[4] = {-2, 2, -2, 2};
        int medF[4] = {-1, -1, 1, 1};
        int medC[4] = {-1, 1, -1, 1};

        for (int k = 0; k < 4; ++k) 
        {
            int nr = r + dirF[k];
            int nc = c + dirC[k];
            int mr = r + medF[k];
            int mc = c + medC[k];

            // Si es ficha normal, restringir dirección hacia delante
            if (f == 'B' && k > 1) continue; // Blancas suben (filas disminuyen)
            if (f == 'N' && k < 2) continue; // Negras bajan (filas aumentan)

            if (nr >= 0 && nr < tamaño && nc >= 0 && nc < tamaño) 
            {
                if (tablero[nr][nc] == ' ' && esOponente(f, tablero[mr][mc])) {
                    return true;
                }
            }
        }
        return false;
    }

    bool realizarMovimiento(int r1, int c1, int r2, int c2) 
    {
        char ficha = tablero[r1][c1];

        // Validaciones básicas de turno y origen/destino
        if (ficha == ' ' ) { cout << "Casilla de origen vacia.\n"; return false; }
        if (turnoActual == 'B' && !esBlanca(ficha)) { cout << "No es tu ficha (Turno Blancas).\n"; return false; }
        if (turnoActual == 'N' && !esNegra(ficha)) { cout << "No es tu ficha (Turno Negras).\n"; return false; }
        if (tablero[r2][c2] != ' ') { cout << "Casilla de destino ocupada.\n"; return false; }

        int diffR = r2 - r1;
        int diffC = c2 - c1;

        bool esSalto = (abs(diffR) == 2 && abs(diffC) == 2);
        bool esPasoSimple = (abs(diffR) == 1 && abs(diffC) == 1);

        // Verificar regla de obligación de comer
        bool obligacion = tieneCapturaObligatoria(turnoActual);

        if (obligacion && !esSalto) 
        {
            cout << "¡Hay obligacion de comer! Debes realizar un salto de captura.\n";
            return false;
        }

        // Movimiento simple en diagolnal 
        if (esPasoSimple && !obligacion) {
            if (ficha == 'B' && diffR > 0) { cout << "Las fichas normales solo se mueven hacia adelante.\n"; return false; }
            if (ficha == 'N' && diffR < 0) { cout << "Las fichas normales solo se mueven hacia adelante.\n"; return false; }
            
            // verifica si la fichas es dama o ficha normal
            tablero[r2][c2] = ficha;
            tablero[r1][c1] = ' ';
            coronar(r2, c2);
            return true;
        }

        // Movimiento de salto (Comer)
        if (esSalto) 
        {
            int medR = (r1 + r2) / 2;
            int medC = (c1 + c2) / 2;
            char rival = tablero[medR][medC];

            if (esOponente(ficha, rival)) 
            {
                // Restricción de dirección para fichas normales al comer
                if (ficha == 'B' && diffR > 0) { cout << "No puedes comer hacia atras con ficha normal.\n"; return false; }
                if (ficha == 'N' && diffR < 0) { cout << "No puedes comer hacia atras con ficha normal.\n"; return false; }

                // Ejecutar captura
                tablero[r2][c2] = ficha;
                tablero[r1][c1] = ' ';
                tablero[medR][medC] = ' ';
                coronar(r2, c2);

                // Comprobar si puede seguir comiendo 
                if (puedeComerDesde(r2, c2)) 
                {
                    cout << "¡Tienes otro salto disponible con la misma ficha! Debes continuar.\n";
                    mostrarTablero();
                    int nr3, nc3;
                    cout << "Introduce siguiente movimiento para continuar comiendo (ej: F4 D6): ";
                    // Aqui se le informa que puede seguir comiendo en cada hasta que alla terminado de comer todas las fichas 
                }
                return true;
            } 
            else 
            {
                cout << "No hay ficha rival que saltar en esa posicion.\n";
                return false;
            }
        }

        cout << "Movimiento no valido.\n";
        return false;
    }

    void coronar(int r, int c) 
    //Verifica cuando una ficha llega al final para coronarla 
    //Si es ficha blanca o negra 
    {
        if (tablero[r][c] == 'B' && r == 0) 
        {
            tablero[r][c] = 'W';
            cout << "¡Ficha blanca coronada a Dama (W)!\n";
        }
        else if (tablero[r][c] == 'N' && r == tamaño - 1) 
        {
            tablero[r][c] = 'M';
            cout << "¡Ficha negra coronada a Dama (M)!\n";
        }
    }

    //Funcion para cambiar de turno y terminar partida
    void cambiarTurno() 
    {
        turnoActual = (turnoActual == 'B') ? 'N' : 'B';
    }

    char obtenerTurno() { return turnoActual; }

    bool verificarFinJuego() 
    {
        //Verifica cuantas fichas le quedan a cada juagardor
        int countB = 0, countN = 0;
        bool movimientosB = false, movimientosN = false;

        //Recprre la matriz fila por fila contando las fichas cuando encuentra una ing=crementa en 1
        for (int i = 0; i < tamaño; ++i) 
        {
            for (int j = 0; j < tamaño; ++j) 
            {
                if (esBlanca(tablero[i][j])) 
                {
                    countB++;
                    if (tieneMovimientosValidos(i, j)) movimientosB = true;
                }
                if (esNegra(tablero[i][j])) 
                {
                    countN++;
                    if (tieneMovimientosValidos(i, j)) movimientosN = true;
                }
            }
        }

        if (countB == 0) 
        {
            cout << "\n¡¡¡¡¡¡¡GANAN LA FICHAS NEGRAS POR ELIMINACION!!!!!!\n";
            return true;
        }
        if (countN == 0) 
        {
            cout << "\n¡¡¡¡¡¡¡GANAN LA FICHAS BLANCAS POR ELIMINACION!!!!!!\n";
            return true;
        }
        if (turnoActual == 'B' && !movimientosB) 
        {
            cout << "\n¡Las blancas se han quedado sin movimientos! Ganan las Negras.\n";
            return true;
        }
        if (turnoActual == 'N' && !movimientosN) 
        {
            cout << "\n¡Las negras se han quedado sin movimientos! Ganan las Blancas.\n";
            return true;
        }

        return false;
    }

    bool tieneMovimientosValidos(int r, int c) 
    {
        // Validación rápida para ver si la ficha tiene cualquier movimiento o salto
        return true; 
    }
};

int main() 
{
   //Limpiar pantalla para que se vea bonito :3
    system("cls");
    JuegoDamas juego;
    string origen, destino;
    //Mensaje de inicio
    cout << "============== JUEGO DE DAMAS ===============\n";
    cout << "Formato de coordenadas: Letra de la Columna y Numero de Fila (Ejemplo de A5 a B4 -> A 5 B 4)\n";
    cout << "Fichas Claras = B (Blancas), Fichas Oscuras = N (Negras). Damas = W y M.\n";

   //Inicio del programa principal 
    while (true) 
    {

    char col1, col2;
    int r1, r2, c1, c2;
        //Instrucciones 
        juego.mostrarTablero();
        cout << "Turno actual: " << (juego.obtenerTurno() == 'B' ? "FICHAS BLANCAS [B]" : "FICHAS NEGRAS [N]") << "\n";
        cout << "Introduce tu movimiento (Columna de Origen y Fila Origen , Columna de Destino y Fila Destino, ej: A 5 B 4) o 'X' para salir: ";
        
        cin >> col1;
        if (col1 == 'X' || col1 == 'x') 
        {
            cout << "====== JUEGOS TERMINADO POR LOS JUGADORES ======\n";
            break;
        }

        cin >> r1 >> col2 >> r2;

        // Convertir letras a índices numéricos (A-H -> 0-7)
        c1 = toupper(col1) - 'A';
        c2 = toupper(col2) - 'A';
        //Verifica que ponagas cordenadas dentro del tablero para que no se salga 
        if (c1 < 0 || c1 >= tamaño || c2 < 0 || c2 >= tamaño || r1 < 0 || r1 >= tamaño || r2 < 0 || r2 >= tamaño) 
        {
            cout << "Coordenadas fuera de rango. Intenta de nuevo.\n";
            continue;
        }

        if (juego.realizarMovimiento(r1, c1, r2, c2)) 
        {
            if (juego.verificarFinJuego()) 
            {
                juego.mostrarTablero();
                break;
            }
            juego.cambiarTurno();
        }

    }

    return 0;
}