/*****************************************************************//**
 * \file   funcoes.c
 * \brief  Implementação das funções para manipulação de antenas e efeitos nefastos.
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
  * \param freq Frequência da antena.
  * \param x Coordenada x da antena.
  * \param y Coordenada y da antena.
  * \return Ponteiro para a nova antena criada.
  */
Antena* CriarAntena(char freq, int x, int y) {
    Antena* aux = (Antena*)malloc(sizeof(Antena)); // Aloca memória para uma nova antena
    if (aux != NULL) {
        aux->freq = freq; // Define a frequência da antena
        aux->x = x; // Define a coordenada x da antena
        aux->y = y; // Define a coordenada y da antena
        aux->prox = NULL; // Define o próximo ponteiro como NULL
    }
    return aux; // Retorna o ponteiro para a nova antena
}


/**
 * \brief Insere uma antena na lista de forma ordenada.
 *
 * \param inicio Ponteiro para o início da lista de antenas.
 * \param aux Ponteiro para a antena a ser inserida.
 * \return Ponteiro para o início da lista atualizada.
 */
Antena* InserirAntena(Antena* inicio, char freq, int x, int y) {
    // Criar a nova antena

    if (x < 0 || x >= GRID_TAM || y < 0 || y >= GRID_TAM) {
        return NULL; // Não insere
    }

    Antena* aux = CriarAntena(freq, x, y);
    if (aux == NULL) {
        return inicio; // Verifica se a antena foi criada com sucesso
    }
    // Verificar se já existe uma antena nas mesmas coordenadas
    Antena* atual = inicio;
    while (atual != NULL) {
        if (atual->x == x && atual->y == y) { // Verifica se já existe uma antena nas mesmas coordenadas
            free(aux); // Liberar a memória da nova antena
            return inicio; // Retorna o início da lista sem alterações
        }
        atual = atual->prox; // Avança para a próxima antena na lista
    }

    // Inserir a antena no início da lista
    aux->prox = inicio; // Define o próximo ponteiro da nova antena como o início da lista
    return aux; // Retorna a nova antena como o início da lista
}

/**
 * \brief Remove uma antena da lista.
 *
 * \param h Ponteiro para o início da lista de antenas.
 * \param freq Frequência da antena a ser removida.
 * \param x Coordenada x da antena a ser removida.
 * \param y Coordenada y da antena a ser removida.
 * \param removida Ponteiro para indicar se a remoção foi bem-sucedida.
 * \return Ponteiro para o início da lista atualizada.
 */

Antena* RemoverAntena(Antena* h, char freq, int x, int y, int* removida) {
    Antena* aux = h;
    Antena* anterior = NULL;
    *removida = 0;
    while (aux != NULL) {
        if (aux->freq == freq && aux->x == x && aux->y == y) {
			if (anterior == NULL) // Se a antena a ser removida é a primeira da lista
                h = aux->prox;
            else
                anterior->prox = aux->prox;
            free(aux);
            *removida = 1;
			return h; // Retorna o início da lista atualizada
        }
		anterior = aux; // Avança para a próxima antena na lista
        aux = aux->prox;
    }
	return h; // Retorna o início da lista sem alterações
}

/**
 * \brief Salva a lista de antenas em um ficheiro de texto.
 *
 * \param lista Ponteiro para o início da lista de antenas.
 * \param nomeFicheiro Nome do ficheiro onde as antenas serão salvas.
 * \return true se o salvamento foi bem-sucedido, false caso contrário.
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
				
				if (auxAntena->x == j && auxAntena->y == i) { // Verifica se a antena está na posição atual
                    simbolo = auxAntena->freq;
                    break;
                }
				auxAntena = auxAntena->prox; // Avança para a próxima antena na lista
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
 * \return Ponteiro para o início da lista de antenas ou NULL se ocorrer um erro.
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
			fscanf(ficheiro, " %c", &simbolo); // Lê o símbolo do ficheiro
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
 * \param lista Ponteiro para o início da lista de antenas.
 * \param efeitos Ponteiro para o início da lista de efeitos nefastos.
 * \return Ponteiro para o início da lista de antenas.
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
            // Se não encontrou antena, procura efeito nefasto
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
 * \brief Salva a lista de antenas em um ficheiro binário.
 *
 * \param lista Ponteiro para o início da lista de antenas.
 * \return true se o salvamento foi bem-sucedido, false caso contrário.
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
 * \brief Carrega as antenas de um ficheiro binário.
 *
 * \param nomeFicheiro Nome do ficheiro binário a ser carregado.
 * \return Ponteiro para o início da lista de antenas ou NULL se ocorrer um erro.
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
 * \brief Destrói a lista de antenas, liberando a memória alocada.
 *
 * \param h Ponteiro para o início da lista de antenas.
 */
