/*****************************************************************//**
 * \file   main.c
 * \brief  Programa principal para manipulação de antenas e efeitos nefastos.
 *
 * \author matos
 * \date   March 2025
 *********************************************************************/

#define CRT_SECURE_NO_WARNINGS

#include "dados.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "funcoes.h"


int main() {
    Antena* lista = NULL;
    EfeitoNefasto* efeitos = NULL;
    GR* grafo = CriarGrafo();
    int opcao;
    int op_antena;
    int op_grafo;
    int id_counter = 1;


	//Carregar antenas

    lista = CarregarAntenasDeBin("antenas.bin");
    if (lista != NULL) {
        printf("Antenas carregadas automaticamente do ficheiro BIN.\n");
    }
    else {
        lista = CarregarAntenasDeTxt("antenas.txt");
        if (lista != NULL) {
            printf("Antenas carregadas automaticamente do ficheiro TXT.\n");
        }
        else {
            printf("Nenhum ficheiro de antenas encontrado. Lista vazia.\n");
        }
    }
	// Carregar grafo
    Vertice* vertices = CarregarGrafoDeBin("grafo.bin");
    if (vertices != NULL) {
        printf("Grafo carregado automaticamente do ficheiro BIN.\n");
    }
    else {
        vertices = CarregarGrafoDeTxt("grafo.txt");
        if (vertices != NULL) {
            printf("Grafo carregado automaticamente do ficheiro TXT.\n");
        }
        else {
            printf("Nenhum ficheiro de grafo encontrado. Grafo vazio.\n");
        }
    }

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1 - Menu Antenas\n");
        printf("2 - Menu Grafo\n");
        printf("3 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            do {
                printf("\n--- MENU ANTENAS ---\n");
                printf("1 - Inserir Antena\n");
                printf("2 - Remover Antena\n");
                printf("3 - Listar Antenas\n");
                printf("4 - Voltar\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &op_antena);

                switch (op_antena) {
				case 1: { // Inserir Antena
                    char freq;
                    int x, y;
                    printf("\nFrequencia da antena: ");
                    scanf(" %c", &freq);
                    printf("Coordenada X: ");
                    scanf("%d", &x);
                    printf("Coordenada Y: ");
                    scanf("%d", &y);
                    Antena* resultado = InserirAntena(lista, freq, x, y); 
                    if (resultado == NULL) {
                        printf("Coordenadas fora do grid 10x10!\n");
                    }
                    else if (resultado != lista) {
                        lista = resultado;
                        printf("\nAntena inserida!\n");
                    }
                    else {
                        printf("Ja existe uma antena nas coordenadas (%d, %d).\n", x, y);
                    }
                    SalvarAntenasEmFicheiroBin(lista);
                    SalvarAntenasEmTxt(lista, "antenas.txt");
                    break;
                }
				case 2: { // Remover Antena
                    char freq;
                    int x, y;
                    printf("\nFrequencia da antena a remover: ");
                    scanf(" %c", &freq);
                    printf("Coordenada X: ");
                    scanf("%d", &x);
                    printf("Coordenada Y: ");
                    scanf("%d", &y);
                    int removida = 0;
                    lista = RemoverAntena(lista, freq, x, y, &removida);
                    SalvarAntenasEmFicheiroBin(lista);
                    SalvarAntenasEmTxt(lista, "antenas.txt");
                    if (removida != 0)
                        printf("Antena removida.\n");
                    else
                        printf("Antena não encontrada.\n");
                    break;
                }
                case 3: { // Listar Antenas
                    printf("\nLista de Antenas:\n");

                    // Atualizar efeitos nefastos antes de listar
                    if (DestruirListaEfeitos(efeitos)) {
                        printf("Lista de efeitos nefastos destruida com sucesso.\n");
                    }
                    efeitos = efeitoNefasto(lista);

                    // Exibir a lista de antenas e efeitos
                    ListarAntenas(lista, efeitos);
                    break;
                }
                case 4:
                    break;
                default:
                    printf("\nOpcao invalida\n");
                }
            } while (op_antena != 4);
        }
        else if (opcao == 2) {
            do {
                printf("\n--- MENU GRAFO ---\n");
                printf("1 - Inserir Vertice\n");
                printf("2 - Remover Vertice\n");
                printf("3 - Mostrar Grafo\n");
                printf("4 - Mostrar vértices\n");
                printf("5 - Procura em Profundidade(DFS)\n");
                printf("6 - Procura em Largura (BFS)\n");
                printf("7 - Voltar\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &op_grafo);

                switch (op_grafo) {
                case 1: {
                    char freq;
                    int x, y;
                    printf("Frequência : ");
                    scanf(" %c", &freq);
                    printf("Coordenada x: ");
                    scanf("%d", &x);
                    printf("Coordenada y: ");
                    scanf("%d", &y);

                    if (InserirVertice(grafo, id_counter, freq, x, y)) {
                        Vertice* v = EncontrarVerticePorId(grafo, id_counter);
                        InserirAresta(grafo, v);
                        printf("Vertice inserido! ID: %d\n", id_counter);
                        printf("Ligacoes automaticas criadas.\n");
                        id_counter++;
                        // Atualiza os ficheiros após inserção
                        SalvarGrafoEmBin(grafo->inicio, "grafo.bin");
                        SalvarGrafoEmTxt(grafo->inicio, "grafo.txt");
                    }
                    else {
                        printf("Já existe vertice nessas coordenadas.\n");
                    }
                    break;
                }
                case 2: {
                    int id;
                    printf("ID do vertice a remover: ");
                    scanf("%d", &id);
                    if (RemoverVertice(grafo, id)) {
                        printf("Vertice removido.\n");
                        // Atualiza os ficheiros após remoção
                        SalvarGrafoEmBin(grafo->inicio, "grafo.bin");
                        SalvarGrafoEmTxt(grafo->inicio, "grafo.txt");
                    }
                    else {
                        printf("Vertice nao encontrado.\n");
                    }
                    break;
                }
                case 3: {
                    if (!MostrarGrid(grafo->inicio)) {
                        printf("Nao foi possivel mostrar o grafo.\n");
                    }
                    break;
                }
                case 4: {
                    MostrarVertices(grafo);
                    break;
                }
                case 5: {
                    int idOrigem;
                    printf("ID da antena de origem para DFS: ");
                    scanf("%d", &idOrigem);
                    ProcuraProfundidade(grafo, idOrigem);
                    break;
                }
                case 6: {
                    int idOrigem;
                    printf("ID da antena de origem para BFS: ");
                    scanf("%d", &idOrigem);
                    ProcuraLargura(grafo, idOrigem);
                    break;
                }
                case 7:
                    break;
                default:
                    printf("Opcao invalida.\n");
                }
            } while (op_grafo != 7);
        }
    } while (opcao != 3);

    if (efeitos != NULL) {
        DestruirListaEfeitos(efeitos);
        efeitos = NULL;
    }
    DestruirListaAntenas(lista);
    DestruirGrafo(grafo);

    printf("Programa terminado.\n");
    return 0;
}