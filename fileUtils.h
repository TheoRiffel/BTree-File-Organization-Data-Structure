#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionalidades.h"

typedef void (*acaoFiltro)(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo,
	long int posicao, buscaParams_t* novosVals, indice_t* indice);
void acaoFiltro_imprimirRegistro(cabecalho_t* cab, registro_t* reg, FILE* fptr,
	int tipo, long int posicao, buscaParams_t* novosVals, indice_t* indice);
void acaoFiltro_atualizarRegistro(cabecalho_t* cab, registro_t* reg,
	FILE* fptr, int tipo, long int posicao, buscaParams_t* novosVals, indice_t* indice);
void percorreArquivoAplicandofiltro(FILE* fptr, buscaParams_t* busca, int tArquivo,
	cabecalho_t* cab, int final, acaoFiltro acao, idsRemovidos_t* removidos, buscaParams_t* filtros, indice_t* indice);
void atualizarRegistro(int tipo, registro_t* r, FILE* fptr);
void escreverNoArquivo(FILE* arquivo, registro_t* registro, cabecalho_t* cabecalho, int tipo);
void escreverCabecalho(FILE* arquivo, cabecalho_t* cabecalho, int tipo);
void atualizarCabecalho(FILE* arquivo, cabecalho_t* cabecalho, int tipo);
void binarioNaTela(char* nomeArquivoBinario);
static void leVariaveis(FILE* fptr, registro_t* reg, long int posInicio, int tipo);
static void leEstaticos(FILE* fptr, registro_t* reg);
static void lerDescricoes(FILE* fptr, cabecalho_t* cab);
registro_t* lerRegistro(int tipo, FILE* fptr, long int finalArquivo);
cabecalho_t* lerCabecalho(int tipo, FILE* fptr);
void remocaoLogica(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo, long int posicao, buscaParams_t* novosVals, indice_t* indice);
idsRemovidos_t* removerRegistro(FILE* dados, FILE* index, cabecalho_t* cabecalho, int tipo);
void updateRegistros(arquivo_t* arq, buscaParams_t* busca, buscaParams_t* novosVal);
arquivo_t* inicializaStrcArquivo();
void insereRegistroNoArquivo(arquivo_t* arq, indice_t* indice);
void liberarStrctArq(arquivo_t* arq);

#endif