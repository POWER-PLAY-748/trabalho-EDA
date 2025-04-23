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
	char freq; // Frequ�ncia da antena
	int x, y; // Coordenadas da antena
	struct Antena* prox; // Ponteiro para a pr�xima antena
} Antena;
/**
 * \brief Estrutura que representa um efeito nefasto.
 */
 // Estrutura do Efeito Nefasto
typedef struct EfeitoNefasto {
	int x, y; // Coordenadas do efeito nefasto
	struct EfeitoNefasto* prox;	// Ponteiro para o pr�ximo efeito nefasto
} EfeitoNefasto; 
