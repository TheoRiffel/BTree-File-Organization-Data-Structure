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
	fread(&cab->proxRRN, sizeof(int), 1, BTree_file);
	fread(&cab->nroNos, sizeof(int), 1, BTree_file);
	
	int qnt_lixo = tipo == 1? (TAM_REG_BTREE1 - TAM_CABECALHO_BTREE): (TAM_REG_BTREE2 - TAM_CABECALHO_BTREE);
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

	// no->RRNregistroBTree = tipo == 1? (ftell(BTree_file) / TAM_REG_BTREE1) - 1: (ftell(BTree_file) / TAM_REG_BTREE2) - 1;
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

void printaRegistroBTree(registroBTree_t *no, int tipo)
{
	printf("tipo nó: %c\n", no->tipoNo);
	printf("Nro Chaves: %d\n", no->nroChaves);

	for (int i = 0; i < ORDEM_ARVORE - 1; i++)
    {
		printf("Chave nº %d, id: %d\n", i, no->chave[i].id);
        if (tipo == 1)
			printf("RRN: %d\n", no->chave[i].RRN);

        else if (tipo == 2)
			printf("BO: %d\n", no->chave[i].RRN);
    }

    for (int i = 0; i < ORDEM_ARVORE; i++)
		printf("Ptr %d: %d\t", i, no->ptr[i]);

	printf("\n\n");
}	

chave_t* searchBTree(FILE* BTree_file, int RRN_no, int chave, int tipo)
{
	if (RRN_no == -1)
		return NULL;

	if (tipo == 1)
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE1) + TAM_REG_BTREE1, SEEK_SET);
	else
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE2) + TAM_REG_BTREE2, SEEK_SET);

	printf("RRN de onde estou %d, %d, %d\n", RRN_no, TAM_REG_BTREE1, RRN_no * TAM_REG_BTREE1);
	printf("onde estou %ld\n", ftell(BTree_file));

	registroBTree_t *no = lerRegistroBTree(BTree_file, tipo);
	printaRegistroBTree(no, tipo);

	int chave_index = buscaBinariaChavesBTree(chave, no->chave, 0, no->nroChaves - 1);

	// se eu achar no nó atual
	if (chave_index != -1) 
		return &no->chave[chave_index];
	
	//se eu não achar, procuro em qual filho a minha chave hipoteticamente estaria
	int proxRRNregisterBTree;
	int i = 0;
	while (i < no->nroChaves && chave > no->chave[i].id)
		i++;
	proxRRNregisterBTree = no->ptr[i];

	// liberarRegistroBTree(no);
	free(no);

	return searchBTree(BTree_file, proxRRNregisterBTree, chave, tipo);
}
