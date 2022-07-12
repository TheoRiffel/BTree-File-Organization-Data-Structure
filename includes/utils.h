#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funcionalidades.h"

char* readUntil(FILE* a, char delimiter);
int readNumberUntil(FILE* a, char delimiter);
void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter);
void lerRegistrocsv(FILE* arquivo, registro_t* registro, int tipo);
void liberar(registro_t* r);
void inicializarCabecalho(cabecalho_t* cabecalho);
registro_t* inicializarRegistro();
void imprimirRegistro(registro_t* r, cabecalho_t* c);
void liberaRegistro(registro_t* reg);
void liberaStructBusca(buscaParams_t* busca);
void escreverLixo(FILE* binario, int tam, char* lixo);
void inicializaStructBusca(buscaParams_t* busca, int numFiltros, FILE* fptr);
int ehValidoFiltro(registro_t* reg, buscaParams_t* busca);
bool confereArquivos(FILE* a, FILE* b);
bool arquivoConsistente(char status);
void liberaStrings(char* a, char* b, char* c);
int buscaBinaria(int chave, registroIndice_t* vetor, int esq, int dir);
int buscaBinariaChavesBTree(int chave, chave_t* vetor, int esq, int dir);
int compara(const void* reg1, const void* reg2);
void liberaMemSelect(int delCab, char* tArq, char* aEnt, FILE* fptr, cabecalho_t* cab);
int campoEstaPreenchido(char codigo, registro_t* reg);
registro_t* lerRegistroStdin(int tipoRegistro);
buscaParams_t* lerFiltrosUpdate();

#endif
