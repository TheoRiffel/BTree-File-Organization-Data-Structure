#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "indexUtils.h"
#include "utils.h"
#include "fileUtils.h"

/**
 * @brief Escreve o cabe�alho em um arquivo bin�rio
 *
 * @param binario : ponteiro para o arquivo
 * @param cabecalho : struct contendo os dados do cabe�alho
 * @param tipo : tipo do arquivo
 */
void escreverCabecalho(FILE* binario, cabecalho_t* cabecalho, int tipo)
{
	inicializarCabecalho(cabecalho);

	fseek(binario, 0, 0);
	fwrite(&cabecalho->status, 1, 1, binario);

	if (tipo == 1)
		fwrite(&cabecalho->topoA, sizeof(int), 1, binario);
	else if (tipo == 2)
		fwrite(&cabecalho->topoB, sizeof(long int), 1, binario);

	fwrite(cabecalho->descricao, 1, 40, binario);
	fwrite(cabecalho->desC1, 1, 22, binario);
	fwrite(cabecalho->desC2, 1, 19, binario);
	fwrite(cabecalho->desC3, 1, 24, binario);
	fwrite(cabecalho->desC4, 1, 8, binario);
	fwrite(&cabecalho->codC5, 1, 1, binario);
	fwrite(cabecalho->desC5, 1, 16, binario);
	fwrite(&cabecalho->codC6, 1, 1, binario);
	fwrite(cabecalho->desC6, 1, 18, binario);
	fwrite(&cabecalho->codC7, 1, 1, binario);
	fwrite(cabecalho->desC7, 1, 19, binario);

	if (tipo == 1)
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, binario);
	else if (tipo == 2)
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, binario);

	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, binario);
}

/**
 * @brief Atualiza o cabe�alho ap�s altera��es feitas na tabela (arquivo bin�rio)
 *
 * @param binario : ponteiro para o arquivo
 * @param cabecalho : ponteiro para a struct que cont�m os dados do cabe�alho
 * @param tipo : tipo do arquivo
 */
void atualizarCabecalho(FILE* binario, cabecalho_t* cabecalho, int tipo)
{
	fseek(binario, 0, 0);
	fwrite(&cabecalho->status, 1, 1, binario);

	if (tipo == 1)
		fwrite(&cabecalho->topoA, sizeof(int), 1, binario);
	else if (tipo == 2)
		fwrite(&cabecalho->topoB, sizeof(long int), 1, binario);

	if (tipo == 1)
	{
		fseek(binario, 174, 0);
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, binario);
	}
	else if (tipo == 2)
	{
		fseek(binario, 178, 0);
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, binario);
	}
	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, binario);
}

/**
 * @brief Escreve um �nico registro em um arquivo bin�rio
 *
 * @param binario : Ponteiro para o arquivo
 * @param registro : Ponteiro para a struct que cont�m os dados
 * do registro
 * @param cabecalho : Ponteiro para a struct que cont�m os dados
 * do cabe�alho
 * @param tipo : Tipo do arquivo
 */
void escreverNoArquivo(FILE* binario, registro_t* registro, cabecalho_t* cabecalho, int tipo)
{
	long int tam = ftell(binario);
	fwrite(&registro->removido, 1, 1, binario);
	if (tipo == 2)
		fwrite(&registro->tamRegistro, sizeof(int), 1, binario);
	if (tipo == 1)
		fwrite(&registro->proxA, sizeof(int), 1, binario);
	else if (tipo == 2)
		fwrite(&registro->proxB, sizeof(long int), 1, binario);
	fwrite(&registro->id, sizeof(int), 1, binario);
	fwrite(&registro->ano, sizeof(int), 1, binario);
	fwrite(&registro->qtt, sizeof(int), 1, binario);
	fwrite(registro->sigla, 1, 2, binario);

	if (registro->cidade != NULL)
	{
		fwrite(&registro->tamCidade, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC5, 1, 1, binario);
		fwrite(registro->cidade, 1, registro->tamCidade, binario);
	}

	if (registro->marca != NULL)
	{
		fwrite(&registro->tamMarca, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC6, 1, 1, binario);
		fwrite(registro->marca, 1, registro->tamMarca, binario);
	}

	if (registro->modelo != NULL)
	{
		fwrite(&registro->tamModelo, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC7, 1, 1, binario);
		fwrite(registro->modelo, 1, registro->tamModelo, binario);
	}

	tam = ftell(binario) - tam;
	if (tipo == 1) escreverLixo(binario, 97 - tam, "$");
}

