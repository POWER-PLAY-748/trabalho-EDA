/*****************************************************************//**
 * \file   funcoes.c
 * \brief  Implementa��o das fun��es para manipula��o de antenas e efeitos nefastos.
 *
 * \author matos
 * \date   March 2025
 *********************************************************************/

#include "dados.h"
#define GRID_TAM 10
#define CRT_SECURE_NO_WARNINGS

#pragma region Antenas
 /**
  * \brief Cria uma nova antena.
  *
  * \param freq Frequ�ncia da antena.
  * \param x Coordenada x da antena.
  * \param y Coordenada y da antena.
  * \return Ponteiro para a nova antena criada.
  */
Antena* CriarAntena(char freq, int x, int y) {
    Antena* aux = (Antena*)malloc(sizeof(Antena)); // Aloca mem�ria para uma nova antena
    if (aux != NULL) {
        aux->freq = freq; // Define a frequ�ncia da antena
        aux->x = x; // Define a coordenada x da antena
        aux->y = y; // Define a coordenada y da antena
        aux->prox = NULL; // Define o pr�ximo ponteiro como NULL
    }
    return aux; // Retorna o ponteiro para a nova antena
}


/**
 * \brief Insere uma antena na lista de forma ordenada.
 *
 * \param inicio Ponteiro para o in�cio da lista de antenas.
 * \param aux Ponteiro para a antena a ser inserida.
 * \return Ponteiro para o in�cio da lista atualizada.
 */
Antena* InserirAntena(Antena* inicio, char freq, int x, int y) {
    // Criar a nova antena

    if (x < 0 || x >= GRID_TAM || y < 0 || y >= GRID_TAM) {
        return NULL; // N�o insere
    }

    Antena* aux = CriarAntena(freq, x, y);
    if (aux == NULL) {
        return inicio; // Verifica se a antena foi criada com sucesso
    }
    // Verificar se j� existe uma antena nas mesmas coordenadas
    Antena* atual = inicio;
    while (atual != NULL) {
        if (atual->x == x && atual->y == y) { // Verifica se j� existe uma antena nas mesmas coordenadas
            free(aux); // Liberar a mem�ria da nova antena
            return inicio; // Retorna o in�cio da lista sem altera��es
        }
        atual = atual->prox; // Avan�a para a pr�xima antena na lista
    }

    // Inserir a antena no in�cio da lista
    aux->prox = inicio; // Define o pr�ximo ponteiro da nova antena como o in�cio da lista
    return aux; // Retorna a nova antena como o in�cio da lista
}

/**
 * \brief Remove uma antena da lista.
 *
 * \param h Ponteiro para o in�cio da lista de antenas.
 * \param freq Frequ�ncia da antena a ser removida.
 * \param x Coordenada x da antena a ser removida.
 * \param y Coordenada y da antena a ser removida.
 * \param removida Ponteiro para indicar se a remo��o foi bem-sucedida.
 * \return Ponteiro para o in�cio da lista atualizada.
 */

Antena* RemoverAntena(Antena* h, char freq, int x, int y, int* removida) {
    Antena* aux = h;
    Antena* anterior = NULL;
    *removida = 0;
    while (aux != NULL) {
        if (aux->freq == freq && aux->x == x && aux->y == y) {
			if (anterior == NULL) // Se a antena a ser removida � a primeira da lista
                h = aux->prox;
            else
                anterior->prox = aux->prox;
            free(aux);
            *removida = 1;
			return h; // Retorna o in�cio da lista atualizada
        }
		anterior = aux; // Avan�a para a pr�xima antena na lista
        aux = aux->prox;
    }
	return h; // Retorna o in�cio da lista sem altera��es
}

/**
 * \brief Salva a lista de antenas em um ficheiro de texto.
 *
 * \param lista Ponteiro para o in�cio da lista de antenas.
 * \param nomeFicheiro Nome do ficheiro onde as antenas ser�o salvas.
 * \return true se o salvamento foi bem-sucedido, false caso contr�rio.
 */

