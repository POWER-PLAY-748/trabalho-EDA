/*****************************************************************//**
 * \file   dados.h
 * \brief  Declaração das estruturas de dados para antenas e efeitos nefastos.
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
	char freq; // Frequência da antena
	int x, y; // Coordenadas da antena
	struct Antena* prox; // Ponteiro para a próxima antena
} Antena;
/**
 * \brief Estrutura que representa um efeito nefasto.
 */
 // Estrutura do Efeito Nefasto
typedef struct EfeitoNefasto {
	int x, y; // Coordenadas do efeito nefasto
	struct EfeitoNefasto* prox;	// Ponteiro para o próximo efeito nefasto
} EfeitoNefasto; 
