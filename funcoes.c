/*****************************************************************//**
 * \file   funcoes.c
 * \brief  Implementação das funções para manipulação de antenas e efeitos nefastos.
 *
 * \author matos
 * \date   March 2025
 *********************************************************************/

#include "dados.h"

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
    Antena* aux = CriarAntena(freq, x, y);
    if (aux == NULL) return inicio; // Verifica se a antena foi criada com sucesso

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
 * \return Ponteiro para o início da lista atualizada ou NULL se a antena não for encontrada.
 */

Antena* RemoverAntena(Antena* h, char freq, int x, int y, int* removida) {
    if (h == NULL) {
        *removida = 0; // Lista vazia, nada para remover
        return NULL;
    }

    if (h->freq == freq && h->x == x && h->y == y) {
        Antena* aux = h;
        h = h->prox;
        free(aux); // Libera a memória da antena removida
        *removida = 1; // Marca que a antena foi removida
        return h;
    }

    h->prox = RemoverAntena(h->prox, freq, x, y, removida);
    return h;
}


/**
 * \brief Lista todas as antenas.
 *
 * \param h Ponteiro para o início da lista de antenas.
 * \param efeitos Ponteiro para a lista de efeitos nefastos.
 */
Antena* ListarAntenas(Antena* lista, EfeitoNefasto* efeitos) {
    // Inicializar uma matriz de caracteres para representar o mapa
    char mapa[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mapa[i][j] = '.'; // Inicializa o mapa com pontos
        }
    }

    // Adicionar antenas ao mapa
    Antena* auxAntena = lista;
    while (auxAntena != NULL) {
        if (auxAntena->x >= 0 && auxAntena->x < 10 && auxAntena->y >= 0 && auxAntena->y < 10) { // Verifica se as coordenadas da antena estão dentro dos limites do mapa
            mapa[auxAntena->y][auxAntena->x] = auxAntena->freq; // Adiciona a antena ao mapa
        }
        auxAntena = auxAntena->prox; // Avança para a próxima antena na lista
    }

    // Adicionar efeitos nefastos ao mapa
    EfeitoNefasto* auxEfeito = efeitos;
    while (auxEfeito != NULL) {
        if (auxEfeito->x >= 0 && auxEfeito->x < 10 && auxEfeito->y >= 0 && auxEfeito->y < 10) { // Verifica se as coordenadas do efeito nefasto estão dentro dos limites do mapa
            if (mapa[auxEfeito->y][auxEfeito->x] == '.') { // Verifica se a posição no mapa está vazia
                mapa[auxEfeito->y][auxEfeito->x] = '#'; // Adiciona o efeito nefasto ao mapa
            }
        }
        auxEfeito = auxEfeito->prox; // Avança para o próximo efeito nefasto na lista
    }

    // Imprimir o mapa no terminal
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%c ", mapa[i][j]); // Imprime o mapa no terminal
        }
        printf("\n"); // Imprime uma nova linha no terminal
    }

    return lista; // Retorna o ponteiro para a lista de antenas
}

/**
 * \brief Salva a lista de antenas em um ficheiro.
 *
 * \param nomeFicheiro Nome do ficheiro onde a lista será salva.
 * \param lista Ponteiro para a lista de antenas.
 */
bool SalvarAntenasEmFicheiro(const char* nomeFicheiro, Antena* lista) {
    FILE* ficheiro = fopen(nomeFicheiro, "w"); // Abre o arquivo em modo de escrita
    if (ficheiro == NULL) {
        return false; // Retorna false em caso de erro
    }

    // Inicializar uma matriz de caracteres para representar o mapa
    char mapa[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mapa[i][j] = '.'; // Inicializa o mapa com pontos
        }
    }

    // Adicionar antenas ao mapa
    Antena* auxAntena = lista;
    while (auxAntena != NULL) {
        if (auxAntena->x >= 0 && auxAntena->x < 10 && auxAntena->y >= 0 && auxAntena->y < 10) {
            mapa[auxAntena->y][auxAntena->x] = auxAntena->freq; // Adiciona a antena ao mapa
        }
        auxAntena = auxAntena->prox;
    }

    // Escrever o mapa no ficheiro usando fprintf
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fprintf(ficheiro, "%c", mapa[i][j]); // Escreve cada caractere do mapa no arquivo
        }
        fprintf(ficheiro, "\n"); // Adiciona uma nova linha no arquivo após cada linha do mapa
    }

    fclose(ficheiro); // Fecha o arquivo
    return true; // Retorna true se o arquivo foi salvo com sucesso
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
                    int nefy1 = compare->y - subtracao; // Calcula a coordenada y do efeito nefasto
                    aux = CriarEfeito(current->x, nefy); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
                    aux = CriarEfeito(compare->x, nefy1); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
                }
                else if ((current->x != compare->x) && (current->y == compare->y)) { // Verifica se as antenas estão na mesma coordenada y e em coordenadas x diferentes
                    int subtracao = current->x - compare->x; // Calcula a diferença entre as coordenadas x
                    int nefx = current->x + subtracao; // Calcula a coordenada x do efeito nefasto
                    int nefx1 = compare->x - subtracao; // Calcula a coordenada x do efeito nefasto
                    aux = CriarEfeito(nefx, current->y); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
                    aux = CriarEfeito(nefx1, compare->y); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
                }
                else if ((current->x != compare->x) && (current->y != compare->y)) { // Verifica se as antenas estão em coordenadas x e y diferentes
                    int subtracaox = current->x - compare->x; // Calcula a diferença entre as coordenadas x
                    int subtracaoy = current->y - compare->y; // Calcula a diferença entre as coordenadas y
                    int nefx = current->x + subtracaox; // Calcula a coordenada x do efeito nefasto
                    int nefx1 = compare->x - subtracaox; // Calcula a coordenada x do efeito nefasto
                    int nefy = current->y + subtracaoy; // Calcula a coordenada y do efeito nefasto
                    int nefy1 = compare->y - subtracaoy; // Calcula a coordenada y do efeito nefasto
                    aux = CriarEfeito(nefx, nefy); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
                    aux = CriarEfeito(nefx1, nefy1); // Cria um novo efeito nefasto
                    efeitos = InserirEfeito(efeitos, aux->x, aux->y); // Insere o efeito nefasto na lista
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