bool SalvarAntenasEmTxt(Antena* lista, const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return false;
    }
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.';
            Antena* auxAntena = lista;
			while (auxAntena != NULL) { // Percorre a lista de antenas
				
				if (auxAntena->x == j && auxAntena->y == i) { // Verifica se a antena est� na posi��o atual
                    simbolo = auxAntena->freq;
                    break;
                }
				auxAntena = auxAntena->prox; // Avan�a para a pr�xima antena na lista
            }
            fprintf(ficheiro, "%c ", simbolo);
        }
        fprintf(ficheiro, "\n");
    }
    fclose(ficheiro);
    return true;
}
/**
 * \brief Carrega as antenas de um ficheiro de texto.
 *
 * \param nomeFicheiro Nome do ficheiro de texto a ser carregado.
 * \return Ponteiro para o in�cio da lista de antenas ou NULL se ocorrer um erro.
 */
Antena* CarregarAntenasDeTxt(const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return NULL; 
    }
    Antena* lista = NULL;
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo; 
			fscanf(ficheiro, " %c", &simbolo); // L� o s�mbolo do ficheiro
            if (simbolo != '.') { 
				lista = InserirAntena(lista, simbolo, j, i); // Insere a antena na lista 
            }
        }
    }
    fclose(ficheiro);
    return lista;
}

/**
 * \brief Lista as antenas e os efeitos nefastos no grid.
 *
 * \param lista Ponteiro para o in�cio da lista de antenas.
 * \param efeitos Ponteiro para o in�cio da lista de efeitos nefastos.
 * \return Ponteiro para o in�cio da lista de antenas.
 */

Antena* ListarAntenas(Antena* lista, EfeitoNefasto* efeitos) {
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.';
            //Procura antena
            Antena* auxAntena = lista;
            while (auxAntena != NULL) {
                if (auxAntena->x == j && auxAntena->y == i) {
                    simbolo = auxAntena->freq;
                    break;
                }
                auxAntena = auxAntena->prox;
            }
            // Se n�o encontrou antena, procura efeito nefasto
            if (simbolo == '.') {
                EfeitoNefasto* auxEfeito = efeitos;
                while (auxEfeito != NULL) {
                    if (auxEfeito->x == j && auxEfeito->y == i) {
                        simbolo = '#';
                        break;
                    }
                    auxEfeito = auxEfeito->prox;
                }
            }
            printf("%c ", simbolo);
        }
        printf("\n");
    }
    return lista;
}

/**
 * \brief Salva a lista de antenas em um ficheiro bin�rio.
 *
 * \param lista Ponteiro para o in�cio da lista de antenas.
 * \return true se o salvamento foi bem-sucedido, false caso contr�rio.
 */

bool SalvarAntenasEmFicheiroBin(Antena* lista) {
    FILE* ficheiro = fopen("antenas.bin", "wb");
    if (ficheiro == NULL) {
        return false;
    }

    Antena* aux = lista;
	while (aux != NULL) { // Percorre a lista de antenas
        fwrite(&(aux->freq), sizeof(char), 1, ficheiro);
        fwrite(&(aux->x), sizeof(int), 1, ficheiro);
        fwrite(&(aux->y), sizeof(int), 1, ficheiro);
        aux = aux->prox;
    }

    fclose(ficheiro);
    return true;
}
/**
 * \brief Carrega as antenas de um ficheiro bin�rio.
 *
 * \param nomeFicheiro Nome do ficheiro bin�rio a ser carregado.
 * \return Ponteiro para o in�cio da lista de antenas ou NULL se ocorrer um erro.
 */
Antena* CarregarAntenasDeBin(const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "rb");
    if (ficheiro == NULL) {
        return NULL;
    }
    Antena* lista = NULL;
    char freq;
    int x, y;
    while (fread(&freq, sizeof(char), 1, ficheiro) == 1 &&
        fread(&x, sizeof(int), 1, ficheiro) == 1 &&
        fread(&y, sizeof(int), 1, ficheiro) == 1) {
        lista = InserirAntena(lista, freq, x, y);
    }
    fclose(ficheiro);
    return lista;
}