bool DestruirListaAntenas(Antena* h) {
    if (h == NULL) {
        return false; // Lista já estava vazia
    }

    while (h != NULL) {
        Antena* aux = h; // Salva o ponteiro para a antena a ser destruída
        h = h->prox;     // Avança para a próxima antena na lista
        free(aux);       // Libera a memória da antena destruída
    }

    return true; // Lista destruída com sucesso
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
    EfeitoNefasto* aux = (EfeitoNefasto*)malloc(sizeof(EfeitoNefasto));// Aloca memória para um novo efeito nefasto
    if (aux != NULL) {
        aux->x = x; // Define a coordenada x do efeito nefasto
        aux->y = y; // Define a coordenada y do efeito nefasto
        aux->prox = NULL; // Define o próximo ponteiro como NULL
    }
    return aux; // Retorna o ponteiro para o novo efeito nefasto
}

/**
 * \brief Insere um efeito nefasto na lista.
 *
 * \param inicio Ponteiro para o início da lista de efeitos nefastos.
 * \param x Coordenada x do efeito nefasto.
 * \param y Coordenada y do efeito nefasto.
 * \return Ponteiro para o início da lista atualizada.
 */
EfeitoNefasto* InserirEfeito(EfeitoNefasto* inicio, int x, int y) {
    EfeitoNefasto* aux = CriarEfeito(x, y); // Cria um novo efeito nefasto
    if (aux == NULL) return inicio; // Verifica se o efeito nefasto foi criado com sucesso
    aux->prox = inicio; // Define o próximo ponteiro do novo efeito nefasto como o início da lista
    return aux; // Retorna o novo efeito nefasto como o início da lista
}

/**
 * \brief Verifica se existem antenas com a mesma frequência e calcula os efeitos nefastos.
 *
 * \param inicio Ponteiro para o início da lista de antenas.
 * \return Ponteiro para o início da lista de efeitos nefastos.
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
            if (current->freq == compare->freq) { // Verifica se as antenas têm a mesma frequência
                if ((current->x == compare->x) && (current->y != compare->y)) { // Verifica se as antenas estão na mesma coordenada x e em coordenadas y diferentes
                    int subtracao = current->y - compare->y; // Calcula a diferença entre as coordenadas y
                    int nefy = current->y + subtracao; // Calcula a coordenada y do efeito nefasto
                    int nefy1 = compare->y - subtracao; 
                    aux = CriarEfeito(current->x, nefy);
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                    aux = CriarEfeito(compare->x, nefy1); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                }
                else if ((current->x != compare->x) && (current->y == compare->y)) { // Verifica se as antenas estão na mesma coordenada y e em coordenadas x diferentes
                    int subtracao = current->x - compare->x; // Calcula a diferença entre as coordenadas x
                    int nefx = current->x + subtracao; // Calcula a coordenada x do efeito nefasto
                    int nefx1 = compare->x - subtracao; 
                    aux = CriarEfeito(nefx, current->y); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                    aux = CriarEfeito(nefx1, compare->y); 
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); 
                }
                else if ((current->x != compare->x) && (current->y != compare->y)) { // Verifica se as antenas estão em coordenadas x e y diferentes
                    int subtracaox = current->x - compare->x; // Calcula a diferença entre as coordenadas x
                    int subtracaoy = current->y - compare->y; // Calcula a diferença entre as coordenadas y
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
            compare = compare->prox; // Avança para a próxima antena na lista
        }
        current = current->prox; // Avança para a próxima antena na lista
    }

    return efeitos; // Retorna a lista de efeitos nefastos
}


/**
 * \brief Destrói a lista de efeitos nefastos, liberando a memória alocada.
 *
 * \param lista Ponteiro para o início da lista de efeitos nefastos.
 */
