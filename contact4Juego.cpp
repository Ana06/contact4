/* Ana María Martínez Gómez CONTACT4-versión para jugar*/

#include <iostream>	 // consola de la libreria standard (STL)
using namespace std; // prefijo de la STL
#include <limits>
#include <math.h>
#include <ctime>		/* time */
#include <stdlib.h>    
#include <random>
#define MAX_INT std::numeric_limits<int>::max()

int profMax=8;
const int N = 6, M =7;

default_random_engine generator;
uniform_real_distribution<double> distribution(0,100);
typedef unsigned short int tTablero[N][M];
typedef unsigned int tColumnas[M];

char menu();
bool menuCortes();
void menuNivel();

void modoHumanoHumano();
void modoMaquinaHumano(int jugador);

void inicializarJuego(tTablero tablero, tColumnas cima);
void inicializarJugadas(tColumnas jugadaCol);
void pintarJuego(tTablero tablero);

void juegaHumano(tColumnas cima, tTablero tablero, int &jugador, bool &fin, int &contador);
void juegaMaquina(tColumnas cima, tColumnas jugadaCol, tTablero tablero, int &contador, int &jugador, bool &fin);
int jugada(tColumnas cima, tColumnas jugadaCol, tTablero tablero, int jugador, int puestas);

int alfabeta(int fila, int columna, int alfa, int beta, tColumnas cima, tColumnas jugadaCol, tTablero tablero, int jugador, int puestas, int profundidad);
int valorHeuristica(int fila, int columna, tTablero tablero, int jugador, int profundidad, tColumnas cima, int puestas);


bool analiza(int fila, int columna, tTablero tablero);
int contrario(int jugador);

int heuristica2(tColumnas cima, tTablero tablero, int puestas);
void evaluaCasillas(int &puntos, tTablero tablero, int i, int j);

int main(int argc, char *argv[]) {
	char i = menu();
	while (i != '0'){
		if (i == '1') modoHumanoHumano();
		else if (i == '2') {
			menuNivel();
			modoMaquinaHumano(1);
		}
		else if (i == '3'){
			menuNivel();
			modoMaquinaHumano(2);
		}
		else cout << "Opcion no valida";
		i = menu();
	}
	return 0;
}


char menu(){ //devuelve un char para evitar problemas si lo que se introduce no es un numero o un numero de dos digitos
	char i;
	cout << "\n\n\t0.-SALIR\n\t1.-Humano contra humano\n\t2.-Maquina contra humano (Empieza la maquina)\n\t3.-Maquina contra humano (Empieza usted)\n\nIntroduzca una opcion: ";
	cin >> i;
	cin.sync();
	return i;
} //Si se mete 2sgfg coje el 2 y descarta el resto

void menuNivel(){ //devuelve un char para evitar problemas si lo que se introduce no es un numero o un numero de dos digitos
	char i;
	bool seleccionado =false;
	while (!seleccionado){
		cout << "\n\n\t1.-Nivel muy facil\n\t2.-Nivel facil \n\t3.-Nivel medio\n\t4.-Nivel dificil\n\t5.-Nivel muy dificil (puede ir lento)\n\nIntroduzca una opcion: ";
		cin >> i;
		cin.sync();
		if(i=='1'){
			profMax=2;
			seleccionado=true;}
		else if(i=='2') {
			profMax=4;
			seleccionado=true;
		}
		else if(i=='3'){
			profMax=6;
			seleccionado=true;
		}
		else if(i=='4'){
			profMax=8;
			seleccionado=true;
		}
		else if(i=='5'){
			profMax=10;
			seleccionado=true;
		}
		else cout <<"\nNivel no válido";
	}
} //Si se mete 2sgfg coje el 2 y descarta el resto

void modoHumanoHumano(){
	tTablero tablero;
	tColumnas cima;
	inicializarJuego(tablero, cima);
	pintarJuego(tablero);
	bool fin = false;
	int jugador = 1;
	int contador = 0; //Cuenta las casillas con ficha
	while (!fin && contador < N*M){
		juegaHumano(cima, tablero, jugador, fin, contador);
	}
	if (fin) cout << "HA GANADO EL JUGADOR " << jugador;
	else cout << "EMPATE";
}