/**
 * \brief Destr�i a lista de antenas, liberando a mem�ria alocada.
 *
 * \param h Ponteiro para o in�cio da lista de antenas.
 */
bool DestruirListaAntenas(Antena* h) {
    if (h == NULL) {
        return false; // Lista j� estava vazia
    }

    while (h != NULL) {
        Antena* aux = h; // Salva o ponteiro para a antena a ser destru�da
        h = h->prox;     // Avan�a para a pr�xima antena na lista
        free(aux);       // Libera a mem�ria da antena destru�da
    }

    return true; // Lista destru�da com sucesso
}
#pragma endregion

#pragma region Efeitos Nefastos
/**
 * \brief Cria um novo efeito nefasto.
 *
 * \param x Coordenada x do efeito nefasto.
 * \param y Coordenada y do efeito nefasto.
 * \return Ponteiro para o novo efeito nefasto criado.
 */
EfeitoNefasto* CriarEfeito(int x, int y) {
    EfeitoNefasto* aux = (EfeitoNefasto*)malloc(sizeof(EfeitoNefasto));// Aloca mem�ria para um novo efeito nefasto
    if (aux != NULL) {
        aux->x = x; // Define a coordenada x do efeito nefasto
        aux->y = y; // Define a coordenada y do efeito nefasto
        aux->prox = NULL; // Define o pr�ximo ponteiro como NULL
    }
    return aux; // Retorna o ponteiro para o novo efeito nefasto
}

/**
 * \brief Insere um efeito nefasto na lista.
 *
 * \param inicio Ponteiro para o in�cio da lista de efeitos nefastos.
 * \param x Coordenada x do efeito nefasto.
 * \param y Coordenada y do efeito nefasto.
 * \return Ponteiro para o in�cio da lista atualizada.
 */
EfeitoNefasto* InserirEfeito(EfeitoNefasto* inicio, int x, int y) {
    EfeitoNefasto* aux = CriarEfeito(x, y); // Cria um novo efeito nefasto
    if (aux == NULL) return inicio; // Verifica se o efeito nefasto foi criado com sucesso
    aux->prox = inicio; // Define o pr�ximo ponteiro do novo efeito nefasto como o in�cio da lista
    return aux; // Retorna o novo efeito nefasto como o in�cio da lista
}

/**
 * \brief Verifica se existem antenas com a mesma frequ�ncia e calcula os efeitos nefastos.
 *
 * \param inicio Ponteiro para o in�cio da lista de antenas.
 * \return Ponteiro para o in�cio da lista de efeitos nefastos.
 */
EfeitoNefasto* efeitoNefasto(Antena* inicio) {
    if (inicio == NULL) {
        return NULL; // Retorna NULL se a lista de antenas estiver vazia
    }

    EfeitoNefasto* efeitos = NULL; // Inicializa a lista de efeitos nefastos como NULL
    EfeitoNefasto* aux;
    Antena* current = inicio;
    Antena* compare;

    while (current != NULL) {
        compare = current->prox;
        while (compare != NULL) {
            if (current->freq == compare->freq) { // Verifica se as antenas t�m a mesma frequ�ncia
                if ((current->x == compare->x) && (current->y != compare->y)) { // Verifica se as antenas est�o na mesma coordenada x e em coordenadas y diferentes
                    int subtracao = current->y - compare->y; // Calcula a diferen�a entre as coordenadas y
                    int nefy = current->y + subtracao; // Calcula a coordenada y do efeito nefasto
                    int nefy1 = compare->y - subtracao; 
                    aux = CriarEfeito(current->x, nefy);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                    aux = CriarEfeito(compare->x, nefy1); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                }
                else if ((current->x != compare->x) && (current->y == compare->y)) { // Verifica se as antenas est�o na mesma coordenada y e em coordenadas x diferentes
                    int subtracao = current->x - compare->x; // Calcula a diferen�a entre as coordenadas x
                    int nefx = current->x + subtracao; // Calcula a coordenada x do efeito nefasto
                    int nefx1 = compare->x - subtracao; 
                    aux = CriarEfeito(nefx, current->y); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                    aux = CriarEfeito(nefx1, compare->y); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                }
                else if ((current->x != compare->x) && (current->y != compare->y)) { // Verifica se as antenas est�o em coordenadas x e y diferentes
                    int subtracaox = current->x - compare->x; // Calcula a diferen�a entre as coordenadas x
                    int subtracaoy = current->y - compare->y; // Calcula a diferen�a entre as coordenadas y
                    int nefx = current->x + subtracaox; // Calcula a coordenada x do efeito nefasto
                    int nefx1 = compare->x - subtracaox; 
                    int nefy = current->y + subtracaoy; // Calcula a coordenada y do efeito nefasto
                    int nefy1 = compare->y - subtracaoy; 
                    aux = CriarEfeito(nefx, nefy); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                    aux = CriarEfeito(nefx1, nefy1); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                }
            }
            compare = compare->prox; // Avan�a para a pr�xima antena na lista
        }
        current = current->prox; // Avan�a para a pr�xima antena na lista
    }

    return efeitos; // Retorna a lista de efeitos nefastos
}


