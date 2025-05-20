#include "dados.h"


// --- Antenas ---
Antena* CriarAntena(char freq, int x, int y);
Antena* InserirAntena(Antena* inicio, char freq, int x, int y);
Antena* RemoverAntena(Antena* h, char freq, int x, int y, int* removida);
Antena* ListarAntenas(Antena* lista, EfeitoNefasto* efeitos);
bool SalvarAntenasEmTxt(Antena* lista, const char* nomeFicheiro);
Antena* CarregarAntenasDeTxt(const char* nomeFicheiro);
bool SalvarAntenasEmFicheiroBin(Antena* lista);
Antena* CarregarAntenasDeBin(const char* nomeFicheiro);
bool DestruirListaAntenas(Antena* h);

// --- Efeitos Nefastos ---
EfeitoNefasto* CriarEfeito(int x, int y);
EfeitoNefasto* InserirEfeito(EfeitoNefasto* inicio, int x, int y);
EfeitoNefasto* efeitoNefasto(Antena* inicio);
EfeitoNefasto* efeitoNefastoVertices(Vertice* inicio);
bool DestruirListaEfeitos(EfeitoNefasto* lista);

// --- Grafo ---
GR* CriarGrafo();
Vertice* CriarVertice(int id, char freq, int x, int y);
Aresta* CriarAresta(int destino);
bool InserirVertice(GR* g, int id, char freq, int x, int y);
Vertice* EncontrarVerticePorId(GR* g, int id);
void MostrarVertices(GR* g);
bool InserirAresta(GR* g, Vertice* novo);
bool RemoverArestas(GR* g, int id);
bool RemoverVertice(GR* g, int id);
EfeitoNefasto* efeitoNefastoVertices(Vertice* inicio);
bool SalvarGrafoEmTxt(Vertice* lista, const char* nomeFicheiro);
Vertice* CarregarGrafoDeTxt(const char* nomeFicheiro);
bool SalvarGrafoEmBin(Vertice* lista, const char* nomeFicheiro);
Vertice* CarregarGrafoDeBin(const char* nomeFicheiro);
bool MostrarGrid(Vertice* listaVertices);
void DFS_Recursivo(GR* g, Vertice* v, bool* visitado);
void ProcuraProfundidade(GR* g, int idOrigem);
void ProcuraLargura(GR* g, int idOrigem);
bool DestruirGrafo(GR* g);