/**
 * @brief Atualiza um único registro em um arquivo binário
 *
 * @param ftpr : Ponteiro para o arquivo
 * @param registro : Ponteiro para a struct que contém os dados
 * do registro
 * @param tipo : Tipo do arquivo
 */
void atualizarRegistro(int tipo, registro_t* r, FILE* fptr)
{
	fwrite(&r->removido, sizeof(char), 1, fptr);
	if (tipo == 1)
	{
		fwrite(&r->proxA, sizeof(int), 1, fptr);
		fseek(fptr, r->tamRegistro - 4, SEEK_CUR);
	}
	else if (tipo == 2)
	{
		fseek(fptr, 4, SEEK_CUR);
		fwrite(&r->proxB, sizeof(long int), 1, fptr);
		fseek(fptr, r->tamRegistro - 8, SEEK_CUR);
	}
}

cabecalho_t* lerCabecalho(int tipo, FILE* fptr) {

	cabecalho_t* cab = malloc(sizeof(cabecalho_t));
	inicializarCabecalho(cab);
	fread(&cab->status, 1, 1, fptr);

	if (tipo == 1) {
		fread(&cab->topoA, 4, 1, fptr);
	}
	else if (tipo == 2) {
		fread(&cab->topoB, 8, 1, fptr);
	}

	lerDescricoes(fptr, cab);

	if (tipo == 1) {
		fread(&cab->proxRRN, 4, 1, fptr);
	}
	else {
		fread(&cab->proxByteOffset, 8, 1, fptr);
	}

	fread(&cab->nroRegRem, 4, 1, fptr);

	return cab;

}


registro_t* lerRegistro(int tipo, FILE* fptr, long int finalArquivo) {
	long int posInicio = ftell(fptr);
	registro_t* reg = inicializarRegistro();

	fread(&reg->removido, 1, 1, fptr);
	if (tipo == 1) {
		fread(&reg->proxA, 4, 1, fptr);
	}
	else {
		fread(&reg->tamRegistro, sizeof(int), 1, fptr);
		fread(&reg->proxB, sizeof(long int), 1, fptr);
	}

	leEstaticos(fptr, reg);
	
	if (tipo == 2 && ftell(fptr) - posInicio - 5 != reg->tamRegistro)
		leVariaveis(fptr, reg, posInicio, tipo);
	else if (tipo == 1)
		leVariaveis(fptr, reg, posInicio, tipo);

	char buffer = '$';
	while (ftell(fptr) != finalArquivo && buffer == '$')
		buffer = fgetc(fptr);

	if(buffer != '$') fseek(fptr, -1, SEEK_CUR);

	return reg;
}

static void lerDescricoes(FILE* fptr, cabecalho_t* cab) {

	fread(&cab->descricao, 40, 1, fptr);
	fread(&cab->desC1, 22, 1, fptr);
	fread(&cab->desC2, 19, 1, fptr);
	fread(&cab->desC3, 24, 1, fptr);
	fread(&cab->desC4, 8, 1, fptr);
	fread(&cab->codC5, 1, 1, fptr);
	fread(&cab->desC5, 16, 1, fptr);
	fread(&cab->codC6, 1, 1, fptr);
	fread(&cab->desC6, 18, 1, fptr);
	fread(&cab->codC7, 1, 1, fptr);
	fread(&cab->desC7, 19, 1, fptr);

}

static void leEstaticos(FILE* fptr, registro_t* reg) {

	fread(&reg->id, sizeof(int), 1, fptr);
	fread(&reg->ano, sizeof(int), 1, fptr);
	fread(&reg->qtt, sizeof(int), 1, fptr);
	fread(&reg->sigla, sizeof(char), 2, fptr);

}