/**
 * \brief Destr�i a lista de efeitos nefastos, liberando a mem�ria alocada.
 *
 * \param lista Ponteiro para o in�cio da lista de efeitos nefastos.
 */
bool DestruirListaEfeitos(EfeitoNefasto* lista) {
    if (lista == NULL) {
        return false; // Lista j� estava vazia
    }

    while (lista != NULL) {
        EfeitoNefasto* aux = lista; // Salva o ponteiro para o efeito nefasto a ser destru�do
        lista = lista->prox;       // Avan�a para o pr�ximo efeito nefasto na lista
        free(aux);                 // Libera a mem�ria do efeito nefasto destru�do
    }

    return true; // Lista destru�da com sucesso
}
#pragma endregion

#pragma region Grafo

/**
 * \brief Cria um grafo.
 *
 * \return Ponteiro para o grafo criado.
 */
GR* CriarGrafo() {
    GR* g = (GR*)malloc(sizeof(GR));
    if (g != NULL) {
        g->numVertices = 0;
        g->inicio = NULL;
    }
    return g;
}

/**
 * \brief Cria um novo v�rtice.
 *
 * \param id ID do v�rtice.
 * \param freq Frequ�ncia do v�rtice.
 * \param x Coordenada x do v�rtice.
 * \param y Coordenada y do v�rtice.
 * \return Ponteiro para o novo v�rtice criado.
 */
Vertice* CriarVertice(int id, char freq, int x, int y) {
    Vertice* v = (Vertice*)malloc(sizeof(Vertice));
	if (v != NULL) { // Verifica se a mem�ria foi alocada com sucesso
        v->id = id;
        v->freq = freq;
        v->x = x;
        v->y = y;
        v->adjacentes = NULL;
        v->prox = NULL;
    }
    return v;
}
/**
 * \brief Cria uma nova aresta.
 *
 * \param destino ID do v�rtice de destino da aresta.
 * \return Ponteiro para a nova aresta criada.
 */
Aresta* CriarAresta(int destino) {
    Aresta* a = (Aresta*)malloc(sizeof(Aresta));
	if (a != NULL) {   // Verifica se a mem�ria foi alocada com sucesso 
        a->destino = destino;
        a->prox = NULL;
    }
    return a;
}
/**
 * \brief Insere um v�rtice no grafo.
 *
 * \param g Ponteiro para o grafo.
 * \param novo Ponteiro para o novo v�rtice a ser inserido.
 */


