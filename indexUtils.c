#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "utils.h"
#include "fileUtils.h"
#include "indexUtils.h"

/**
 * @brief Escreve o cabeçalho em um arquivo binário de índices
 *
 * @param index : ponteiro para o arquivo
 * @param cabecalho : struct contendo os dados do cabeçalho
 */
void escreverCabecalhoIndex(FILE* index, cabecalhoIndex_t* cabecalho)
{
	fseek(index, 0, SEEK_SET);
	fwrite(&cabecalho->status, sizeof(char), 1, index);
}

/**
 * @brief Escreve o cabeçalho em um arquivo binário de índices
 *
 * @param index : ponteiro para o arquivo
 * @return cabecalhoIndex_t : struct contendo os dados do cabeçalho
 */
cabecalhoIndex_t* lerCabecalhoIndex(FILE* index)
{
	cabecalhoIndex_t* cab = malloc(sizeof(cabecalhoIndex_t));
	fread(&cab->status, 1, 1, index);

	return cab;
}

/**
 * @brief Inicializa uma struct que guarda o índice em RAM
 *
 * @return indice_t : struct contendo os dados do indice inicializados
 */
indice_t* inicializarIndice()
{
	indice_t* i = malloc(sizeof(indice_t));
	i->tamanho = 0;
	i->registros = NULL;

	return i;
}

/**
 * @brief Escreve o índice em um arquivo binário de índices
 *
 * @param index_fptr : ponteiro para o arquivo de índices
 * @param indice : struct contendo os dados do índice
 * @param tipo : inteiro informando o tipo do arquivo
 */
void escreverIndice(FILE* index_fptr, indice_t* indice, int tipo)
{
	for (int i = 0; i < indice->tamanho; i++)
	{
		fwrite(&indice->registros[i].id, sizeof(int), 1, index_fptr);

		if (tipo == 1)
			fwrite(&indice->registros[i].RRN, sizeof(int), 1, index_fptr);

		else if (tipo == 2)
			fwrite(&indice->registros[i].byteOffset, sizeof(long int), 1, index_fptr);
	}
}

/**
 * @brief Desaloca a memória utilizada no índice
 *
 * @param indice : struct contendo os dados do índice
 */
void liberarIndice(indice_t* indice)
{
	free(indice->registros);
	free(indice);
}

/**
 * @brief Lê um registro(de índice) a partir de um arquivo binário de índices
 *
 * @param index : ponteiro para o arquivo de índices
 * @param tipo : inteiro informando o tipo do arquivo
 * @return registroIndice_t : struct contendo os dados do registro lido
 */
registroIndice_t* lerRegistroIndex(FILE* index, int tipo)
{
	registroIndice_t* r = malloc(sizeof(registroIndice_t));
	fread(&r->id, sizeof(int), 1, index);
	if (tipo == 1)
		fread(&r->RRN, sizeof(int), 1, index);
	else if (tipo == 2)
		fread(&r->byteOffset, sizeof(long int), 1, index);

	return r;
}

/**
 * @brief Lê um arquivo binário de índices e salva o índice na RAM
 *
 * @param index : ponteiro para o arquivo de índices
 * @param tipo : inteiro informando o tipo do arquivo
 * @return indice_t : struct contendo os dados do índice
 */
indice_t* indiceParaRAM(FILE* index, int tipo)
{
	indice_t* i = malloc(sizeof(indice_t));
	i->tamanho = 0;
	i->registros = NULL;

	char buffer;
    int count = 0;
	fseek(index, TAM_CABECALHO_IND, SEEK_SET);

    while (fread(&buffer, 1, 1, index) != 0)
    {
		fseek(index, -1, SEEK_CUR);

		registroIndice_t* registroIndice = lerRegistroIndex(index, tipo);
        i->registros = realloc(i->registros, (count + 1) * sizeof(registroIndice_t));
        i->registros[count] = (*registroIndice);
        
        count++;
        free(registroIndice);
    }
    i->tamanho = count;

	return i;
}

/**
 * @brief Busca um registro(de índice) a partir de um índice na RAM
 *
 * @param indice : ponteiro para a struct contendo os dados do índice
 * @param chave : id a ser buscado
 * @return registroIndice_t : struct contendo os dados do registro buscado
 */
registroIndice_t* buscaIndex(indice_t* indice, int chave)
{
	registroIndice_t* registro = NULL;
	int posicao = buscaBinaria(chave, indice->registros, 0, indice->tamanho);

	if (posicao == -1) // registro não achado pela busca binária
		return NULL;

	else
		registro = &(indice->registros[posicao]);

	return registro;
}

/**
 * @brief Remove um registro de um índice na RAM
 *
 * @param indice : ponteiro para a struct contendo os dados do índice
 * @param id : vetor de ids a serem removidos
 * @param tipo : inteiro informando o tipo do arquivo
 * @param qttId : quantidade de ids a serem removidos
 * @return indice_t : struct contendo os dados do índice após as remoções
 */
indice_t* removerRegistroIndex(indice_t* indice, int* id, int tipo, int qttId)
{
	for (int i = 0; i < qttId; i++)
	{
		int posicao = buscaBinaria(id[i], indice->registros, 0, indice->tamanho);
		
		if (posicao == -1)
		{ // registro não achado pela busca binária
			return indice;
		}

		for (int i = posicao; i < indice->tamanho - 1; i++)
			indice->registros[i] = indice->registros[i + 1];

		indice->tamanho--;
		indice->registros = realloc(indice->registros, indice->tamanho * sizeof(registroIndice_t));
	}
	return indice;
}