static void leVariaveis(FILE* fptr, registro_t* reg, long int posInicio, int tipo) {

	char codigo;
	int tamanho;

	reg->codC5 = reg->codC6 = reg->codC7 = '$';

	for (int i = 0; i < 3; i++) {
		
		if (tipo == 2 && ftell(fptr) - posInicio - 5 == reg->tamRegistro) break;

		char buffer;
		fread(&buffer, sizeof(char), 1, fptr);
		if (buffer == '$') break;
		else fseek(fptr, -1, SEEK_CUR);

		fread(&tamanho, sizeof(int), 1, fptr);
		fread(&codigo, sizeof(char), 1, fptr);

		if (feof(fptr)) return;

		if (codigo == '$') {
			break;
		}

		switch (codigo) {
		case '0': reg->tamCidade = tamanho;
			reg->cidade = malloc(sizeof(char) * tamanho);
			fread(reg->cidade, sizeof(char), tamanho, fptr);
			break;
		case '1': reg->tamMarca = tamanho;
			reg->marca = malloc(sizeof(char) * tamanho);
			fread(reg->marca, sizeof(char), tamanho, fptr);
			break;
		case '2': reg->tamModelo = tamanho;
			reg->modelo = malloc(sizeof(char) * tamanho);
			fread(reg->modelo, sizeof(char), tamanho, fptr);
			break;
		default: 
			return;
			break;
		}

		switch (i) {
		case 0: reg->codC5 = codigo;
			break;
		case 1: reg->codC6 = codigo;
			break;
		case 2: reg->codC7 = codigo;
			break;
		}
		if (codigo == '2') break;
	}

}

int maiorReg = 0; // variável global que guarda o tamanho do maior registro removido

/**
 * @brief Remove logicamente um registro do arquivo de dados
 *
 * @param cab : cabeçalho do arquivo
 * @param reg : registro a ser removido
 * @param fptr : ponteiro para o arquivo
 * @param tipo : tipo do arquivo
 * @param posicao : offset do registro a ser removido
 */
void remocaoLogica(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo, long int posicao)
{
	reg->removido = REGISTRO_REMOVIDO;
	cab->nroRegRem++;

	if (tipo == 1)
	{ // insere na pilha de removidos

		reg->proxA = cab->topoA;
		int RRN = ((posicao - TAM_CABECALHO1) / TAM_REG);
		cab->topoA = RRN;
	}
	else if (tipo == 2)
	{// insere na lista de forma ordenada, uma vez que a inserção ordenada
	 // diminui a complexidade para a inserção de novos registros, melhorando 
	 // a experiência do usuário

		// caso : inserção no início da lista
		if (reg->tamRegistro > maiorReg)
		{
			maiorReg = reg->tamRegistro;
			reg->proxB = cab->topoB;
			cab->topoB = posicao;
		}
		// caso : buscar o lugar a ser inserido
		else
		{
			bool inserido = false;
			fseek(fptr, cab->topoB, SEEK_SET);
			long int posAnterior = ftell(fptr);
			long int prox;
			registro_t* newReg = lerRegistro(tipo, fptr, cab->proxByteOffset);

			while (inserido == false)
			{
				if (newReg->tamRegistro < reg->tamRegistro) // achei o lugar
				{
					prox = ftell(fptr) - newReg->tamRegistro - 5;
					fseek(fptr, posAnterior, SEEK_SET);
					newReg = lerRegistro(tipo, fptr, cab->proxByteOffset);
					newReg->proxB = posicao;
					fseek(fptr, posAnterior, SEEK_SET);
					atualizarRegistro(tipo, newReg, fptr);

					fseek(fptr, posicao, SEEK_SET);
					reg->proxB = prox;
					atualizarRegistro(tipo, reg, fptr);

					inserido = true;
				}
				else
				{
					posAnterior = ftell(fptr) - newReg->tamRegistro - 5;
					
					if (newReg->proxB == -1)//inserir no final da lista
					{
						newReg->proxB = posicao;
						fseek(fptr, posAnterior, SEEK_SET);
						atualizarRegistro(tipo, newReg, fptr);
						reg->proxB = -1;
						fseek(fptr, posicao, SEEK_SET);
						atualizarRegistro(tipo, reg, fptr);

						inserido = true;
					}
					else //continuar a busca
					{
						fseek(fptr, newReg->proxB, SEEK_SET);
						newReg = lerRegistro(tipo, fptr, cab->proxByteOffset);
					}
				}
			}
		}

	}
	fseek(fptr, posicao, SEEK_SET);
	atualizarRegistro(tipo, reg, fptr);

}

