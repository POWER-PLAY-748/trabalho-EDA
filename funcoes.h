#include "dados.h"

// Funções para manipular antenas
Antena* CriarAntena(char freq, int x, int y);
Antena* InserirAntena(Antena* lista, char freq, int x, int y);
Antena* RemoverAntena(Antena* h, char freq, int x, int y, int*removida);
Antena* ListarAntenas(Antena* lista, EfeitoNefasto* efeitos);
bool DestruirListaAntenas(Antena* h);
bool SalvarAntenasEmFicheiro(const char* nomeFicheiro, Antena* lista);
EfeitoNefasto* CriarEfeito(int x, int y);
EfeitoNefasto* InserirEfeito(EfeitoNefasto* inicio, int x, int y);
EfeitoNefasto* efeitoNefasto(Antena* inicio);
bool DestruirListaEfeitos(EfeitoNefasto* lista); 