bool InserirVertice(GR* g, int id, char freq, int x, int y) {
    if (g == NULL) return false;
    if (x < 0 || x >= GRID_TAM || y < 0 || y >= GRID_TAM) return false;

    // Verifica duplicados
    Vertice* v = g->inicio;
    while (v != NULL) {
        if (v->x == x && v->y == y) {
            return false; // J� existe v�rtice nessas coordenadas
        }
        v = v->prox;
    }

    // Cria o novo v�rtice
    Vertice* novo = CriarVertice(id, freq, x, y);
    if (novo == NULL) return false;

    // Insere no in�cio da lista
    novo->prox = g->inicio;
    g->inicio = novo;
    g->numVertices++;
    return true;
}
/**
 * \brief Encontra um v�rtice no grafo pelo ID.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do v�rtice a ser encontrado.
 * \return Ponteiro para o v�rtice encontrado ou NULL se n�o encontrado.
 */
Vertice* EncontrarVerticePorId(GR* g, int id) {
    Vertice* v = g->inicio;
    while (v != NULL) {
        if (v->id == id) {
            return v;
        }
        v = v->prox;
    }
    return NULL;
}
/**
 * \brief Mostra os v�rtices do grafo.
 *
 * \param g Ponteiro para o grafo.
 */
void MostrarVertices(GR* g) {
    if (g == NULL || g->inicio == NULL) {
        printf("Grafo vazio.\n");
        return;
    }
    Vertice* v = g->inicio;
    while (v != NULL) {
        printf("Vertice %d (Antena %c em [%d,%d]) -> Adjacentes:", v->id, v->freq, v->x, v->y);
        Aresta* a = v->adjacentes;
        while (a != NULL) {
            printf(" %d", a->destino);
            a = a->prox;
        }
        printf("\n");
        v = v->prox;
    }
}

/**
 * \brief Insere arestas entre o novo v�rtice e os outros v�rtices com a mesma frequ�ncia.
 *
 * \param g Ponteiro para o grafo.
 * \param novo Ponteiro para o novo v�rtice a ser inserido.
 * \return true se as arestas foram inseridas com sucesso, false caso contr�rio.
 */
bool InserirAresta(GR* g, Vertice* novo) {
    if (g == NULL || novo == NULL) return false;
    Vertice* v = g->inicio;
    while (v != NULL) {
        if (v != novo && v->freq == novo->freq) {
            // Verifica se j� existe a liga��o
            Aresta* a = novo->adjacentes;
            while (a != NULL) {
                if (a->destino == v->id) {
                    break; // J� existe, n�o cria
                }
                a = a->prox;
            }
            if (a == NULL) {
                // Liga nos dois sentidos (grafo n�o orientado)
                Aresta* a1 = CriarAresta(v->id);
                if (a1 != NULL) {
                    a1->prox = novo->adjacentes;
                    novo->adjacentes = a1;
                }
                Aresta* a2 = CriarAresta(novo->id);
                if (a2 != NULL) {
                    a2->prox = v->adjacentes;
                    v->adjacentes = a2;
                }
                return true; // Liga��o criada
            }
        }
        v = v->prox;
    }
    return false; // Nenhuma liga��o criada
}

/**
 * \brief Remove arestas do v�rtice com o ID especificado.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do v�rtice cujas arestas ser�o removidas.
 * \return true se as arestas foram removidas com sucesso, false caso contr�rio.
 */
bool RemoverArestas(GR* g, int id) {
    if (g == NULL){
        return false;
	}
    Vertice* v = g->inicio;
    // Remove arestas de outros v�rtices que apontam para 'id'
    while (v != NULL) {
        if (v->id != id) {
            Aresta* a = v->adjacentes;
            Aresta* ant = NULL;
            while (a != NULL) {
                if (a->destino == id) {
                    if (ant != NULL) {
                        ant->prox = a->prox;
                    }
                    else {
                        v->adjacentes = a->prox;
                    }
                    free(a);
                    return true; // Removeu uma aresta, retorna imediatamente
                }
                ant = a;
                a = a->prox;
            }
        }
        v = v->prox;
    }
    // Remove todas as arestas do pr�prio v�rtice
    Vertice* alvo = g->inicio;
    while (alvo != NULL && alvo->id != id) {
        alvo = alvo->prox;
    }
    if (alvo != NULL && alvo->adjacentes != NULL) {
        Aresta* a = alvo->adjacentes;
        while (a != NULL) {
            Aresta* temp = a;
            a = a->prox;
            free(temp);
            return true; // Removeu pelo menos uma aresta do pr�prio v�rtice
        }
        alvo->adjacentes = NULL;
    }
    return false; // N�o removeu nenhuma aresta
}