void modoMaquinaHumano(int jugador){//la máquina es el jugador1 
	tTablero tablero;
	tColumnas cima;
	inicializarJuego(tablero, cima);
	pintarJuego(tablero);
	int contador = 0; //Casillas llenas
	bool fin = false;
	tColumnas jugadaCol;
	inicializarJugadas(jugadaCol);
	while (!fin && contador < N*M){
		if (jugador == 1)//Juega la maquina
			juegaMaquina(cima, jugadaCol, tablero, contador, jugador, fin);
		else juegaHumano(cima, tablero, jugador, fin, contador);
	}
	if (fin && jugador == 2) cout << "HA GANADO";
	else if (fin)cout << "HA PERDIDO";
	else cout << "EMPATE";
}



void inicializarJuego(tTablero tablero, tColumnas cima){
	for (int j = 0; j < M; j++) {
		tablero[0][j] = 0;
		cima[j] = 0;
	}
	for (int i = 1; i< N; i++)
		for (int j = 0; j < M; j++)
			tablero[i][j] = 0;
}

void inicializarJugadas(tColumnas jugadaCol){
	//Queremos probar las posiciones del centro hacia afuera
	int i = M/2, j = 0, z =0;
	while(j<M){
		if(j%2 == 0){
			jugadaCol[j] = i + z;
			z++;
		}
		else
			jugadaCol[j] = i -z;
		j++;
	}

}

void pintarJuego(tTablero tablero){
	cout << endl;
	for (int i = N - 1; i >= 0; i--){
		for (int j = 0; j < M; j++){
			if (tablero[i][j] == 1) cout << "| X ";
			else if (tablero[i][j] == 2) cout << "| O ";
			else cout << "|   ";
		}
		cout << "|" << endl;
	}
	cout << endl;
}


void juegaHumano(tColumnas cima, tTablero tablero, int &jugador, bool &fin, int &contador){
	{
		int i;
		cout << "SU TURNO JUGADOR " << jugador << endl;
		cout << "Introduzca una columna[1-" << M << "]: ";
		cin >> i;
		i--;
		if (cima[i] <N){
			tablero[cima[i]][i] = jugador;
			pintarJuego(tablero);
			fin = analiza(cima[i], i, tablero);
			cima[i] ++;
			contador++;
			if (!fin)//Si se ha terminado no necesitamos saber a quien le toca
				jugador = contrario(jugador);
			}
		else cout << "Jugada no valida" << endl << endl;
		}
}

void juegaMaquina(tColumnas cima, tColumnas jugadaCol, tTablero tablero, int &contador, int &jugador, bool &fin){
	int i = jugada(cima, jugadaCol, tablero, jugador, contador);
	tablero[cima[i]][i] = jugador;
	pintarJuego(tablero);
	fin = analiza(cima[i], i, tablero);
	cima[i] ++;
	contador++;
	if (!fin)//Si se ha terminado no necesitamos saber a quien le toca
		jugador = contrario(jugador);
}

