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
	fwrite(&cabecalho->noRaiz, sizeof(int), 1, BTree_file);
	fwrite(&cabecalho->proxRRN, sizeof(int), 1, BTree_file);
	fwrite(&cabecalho->nroNos, sizeof(int), 1, BTree_file);

	int qnt_lixo = tipo == 1? (TAM_REG_BTREE1 - TAM_CABECALHO_BTREE) : (TAM_REG_BTREE2 - TAM_CABECALHO_BTREE);
	for(int i =0; i < qnt_lixo; i++)
		fwrite("$", sizeof(char), 1, BTree_file);
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
	cab->status = '0';
	cab->noRaiz = 0;
	cab->proxRRN = 0;
	cab->nroNos = 0;
	fread(&cab->status, sizeof(char), 1, BTree_file);
	fread(&cab->noRaiz, sizeof(int), 1, BTree_file);
	fread(&cab->proxRRN, sizeof(int), 1, BTree_file);
	fread(&cab->nroNos, sizeof(int), 1, BTree_file);
	
	//quantidade de lixo a ser preenchido no registro de cabeçalho	
	int qnt_lixo = tipo == 1? (TAM_REG_BTREE1 - TAM_CABECALHO_BTREE): (TAM_REG_BTREE2 - TAM_CABECALHO_BTREE);
	fseek(BTree_file, qnt_lixo, SEEK_CUR);

	return cab;
}

/**
 * @brief inicialização das variáveis do cabeçalho da BTree
 * 
 * @return cabecalhoBTree_t* 
 */
cabecalhoBTree_t* inicializarCabecalhoBTree()
{
	cabecalhoBTree_t* c = malloc(sizeof(cabecalhoBTree_t));
	c->noRaiz = -1;
	c->nroNos = -1;
	c->proxRRN = -1;
	c->status = '1';
}

/**
 * @brief Inicializar variáveis do registro da BTree, além de alocar memória
 * 
 * @return registroBTree_t* ponteiro do registro inicializado
 */
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

/**
 * @brief função que seta todos os valores da chave para valores nulos
 * 
 * @return chave_t struct chave com valores nulos
 */
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
registroBTree_t* inicializarBTree()
{
	registroBTree_t* raiz = inicializarRegistroBTree();
	raiz->tipoNo = NO_RAIZ;
	raiz->RRNregistroBTree = 0;
	raiz->nroChaves = 0;

	return raiz;
}

/**
 * @brief escreve registro de um nó no arquivo binário da BTree
 * 
 * @param BTree_file : ponteiro do arquivo da BTree
 * @param no : registro da BTree
 * @param tipo : inteiro informando o tipo do arquivo
 */
void escreverRegistroBTree(FILE* BTree_file, int RRN, registroBTree_t* no, int tipo)
{
	if (tipo == 1)
		fseek(BTree_file, (RRN * TAM_REG_BTREE1) + TAM_REG_BTREE1, SEEK_SET);
	else if (tipo == 2)
		fseek(BTree_file, (RRN * TAM_REG_BTREE2) + TAM_REG_BTREE2, SEEK_SET);

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
	registroBTree_t* no = inicializarRegistroBTree();
	char buffer;

	no->RRNregistroBTree = tipo == 1? (ftell(BTree_file) / TAM_REG_BTREE1) - 1: (ftell(BTree_file) / TAM_REG_BTREE2) - 1;
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

/**
 * @brief função auxiliar: printa registros da BTree
 * 
 * @param no : registro da BTree
 * @param tipo : inteiro informando o tipo do arquivo
 */
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

/**
 * @brief funcção de busca no arquivo da BTree, percorre o arquivo procurando as chaves recursivamente
 * 
 * @param BTree_file : ponteiro do arquivo da BTree
 * @param RRN_no : RRN do nó atual lido. COMEÇA-SE COM RRN_no IGUAL AO RRN DO NÓ RAÍZ.
 * @param chave : chave a ser buscada
 * @param tipo : inteiro informando o tipo do arquivo
 * @return chave_t* : CASO A BUSCA TENHA SIDO SATISFEITA, RETORNA-SE A CHAVE ECONTRADA
 * 					  CASO NÃO: RETORNA-SE UMA CHAVE VAZIA (ID == -1)
 */
chave_t* searchBTree(FILE* BTree_file, int RRN_no, int chave, int tipo)
{
	if (RRN_no == -1)
		return NULL;

	if (tipo == 1)
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE1) + TAM_REG_BTREE1, SEEK_SET);
	else
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE2) + TAM_REG_BTREE2, SEEK_SET);

	registroBTree_t *no = lerRegistroBTree(BTree_file, tipo);

	//nos elementos de cada registro, faço uma busca binária
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

/**
 * @brief verifica se o registro passado como parâmetro é nó folha (todos os filhos NULOS)
 * 
 * @param no : registro a ser verificado
 * @return true : registro é nó folha
 * @return false : registro não é nó folha
 */