/**
 * \brief Remove um v�rtice do grafo.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do v�rtice a ser removido.
 * \return true se o v�rtice foi removido com sucesso, false caso contr�rio.
 */
bool RemoverVertice(GR* g, int id) {
    if (g == NULL || g->inicio == NULL) {
        return false;
	}
    RemoverArestas(g, id);
	Vertice* atual = g->inicio; // Ponteiro para percorrer a lista de v�rtices
    Vertice* anterior = NULL;
    while (atual != NULL && atual->id != id) {
		anterior = atual; // Salva o ponteiro do v�rtice anterior
        atual = atual->prox;
    }
    if (atual == NULL) {
        return false;
	}
    if (anterior != NULL) {
        anterior->prox = atual->prox;
    }
    else {
		g->inicio = atual->prox; // Se o v�rtice a ser removido � o primeiro
    }
    free(atual);
    g->numVertices--;
    return true;
}


/**
 * \brief Verifica se existem antenas com a mesma frequ�ncia e calcula os efeitos nefastos.
 *
 * \param inicio Ponteiro para o in�cio da lista de v�rtices.
 * \return Ponteiro para o in�cio da lista de efeitos nefastos.
 */

EfeitoNefasto* efeitoNefastoVertices(Vertice* inicio) {
    if (inicio == NULL) {
        return NULL;
    }

    EfeitoNefasto* efeitos = NULL;
    EfeitoNefasto* aux;
    Vertice* current = inicio;
    Vertice* compare;

    while (current != NULL) {
        compare = current->prox;
        while (compare != NULL) {
            if (current->freq == compare->freq) {
                if ((current->x == compare->x) && (current->y != compare->y)) {
                    int subtracao = current->y - compare->y;
                    int nefy = current->y + subtracao;
                    int nefy1 = compare->y - subtracao;
                    aux = CriarEfeito(current->x, nefy);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                    aux = CriarEfeito(compare->x, nefy1);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                }
                else if ((current->x != compare->x) && (current->y == compare->y)) {
                    int subtracao = current->x - compare->x;
                    int nefx = current->x + subtracao;
                    int nefx1 = compare->x - subtracao;
                    aux = CriarEfeito(nefx, current->y);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                    aux = CriarEfeito(nefx1, compare->y);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                }
                else if ((current->x != compare->x) && (current->y != compare->y)) {
                    int subtracaox = current->x - compare->x;
                    int subtracaoy = current->y - compare->y;
                    int nefx = current->x + subtracaox;
                    int nefx1 = compare->x - subtracaox;
                    int nefy = current->y + subtracaoy;
                    int nefy1 = compare->y - subtracaoy;
                    aux = CriarEfeito(nefx, nefy);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                    aux = CriarEfeito(nefx1, nefy1);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y);
                }
            }
            compare = compare->prox;
        }
        current = current->prox;
    }

    return efeitos;
}

/**
 * \brief Salva o grafo em um ficheiro de texto.
 *
 * \param lista Ponteiro para a lista de v�rtices.
 * \param nomeFicheiro Nome do ficheiro onde o grafo ser� salvo.
 * \return true se o salvamento foi bem-sucedido, false caso contr�rio.
 */

bool SalvarGrafoEmTxt(Vertice* lista, const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return false;
    }
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.'; 
			Vertice* aux = lista; // Inicializa o ponteiro auxiliar para percorrer a lista de v�rtices
            while (aux != NULL) { 
                if (aux->x == j && aux->y == i) { 
                    simbolo = aux->freq; 
					break;  // Se encontrou o v�rtice, armazena a frequ�ncia
                }
                aux = aux->prox;
            }
            fprintf(ficheiro, "%c ", simbolo);
        }
        fprintf(ficheiro, "\n");
    }
    fclose(ficheiro);
    return true;
}