int jugada(tColumnas cima, tColumnas jugadaCol, tTablero tablero, int jugador, int puestas){

	//caso general, recursión
	int mejorCol; //colMejor nos da la mejor posicición de las evaluadas
	int aux; //guardar la valoración de la pos actual
	int mejorVal = -MAX_INT; //Mejor valoración si estamos max
	int peorVal = MAX_INT; //Mejor valoración si estamos min
	int contador = 0;
	int iguales =1;
	double valor;
	//elegimos una columna en la que se pueda poner
	while (cima[jugadaCol[contador]] == N)
		contador++;
	do{//mientras queden jugadas.
		int i = jugadaCol[contador];
		tablero[cima[i]][i] = jugador;//Hacemos siguiente jugada sobre el tablero copia
		cima[i]++;
		aux= alfabeta(cima[i]-1,  i, -MAX_INT, MAX_INT, cima, jugadaCol, tablero, jugador, puestas+1, 0);
		//DESHACEMOS CAMBIOS
		cima[i]--;
		tablero[cima[i]][i] = 0;
		if (jugador == 1){//Máximiza
			if (aux == 100) return i;
			if (aux > mejorVal){
				mejorVal = aux;
				mejorCol = i;
				iguales = 1;
			}
			else if (aux == mejorVal){
				iguales ++;
				valor =distribution(generator);
				if (valor < 100/iguales){
					mejorVal = aux;
					mejorCol = i; }
			}
		}
		else{//Minimiza
			if (aux == -100) return i;
			if (aux < peorVal){
				peorVal = aux;
				mejorCol = i;
				iguales=1;
			}
			else if (aux == peorVal){
				iguales ++;
				valor =distribution(generator);
				if (valor < 100/iguales){
					peorVal = aux;
					mejorCol = i; }
			}
		}
		contador++;
		//si queda alguna jugada que se pueda hacer la buscamos
		while (contador < M && cima[jugadaCol[contador]] == N) contador++;
	} while (contador< M);
	return mejorCol;
}

int alfabeta(int fila, int columna, int alfa, int beta, tColumnas cima, tColumnas jugadaCol, tTablero tablero, int jugador, int puestas, int profundidad){
	int heuristica = valorHeuristica(fila, columna, tablero, jugador, profundidad, cima, puestas);
	if (heuristica != 200){//Si hemos profundizado lo suficiente (200 es un valor que indica que no lo hemos hecho)
		return heuristica;
	}
	//Caso recursivo
	int aux; //guardar la valoración de la pos actual
	jugador = contrario(jugador);//Ahora va a jugar el otro jugador
	int contador = 0;
	//elegimos una columna en la que se pueda poner
	while (cima[jugadaCol[contador]] == N)
		contador++;
	do{//mientras queden jugadas.
		int i = jugadaCol[contador];
		tablero[cima[i]][i] = jugador;//Hacemos siguiente jugada sobre el tablero copia
		cima[i]++;
		aux= alfabeta(cima[i]-1,  i, alfa, beta, cima, jugadaCol, tablero, jugador, puestas + 1, profundidad+1);
		//DESHACEMOS CAMBIOS
		cima[i]--;
		tablero[cima[i]][i] = 0;
		if (jugador == 1){//Máximiza
			if (aux == 100){
				return 100;
			}
			if (aux > alfa) alfa = aux;
			if(beta <= alfa) return alfa;
		}
		else{//Minimiza
			if (aux == -100){
				return -100;
			}
			if (aux < beta) beta = aux;
			if(beta <= alfa) return beta;
		}
		contador++;
		//si queda alguna jugada que se pueda hacer la buscamos
		while (contador < M && cima[jugadaCol[contador]] == N) contador++;
	} while (contador< M);

	if (jugador == 1){
		return alfa;
	}
	else{
		return beta;
	}
}

int valorHeuristica(int fila, int columna, tTablero tablero, int jugador, int profundidad, tColumnas cima, int puestas){
	if (analiza(fila, columna, tablero)){
		if (jugador == 1)return 100;
		else return -100;
	}
	//EMPATE (nadie ha ganado y no quedan jugadas
	if (puestas == N*M) return 0;
	
		
	if(profundidad == profMax){
		return heuristica2(cima, tablero, puestas);
	}

	return 200;
}