/**
 * @brief Remove um registro do arquivo de dados a partir de buscas
 * e da chamada de funções de remoção lógica
 *
 * @param dados : ponteiro para o arquivo de dados
 * @param index : ponteiro para o arquivo de índices
 * @param cabecalho : cabeçalho do arquivo
 * @param tipo : tipo do arquivo
 */
idsRemovidos_t* removerRegistro(FILE* dados, FILE* index, cabecalho_t* cabecalho, int tipo)
{
	buscaParams_t* busca = malloc(sizeof(buscaParams_t));
	idsRemovidos_t* removidos = malloc(sizeof(idsRemovidos_t));
	removidos->tamanho = 0;
	removidos->ids = NULL;

	int numFiltros = readNumberUntil(stdin, ' ');
	inicializaStructBusca(busca, numFiltros, stdin);
	
	if (strcmp(busca->filtros[0], "id") != 0)
	{ // se é fornecido o id, busca no arquivo de índices
		registroIndice_t* reg = buscaIndex(indiceParaRAM(index, tipo), atoi(busca->filtros[0]));
		if (reg != NULL)
		{
			long int pos = 0, final = 0;
			if (tipo == 1)
			{
				pos = (reg->RRN * TAM_REG) + TAM_CABECALHO1;
				final = TAM_CABECALHO1 + (cabecalho->proxRRN * TAM_REG);
			}
			else if (tipo == 2)
			{
				pos = reg->byteOffset;
				final = cabecalho->proxByteOffset;
			}

			fseek(dados, pos, SEEK_SET);
			registro_t* registro = lerRegistro(tipo, dados, final);

			if (ehValidoFiltro(registro, busca))
			{ // se o campo corresponde ao registro, remove
				if (removidos != NULL)
				{
					removidos->ids = realloc(removidos->ids,
						(removidos->tamanho + 1) * sizeof(int));
					removidos->ids[removidos->tamanho] = reg->id;
					removidos->tamanho++;
				}
				remocaoLogica(cabecalho, registro, dados, tipo, pos);
			}
		}
	}
	else
	{
		if (tipo == 1)
		{ // remoção em registros de tamanho fixo
			percorreArquivoAplicandofiltro(dados, busca, tipo, cabecalho,
				cabecalho->proxRRN * TAM_REG, &remocaoLogica, removidos);
		}
		else if (tipo == 2)
		{ // remoção em registros de tamanho variável (worst fit)
			percorreArquivoAplicandofiltro(dados, busca, tipo, cabecalho,
				cabecalho->proxByteOffset, &remocaoLogica, removidos);
		}
	}
	liberaStructBusca(busca);

	return removidos;
}

void acaoFiltro_imprimirRegistro(cabecalho_t *cab, registro_t* reg, FILE* fptr, int tipo, long int posicao){
	imprimirRegistro(reg, cab);
}

void percorreArquivoAplicandofiltro(FILE* fptr, buscaParams_t* busca, int tArquivo, 
	cabecalho_t* cab, int final, acaoFiltro acao, idsRemovidos_t* removidos) {
	
	if (tArquivo == 1) fseek(fptr, TAM_CABECALHO1, SEEK_SET);
	if (tArquivo == 2) fseek(fptr, TAM_CABECALHO2, SEEK_SET);

	long int posicao;
	do
	{
		//Lendo registro
		long int final = 0;
		if (tArquivo == 1) final = (cab->proxRRN * TAM_REG) + TAM_CABECALHO1;
		else if (tArquivo == 2) final = cab->proxByteOffset;
		posicao = ftell(fptr);
		registro_t* reg = lerRegistro(tArquivo, fptr, final);

		//Se registro não estiver removido e bater com o filtro, executa ação
		if (reg->removido == '0')
		{
			if (ehValidoFiltro(reg, busca))
			{
				if (removidos != NULL)
				{
					removidos->ids = realloc(removidos->ids,
						(removidos->tamanho + 1) * sizeof(int));
					removidos->ids[removidos->tamanho] = reg->id;
					removidos->tamanho++;
				}

				acao(cab, reg, fptr, tArquivo, posicao);
			}
		}

		liberaRegistro(reg);
	} while (ftell(fptr) < final);

}