/**
 * \brief Carrega o grafo de um ficheiro de texto.
 *
 * \param nomeFicheiro Nome do ficheiro de texto a ser carregado.
 * \return Ponteiro para a lista de v�rtices ou NULL se ocorrer um erro.
 */

Vertice* CarregarGrafoDeTxt(const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        return NULL;
    }
    Vertice* lista = NULL;
    int id = 1;
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo;
			fscanf(ficheiro, " %c", &simbolo); // L� o s�mbolo do ficheiro
            if (simbolo != '.') {
				Vertice* novo = CriarVertice(id++, simbolo, j, i); // Cria um novo v�rtice
                novo->prox = lista; 
                lista = novo;
            }
        }
    }
    fclose(ficheiro);
    return lista;
}

/**
 * \brief Salva o grafo em um ficheiro bin�rio.
 *
 * \param lista Ponteiro para a lista de v�rtices.
 * \param nomeFicheiro Nome do ficheiro onde o grafo ser� salvo.
 * \return true se o salvamento foi bem-sucedido, false caso contr�rio.
 */

bool SalvarGrafoEmBin(Vertice* lista, const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "wb");
    if (ficheiro == NULL) {
        return false;
    }
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.';
            Vertice* aux = lista;
			while (aux != NULL) { // Percorre a lista de v�rtices
                if (aux->x == j && aux->y == i) {
                    simbolo = aux->freq;
                    break;
                }
				aux = aux->prox; // Avan�a para o pr�ximo v�rtice na lista
            }
            fwrite(&simbolo, sizeof(char), 1, ficheiro);
        }
    }
    fclose(ficheiro);
    return true;
}

/**
 * \brief Carrega o grafo de um ficheiro bin�rio.
 *
 * \param nomeFicheiro Nome do ficheiro bin�rio a ser carregado.
 * \return Ponteiro para a lista de v�rtices ou NULL se ocorrer um erro.
 */

Vertice* CarregarGrafoDeBin(const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "rb");
    if (ficheiro == NULL) {
        return NULL;
    }
    Vertice* lista = NULL;
    int id = 1;
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < 10; j++) {
            char simbolo;
            if (fread(&simbolo, sizeof(char), 1, ficheiro) == 1) {
                if (simbolo != '.') {
                    Vertice* novo = CriarVertice(id++, simbolo, j, i);
                    novo->prox = lista;
                    lista = novo;
                }
            }
        }
    }
    fclose(ficheiro);
    return lista;
}

/**
 * \brief Mostra o grid com as antenas e os efeitos nefastos.
 *
 * \param listaVertices Ponteiro para a lista de v�rtices.
 * \return true se a exibi��o foi bem-sucedida, false caso contr�rio.
 */

bool MostrarGrid(Vertice* listaVertices) {
    if (listaVertices == NULL) {
        printf("Grafo vazio.\n");
        return false;
    }

    // Calcula os efeitos nefastos a partir dos v�rtices
    EfeitoNefasto* efeitos = efeitoNefastoVertices(listaVertices);

    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.';

            // Procura v�rtice
            Vertice* aux = listaVertices;
            while (aux != NULL) {
                if (aux->x == j && aux->y == i) {
                    simbolo = aux->freq;
                    break;
                }
                aux = aux->prox;
            }

            // Se n�o encontrou v�rtice, procura efeito nefasto
            if (simbolo == '.') {
                EfeitoNefasto* auxEfeito = efeitos;
                while (auxEfeito != NULL) {
                    if (auxEfeito->x == j && auxEfeito->y == i) {
                        simbolo = '#';
                        break;
                    }
                    auxEfeito = auxEfeito->prox;
                }
            }

            printf("%c ", simbolo);
        }
        printf("\n");
    }

    DestruirListaEfeitos(efeitos);
    return true;
}

/**
 * \brief Realiza uma busca em profundidade recursiva no grafo.
 *
 * \param g Ponteiro para o grafo.
 * \param v Ponteiro para o v�rtice atual.
 * \param visitado Array de booleanos para marcar os v�rtices visitados.
 */