bool verificaNoFolha(registroBTree_t *no)
{
	for (int i = 0; i < no->nroChaves; i++)
		if (no->ptr[i] != -1)
			return false;
	return true;
}

/**
 * @brief verifica se o registro atual contém a quantidade limite de chaves
 * 
 * @param no : registro a ser verificado	
 * @return true : registro com espaço indisponível para novas chaves
 * @return false : registro com espaço suficiente para novas chaves
 */
bool verificaTaxaOcupacao(registroBTree_t *no)
{
	if (no->nroChaves < TAXA_OCUPACAO)
		return false;
	return true;
}

/**
 * @brief função que insere uma chave em registro com espaço disponível, reorganizando-o para manter-se
 * 		  a ordenação das chaves pelo seu ID.
 * 
 * @param chave 
 * @param no 
 * @param RRN_abaixo 
 * @param tipo 
 */
void insertChaveNo(chave_t chave, registroBTree_t *no, int RRN_abaixo, int tipo)
{
	int i;

	//insertion sort para ordenação da nova chave
	for (i = no->nroChaves; i > 0 && chave.id < no->chave[i - 1].id ; i--)
	{
		no->chave[i] =  no->chave[i - 1];
		no->ptr[i + 1] = no->ptr[i];
	}
	no->nroChaves++;
	no->chave[i] = chave;

	no->ptr[i + 1] = RRN_abaixo;
}

/**
 * @brief função que 'quebra' um registro em dois, quando o seu limite de chaves é alcançado.
 * 		  Inicializa-se um registro novo (novoNo) à direita do registro atual 'quebrado', com RRN == proxRRN do cabeçalho
 * 			
 * 		  * ELEMENTO MAIS À ESQUERDA DO NOVO VETOR É PROMOVIDO AO REGISTRO PAI.
 * 
 * @param cab : cabeçalho da BTree
 * @param key : chave a ser inserida
 * @param r_child : RRN da chave
 * @param no : registro a ser inserido a chave
 * @param promo_key : variável passado por parâmetro que é a chave promovida
 * @param promo_r_child : variável passado por parâmetro que é a chave promovida
 * @param novoNo : Novo nó criado
 */
void splitNo(cabecalhoBTree_t *cab, chave_t key, int r_child, registroBTree_t *no, chave_t *promo_key, int *promo_r_child, registroBTree_t *novoNo)
{
	int i;
	int mid;
	chave_t chavesTemp[TAXA_OCUPACAO + 1];
	int ptrTemp[ORDEM_ARVORE + 1];

	// copio os elementos do nó para um vetor temporário dos elementos
	for (i = 0; i < no->nroChaves; i++)
	{
		chavesTemp[i] = no->chave[i];
		ptrTemp[i] = no->ptr[i];
	}
	ptrTemp[i] = no->ptr[i];

	// faço a ordenação dos elementos com a nova chave no vetor temporário
	for (i = no->nroChaves; key.id < chavesTemp[i - 1].id && i > 0; i--)
	{
		chavesTemp[i] = chavesTemp[i - 1];
		ptrTemp[i + 1] = ptrTemp[i];
	}
	chavesTemp[i] = key;
	ptrTemp[i + 1] = r_child;

	*promo_r_child = cab->proxRRN;

	// definir os tipos dos nós
	if(no->tipoNo == NO_RAIZ && cab->nroNos < 3)
		no->tipoNo = NO_FOLHA;
	else if (no->tipoNo == NO_RAIZ)
		no->tipoNo = NO_INTERMEDIARIO;

	novoNo->tipoNo = no->tipoNo;

	// REDISTRIBUIÇÃO DOS ELEMENTOS DO VETOR NO E NOVONO
	for (i = 0; i < TAXA_MINIMA; i++)
	{
		no->chave[i] = chavesTemp[i];
		no->ptr[i] = ptrTemp[i];	
		if(i + 1 + TAXA_MINIMA <= TAXA_OCUPACAO)
			novoNo->chave[i] = chavesTemp[i + 1 + TAXA_MINIMA];

		novoNo->ptr[i] = ptrTemp[i + 1 + TAXA_MINIMA];
	}

	no->chave[TAXA_MINIMA] = retiraChave();
	no->ptr[TAXA_MINIMA + 1] = REFERENCIA_NULA;

	no->ptr[TAXA_MINIMA] = ptrTemp[TAXA_MINIMA];
	no->nroChaves = TAXA_MINIMA;

	//PARIDADE DA ORDEM DA ARVORE
	if(ORDEM_ARVORE % 2 != 0)
		novoNo->ptr[TAXA_MINIMA] = ptrTemp[i + 1 + TAXA_MINIMA];
	else
		novoNo->ptr[TAXA_MINIMA - 1] = ptrTemp[i + TAXA_MINIMA];
	
	novoNo->nroChaves = TAXA_OCUPACAO - TAXA_MINIMA;

	*promo_key = chavesTemp[TAXA_MINIMA];
}

