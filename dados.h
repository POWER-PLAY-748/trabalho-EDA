/*****************************************************************//**
 * \file   dados.h
 * \brief  Declara��o das estruturas de dados para antenas e efeitos nefastos.
 *
 * \author matos
 * \date   March 2025
 *********************************************************************/

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

 /**
  * \brief Estrutura que representa uma antena.
  */

  // Estrutura da Antena
typedef struct Antena {
	char freq; 
	int x, y; 
	struct Antena* prox; // Ponteiro para a pr�xima antena
} Antena;
/**
 * \brief Estrutura que representa um efeito nefasto.
 */
 // Estrutura do Efeito Nefasto
typedef struct EfeitoNefasto {
	int x, y; 
	struct EfeitoNefasto* prox;	// Ponteiro para o pr�ximo efeito nefasto
} EfeitoNefasto; 
/**
 * \brief Estrutura que representa um v�rtice do grafo.
 */
 // Estrutura do V�rtice
typedef struct Vertice {
	int id;
	char freq;
	int x, y;
	struct Aresta* adjacentes; // Lista de arestas (adjac�ncias)
	struct Vertice* prox;      
} Vertice;
/**
 * \brief Estrutura que representa uma aresta do grafo.
 */
 // Estrutura da Aresta
typedef struct Aresta {
	int destino; // ID do v�rtice de destino
	struct Aresta* prox;
} Aresta;
/**
 * \brief Estrutura que representa um grafo.
 */
 // Estrutura do Grafo
typedef struct GR {
	int numVertices;
	Vertice* inicio; // Ponteiro para o in�cio da lista de v�rtices
} GR;