int heuristica2(tColumnas cima, tTablero tablero, int puestas){
	int puntos =0;
	if(M%2 !=0){
		for(int j = 1; j < M-1; j++){
			int i=cima[j];
			if(i >=N-1) i=N-2;//Si esta llena hay que empezar a explorar uno más abajo
			for(; i >=1; i--){
				//casillas centrales
					if(i == (M-1)/2){
						if(tablero[i][j]==1) puntos=puntos+2;
						else if(tablero[i][j]==2) puntos=puntos-2;
					}
					else if(i== (M-1)/2 -1 || i== (M-1)/2 +1){
						if(tablero[i][j]==1) puntos++;
						else if(tablero[i][j]==2) puntos--;
					}
				//evaluación de todas las casillas
					evaluaCasillas(puntos, tablero, i, j);
			}
		}
	}
	else{
		for(int j = 1; j < M-1; j++){
			int i=cima[j];
			if(i >=N-1) i=N-2;//Si esta llena hay que empezar a explorar uno más abajo
			for(; i >=1; i--){
				//casillas centrales
					if(i == (M-1)/2 || i == (M-1)/2+1){
						if(tablero[i][j]==1) puntos=puntos+2;
						else if(tablero[i][j]==2) puntos=puntos-2;
					}
					else if(i== (M-1)/2 -1 || i== (M-1)/2 +2){
						if(tablero[i][j]==1) puntos++;
						else if(tablero[i][j]==2) puntos--;
					}
				//evaluación de todas las casillas
					evaluaCasillas(puntos, tablero, i, j);
			}
		}
	}
	int divisor=8*puestas + 4*N;
	return puntos*99/divisor;

}

void evaluaCasillas(int &puntos, tTablero tablero, int i, int j){
	if(tablero[i][j] == 1){
			if(tablero[i+1][j]==1)puntos=puntos+2;
			else if(tablero[i+1][j]==0)puntos ++;
			if(tablero[i+1][j+1]==1)puntos=puntos+2;
			else if(tablero[i+1][j+1]==0)puntos++;
			if(tablero[i+1][j-1]==1)puntos=puntos+2;
			else if(tablero[i+1][j-1]==0)puntos ++;
			if(tablero[i][j+1]==1)puntos=puntos+2;
			else if(tablero[i][j+1]==0)puntos++;
			if(tablero[i][j-1]==1)puntos=puntos+2;
			else if(tablero[i][j-1]==0)puntos++;
			if(tablero[i-1][j+1]==1)puntos=puntos+2;
			else if(tablero[i-1][j+1]==0)puntos++;
			if(tablero[i-1][j-1]==1)puntos=puntos+2;
			else if(tablero[i-1][j-1]==0)puntos ++;
			if(tablero[i-1][j]==1)puntos=puntos+2;
			else if(tablero[i-1][j]==0)puntos++;
	}
	else if(tablero[i][j] == 2){
			if(tablero[i+1][j]==2)puntos=puntos-2;
			else if(tablero[i+1][j]==0)puntos --;
			if(tablero[i+1][j+1]==2)puntos=puntos-2;
			else if(tablero[i+1][j+1]==0)puntos--;
			if(tablero[i+1][j-1]==2)puntos=puntos-2;
			else if(tablero[i+1][j-1]==0)puntos --;
			if(tablero[i][j+1]==2)puntos=puntos-2;
			else if(tablero[i][j+1]==0)puntos--;
			if(tablero[i][j-1]==2)puntos=puntos-2;
			else if(tablero[i][j-1]==0)puntos--;
			if(tablero[i-1][j+1]==2)puntos=puntos-2;
			else if(tablero[i-1][j+1]==0)puntos--;
			if(tablero[i-1][j-1]==2)puntos=puntos-2;
			else if(tablero[i-1][j-1]==0)puntos --;
			if(tablero[i-1][j]==2)puntos=puntos-2;
			else if(tablero[i-1][j]==0)puntos--;
	}
}