void DFS_Recursivo(GR* g, Vertice* v, bool* visitado) {
    if (v == NULL) {
        return;
    }

    visitado[v->id] = true;
    printf("Antena ID %d\n", v->id);

    Aresta* a = v->adjacentes;

    while (a != NULL) {
        Vertice* vizinho = EncontrarVerticePorId(g, a->destino);

        if (vizinho != NULL && !visitado[vizinho->id]) {
            DFS_Recursivo(g, vizinho, visitado);
        }

        a = a->prox;
    }
}
/**
 * \brief Realiza uma busca em profundidade no grafo a partir de um v�rtice de origem.
 *
 * \param g Ponteiro para o grafo.
 * \param idOrigem ID do v�rtice de origem.
 */
void ProcuraProfundidade(GR* g, int idOrigem) {
    if (g == NULL) {
        return;
    }

    int maxId = 0;
    Vertice* v = g->inicio; 

    while (v != NULL) {
        if (v->id > maxId) {
            maxId = v->id;
        }
        v = v->prox;
    }

	bool* visitado = (bool*)calloc(maxId + 1, sizeof(bool)); // Aloca vetor de visitados

    Vertice* origem = EncontrarVerticePorId(g, idOrigem); 

    if (origem != NULL) {
        printf("DFS a partir da antena [%d,%d]:\n", origem->x, origem->y);
        DFS_Recursivo(g, origem, visitado);
    }
    else {
        printf("Antena de origem n�o encontrada.\n");
    }

    free(visitado);
}

/**
 * \brief Realiza uma busca em largura no grafo a partir de um v�rtice de origem.
 *
 * \param g Ponteiro para o grafo.
 * \param idOrigem ID do v�rtice de origem.
 */


void ProcuraLargura(GR* g, int idOrigem)
{
    if (g == NULL)
    {
        return;
    }

    // Descobre o maior id para alocar o vetor de visitados
    int maxId = 0;
    Vertice* v = g->inicio;
    while (v != NULL)
    {
        if (v->id > maxId)
        {
            maxId = v->id;
        }
        v = v->prox;
    }

    bool* visitado = (bool*)calloc(maxId + 1, sizeof(bool));
    Vertice* origem = EncontrarVerticePorId(g, idOrigem);
    if (origem == NULL)
    {
        printf("Antena de origem n�o encontrada.\n");
        free(visitado);
        return;
    }

    printf("BFS a partir da antena [%d,%d]:\n", origem->x, origem->y);

    // Fila simples usando array
    int* fila = (int*)malloc((maxId + 1) * sizeof(int));
    int inicio = 0;
    int fim = 0;
    fila[fim++] = origem->id;
    visitado[origem->id] = true;

    while (inicio < fim)
    {
        int idAtual = fila[inicio++];
        Vertice* atual = EncontrarVerticePorId(g, idAtual);
        if (atual != NULL)
        {
            printf("Antena ID %d\n", atual->id);
        }

        Aresta* a = atual->adjacentes;
        while (a != NULL)
        {
            if (visitado[a->destino] == false)
            {
                fila[fim++] = a->destino;
                visitado[a->destino] = true;
            }
            a = a->prox;
        }
    }

    free(fila);
    free(visitado);
}

/**
 * \brief Destr�i o grafo, liberando a mem�ria alocada.
 *
 * \param g Ponteiro para o grafo a ser destru�do.
 * \return true se o grafo foi destru�do com sucesso, false caso contr�rio.
 */

bool DestruirGrafo(GR* g) {
    if (g != NULL) {
        Vertice* v = g->inicio;
        while (v != NULL) {
            Aresta* a = v->adjacentes;
            while (a != NULL) {
                Aresta* tempA = a;
                a = a->prox;
                free(tempA);
            }
            Vertice* tempV = v;
            v = v->prox;
            free(tempV);
        }
        free(g);
        return true;
    }
    return false;
}
#pragma endregion
