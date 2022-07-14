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

	int qnt_lixo = tipo == 1? (TAM_REG_BTREE1 - TAM_CABECALHO_BTREE) : (TAM_REG_BTREE2 - TAM_CABECALHO_BTREE);
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
	
	for (int i = 0; i < TAXA_OCUPACAO; i++) 
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

chave_t retiraChave()
{
	chave_t chave_nula;
	chave_nula.id = -1;
	chave_nula.RRN = -1;
	chave_nula.byteOffset = -1;

	return chave_nula;
}

/**
 * @brief Inicializa uma struct que guarda a Btree em RAM
 *
 * @return indice_t : struct contendo os dados do indice inicializados
 */
registroBTree_t* inicializarBTree(FILE* BTree_file, int tipo)
{
	registroBTree_t* raiz = inicializarRegistroBTree();
	raiz->tipoNo = NO_RAIZ;
	raiz->RRNregistroBTree = 0;

	escreverRegistroBTree(BTree_file, raiz, tipo);
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

	for (int i = 0; i < TAXA_OCUPACAO; i++)
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

    for (int i = 0; i < TAXA_OCUPACAO; i++)
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

	for (int i = 0; i < TAXA_OCUPACAO; i++)
    {
		printf("Chave nº %d, id: %d\n", i, no->chave[i].id);
        if (tipo == 1)
			printf("RRN: %d\n", no->chave[i].RRN);

        else if (tipo == 2)
			printf("BO: %lld\n", no->chave[i].byteOffset);
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

	registroBTree_t *no = lerRegistroBTree(BTree_file, tipo);
	// printaRegistroBTree(no, tipo);

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

bool verificaNoFolha(registroBTree_t *no)
{
	for (int i = 0; i < no->nroChaves; i++)
		if (no->ptr[i] != -1)
			return false;
	return true;
}

bool verificaTaxaOcupacao(registroBTree_t *no)
{
	if (no->nroChaves < TAXA_OCUPACAO)
		return false;
	return true;
}

void insertChaveNo(chave_t chave, registroBTree_t *no, int RRN_abaixo, int tipo)
{
	int i;
	for (i = no->nroChaves; chave.id < no->chave[i - 1].id && i > 0; i--)
	{
		no->chave[i] =  no->chave[i - 1];
		no->ptr[i + 1] = no->ptr[i];
	}
	no->nroChaves++;
	no->chave[i] = chave;
	no->ptr[i + 1] = no->ptr[i];
	no->ptr[i] = RRN_abaixo;
}

chave_t split(registroBTree_t *no, chave_t chave, int RRN_abaixo)
{
	int i;
	int mid;
	chave_t chavesTemp[TAXA_OCUPACAO + 1];
	int ptrTemp[ORDEM_ARVORE + 1];

	for (i = 0; i < no->nroChaves; i++)
	{
		chavesTemp[i] = no->chave[i];
		ptrTemp[i] = no->ptr[i];
	}
	ptrTemp[i] = no->ptr[i];

	for (i = no->nroChaves; chave.id < chavesTemp[i - 1].id && i > 0; i--)
	{
		chavesTemp[i] = chavesTemp[i - 1];
		ptrTemp[i + 1] = ptrTemp[i + 1];
	}
	chavesTemp[i] = chave;
	ptrTemp[i + 1] = ptrTemp[i];
	ptrTemp[i] = RRN_abaixo;

	registroBTree_t *novoNo = inicializarRegistroBTree();

	//preciso pegar o cabeçalho? talvez
	for (i = 0; i < TAXA_MINIMA; i++)
	{
		no->chave[i] = chavesTemp[i];
		no->ptr[i] = ptrTemp[i];
		no->chave[i + TAXA_MINIMA] = retiraChave();
		no->ptr[i + 1 + TAXA_MINIMA] = REFERENCIA_NULA;
		
		novoNo->chave[i] = chavesTemp[i + 1 + TAXA_MINIMA];
		novoNo->ptr[i] = ptrTemp[i + 1 + TAXA_MINIMA];
	}
	no->ptr[TAXA_MINIMA] = ptrTemp[TAXA_MINIMA];
	no->nroChaves = TAXA_MINIMA;

	novoNo->ptr[TAXA_MINIMA] = ptrTemp[i + 1 + TAXA_MINIMA];
	novoNo->nroChaves = TAXA_OCUPACAO - TAXA_MINIMA;

	if (no->tipoNo == '0')
	{
		registroBTree_t	*novoNoRaiz;
		novoNoRaiz->tipoNo = '0';
	}
	
	return chavesTemp[TAXA_MINIMA];
}

bool insertBTree(FILE* BTree_file, cabecalhoBTree_t* cab, int RRN_no, int* RRNPromovido, chave_t* chave, int tipo)
{
	/*
		função recursiva:
			desço até o nó folha onde ficaria a minha chave
			se couber a minha chave, insiro
			senão crio um novo nó, split.

			subo na recursão até que promoção == false
	*/
 	// vou para o b.o. do nó
	if (tipo == 1)
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE1) + TAM_REG_BTREE1, SEEK_SET);
	else
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE2) + TAM_REG_BTREE2, SEEK_SET);
	
	// se RRN_no é -1, quer dizer que o nó anterior da recursão é folha, tento fazer a inserção de 'chave' nele
	if (RRN_no == -1)
		return true; 

	// busca da chave
	registroBTree_t *no = lerRegistroBTree(BTree_file, tipo);
	if (buscaBinariaChavesBTree(chave->id, no->chave, 0, no->nroChaves) == -1)
	{
		printf("Inserção de elemento repetido!!");
		exit(0);
	}

	int i = 0;
	while (i < no->nroChaves && chave->id > no->chave[i].id)
		i++;
	int proxRRNregisterBTree = no->ptr[i];

	bool promocao = insertBTree(BTree_file, cab, proxRRNregisterBTree, &proxRRNregisterBTree, chave, tipo);

	// se não precisa de promoção, ignoro
	if (!promocao) return false;
	
	// verificação se precisa splitar o no
	if (verificaTaxaOcupacao(no))
	{
		// a chave a ser inserida será aquela a ser promovida
		chave_t promovida = split(no, (*chave), proxRRNregisterBTree);
		*chave = promovida;
		(*RRNPromovido) = cab->proxRRN;
		cab->proxRRN++;
		return true;
	}
	else
	{
		insertChaveNo((*chave), no, *RRNPromovido, tipo);
		return false;
	}

}