bool analiza(int fila, int columna, tTablero tablero){
	if (fila - 3 >= 0 && tablero[fila][columna] == tablero[fila - 1][columna] && tablero[fila][columna] == tablero[fila - 2][columna] && tablero[fila][columna] == tablero[fila - 3][columna])//4 cruces en la misma columna
		return true;
	if (columna - 3 >= 0 && tablero[fila][columna] == tablero[fila][columna - 1] && tablero[fila][columna] == tablero[fila][columna - 2] && tablero[fila][columna] == tablero[fila][columna - 3]) //4 cruces en la misma fila
		return true;
	if (columna - 2 >= 0 && columna + 1 < M && tablero[fila][columna] == tablero[fila][columna - 1] && tablero[fila][columna] == tablero[fila][columna - 2] && tablero[fila][columna] == tablero[fila][columna + 1]) //4 cruces en la misma fila
		return true;
	if (columna - 1 >= 0 && columna + 2 < M && tablero[fila][columna] == tablero[fila][columna - 1] && tablero[fila][columna] == tablero[fila][columna + 1] && tablero[fila][columna] == tablero[fila][columna + 2]) //4 cruces en la misma fila
		return true;
	if (columna + 3 < M && tablero[fila][columna] == tablero[fila][columna + 1] && tablero[fila][columna] == tablero[fila][columna + 2] && tablero[fila][columna] == tablero[fila][columna + 3])//4 cruces en la misma fila
		return true;
	if (columna - 3 >= 0 && fila + 3 <N && tablero[fila][columna] == tablero[fila + 1][columna - 1] && tablero[fila][columna] == tablero[fila + 2][columna - 2] && tablero[fila][columna] == tablero[fila + 3][columna - 3])//4 cruces en la misma diagonal hacia abajo
		return true;
	if (columna - 2 >= 0 && columna + 1 < M && fila + 2 <N && fila - 1 >= 0 && tablero[fila][columna] == tablero[fila + 1][columna - 1] && tablero[fila][columna] == tablero[fila + 2][columna - 2] && tablero[fila][columna] == tablero[fila - 1][columna + 1])//4 cruces en la misma diagonal hacia abajo
		return true;
	if (columna - 1 >= 0 && columna + 2 < M && fila + 1 <N && fila - 2 >= 0 && tablero[fila][columna] == tablero[fila + 1][columna - 1] && tablero[fila][columna] == tablero[fila - 1][columna + 1] && tablero[fila][columna] == tablero[fila - 2][columna + 2])//4 cruces en la misma diagonal hacia abajo
		return true;
	if (columna + 3 < M && fila - 3 >= 0 && tablero[fila][columna] == tablero[fila - 1][columna + 1] && tablero[fila][columna] == tablero[fila - 2][columna + 2] && tablero[fila][columna] == tablero[fila - 3][columna + 3])//4 cruces en la misma diagonal hacia abajo
		return true;
	if (columna + 3 <M && fila + 3 <N && tablero[fila][columna] == tablero[fila + 1][columna + 1] && tablero[fila][columna] == tablero[fila + 2][columna + 2] && tablero[fila][columna] == tablero[fila + 3][columna + 3])//4 cruces en la misma diagonal hacia arriba
		return true;
	if (columna + 2 <M && columna - 1 >= 0 && fila + 2 <N &&  fila - 1 >= 0 && tablero[fila][columna] == tablero[fila - 1][columna - 1] && tablero[fila][columna] == tablero[fila + 1][columna + 1] && tablero[fila][columna] == tablero[fila + 2][columna + 2])//4 cruces en la misma diagonal hacia arriba
		return true;
	if (columna + 1 <M && columna - 2 >= 0 && fila + 1 <N &&  fila - 2 >= 0 && tablero[fila][columna] == tablero[fila - 2][columna - 2] && tablero[fila][columna] == tablero[fila - 1][columna - 1] && tablero[fila][columna] == tablero[fila + 1][columna + 1])//4 cruces en la misma diagonal hacia arriba
		return true;
	if (columna - 3 >= 0 && fila - 3 >= 0 && tablero[fila][columna] == tablero[fila - 1][columna - 1] && tablero[fila][columna] == tablero[fila - 2][columna - 2] && tablero[fila][columna] == tablero[fila - 3][columna - 3])//4 cruces en la misma diagonal hacia arriba
		return true;
	return false;
}


int contrario(int jugador){
	if (jugador == 1)
		return 2;
	return 1;
}