bool DestruirListaEfeitos(EfeitoNefasto* lista) {
    if (lista == NULL) {
        return false; // Lista já estava vazia
    }

    while (lista != NULL) {
        EfeitoNefasto* aux = lista; // Salva o ponteiro para o efeito nefasto a ser destruído
        lista = lista->prox;       // Avança para o próximo efeito nefasto na lista
        free(aux);                 // Libera a memória do efeito nefasto destruído
    }

    return true; // Lista destruída com sucesso
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
 * \brief Cria um novo vértice.
 *
 * \param id ID do vértice.
 * \param freq Frequência do vértice.
 * \param x Coordenada x do vértice.
 * \param y Coordenada y do vértice.
 * \return Ponteiro para o novo vértice criado.
 */
Vertice* CriarVertice(int id, char freq, int x, int y) {
    Vertice* v = (Vertice*)malloc(sizeof(Vertice));
	if (v != NULL) { // Verifica se a memória foi alocada com sucesso
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
 * \param destino ID do vértice de destino da aresta.
 * \return Ponteiro para a nova aresta criada.
 */
Aresta* CriarAresta(int destino) {
    Aresta* a = (Aresta*)malloc(sizeof(Aresta));
	if (a != NULL) {   // Verifica se a memória foi alocada com sucesso 
        a->destino = destino;
        a->prox = NULL;
    }
    return a;
}
/**
 * \brief Insere um vértice no grafo.
 *
 * \param g Ponteiro para o grafo.
 * \param novo Ponteiro para o novo vértice a ser inserido.
 */


bool InserirVertice(GR* g, int id, char freq, int x, int y) {
    if (g == NULL) return false;
    if (x < 0 || x >= GRID_TAM || y < 0 || y >= GRID_TAM) return false;

    // Verifica duplicados
    Vertice* v = g->inicio;
    while (v != NULL) {
        if (v->x == x && v->y == y) {
            return false; // Já existe vértice nessas coordenadas
        }
        v = v->prox;
    }

    // Cria o novo vértice
    Vertice* novo = CriarVertice(id, freq, x, y);
    if (novo == NULL) return false;

    // Insere no início da lista
    novo->prox = g->inicio;
    g->inicio = novo;
    g->numVertices++;
    return true;
}
/**
 * \brief Encontra um vértice no grafo pelo ID.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do vértice a ser encontrado.
 * \return Ponteiro para o vértice encontrado ou NULL se não encontrado.
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
 * \brief Mostra os vértices do grafo.
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
 * \brief Insere arestas entre o novo vértice e os outros vértices com a mesma frequência.
 *
 * \param g Ponteiro para o grafo.
 * \param novo Ponteiro para o novo vértice a ser inserido.
 * \return true se as arestas foram inseridas com sucesso, false caso contrário.
 */
bool InserirAresta(GR* g, Vertice* novo) {
    if (g == NULL || novo == NULL) return false;
    Vertice* v = g->inicio;
    while (v != NULL) {
        if (v != novo && v->freq == novo->freq) {
            // Verifica se já existe a ligação
            Aresta* a = novo->adjacentes;
            while (a != NULL) {
                if (a->destino == v->id) {
                    break; // Já existe, não cria
                }
                a = a->prox;
            }
            if (a == NULL) {
                // Liga nos dois sentidos (grafo não orientado)
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
                return true; // Ligação criada
            }
        }
        v = v->prox;
    }
    return false; // Nenhuma ligação criada
}

/**
 * \brief Remove arestas do vértice com o ID especificado.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do vértice cujas arestas serão removidas.
 * \return true se as arestas foram removidas com sucesso, false caso contrário.
 */
bool RemoverArestas(GR* g, int id) {
    if (g == NULL){
        return false;
	}
    Vertice* v = g->inicio;
    // Remove arestas de outros vértices que apontam para 'id'
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
    // Remove todas as arestas do próprio vértice
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
            return true; // Removeu pelo menos uma aresta do próprio vértice
        }
        alvo->adjacentes = NULL;
    }
    return false; // Não removeu nenhuma aresta
}

/**
 * \brief Remove um vértice do grafo.
 *
 * \param g Ponteiro para o grafo.
 * \param id ID do vértice a ser removido.
 * \return true se o vértice foi removido com sucesso, false caso contrário.
 */
bool RemoverVertice(GR* g, int id) {
    if (g == NULL || g->inicio == NULL) {
        return false;
	}
    RemoverArestas(g, id);
	Vertice* atual = g->inicio; // Ponteiro para percorrer a lista de vértices
    Vertice* anterior = NULL;
    while (atual != NULL && atual->id != id) {
		anterior = atual; // Salva o ponteiro do vértice anterior
        atual = atual->prox;
    }
    if (atual == NULL) {
        return false;
	}
    if (anterior != NULL) {
        anterior->prox = atual->prox;
    }
    else {
		g->inicio = atual->prox; // Se o vértice a ser removido é o primeiro
    }
    free(atual);
    g->numVertices--;
    return true;
}


/**
 * \brief Verifica se existem antenas com a mesma frequência e calcula os efeitos nefastos.
 *
 * \param inicio Ponteiro para o início da lista de vértices.
 * \return Ponteiro para o início da lista de efeitos nefastos.
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
 * \param lista Ponteiro para a lista de vértices.
 * \param nomeFicheiro Nome do ficheiro onde o grafo será salvo.
 * \return true se o salvamento foi bem-sucedido, false caso contrário.
 */

bool SalvarGrafoEmTxt(Vertice* lista, const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "w");
    if (ficheiro == NULL) {
        return false;
    }
    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.'; 
			Vertice* aux = lista; // Inicializa o ponteiro auxiliar para percorrer a lista de vértices
            while (aux != NULL) { 
                if (aux->x == j && aux->y == i) { 
                    simbolo = aux->freq; 
					break;  // Se encontrou o vértice, armazena a frequência
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
 * \return Ponteiro para a lista de vértices ou NULL se ocorrer um erro.
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
			fscanf(ficheiro, " %c", &simbolo); // Lê o símbolo do ficheiro
            if (simbolo != '.') {
				Vertice* novo = CriarVertice(id++, simbolo, j, i); // Cria um novo vértice
                novo->prox = lista; 
                lista = novo;
            }
        }
    }
    fclose(ficheiro);
    return lista;
}

