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
registroBTree_t* inicializarBTree();
void escreverRegistroBTree(FILE* BTree_file, registroBTree_t* no, int tipo);
void printaRegistroBTree(registroBTree_t *no, int tipo);
registroBTree_t* lerRegistroBTree(FILE* BTree_file, int tipo);
chave_t* searchBTree(FILE* BTree_file, int RRN_no, int chave, int tipo);

#endif