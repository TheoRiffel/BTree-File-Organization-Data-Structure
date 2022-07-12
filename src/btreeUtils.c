#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../includes/funcionalidades.h"
#include "../includes/utils.h"
#include "../includes/fileUtils.h"
#include "../includes/indexUtils.h"

/**
 * @brief Escreve o cabe�alho em um arquivo bin�rio da Btree
 *
 * @param index : ponteiro para o arquivo
 * @param cabecalho : struct contendo os dados do cabe�alho
 */
void escreverCabecalhoBTree(FILE* BTree_file, cabecalhoBTree_t* cabecalho, int tipo)
{
	fseek(BTree_file, 0, SEEK_SET);
	fwrite(&cabecalho->status, sizeof(char), 1, BTree_file);
	fwrite(&cabecalho->noRaiz, sizeof(char), 1, BTree_file);
	fwrite(&cabecalho->nroNos, sizeof(char), 1, BTree_file);

	int qnt_lixo = tipo == 1? 32 : 44;
	fwrite("$", sizeof(char), qnt_lixo, BTree_file);
}

/**
 * @brief Escreve o cabe�alho em um arquivo bin�rio da Btree
 *
 * @param index : ponteiro para o arquivo
 * @return cabecalhoBTree_t : struct contendo os dados do cabe�alho
 */
cabecalhoBTree_t* lerCabecalhoBTree(FILE* BTree_file, int tipo)
{
	cabecalhoBTree_t* cab = malloc(sizeof(cabecalhoBTree_t));
	fread(&cab->status, sizeof(char), 1, BTree_file);
	fread(&cab->noRaiz, sizeof(int), 1, BTree_file);
	fread(&cab->nroNos, sizeof(int), 1, BTree_file);
	
	int qnt_lixo = tipo == 1? 32 : 44;
	fseek(BTree_file, qnt_lixo, SEEK_CUR);

	return cab;
}

registroBTree_t *inicializarRegistroBTree()
{
	registroBTree_t* no = malloc(sizeof(registroBTree_t));
	no->tipoNo = NO_NAO_DEFINIDO;
	no->nroChaves = 0;
	no->RRNregistroBTree = -1;
	
	for (int i = 0; i < ORDEM_ARVORE - 1; i++) 
	{
		no->chave[i].id = -1;
		no->chave[i].RRN = -1;
		no->chave[i].byteOffset = -1;
	}

	for (int i = 0; i < ORDEM_ARVORE; i++)
		no->ptr[i] = -1;

	return no;
}

/**
 * @brief libera memória da registro da BTree
 * 
 * @param indice 
 */
void liberarRegistroBTree(registroBTree_t* no)
{
	free(no);
}

/**
 * @brief Inicializa uma struct que guarda a Btree em RAM
 *
 * @return indice_t : struct contendo os dados do indice inicializados
 */
registroBTree_t* inicializarBTree()
{
	registroBTree_t* raiz = inicializarRegistroBTree();
	raiz->tipoNo = NO_RAIZ;
	raiz->RRNregistroBTree = 0;

	return raiz;
}

/**
 * @brief escreve registro de um nó no arquivo binário da BTree
 * 
 * @param BTree_file : ponteiro do arquivo da BTree
 * @param no : registro da BTree
 * @param tipo : inteiro informando o tipo do arquivo
 */
void escreverRegistroBTree(FILE* BTree_file, registroBTree_t* no, int tipo)
{
	fwrite(&no->tipoNo, sizeof(char), 1, BTree_file);
	fwrite(&no->nroChaves, sizeof(int), 1, BTree_file);

	for (int i = 0; i < ORDEM_ARVORE - 1; i++)
	{
		fwrite(&no->chave[i].id, sizeof(int), 1, BTree_file);

		if (tipo == 1)
			fwrite(&no->chave[i].RRN, sizeof(int), 1, BTree_file);

		else if (tipo == 2)
			fwrite(&no->chave[i].byteOffset, sizeof(long int), 1, BTree_file);
	}

	for (int i = 0; i < ORDEM_ARVORE; i++)
		fwrite(&no->ptr[i], sizeof(int), 1, BTree_file);
}

/**
 * @brief L� um registro (nó) a partir de um arquivo bin�rio da BTree
 *
 * @param index : ponteiro para o arquivo da BTree
 * @param tipo : inteiro informando o tipo do arquivo
 * @return registroBTree_t : struct contendo os dados do registro lido
 */
registroBTree_t* lerRegistroBTree(FILE* BTree_file, int tipo)
{
	registroBTree_t* no = malloc(sizeof(registroBTree_t));

	no->RRNregistroBTree = tipo == 1? (ftell(BTree_file) / TAM_REG_BTREE1) - 1: (ftell(BTree_file) / TAM_REG_BTREE2) - 1;
	fread(&no->tipoNo, sizeof(char), 1, BTree_file);
	fread(&no->nroChaves, sizeof(int), 1, BTree_file);

    for (int i = 0; i < ORDEM_ARVORE - 1; i++)
    {
	    fread(&no->chave[i].id, sizeof(int), 1, BTree_file);
        if (tipo == 1)
            fread(&no->chave[i].RRN, sizeof(int), 1, BTree_file);
        else if (tipo == 2)
            fread(&no->chave[i].byteOffset, sizeof(long int), 1, BTree_file);
    }

    for (int i = 0; i < ORDEM_ARVORE; i++)
        fread(&no->ptr[i], sizeof(int), 1, BTree_file);

	return no;
}

chave_t* searchBTree(FILE* BTree_file,registroBTree_t* no, int chave, int tipo)
{
	if (no->RRNregistroBTree == -1)
		return NULL;

	int encontrado;
	// int chave_index = buscaBinaria(chave, no->chave, 0, ORDEM_ARVORE - 1);

	// se eu achar no nó atual
	// if (chave_index != -1) 
		// return &no->chave[chave_index];
	
	//se eu não achar
	// int proxRRNregisterBTree = 	


	

	//
}