/**
 * \brief Salva o grafo em um ficheiro binário.
 *
 * \param lista Ponteiro para a lista de vértices.
 * \param nomeFicheiro Nome do ficheiro onde o grafo será salvo.
 * \return true se o salvamento foi bem-sucedido, false caso contrário.
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
			while (aux != NULL) { // Percorre a lista de vértices
                if (aux->x == j && aux->y == i) {
                    simbolo = aux->freq;
                    break;
                }
				aux = aux->prox; // Avança para o próximo vértice na lista
            }
            fwrite(&simbolo, sizeof(char), 1, ficheiro);
        }
    }
    fclose(ficheiro);
    return true;
}

/**
 * \brief Carrega o grafo de um ficheiro binário.
 *
 * \param nomeFicheiro Nome do ficheiro binário a ser carregado.
 * \return Ponteiro para a lista de vértices ou NULL se ocorrer um erro.
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
 * \param listaVertices Ponteiro para a lista de vértices.
 * \return true se a exibição foi bem-sucedida, false caso contrário.
 */

bool MostrarGrid(Vertice* listaVertices) {
    if (listaVertices == NULL) {
        printf("Grafo vazio.\n");
        return false;
    }

    // Calcula os efeitos nefastos a partir dos vértices
    EfeitoNefasto* efeitos = efeitoNefastoVertices(listaVertices);

    for (int i = 0; i < GRID_TAM; i++) {
        for (int j = 0; j < GRID_TAM; j++) {
            char simbolo = '.';

            // Procura vértice
            Vertice* aux = listaVertices;
            while (aux != NULL) {
                if (aux->x == j && aux->y == i) {
                    simbolo = aux->freq;
                    break;
                }
                aux = aux->prox;
            }

            // Se não encontrou vértice, procura efeito nefasto
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
 * \param v Ponteiro para o vértice atual.
 * \param visitado Array de booleanos para marcar os vértices visitados.
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
 * \brief Realiza uma busca em profundidade no grafo a partir de um vértice de origem.
 *
 * \param g Ponteiro para o grafo.
 * \param idOrigem ID do vértice de origem.
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
        printf("Antena de origem não encontrada.\n");
    }

    free(visitado);
}

/**
 * \brief Realiza uma busca em largura no grafo a partir de um vértice de origem.
 *
 * \param g Ponteiro para o grafo.
 * \param idOrigem ID do vértice de origem.
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
        printf("Antena de origem não encontrada.\n");
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
 * \brief Destrói o grafo, liberando a memória alocada.
 *
 * \param g Ponteiro para o grafo a ser destruído.
 * \return true se o grafo foi destruído com sucesso, false caso contrário.
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