/**
 * @brief função que insere uma chave na arvore binária.
 * 
 * @param cab : cabeçalho da BTRee
 * @param BTree_file : ponteiro do arquivo BTree 
 * @param RRN_no : RRN nó (PASSA-SE INICIALMENTE O RRN NÓ RAIZ)
 * @param chave : chave a ser inserida
 * @param promo_r_child : RRN da chave caso haja promoção
 * @param promo_key : chabe caso haja promoção
 * @param tipo : tipo do arquivo
 * @return true : TEM PROMOÇÃO NA CHAVE
 * @return false : NÃO TEM PROMOÇÃO NA CHAVE
 */
bool insertNo(cabecalhoBTree_t *cab, FILE *BTree_file, int RRN_no, chave_t chave, int *promo_r_child, chave_t *promo_key, int tipo)
{
	registroBTree_t *no, *novo_no;
	bool promoted;
	int p_b_rrn;
	chave_t p_b_key;

	// se RRN_no é -1, quer dizer que o nó anterior da recursão é folha, tento fazer a inserção de 'chave' nele
	if (RRN_no == -1)
	{
		*promo_key = chave;
		*promo_r_child = REFERENCIA_NULA;
		return true;
	}

	if (tipo == 1)
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE1) + TAM_REG_BTREE1, SEEK_SET);
	else
		fseek(BTree_file, (RRN_no * TAM_REG_BTREE2) + TAM_REG_BTREE2, SEEK_SET);

	// busca da chave
	no = lerRegistroBTree(BTree_file, tipo);

	if(no->nroChaves > 0)
	{
		if (buscaBinariaChavesBTree(chave.id, no->chave, 0, no->nroChaves) != -1)
		{
			printf("Inserção de elemento repetido!!");
			free(no);
			return false;
		}
	}
	
	int i = 0;
	while (i < no->nroChaves && chave.id > no->chave[i].id)
		i++;
	int proxRRNregisterBTree = no->ptr[i];

	// Começo da função recursiva
	bool promocao = insertNo(cab, BTree_file, proxRRNregisterBTree, chave, &p_b_rrn, &p_b_key, tipo);

	// caso não haja promoção, eu só desempilho a stack toda
	if (!promocao)
	{
		free(no);
		return promocao;
	}
	
	//caso de inserção com split
	if (verificaTaxaOcupacao(no))
	{
		novo_no = inicializarRegistroBTree();
		splitNo(cab, p_b_key, p_b_rrn, no, promo_key, promo_r_child, novo_no);
		escreverRegistroBTree(BTree_file, RRN_no, no, tipo);
		escreverRegistroBTree(BTree_file, *promo_r_child, novo_no, tipo);
		cab->nroNos++;
		cab->proxRRN++;
		free(novo_no);
		free(no);
		return true;
	}
	else // insiro a chave no vetor normalmente
	{
		insertChaveNo(p_b_key, no, p_b_rrn, tipo);
		escreverRegistroBTree(BTree_file, RRN_no, no, tipo);

		free(no);
		return false;
	}
}
/**
 * @brief Função que insere uma chave na árvore B 
 * 
 * @param BTree_file ponteiro para o arquivo da árvore B
 * @param cabecalho struct contendo as informações do cabeçalho da árvore
 * @param chave chave a ser inserida
 * @param tipo tipo do arquivo
 */
void insertBTree(FILE* BTree_file, cabecalhoBTree_t *cabecalho, chave_t chave, int tipo)
{
	bool promoted;
	int promoRRN;
	registroBTree_t *raiz;
	chave_t chave_promo;

	if (cabecalho->noRaiz == -1) // se não tiver uma raiz, crio a árvore com a chave na raiz
	{
		raiz = inicializarBTree();
		insertChaveNo(chave, raiz, -1, tipo);
		escreverRegistroBTree(BTree_file, raiz->RRNregistroBTree, raiz, tipo);
		cabecalho->noRaiz = raiz->RRNregistroBTree;
		cabecalho->nroNos = 1;
		cabecalho->proxRRN = 1;

		free(raiz);
		return;
	}
	// se tiver, insiro a chave e retorno uma possível promoção após split na raiz
	promoted = insertNo(cabecalho, BTree_file, cabecalho->noRaiz, chave, &promoRRN, &chave_promo, tipo);

	if (promoted) // se tiver split na raiz, cria uma nova com o valor que foi promovido   
	{             // e escreve no arquivo
		raiz = inicializarRegistroBTree();
		raiz->tipoNo = NO_RAIZ;
		raiz->nroChaves = 1;
		raiz->chave[0] = chave_promo;
		raiz->ptr[0] = cabecalho->noRaiz;
		raiz->ptr[1] = promoRRN;
		raiz->RRNregistroBTree = cabecalho->proxRRN;
		escreverRegistroBTree(BTree_file, raiz->RRNregistroBTree, raiz, tipo);
		cabecalho->noRaiz = raiz->RRNregistroBTree;
		cabecalho->nroNos++;
		cabecalho->proxRRN++;
	
		free(raiz);
	}

}