arquivo_t* inicializaStrcArquivo() {

	char* tipoArquivo = readUntil(stdin, ' ');
	char* arquivoEntrada = readUntil(stdin, ' ');

	FILE* fptr = fopen(arquivoEntrada, "r+b");

	if (!fptr)
	{
		printf("Falha ao abrir o arquivo\n");
		exit(1);
	}

	arquivo_t* arq = malloc(sizeof(arquivo_t));

	if (!strcmp(tipoArquivo, "tipo1"))
	{
		arq->tipo = 1;
		arq->cab = lerCabecalho(1, fptr);
		arq->final = TAM_CABECALHO1 + arq->cab->proxRRN * TAM_REG;
	}
	else if (!strcmp(tipoArquivo, "tipo2"))
	{
		arq->tipo == 2;
		arq->cab = lerCabecalho(2, fptr);
		arq->final = arq->cab->proxByteOffset;
	}
	else
	{
		printf("Falha no processamento do arquivo.\n");
		liberaMemSelect(0, tipoArquivo, arquivoEntrada, fptr, arq->cab);
		exit(1);
	}

	return arq;
}

static long int retornaIndiceDisponivel(arquivo_t* arq, int* finalDoArquivo)
{

	if (arq->tipo == 1)
	{
		if (arq->cab->topoA == -1)
		{
			*finalDoArquivo = 1;
			return TAM_CABECALHO1 + arq->cab->proxRRN * TAM_REG;
		}
		*finalDoArquivo = 0;
		return TAM_CABECALHO1 + arq->cab->topoA * TAM_REG;
	}

	if (arq->cab->topoB == -1)
	{
		*finalDoArquivo = 1;
		return arq->cab->proxByteOffset;
	}
	*finalDoArquivo = 0;
	return arq->cab->topoB;
}

static void insereRegistroRRN(arquivo_t* arq, int posicaoInsercao, int fimArquivo)
{

	fseek(arq->fptr, posicaoInsercao, SEEK_SET);

	if (!fimArquivo)
	{
		// verificando se ele está deletado
		if (fgetc(arq->fptr) != REGISTRO_REMOVIDO)
		{
			printf("Falha no processamento do arquivo.");
			return;
		}
		// Armazenando a posição do próx RRN livre
		fread(&arq->cab->topoA, 4, 1, arq->fptr);
		fseek(arq->fptr, -5, SEEK_CUR);
	}

	escreverNoArquivo(arq->fptr, arq->reg, arq->cab, arq->tipo);

	if (fimArquivo)
	{
		arq->cab->proxRRN += 1;
	}
	arq->cab->nroRegRem -= 1;
}

static void insereRegistroOffset(arquivo_t* arq, long int posicaoInsercao, int fimArquivo)
{

	fseek(arq->fptr, posicaoInsercao, SEEK_SET);

	long int proxbyteOffset;

	if (!fimArquivo)
	{

		// verificando se ele está deletado
		if (fgetc(arq->fptr) != REGISTRO_REMOVIDO)
		{
			printf("Falha no processamento do arquivo.");
			return;
		}
		fseek(arq->fptr, 4, SEEK_CUR);

		fread(&arq->cab->topoB, 8, 1, arq->fptr);
		fseek(arq->fptr, -13, SEEK_CUR);
	}
	else
	{
		arq->cab->proxByteOffset += arq->reg->tamRegistro;
	}

	escreverNoArquivo(arq->fptr, arq->reg, arq->cab, arq->tipo);

	arq->cab->nroRegRem -= 1;
}

static void atualizaArquivoIndexacaoTipo1(FILE* arqIndex, int id, int RRN)
{

	indice_t* indexLst = indiceParaRAM(arqIndex, 1);
}

void insereRegistroNoArquivo(arquivo_t* arq)
{
	int flagFimDeArquivo = 0;
	if (arq->tipo == 1)
	{
		int posicaoInsercao = (int)retornaIndiceDisponivel(arq, &flagFimDeArquivo);
		insereRegistroRRN(arq, posicaoInsercao, flagFimDeArquivo);

		//colocar funçoes nova da maju de indice

		return;
	}

	long int posicaoInsercao = retornaIndiceDisponivel(arq, &flagFimDeArquivo);
	insereRegistroOffset(arq, posicaoInsercao, flagFimDeArquivo);
}