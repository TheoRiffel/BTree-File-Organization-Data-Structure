#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../includes/funcionalidades.h"
#include "../includes/indexUtils.h"
#include "../includes/utils.h"
#include "../includes/fileUtils.h"

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

static void escreverNoArquivoVariaveis(FILE* binario, registro_t* registro, cabecalho_t* cabecalho)
{

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

	escreverNoArquivoVariaveis(binario, registro, cabecalho);

	tam = ftell(binario) - tam;
	if (tipo == 1)
		escreverLixo(binario, 97 - tam, "$");
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

/**
 * @brief lê um arquivo binário e retorna o cabeçalho inicializado
 * @param tipo tipo do arquivo que está sendo lido
 * @param fptr ponteiro para o arquivo binário
 */
cabecalho_t* lerCabecalho(int tipo, FILE* fptr)
{

	cabecalho_t* cab = malloc(sizeof(cabecalho_t));
	inicializarCabecalho(cab);
	fread(&cab->status, 1, 1, fptr);

	if (tipo == 1)
	{
		fread(&cab->topoA, 4, 1, fptr);
	}
	else if (tipo == 2)
	{
		fread(&cab->topoB, 8, 1, fptr);
	}

	lerDescricoes(fptr, cab);

	if (tipo == 1)
	{
		fread(&cab->proxRRN, 4, 1, fptr);
	}
	else
	{
		fread(&cab->proxByteOffset, 8, 1, fptr);
	}

	fread(&cab->nroRegRem, 4, 1, fptr);

	return cab;
}

/**
 * @brief lê um registro e retorna a struct registro_t com os dados recebido
 * @param tipo tipo 1 ou 2 corresponde ao tipo do arquivo
 * @param fptr filepointer do arquivo binário que está sendo lido
 */
registro_t* lerRegistro(int tipo, FILE* fptr, long int finalArquivo)
{
	long int posInicio = ftell(fptr);
	registro_t* reg = inicializarRegistro();

	fread(&reg->removido, 1, 1, fptr);
	if (tipo == 1)
	{
		fread(&reg->proxA, 4, 1, fptr);
	}
	else
	{
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

	if (buffer != '$')
		fseek(fptr, -1, SEEK_CUR);

	return reg;
}

/**
 * @brief lê campos d descrição de um arquivo binário e atribui ao cabeçalho fornecido
 * @param fptr ponteiro para o arquivo binário
 * @param cab ponteiro para o cabeçalho
 */
static void lerDescricoes(FILE* fptr, cabecalho_t* cab)
{

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

/**
 * @brief lê campos estáticos de um arquivo binário e atribui a um registro
 * @param fptr ponteiro para o arquivo binário
 * @param reg ponteiro para o registro que recebe-ra os dados
 */
static void leEstaticos(FILE* fptr, registro_t* reg)
{

	fread(&reg->id, sizeof(int), 1, fptr);
	fread(&reg->ano, sizeof(int), 1, fptr);
	fread(&reg->qtt, sizeof(int), 1, fptr);
	fread(&reg->sigla, sizeof(char), 2, fptr);
}

/**
 * @brief lê campos variáveis de um arquivo binário e atribui a um registro
 * @param fptr ponteiro para o arquivo binário
 * @param reg ponteiro para o registro que recebe-ra os dados
 */
static void leVariaveis(FILE* fptr, registro_t* reg, long int posInicio, int tipo)
{

	char codigo;
	int tamanho;

	reg->codC5 = reg->codC6 = reg->codC7 = '$';

	for (int i = 0; i < 3; i++)
	{

		if (tipo == 2 && ftell(fptr) - posInicio - 5 == reg->tamRegistro)
			break;

		char buffer;
		fread(&buffer, sizeof(char), 1, fptr);
		if (buffer == '$')
			break;
		else
			fseek(fptr, -1, SEEK_CUR);

		fread(&tamanho, sizeof(int), 1, fptr);
		fread(&codigo, sizeof(char), 1, fptr);

		if (feof(fptr))
			return;

		if (codigo == '$')
		{
			break;
		}

		switch (codigo)
		{
		case '0':
			reg->tamCidade = tamanho;
			reg->cidade = malloc(sizeof(char) * tamanho);
			fread(reg->cidade, sizeof(char), tamanho, fptr);
			break;
		case '1':
			reg->tamMarca = tamanho;
			reg->marca = malloc(sizeof(char) * tamanho);
			fread(reg->marca, sizeof(char), tamanho, fptr);
			break;
		case '2':
			reg->tamModelo = tamanho;
			reg->modelo = malloc(sizeof(char) * tamanho);
			fread(reg->modelo, sizeof(char), tamanho, fptr);
			break;
		default:
			return;
			break;
		}

		switch (i)
		{
		case 0:
			reg->codC5 = codigo;
			break;
		case 1:
			reg->codC6 = codigo;
			break;
		case 2:
			reg->codC7 = codigo;
			break;
		}
		if (codigo == '2')
			break;
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
void remocaoLogica(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo, long int posicao, buscaParams_t* novosVals, indice_t* indice)
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
	{ // insere na lista de forma ordenada, uma vez que a inserção ordenada
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

					if (newReg->proxB == -1) // inserir no final da lista
					{
						newReg->proxB = posicao;
						fseek(fptr, posAnterior, SEEK_SET);
						atualizarRegistro(tipo, newReg, fptr);
						reg->proxB = -1;
						fseek(fptr, posicao, SEEK_SET);
						atualizarRegistro(tipo, reg, fptr);

						inserido = true;
					}
					else // continuar a busca
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

static void removerRegistros(arquivo_t* arq, buscaParams_t* busca, idsRemovidos_t* removidos)
{

	if (strcmp(busca->filtros[0], "id") != 0)
	{ // se é fornecido o id, busca no arquivo de índices
		registroIndice_t* reg = buscaIndex(indiceParaRAM(arq->indexfptr, arq->tipo), atoi(busca->filtros[0]));
		if (reg != NULL)
		{
			long int pos = 0, final = 0;
			if (arq->tipo == 1)
			{
				pos = (reg->RRN * TAM_REG) + TAM_CABECALHO1;
				final = TAM_CABECALHO1 + (arq->cab->proxRRN * TAM_REG);
			}
			else if (arq->tipo == 2)
			{
				pos = reg->byteOffset;
				final = arq->cab->proxByteOffset;
			}

			fseek(arq->fptr, pos, SEEK_SET);
			registro_t* registro = lerRegistro(arq->tipo, arq->fptr, final);

			if (ehValidoFiltro(registro, busca))
			{ // se o campo corresponde ao registro, remove
				if (removidos != NULL)
				{
					removidos->ids = realloc(removidos->ids,
						(removidos->tamanho + 1) * sizeof(int));
					removidos->ids[removidos->tamanho] = reg->id;
					removidos->tamanho++;
				}
				remocaoLogica(arq->cab, registro, arq->fptr, arq->tipo, pos, NULL, NULL);
			}
		}
	}
	else
	{
		if (arq->tipo == 1)
		{ // remoção em registros de tamanho fixo
			percorreArquivoAplicandofiltro(arq->fptr, busca, arq->tipo, arq->cab,
				arq->cab->proxRRN * TAM_REG, &remocaoLogica, removidos, NULL, NULL);
		}
		else if (arq->tipo == 2)
		{ // remoção em registros de tamanho variável (worst fit)
			percorreArquivoAplicandofiltro(arq->fptr, busca, arq->tipo, arq->cab,
				arq->cab->proxByteOffset, &remocaoLogica, removidos, NULL, NULL);
		}
	}
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
	arquivo_t* arq = malloc(sizeof(arquivo_t));
	removidos->tamanho = 0;
	removidos->ids = NULL;
	arq->cab = cabecalho;
	arq->tipo = tipo;
	arq->fptr = dados;
	arq->indexfptr = index;

	int numFiltros = readNumberUntil(stdin, ' ');
	inicializaStructBusca(busca, numFiltros, stdin);

	removerRegistros(arq, busca, removidos);

	liberaStructBusca(busca);
	return removidos;
}

void acaoFiltro_imprimirRegistro(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo, long int posicao, buscaParams_t* novosVals, indice_t* indice)
{
	imprimirRegistro(reg, cab);
}

/**
 * @brief realiza uma busca pazrametriada por todo arquivo
 * e aplica a função que for fornecida por argumento nos registros válidos
 *
 * @param fptr: arquivo de referência para busca
 * @param busca: struct de busca com filtros
 * @param tArquivo: tipo do arquivo
 * @param cab: cabeçalho do arquivo
 * @param final: final do arquivo
 * @param idsRemovidos: Utilizado para a função
 * @param buscaParams: filtros
 */
void percorreArquivoAplicandofiltro(FILE* fptr, buscaParams_t* busca, int tArquivo,
	cabecalho_t* cab, int final, acaoFiltro acao, idsRemovidos_t* removidos, buscaParams_t* filtros, indice_t* indice)
{

	if (tArquivo == 1)
		fseek(fptr, TAM_CABECALHO1, SEEK_SET);
	if (tArquivo == 2)
		fseek(fptr, TAM_CABECALHO2, SEEK_SET);

	long int posicao;
	do
	{
		// Lendo registro
		long int final = 0;
		if (tArquivo == 1)
			final = (cab->proxRRN * TAM_REG) + TAM_CABECALHO1;
		else if (tArquivo == 2)
			final = cab->proxByteOffset;
		posicao = ftell(fptr);
		registro_t* reg = lerRegistro(tArquivo, fptr, final);

		// Se registro não estiver removido e bater com o filtro, executa ação
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

				acao(cab, reg, fptr, tArquivo, posicao, filtros, indice);
			}
		}

		liberaRegistro(reg);
	} while (ftell(fptr) < final);
}

/**
 * @brief inicializa a struct de arquivo
 *
 */
arquivo_t* inicializaStrcArquivo()
{

	arquivo_t* arq = malloc(sizeof(arquivo_t));

	char* tipoArquivo = readUntil(stdin, ' ');
	arq->nomeArqDados = readUntil(stdin, ' ');

	arq->fptr = fopen(arq->nomeArqDados, "r+b");

	if (!arq->fptr)
	{
		printf("Falha ao abrir o arquivo\n");
		exit(1);
	}

	if (!strcmp(tipoArquivo, "tipo1"))
	{
		arq->tipo = 1;
		arq->cab = lerCabecalho(1, arq->fptr);
		arq->final = TAM_CABECALHO1 + arq->cab->proxRRN * TAM_REG;
	}
	else if (!strcmp(tipoArquivo, "tipo2"))
	{
		arq->tipo = 2;
		arq->cab = lerCabecalho(2, arq->fptr);
		arq->final = arq->cab->proxByteOffset;
	}
	else
	{
		printf("Falha no processamento do arquivo.\n");
		liberaMemSelect(0, tipoArquivo, arq->nomeArqDados, arq->fptr, arq->cab);
		exit(1);
	}

	return arq;
}
/**
 * @brief Verifica se um registrador pode ser encaixado em uma posição
 * @return retorna booleano
 */
static int verificaFit(arquivo_t* arq, long int pos)
{
	// Armazena a posição inicial para não afetar o fluxo do restante do programa
	long int posInicial = ftell(arq->fptr);
	// Vai até a posição onde o novo registro será inserido
	fseek(arq->fptr, pos, SEEK_SET);
	// armazena o tamanho do registro no espaço onde será feita inserção
	registro_t* reg = lerRegistro(2, arq->fptr, arq->final);

	int espacoDisp = reg->tamRegistro;
	liberaRegistro(reg);
	// volta para posição inicial
	fseek(arq->fptr, posInicial, SEEK_SET);
	// retorna se o espaço é maior ou igual ao tamanho do registro
	return espacoDisp >= arq->reg->tamRegistro;
}
/**
 * @brief recebe um arquivo e indica qual é a posição disponível para inserir
 * o registro dentro do arquivo
 * @param arq: o arquivo que que será verificado
 * @param finalDoArquivo: uma flag que indica se a posição retornada corresponde
 * ao final do registro
 */
static long int retornaIndiceDisponivel(arquivo_t* arq, int* finalDoArquivo)
{

	if (arq->tipo == 1)
	{
		if (arq->cab->topoA == -1)
		{
			*finalDoArquivo = 1;
			return arq->cab->proxRRN;
		}
		*finalDoArquivo = 0;
		return arq->cab->topoA;
	}

	if (arq->cab->topoB == -1)
	{
		*finalDoArquivo = 1;
		return arq->cab->proxByteOffset;
	}
	if (!verificaFit(arq, arq->cab->topoB))
	{
		*finalDoArquivo = 1;
		return arq->cab->proxByteOffset;
	}

	*finalDoArquivo = 0;
	return arq->cab->topoB;
}
/**
 * @brief Insere um registro em um aqruivo do tipo 1
 * @param arq: struct arquivo com dados a serem inseridos
 * @param RRN: posição onde o registro será inserido
 * @param fim: flag fim que indica se a inserção será realizada no final
 * do arquivo
 */
static void insereRegistroRRN(arquivo_t* arq, int RRN, int fimArquivo)
{
	// Calcula posição no arquivo
	int posicaoInsercao = TAM_CABECALHO1 + (RRN * TAM_REG);

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

	// atualiza header
	if (fimArquivo)
		arq->cab->proxRRN += 1;
	else
		arq->cab->nroRegRem -= 1;
}
/**
 * @brief Insere um registro em um aqruivo do tipo 2
 * @param arq: struct arquivo com dados a serem inseridos
 * @param posicaoInsercao: posição onde o registro será inserido
 * @param fim: flag fim que indica se a inserção será realizada no final
 * do arquivo
 */
static void insereRegistroOffset(arquivo_t* arq, long int posicaoInsercao, int fimArquivo)
{
	// Vai até a posição de inserção
	fseek(arq->fptr, posicaoInsercao, SEEK_SET);
	// Armazena tamanho atual do registrador para calcular lixo depois
	int tamRegistro = arq->reg->tamRegistro - 5;
	long int proxbyteOffset;

	if (!fimArquivo)
	{

		// verificando se ele está deletado
		if (fgetc(arq->fptr) != REGISTRO_REMOVIDO)
		{
			printf("Falha no processamento do arquivo.");
			return;
		}

		fread(&arq->reg->tamRegistro, 4, 1, arq->fptr);

		fread(&arq->cab->topoB, 8, 1, arq->fptr);
		fseek(arq->fptr, -13, SEEK_CUR);

		arq->cab->nroRegRem -= 1;
	}
	else
	{
		arq->cab->proxByteOffset += tamRegistro;
	}

	// escreve dados no arquivo
	escreverNoArquivo(arq->fptr, arq->reg, arq->cab, arq->tipo);
	if (!fimArquivo)
		// Se não for o final do arquivo, completa a diferença entre o novo e o velho registro
		// com lixo para manter o mesmo tamanho
		escreverLixo(arq->fptr, arq->reg->tamRegistro - tamRegistro, "$");
}

/**
 * @brief Insere um novo registro no arquivo de indice
 * @param arq: struct arquivo com dados do arquivo
 * @param posicao: Posição (RRN/offset) do registro no arquivo de dados
 * @param indice: vetor de structs na RAM que representa a lista de indices
 */
static void atualizaArquivoIndex(arquivo_t* arq, long int posicao, indice_t* indice)
{
	registroIndice_t* registroIndice = malloc(sizeof(registroIndice_t));

	registroIndice->id = arq->reg->id;
	if (arq->tipo == 1)
		registroIndice->RRN = (int)posicao;
	else
		registroIndice->byteOffset = posicao;
	indice->tamanho += 1;
	indice->registros = realloc(indice->registros, indice->tamanho * sizeof(registroIndice_t));
	indice->registros[indice->tamanho - 1] = *registroIndice;
	qsort(indice->registros, indice->tamanho, sizeof(registroIndice_t), compara);
}

/**
 * @brief função que insere registro no arquivo e atuliza arquivo de indice
 * @param arq: struct com dados do arquivo
 * @param indice: representação do arquivo de indices na RAM
 */
void insereRegistroNoArquivo(arquivo_t* arq, indice_t* indice)
{
	int flagFimDeArquivo = 0;
	if (arq->tipo == 1)
	{
		// Recebe o RRN da função retornaIndice
		int RRN = (int)retornaIndiceDisponivel(arq, &flagFimDeArquivo);
		// calcula a poição e chama a ins
		insereRegistroRRN(arq, RRN, flagFimDeArquivo);
		atualizaArquivoIndex(arq, RRN, indice);

		return;
	}

	long int posicaoInsercao = retornaIndiceDisponivel(arq, &flagFimDeArquivo);
	insereRegistroOffset(arq, posicaoInsercao, flagFimDeArquivo);
	atualizaArquivoIndex(arq, posicaoInsercao, indice);
}

/**
 * @brief Função para liberar memória alocada pela struct arquivo
 * @param arq: struct arquivo
 */
void liberarStrctArq(arquivo_t* arq)
{
	if (arq->cab)
		free(arq->cab);
	free(arq->nomeArqDados);
	free(arq->nomeArqIndes);

	free(arq);
}

static void atualizaTamanhoReg(arquivo_t* arq, buscaParams_t* novosVal)
{
	// Calcular tamanho futuro registro
	int diff = 0;

	if (novosVal->ehBuscado[4])
	{

		int tamCidade = novosVal->filtros[4] == NULL ? 0 : strlen(novosVal->filtros[4]);
		if (!campoEstaPreenchido('4', arq->reg))
			diff += tamCidade;
		else
			diff += tamCidade - arq->reg->tamCidade;
	}

	if (novosVal->ehBuscado[5])
	{
		int tamMarca = novosVal->filtros[5] == NULL ? 0 : strlen(novosVal->filtros[5]);
		if (!campoEstaPreenchido('5', arq->reg))
			diff += tamMarca;
		else
			diff += tamMarca - arq->reg->tamMarca;
	}

	if (novosVal->ehBuscado[6])
	{
		int tamMarca = novosVal->filtros[6] == NULL ? 0 : strlen(novosVal->filtros[6]);
		if (!campoEstaPreenchido('6', arq->reg))
			diff += tamMarca;
		else
			diff += tamMarca - arq->reg->tamMarca;
	}
	// Adicionando a diferença ao registro que será verificado
	arq->reg->tamRegistro += diff;
}

static void atualizaCodigos(arquivo_t* arq)
{
	int numPreenchidos = 0;
	if (arq->reg->cidade != NULL)
	{
		arq->reg->codC5 = 0;
		numPreenchidos++;
	}
	if (arq->reg->marca != NULL)
	{
		if (numPreenchidos)
		{
			arq->reg->codC6 = 1;
		}
		else
		{
			arq->reg->codC5 = 1;
		}
		numPreenchidos++;
	}
	if (arq->reg->modelo != NULL)
	{
		if (numPreenchidos > 1)
		{
			arq->reg->codC7 = 2;
		}
		else if (numPreenchidos == 1)
		{
			arq->reg->codC6 = 2;
		}
		else
		{
			arq->reg->codC5 = 2;
		}
	}
}

void atualizaRegistroNovosVal(arquivo_t* arq, buscaParams_t* novosVal)
{

	if (novosVal->ehBuscado[0])
	{
		int lastId;
		fseek(arq->fptr, 13, SEEK_CUR);
		fread(&lastId, 4, 1, arq->fptr);
		// Escreve novo id no registrador
		fseek(arq->fptr, -4, SEEK_CUR);
		int tmp = atoi(novosVal->filtros[0]);
		fwrite(&tmp, 4, 1, arq->fptr);
		arq->reg->id = tmp;

		// Atualiza no arquivo de index o id
		registroIndice_t* regIndex = malloc(sizeof(registroIndice_t));
		regIndex->id = tmp;
		// regIndex->byteOffset = posicao;

		// atualizarRegistroIndex(indice,regIndex,tipo,lastId);
	}
	if (novosVal->ehBuscado[1])
		arq->reg->ano = atoi(novosVal->filtros[1]);
	if (novosVal->ehBuscado[2])
		arq->reg->qtt = atoi(novosVal->filtros[2]);
	if (novosVal->ehBuscado[3])
	{
		arq->reg->sigla[0] = novosVal->filtros[3][0];
		arq->reg->sigla[1] = novosVal->filtros[3][1];
	}

	if (novosVal->filtros[4])
	{
		arq->reg->cidade = novosVal->filtros[4];
	}
	if (novosVal->filtros[5])
	{
		arq->reg->marca = novosVal->filtros[5];
	}
	if (novosVal->filtros[6])
	{
		arq->reg->modelo = novosVal->filtros[6];
	}

	atualizaCodigos(arq);

	atualizaTamanhoReg(arq, novosVal);
}

/**
 * @brief Esta função atualiza um Registro
 * @param cab: cabeçalho do arquivo que está sendo manipulado
 * @param fptr: ponteiro do arquivo que está sendo manipulado
 * @param tipo: tipo do arquivo qie está sendo manipulado
 * @param posicao: posicao do registro que será manipulado
 * @param novosVals: struct com dados que serão alterados
 */
void acaoFiltro_atualizarRegistro(cabecalho_t* cab, registro_t* reg, FILE* fptr, int tipo, long int posicao,
	buscaParams_t* novosVals, indice_t* indice)
{
	//@TODO: Lembrar de quebrar esse cara em mais funções
	// posições e tamanhos dos registros estáticos
	long int Posinicial = ftell(fptr);

	int vetPosicoes[] = { 5, 9, 13, 17 };
	int vetTamanhos[] = { 4, 4, 4, 2 };

	if (tipo == 2)
	{
		int finalArquivo;
		arquivo_t* arq = malloc(sizeof(arquivo_t));
		arq->tipo = tipo;
		arq->cab = cab;
		arq->reg = reg;
		arq->fptr = fptr;
		arq->indice = indice;

		atualizaTamanhoReg(arq, novosVals);
		//Retorna qual é o espaço disponível para inserir esse registro
		int posInsert = retornaIndiceDisponivel(arq, &finalArquivo);

		if (finalArquivo)
		{
			// deletar registro posiçãoInsert e inserir reg novo

			registro_t* reg = lerRegistro(arq->tipo, arq->fptr, arq->final);
			remocaoLogica(arq->cab, arq->reg, arq->fptr, arq->tipo, posicao, NULL, NULL);

			atualizaRegistroNovosVal(arq, novosVals);

			insereRegistroOffset(arq, posInsert, finalArquivo);
			atualizaArquivoIndex(arq, posInsert, indice);

			fseek(arq->fptr, Posinicial, SEEK_SET);
			fseek(fptr, Posinicial, SEEK_SET);

			liberarStrctArq(arq);
			return;
		}
		liberarStrctArq(arq);

		for (int i = 0; i < 4; i++)
			vetPosicoes[i] += 8;
	}

	for (int i = 0; i < 7; i++)
	{
		if (novosVals->ehBuscado[i])
		{
			fseek(fptr, posicao, SEEK_SET);

			if (i == 0)
			{
				// Se for tipo 1, aramazena o id anterior primeiro.
				int lastId;
				fseek(fptr, vetPosicoes[i], SEEK_CUR);
				fread(&lastId, 4, 1, fptr);
				// Escreve no arquivo o novo id
				fseek(fptr, -4, SEEK_CUR);
				int tmp = atoi(novosVals->filtros[i]);
				fwrite(&tmp, vetTamanhos[i], 1, fptr);

				// Atualiza no arquivo de index o id
				registroIndice_t* regIndex = malloc(sizeof(registroIndice_t));
				regIndex->id = tmp;
				if (tipo == 1)
					regIndex->RRN = (posicao - TAM_CABECALHO1) / TAM_REG;
				if (tipo == 2)
					regIndex->byteOffset = posicao;

				atualizarRegistroIndex(indice, regIndex, tipo, lastId);
			}
			else if (i < 3)
			{
				// se o campo for estático, utilizar a seguinte abordagem:
				fseek(fptr, vetPosicoes[i], SEEK_CUR);
				int tmp = atoi(novosVals->filtros[i]);
				fwrite(&tmp, vetTamanhos[i], 1, fptr);
			}
			else if (i == 3)
			{
				// para o caso da sigla, utilizar a seguinte abordagem:
				fseek(fptr, vetPosicoes[i], SEEK_CUR);
				fwrite(&novosVals->filtros[i][0], 1, 1, fptr);
				fwrite(&novosVals->filtros[i][1], 1, 1, fptr);
			}
			else
			{
				// Caso seja necessária a atualização dos campos variáveis,
				// instandiamos um registrador e reescrevemos toda a parte dos campos
				// variáveis.

				registro_t* regi = lerRegistro(1, fptr, TAM_CABECALHO1 + (cab->proxRRN * TAM_REG));
				fseek(fptr, posicao, SEEK_SET);

				// pulando os campos estáticos
				fseek(fptr, 19, SEEK_CUR);

				switch (i)
				{
				case 4:
					regi->cidade = novosVals->filtros[4];
					break;
				case 5:
					regi->marca = novosVals->filtros[5];
					break;
				case 6:
					regi->modelo = novosVals->filtros[6];
					break;
				}

				escreverNoArquivoVariaveis(fptr, regi, cab);
				// preenchendo espaço restante com lixo
				if (tipo == 1)
				{
					int tamanho = ftell(fptr) - posicao;
					escreverLixo(fptr, 97 - tamanho, "$");
				}
				else
				{
				}
			}
		}
	}

	fseek(fptr, Posinicial, SEEK_SET);
}

void updateRegistros(arquivo_t* arq, buscaParams_t* busca, buscaParams_t* novosVal)
{

	if (arq->tipo == 1)
	{

		//Caso não seja um update que envolve o id, atualiza apenas o registro
		if (!busca->ehBuscado[0])
		{
			percorreArquivoAplicandofiltro(arq->fptr, busca, arq->tipo, arq->cab,
				arq->final, &acaoFiltro_atualizarRegistro, 0, novosVal, NULL);
			return;
		}

		int id = atoi(busca->filtros[0]);
		//Caso busque por id, faz a busca pelo arquivo de indices.
		registroIndice_t* regIndex = buscaIndex(arq->indice, id);
		if (regIndex != NULL)
		{

			long int pos = TAM_CABECALHO1 + (regIndex->RRN * TAM_REG);
			long int posAtual = ftell(arq->fptr);
			fseek(arq->fptr, pos, SEEK_SET);
			//Inicializa o registro da posição que será substituida
			arq->reg = lerRegistro(arq->tipo, arq->fptr, arq->final);
			fseek(arq->fptr, posAtual, SEEK_SET);

			//Valida se está de acordo com o filtro parametrizado

			if (ehValidoFiltro(arq->reg, busca))
			{
				acaoFiltro_atualizarRegistro(arq->cab, arq->reg, arq->fptr, arq->tipo, pos, novosVal, arq->indice);
			}
			liberaRegistro(arq->reg);
		}

		return;
	}

	//Caso não seja um update que envolve o id, atualiza apenas o registro
	if (!busca->ehBuscado[0])
	{
		percorreArquivoAplicandofiltro(arq->fptr, busca, arq->tipo, arq->cab,
			arq->final, &acaoFiltro_atualizarRegistro, 0, novosVal, arq->indice);
		return;
	}

	//Caso seja, faz a busca por indice e 
	//atualiza o arquivo de indices também
	int id = atoi(busca->filtros[0]);
	registroIndice_t* regIndex = buscaIndex(arq->indice, id);
	if (regIndex != NULL)
	{

		long int pos = regIndex->byteOffset;
		long int posAtual = ftell(arq->fptr);
		fseek(arq->fptr, pos, SEEK_SET);
		//Inicializa o registro da posição que será substituida
		arq->reg = lerRegistro(arq->tipo, arq->fptr, arq->final);
		fseek(arq->fptr, posAtual, SEEK_SET);

		//Valida se está de acordo com o filtro parametrizado
		if (ehValidoFiltro(arq->reg, busca))
		{
			acaoFiltro_atualizarRegistro(arq->cab, arq->reg, arq->fptr, arq->tipo, pos, novosVal, arq->indice);
		}
	}

}