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

 /**
  * \brief Função principal do programa.
  *
  * Exibe um menu para o usuário inserir, remover, listar antenas e sair do programa.
  * Atualiza os efeitos nefastos e salva a lista de antenas e efeitos em um ficheiro.
  *
  * \return int Código de saída do programa.
  */
int main() {
    Antena* lista = NULL;
    EfeitoNefasto* efeitos = NULL;
    int opcao;

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Inserir Antena\n");
        printf("2 - Remover Antena\n");
        printf("3 - Listar Antenas\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
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
            if (resultado != lista) { // Verifica se a lista foi alterada
                lista = resultado; // Atualiza a lista após a inserção
                printf("\nAntena inserida!\n");
            }
            else {
                printf("Ja existe uma antena nas coordenadas (%d, %d).\n", x, y);
            }

            // Salvar a lista no arquivo
            if (SalvarAntenasEmFicheiro("antenas.txt", lista)) {
                printf("Ficheiro 'antenas.txt' atualizado.\n");
            }
            else {
                printf("Erro ao salvar a lista de antenas.\n");
            }
        }
              break;

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

            if (removida) {
                printf("Antena removida.\n");

                // Salvar a lista no arquivo
                if (SalvarAntenasEmFicheiro("antenas.txt", lista)) {
                    printf("Ficheiro 'antenas.txt' atualizado.\n");
                }
                else {
                    printf("Erro ao salvar a lista de antenas.\n");
                }
            }
            else {
                printf("Antena nao encontrada.\n");
            }
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
            lista = ListarAntenas(lista, efeitos); 
            break;
        }
        case 4: // Sair
            printf("\nSaindo...\n");
            break;

        default: // Opção inválida
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 4);

    // Destruir a lista de antenas ao sair
    if (DestruirListaAntenas(lista)) {
        lista = NULL; // Atualiza o ponteiro para NULL após destruir a lista
        printf("Lista de antenas destruida com sucesso.\n");
    }
    else {
        printf("A lista de antenas ja estava vazia.\n");
    }

    // Destruir a lista de efeitos nefastos ao sair
    if (DestruirListaEfeitos(efeitos)) {
        efeitos = NULL; // Atualiza o ponteiro para NULL após destruir a lista
        printf("Lista de efeitos nefastos destruída com sucesso.\n");
    }
    else {
        printf("A lista de efeitos nefastos ja estava vazia.\n");
    }

    return 0;
}