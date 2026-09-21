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
 //Con un arreglo bidiencional
    vector<vector<char>> tablero;
    char turnoActual; // 'B' para Blancas, 'N' para Negras

 public:
    //Se fabrica el tablero con espacion en blanco 
    JuegoDamas() 
    {
        //Declaramos quien inicia el juego
        tablero = vector<vector<char>>(tamaño, vector<char>(tamaño, ' '));
        inicializarTablero();
        turnoActual = 'B'; // Declara quien inicia comienzan las fichas Blancas
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
            cout < < "  ---------------------------------\n";
        }
        cout << "    A   B   C   D   E   F   G   H\n\n";
    }

    //Se verifica si es una ficha normal o si es una dama 
    //ya sea dama blanca o dama negra 
    bool esBlanca(char f) { return f == 'B' || f == 'W'; }
    bool esNegra(char f) { return f == 'N' || f == 'M'; }
    
    //comapara dos fichas y regresa un verdadero si es bandos contrario
    bool esOponente(char f1, char f2) 
    {
        //Aqui se hace la coparacion
        if (esBlanca(f1) && esNegra(f2)) return true;
        if (esNegra(f1) && esBlanca(f2)) return true;
        //Si son del mismo bando regresa un falso
        return false;
    }

    // revisa si tenemos almenos una captura obligatoria en todo el tablero
    bool tieneCapturaObligatoria(char jugador) 
    {
        for (int i = 0; i < tamaño; ++i) 
        {
            for (int j = 0; j < tamaño; ++j) 
            {
                //Identifica las fichas propias y las damas 
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
        //estas de -2 representa donde va a aterrizar la ficha 
        int dirF[4] = {-2, -2, 2, 2};
        int dirC[4] = {-2, 2, -2, 2};
        //Aqui verifica con -1 las fichas qeu estan alrededor si son enemigas o no
        int medF[4] = {-1, -1, 1, 1};
        int medC[4] = {-1, 1, -1, 1};

        for (int k = 0; k < 4; ++k) 
        {
            int nr = r + dirF[k];
            int nc = c + dirC[k];
            int mr = r + medF[k];
            int mc = c + medC[k];

            // Si es ficha normal, restringir dirección hacia delante
            if (f == 'B' && k > 1 && tablero[r][c] != 'W') continue; // Blancas suben 
            if (f == 'N' && k < 2 && tablero[r][c] != 'M') continue; // Negras bajan 

            //Verifica que la ficha no se salga del tablero bro
            if (nr >= 0 && nr < tamaño && nc >= 0 && nc < tamaño) 
            {
                //aqui se verifica que la casilla este basia y con la funcion esOponente verifica si es una ficha rival
                if (tablero[nr][nc] == ' ' && esOponente(f, tablero[mr][mc])) {
                    return true;
                }
            }
        }
        return false;
    }

    // Realizar movimiento indicando origen y dirección (I / D) con multi-salto automático ---
    bool realizarMovimientoPorDireccion(int r1, int c1, char dir) 
    {
        char ficha = tablero[r1][c1];

        // Aqui valida que allas seleccionado una ficha y qeu la ficha sea tuya
        if (ficha == ' ') { cout << "Casilla de origen vacia.\n"; return false; }
        if (turnoActual == 'B' && !esBlanca(ficha)) { cout << "No es tu ficha (Turno Blancas).\n"; return false; }
        if (turnoActual == 'N' && !esNegra(ficha)) { cout << "No es tu ficha (Turno Negras).\n"; return false; }
        //Verifica queA5 el jugador escoja una direcion valida 
        dir = toupper(dir);
        if (dir != 'I' && dir != 'D') { cout << "Direccion invalida. Usa 'I' (Izquierda) o 'D' (Derecha).\n"; return false; }

        int dC = (dir == 'I') ? -1 : 1;
        bool obligacion = tieneCapturaObligatoria(turnoActual);

        int r2 = -1, c2 = -1;
        bool esSaltoValido = false;
        bool esPasoValido = false;

        int filasDestino[4];
        int numOpciones = 0;
        //Verifica el salto de las fichas normales y reinas
        if (ficha == 'B') {
            filasDestino[0] = r1 - 2; filasDestino[1] = r1 - 1; 
            numOpciones = 2;
        } else if (ficha == 'N') {
            filasDestino[0] = r1 + 2; filasDestino[1] = r1 + 1; 
            numOpciones = 2;
        } else { 
            filasDestino[0] = r1 - 2; filasDestino[1] = r1 - 1;
            filasDestino[2] = r1 + 2; filasDestino[3] = r1 + 1;
            numOpciones = 4;
        }
        //Aque se verifican si los saltos se pueden marcar como validos 
        for (int i = 0; i < numOpciones; ++i) 
        {
            int testR = filasDestino[i];
            int testC = c1 + (abs(testR - r1) == 2 ? dC * 2 : dC);

            //Valida los limites del tablero
            if (testR >= 0 && testR < tamaño && testC >= 0 && testC < tamaño) 
            {
                int diffR = testR - r1;
                int diffC = testC - c1;
                //Verifica si es un paso o un salto para comer 
                bool esUnSalto = (abs(diffR) == 2 && abs(diffC) == 2);

                if (esUnSalto) 
                {
                    //Aqui se verifican las condiciones para comer, que una casilla este basia y que alla una ficha enemiga
                    //entonces se rompe el siclo y da prioridad a comer sibre un turno normal 
                    int medR = (r1 + testR) / 2;
                    int medC = (c1 + testC) / 2;
                    if (tablero[testR][testC] == ' ' && esOponente(ficha, tablero[medR][medC])) 
                    {
                        r2 = testR; c2 = testC;
                        esSaltoValido = true;
                        break; 
                    }
                } 
                //Aqui si es un salto nomral verifica que la casilla este vacia,
                else if (abs(diffR) == 1 && abs(diffC) == 1 && !esSaltoValido) 
                {
                    if (tablero[testR][testC] == ' ') 
                    {
                        r2 = testR; c2 = testC;
                        esPasoValido = true;
                    }
                }
            }
        }

        if (r2 == -1 || c2 == -1) {
            cout << "No hay ningun movimiento valido en esa direccion.\n";
            return false;
        }

        // Verificar regla de obligación de comer
        if (obligacion && !esSaltoValido) 
        {
            cout << "¡Hay obligacion de comer! Debes elegir una direccion donde haya un salto de captura.\n";
            return false;
        }

        // Movimiento simple en diagonal 
        if (esPasoValido && !obligacion) 
        {
            tablero[r2][c2] = ficha;
            tablero[r1][c1] = ' ';
            coronar(r2, c2);
            return true;
        }

        // Movimiento de salto (Comer) con Multi-salto consecutivo automático
        if (esSaltoValido) 
        {
            int currR = r1, currC = c1;
            int destinoR = r2, destinoC = c2;

            while (true) 
            {
                int medR = (currR + destinoR) / 2;
                int medC = (currC + destinoC) / 2;

                // Ejecutar captura
                tablero[destinoR][destinoC] = tablero[currR][currC];
                tablero[currR][currC] = ' ';
                tablero[medR][medC] = ' ';
                coronar(destinoR, destinoC);

                ficha = tablero[destinoR][destinoC]; 
                currR = destinoR;
                currC = destinoC;

                // Comprobar si puede seguir comiendo consecutivamente
                if (puedeComerDesde(currR, currC)) 
                {
                    mostrarTablero();
                    cout << "¡Tienes otro salto disponible con la misma ficha! Introduce la siguiente direccion ('I' o 'D'): ";
                    char siguienteDir;
                    cin >> siguienteDir;

                    siguienteDir = toupper(siguienteDir);
                    int nuevoDC = (siguienteDir == 'I') ? -1 : 1;
                    
                    bool encontroSiguiente = false;
                    int posiblesFilas[2] = {currR - 2, currR + 2};
                    for (int fIdx = 0; fIdx < 2; ++fIdx) 
                    {
                        int nxtR = posiblesFilas[fIdx];
                        int nxtC = currC + (nuevoDC * 2);
                        if (nxtR >= 0 && nxtR < tamaño && nxtC >= 0 && nxtC < tamaño) 
                        {
                            int mR = (currR + nxtR) / 2;
                            int mC = (currC + nxtC) / 2;
                            if (tablero[nxtR][nxtC] == ' ' && esOponente(ficha, tablero[mR][mC])) 
                            {
                                destinoR = nxtR;
                                destinoC = nxtC;
                                encontroSiguiente = true;
                                break;
                            }
                        }
                    }

                    if (!encontroSiguiente) {
                        cout << "No hay saltos válidos en esa dirección para continuar comiendo. Turno finalizado.\n";
                        break;
                    }
                } 
                else 
                {
                    break; // Termina cuando ya no puede comer mas
                }
            }
            return true;
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
            cout << "\n¡¡¡¡¡GANAN LA FICHAS NEGRAS POR ELIMINACION!!!!!!\n";
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

   //Mensaje de inicio
   cout << "============== JUEGO DE DAMAS (MODO DIRECCIONES) ===============\n";
   cout << "Ejemplo: A 5 I (Mueve la ficha de A5 hacia la izquierda)\n";
   cout << "Fichas Claras = B (Blancas), Fichas Oscuras = N (Negras). Damas = W y M.\n\n";

   //Inicio del programa principal 
   while (true) 
   {
        char col1, dir;
        int r1;
        
        //Instrucciones 
        juego.mostrarTablero();
        cout << "Turno actual: " << (juego.obtenerTurno() == 'B' ? "FICHAS BLANCAS [B]" : "FICHAS NEGRAS [N]") << "\n";
        cout << "Introduce origen y direccion (ej: A 5 D) o 'X' para salir: ";
        
        cin >> col1;
        if (col1 == 'X' || col1 == 'x') 
        {
            cout << "====== JUEGOS TERMINADO POR LOS JUGADORES ======\n";
            break;
        }

        cin >> r1 >> dir;

        // Convierte letras a índices numéricos (A-H -> 0-7)
        int c1 = toupper(col1) - 'A';
        
        //Verifica que ponagas cordenadas dentro del tablero para que no se salga 
        if (c1 < 0 || c1 >= tamaño || r1 < 0 || r1 >= tamaño) 
        {
            cout << "Coordenadas fuera de rango. Intenta de nuevo.\n";
            continue;
        }

        if (juego.realizarMovimientoPorDireccion(r1, c1, dir)) 
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