#ifndef BTREEUTILS_H
#define BTREEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "utils.h"
#include "fileUtils.h"

void escreverCabecalhoBTree(FILE* BTree_file, cabecalhoBTree_t* cabecalho, int tipo);
cabecalhoBTree_t* lerCabecalhoBTree(FILE* BTree_file, int tipo);
registroBTree_t *inicializarRegistroBTree();
void liberarRegistroBTree(registroBTree_t* no);
chave_t retiraChave();
registroBTree_t* inicializarBTree();
void escreverRegistroBTree(FILE* BTree_file, int RRN, registroBTree_t* no, int tipo);
void printaRegistroBTree(registroBTree_t *no, int tipo);
registroBTree_t* lerRegistroBTree(FILE* BTree_file, int tipo);
chave_t* searchBTree(FILE* BTree_file, int RRN_no, int chave, int tipo);
bool verificaNoFolha(registroBTree_t *no);
bool verificaTaxaOcupacao(registroBTree_t *no);
void insertChaveNo(chave_t chave, registroBTree_t *no, int RRN_abaixo, int tipo);
void split2(cabecalhoBTree_t *cab, chave_t key, int r_child, registroBTree_t *no, chave_t *promo_key, int *promo_r_child, registroBTree_t *novoNo);
bool insert2(cabecalhoBTree_t *cab, FILE *BTree_file, int RRN_no, chave_t chave, int *promo_r_child, chave_t *promo_key, int tipo);
void insertBTree(FILE* BTree_file, cabecalhoBTree_t *cabecalho, chave_t chave, int tipo);

#endif