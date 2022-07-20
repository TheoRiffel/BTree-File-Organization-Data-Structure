#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../includes/funcionalidades.h"
#include "../includes/utils.h"
#include "../includes/indexUtils.h"
#include "../includes/fileUtils.h"


void registraCodigos(int numCodigosUtilizados, registro_t* reg, int cod);
static char* readStrQuote(FILE* fptr);

/**
 * @brief Lê uma string de tamanho variável a partir do arquivo 'a',
 * até que seja lido o caractere delimitador
 *
 * @param a : Arquivo fonte
 * @param delimiter : caractere delimitador
 * @return char* : string lida
 */
char* readUntil(FILE* a, char delimiter)
{
	char* word = NULL;

	int i = 0;
	do
	{
		word = realloc(word, (i + 1) * sizeof(char));
		word[i] = fgetc(a);

		if (word[i] == '\r') word[i] = fgetc(a);

		if (word[i] == '\n' || word[i] == delimiter || word[i] == EOF)
		{
			word[i] = '\0';
		}

		i++;
	} while (word[i - 1] != '\0');

	if (word[0] == '\0')
	{
		free(word);
		return NULL;
	}

	return word;
}

/**
 * @brief Lê um número inteiro a partir de um arquivo, até que o
 * caractere delimitador seja encontrado
 *
 * @param a : Arquivo fonte
 * @param delimiter : caractere delimitador
 * @return int : inteiro lido
 */
int readNumberUntil(FILE* a, char delimiter)
{
	char* num = readUntil(a, delimiter);
	if (num == NULL || num[0] == '\0')
	{
		if (num != NULL) free(num);
		return -1;
	}

	int number;

	if (strcmp("NULO", num) == 0) number = -1;
	else number = atoi(num);
	free(num);

	return number;
}

/**
 * @brief Lê uma string estática a partir de um arquivo, até que o
 * caractere delimitador seja encontrado
 *
 * @param a : Arquivo fonte
 * @param tamanho : tamanho da string estática
 * @param s : ponteiro para a string
 * @param delimiter : caractere delimitador
 */
void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter)
{
	int d = 0;
	for (int i = 0; i < tamanho; i++)
	{
		s[i] = fgetc(a);
		if (s[i] == ',')
		{
			d++;
			s[i] = '\0';
			if (i == 0)
			{
				for (int i = 0; i < tamanho; i++)
				{
					s[i] = '$';
				}
			}
			break;
		}
	}
	char delim;
	if (d == 0) delim = fgetc(a);
}

/**
 * @brief Inicializa a struct que contém os valores do cabeçalho
 *
 * @param cabecalho : ponteiro para a struct
 */
void inicializarCabecalho(cabecalho_t* cabecalho)
{
	cabecalho->status = '\0';
	cabecalho->topoA = -1;
	cabecalho->topoB = -1;
	strcpy(cabecalho->descricao, "LISTAGEM DA FROTA DOS VEICULOS NO BRASIL");
	strcpy(cabecalho->desC1, "CODIGO IDENTIFICADOR: ");
	strcpy(cabecalho->desC2, "ANO DE FABRICACAO: ");
	strcpy(cabecalho->desC3, "QUANTIDADE DE VEICULOS: ");
	strcpy(cabecalho->desC4, "ESTADO: ");
	cabecalho->codC5 = '0';
	strcpy(cabecalho->desC5, "NOME DA CIDADE: ");
	cabecalho->codC6 = '1';
	strcpy(cabecalho->desC6, "MARCA DO VEICULO: ");
	cabecalho->codC7 = '2';
	strcpy(cabecalho->desC7, "MODELO DO VEICULO: ");
	cabecalho->proxRRN = 0;
	cabecalho->proxByteOffset = 0;
	cabecalho->nroRegRem = 0;
}

/**
 * @brief Lê um único registro a partir de um arquivo csv
 *
 * @param csv : Arquivo fonte
 * @param registro : Ponteiro para a strutc na qual serão armazenados
 * os dados lidos
 * @param tipo : Tipo do arquivo
 */
void lerRegistrocsv(FILE* csv, registro_t* registro, int tipo)
{
	if (tipo == 1)
		registro->tamRegistro = ESTATICOS1;
	else if (tipo == 2)
		registro->tamRegistro = ESTATICOS2;

	registro->sigla[0] = '\0';
	registro->sigla[1] = '\0';

	registro->id = readNumberUntil(csv, ',');
	registro->ano = readNumberUntil(csv, ',');
	registro->cidade = readUntil(csv, ',');
	registro->qtt = readNumberUntil(csv, ',');
	readStaticUntil(csv, 2, registro->sigla, ',');
	registro->marca = readUntil(csv, ',');
	registro->modelo = readUntil(csv, ',');

	registro->removido = '0';
	registro->proxA = -1;
	registro->proxB = -1;
	if (registro->cidade == NULL)
	{
		registro->tamCidade = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamCidade = strlen(registro->cidade);

	if (registro->marca == NULL)
	{
		registro->tamMarca = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamMarca = strlen(registro->marca);

	if (registro->modelo == NULL)
	{
		registro->tamModelo = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamModelo = strlen(registro->modelo);

	registro->tamRegistro += registro->tamCidade + registro->tamMarca + registro->tamModelo;
}

/**
 * @brief Escreve uma quantidade solicitada de lixo em arquivo binário
 *
 * @param binario : Ponteiro para o arquivo
 * @param tam : Quantidade de lixo a ser escrita
 * @param lixo : String que indica o lixo
 */
void escreverLixo(FILE* binario, int tam, char* lixo)
{
	for (int i = 0; i < tam; i++)
	{
		fwrite(lixo, 1, 1, binario);
	}
}

/**
 * @brief Libera a memória alocada para armazenar os dados
 * de um registro
 *
 * @param r : Ponteiro para a struct que contém os dados do
 * registro
 */
void liberar(registro_t* r)
{
	free(r->cidade);
	free(r->modelo);
	free(r->marca);
	free(r);
}

/**
 * @brief Inicializa os campos variáveis de um registro
 *
 * @return : struct de registro inicializada
 */
registro_t* inicializarRegistro()
{
	registro_t* reg = malloc(sizeof(registro_t));
	reg->id = -1;
	reg->ano = -1;
	reg->qtt = -1;
	reg->tamRegistro = 0;
	reg->codC5 = '$';
	reg->codC6 = '$';
	reg->codC7 = '$';
	reg->cidade = NULL;
	reg->marca = NULL;
	reg->modelo = NULL;
	strcpy(reg->sigla, "$$");

	return reg;
}

/**
 * @brief Imprime, de acordo com as especificações, um único registro
 *
 * @param r : Ponteiro para a struct com os dados do registro
 * @param c : Ponteiro para a struct com os dados do cabeçalho
 */
void imprimirRegistro(registro_t* reg, cabecalho_t* cab) {

	printf("%s", cab->desC6);
	if (campoEstaPreenchido('1', reg)) {
		printf("%.*s\n", reg->tamMarca, reg->marca);
	}
	else {
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC7);
	if (campoEstaPreenchido('2', reg)) {
		printf("%.*s\n", reg->tamModelo, reg->modelo);
	}
	else {
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC2);
	if (reg->ano != -1) {
		printf("%i\n", reg->ano);
	}
	else {
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC5);
	if (campoEstaPreenchido('0', reg)) {
		printf("%.*s\n", reg->tamCidade, reg->cidade);
	}
	else {
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC3);
	if (reg->qtt != -1) {
		printf("%i\n", reg->qtt);
	}
	else {
		printf("NAO PREENCHIDO\n");
	}

	printf("\n");
}

/**
 * @brief Libera a memória alocada para armazenar os dados
 * de um registro
 *
 * @param reg : Ponteiro para a struct que contém os dados do
 * registro
 */
void liberaRegistro(registro_t* reg) {

	if (campoEstaPreenchido('0', reg)) {
		free(reg->cidade);
	}
	if (campoEstaPreenchido('1', reg)) {
		free(reg->marca);
	}
	if (campoEstaPreenchido('2', reg)) {
		free(reg->modelo);
	}

	free(reg);
}

/**
 * @brief Libera a memória alocada nas funções
 * o parâmetro delCab indica se o Cabeçalho está alocado e pode ser liberado sem ter segfault
 * os demais parâmetros são ponteiros para os endereços que serão liberados
 */
void liberaMemSelect(int delCab, char* tArq, char* aEnt, FILE* fptr, cabecalho_t* cab)
{
	free(tArq);
	free(aEnt);
	fclose(fptr);
	if (delCab)
	{
		free(cab);
	}
}

/**
 * @brief informa se o campo do código recebeu alugm valor
 * ou não foi alocado
 */
int campoEstaPreenchido(char codigo, registro_t* reg) {
	return reg->codC5 == codigo || reg->codC6 == codigo || reg->codC7 == codigo;
}

static int validaFiltroInt(registro_t* reg, buscaParams_t* busca, int filtro) {
	int val;
	switch (filtro) {
	case 0: val = reg->id;
		break;
	case 1: val = reg->ano;
		break;
	case 2: val = reg->qtt;
	}

	return atoi(busca->filtros[filtro]) == val;
}

static int strcmpWLen(char* num1, char* num2, int sz1, int sz2) {
	if (sz2 != sz1) {
		return 0;
	}

	for (int i = 0; i < sz1; i++) {
		if (num1[i] != num2[i]) {
			return 0;
		}
	}

	return 1;
}

static int validaFiltroStr(registro_t* reg, buscaParams_t* busca, int filtro) {
	char* val = NULL;
	int tam = 0;

	if (busca->filtros[filtro] == NULL) {
		if (filtro == 4) reg->cidade == NULL;
		if (filtro == 5) reg->marca == NULL;
		if (filtro == 6) reg->modelo == NULL;
	}

	if (filtro > 3 && !campoEstaPreenchido(filtro + 44, reg)) {
		return 0;
	}

	if (filtro == 3) {
		if (!busca->filtros[3]) {
			return reg->sigla[0] == '$';
		}

		if (reg->sigla[0] != busca->filtros[3][0]) return 0;
		if (reg->sigla[1] != busca->filtros[3][1]) return 0;
		return 1;
	}

	switch (filtro) {
	case 4: val = reg->cidade;
		tam = reg->tamCidade;
		break;
	case 5: val = reg->marca;
		tam = reg->tamMarca;
		break;
	case 6: val = reg->modelo;
		tam = reg->tamModelo;
		break;
	}
	char* ref = busca->filtros[filtro];

	return strcmpWLen(ref, val, tam, strlen(ref));
}

int ehValidoFiltro(registro_t* reg, buscaParams_t* busca) {

	for (int i = 0; i < NUM_PARAMETROS; i++) {
		if (busca->ehBuscado[i] && i < 3) {
			if (!validaFiltroInt(reg, busca, i)) {
				return 0;
			}
		}
		else if (busca->ehBuscado[i]) {
			if (!validaFiltroStr(reg, busca, i)) {
				return 0;
			}
		}
	}

	return 1;
}

void binarioNaTela(char* nomeArquivoBinario) { /* Voc� n�o precisa entender o c�digo dessa fun��o. */

	/* Use essa fun��o para compara��o no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (voc� n�o vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char* mb;
	size_t fl;
	FILE* fs;
	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (fun��o binarioNaTela): n�o foi poss�vel abrir o arquivo que me passou para leitura. Ele existe e voc� t� passando o nome certo? Voc� lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char*)malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for (i = 0; i < fl; i++) {
		cs += (unsigned long)mb[i];
	}
	printf("%lf\n", (cs / (double)100));
	free(mb);
	fclose(fs);
}

/**
 * @brief Função que auxilia na ordenação comparando valores
 *
 */
int compara(const void* reg1, const void* reg2)
{
	int id1 = ((registroIndice_t*)reg1)->id;
	int id2 = ((registroIndice_t*)reg2)->id;
	return id1 - id2;
}

/**
 * @brief Libera a memória das strings usadas no programa
 *
 * @param a : string a ser liberada
 * @param b : string a ser liberada
 * @param c : string a ser liberada
 */
void liberaStrings(char* a, char* b, char* c)
{
	if (a != NULL) free(a);
	if (b != NULL) free(b);
	if (c != NULL) free(c);
}

/**
 * @brief Verifica se os arquivos foram abertos corretamente
 *
 * @param a : ponteiro para o primeiro arquivo
 * @param b : ponteiro para o segundo arquivo
 */
bool confereArquivos(FILE* a, FILE* b)
{
	if (a == NULL || b == NULL)
	{
		printf("Falha no processamento do arquivo.\n");
		if (a != NULL) fclose(a);
		if (b != NULL) fclose(b);
		return false;
	}

	else return true;
}

/**
 * @brief Verifica se o arquivo está apto para ser lido
 *
 * @param status : status atual do arquivo
 */
bool arquivoConsistente(char status)
{
	if (status == ARQUIVO_CONSISTENTE) return true;
	else
	{
		printf("Falha no processamento do arquivo.\n");
		return false;
	}
}

/**
 * @brief Realiza a busca binária de uma chave (id) em um vetor de
 * índices contido na RAM
 *
 * @param chave : id a ser buscado
 * @param vetor : vetor de registros do tipo índice
 * @param esq : elemento mais a esquerda
 * @param dir : elemento mais a direita
 */
int buscaBinaria(int chave, registroIndice_t* vetor, int esq, int dir)
{
	if (dir >= esq)
	{
		int mid = esq + (dir - esq) / 2;

		if (vetor[mid].id == chave)
			return mid;

		if (vetor[mid].id > chave)
			return buscaBinaria(chave, vetor, esq, mid - 1);

		return buscaBinaria(chave, vetor, mid + 1, dir);
	}
	return -1;
}

/**
 * @brief busca binária das chaves no registro da BTree
 * 
 * @param chave 
 * @param vetor 
 * @param encontrado 
 * @param esq 
 * @param dir 
 * @return int 
 */
int buscaBinariaChavesBTree(int chave, chave_t vetor[], int esq, int dir)
{
	if (dir >= esq)
	{
		int mid = esq + (dir - esq) / 2;

		if (vetor[mid].id == chave)
			return mid;

		if (vetor[mid].id > chave)
			return buscaBinariaChavesBTree(chave, vetor, esq, mid - 1);

		return buscaBinariaChavesBTree(chave, vetor, mid + 1, dir);
	}
	return -1;
}

static void RecebeFiltros(int numFiltros, buscaParams_t* busca, FILE* arquivo)
{

	for (int i = 0; i < numFiltros; i++)
	{
		//lê o nome do campo que será filtrado e busca por posição onde será inserido
		char* nomeCampo = readUntil(arquivo, ' ');
		for (int j = 0; j < NUM_PARAMETROS; j++)
		{
			//Se o filtro para este campo ainda não foi definido, define ele agora
			if (!busca->ehBuscado[j] && !strcmp(busca->filtros[j], nomeCampo))
			{
				//marca que este campo será filtrado
				busca->ehBuscado[j] = 1;

				if (j < 3)
				{
					//Verifica se o campo é nulo
					char verificacao = getchar();
					if (verificacao == 'N') {
						char descarte[5];
						fread(descarte, 4, 1, arquivo);
						busca->filtros[j] = malloc(sizeof(char) * 2);
						busca->filtros[j] = "-1";
					}
					else {
						ungetc(verificacao, arquivo);
						busca->filtros[j] = readUntil(arquivo, ' ');
					}
				}
				else
				{
					//verifica se campo é nulo
					char ver = getchar();
					if (ver == 'N') {
						char descarte[5];
						fread(descarte, 4, 1, arquivo);
						busca->filtros[j] = NULL;
					}
					else {
						busca->filtros[j] = readUntil(arquivo, '"');
						getchar();
					}
				}
			}
		}
		free(nomeCampo);
	}
}

/**
 * @brief Desaloca a memória usada para a busca
 *
 * @param busca : struct com os campos de busca
 */
void liberaStructBusca(buscaParams_t* busca)
{
	for (int i = 0; i < NUM_PARAMETROS; i++)
		free(busca->filtros[i]);
	free(busca->filtros);
	free(busca);
}

/**
 * @brief Inicializa os campos da struct de filtro
 * @param busca: struct de busca que será inicializada
 * @param numFiltros: a quantidade de filtros que serão inseridos
 * @param fptr: arquivo com os parâmetros da bussca(geralmente stdin)
 */
void inicializaStructBusca(buscaParams_t* busca, int numFiltros, FILE* fptr)
{
	busca->filtros = malloc(sizeof(char*) * NUM_PARAMETROS);

	for (int i = 0; i < NUM_PARAMETROS; i++)
	{
		busca->ehBuscado[i] = 0;
		busca->filtros[i] = malloc(sizeof(char) * 11);
	}

	strcpy(busca->filtros[0], "id");
	strcpy(busca->filtros[1], "ano");
	strcpy(busca->filtros[2], "qtt");
	strcpy(busca->filtros[3], "sigla");
	strcpy(busca->filtros[4], "cidade");
	strcpy(busca->filtros[5], "marca");
	strcpy(busca->filtros[6], "modelo");

	RecebeFiltros(numFiltros, busca, fptr);
}

/**
 * @brief Atribui o código ao campo codC* correto
 *
 * @param tam : Qunatidade de códigos já atribuidos ao registro fornecido
 * @param reg : Ponteiro para o registro
 * @param cod : Codigo do campo que será atribuído ao campo codC*
 */
void registraCodigos(int numCodigosUtilizados, registro_t* reg, int cod)
{
	switch (numCodigosUtilizados)
	{
	case 0:
		reg->codC5 = cod;
		break;
	case 1:
		reg->codC6 = cod;
		break;
	case 2:
		reg->codC7 = cod;
		break;
	}
}

/**
 * @brief Lê registro a partir da stdin e retorna uma struct registro
 * @return o registro lido da stdin
 * @param tipo: O tipo do registro
 */
registro_t* lerRegistroStdin(int tipo)
{
	registro_t* reg = inicializarRegistro();
	reg->id = readNumberUntil(stdin, ' ');
	reg->ano = readNumberUntil(stdin, ' ');
	reg->qtt = readNumberUntil(stdin, ' ');

	char* sigla = readStrQuote(stdin);

	if (sigla) strcpy(reg->sigla, sigla);
	else strcpy(reg->sigla, "$$");

	reg->cidade = readStrQuote(stdin);
	reg->marca = readStrQuote(stdin);
	reg->modelo = readStrQuote(stdin);

	reg->tamRegistro = tipo == 1 ? ESTATICOS1 : ESTATICOS2;
	reg->codC5 = reg->codC6 = reg->codC7 = '$';

	int contadorCodigos = 0;

	if (reg->cidade == NULL)
	{
		reg->tamCidade = 0;
		reg->tamRegistro -= 5;
	}
	else
	{
		reg->tamCidade = strlen(reg->cidade);
		reg->tamRegistro += reg->tamCidade;
		registraCodigos(contadorCodigos++, reg, '0');
	}

	if (reg->marca == NULL)
	{
		reg->tamMarca = 0;
		reg->tamRegistro -= 5;
	}
	else
	{
		reg->tamMarca = strlen(reg->marca);
		reg->tamRegistro += reg->tamMarca;
		registraCodigos(contadorCodigos++, reg, '1');
	}

	if (reg->modelo == NULL)
	{
		reg->tamModelo = 0;
		reg->tamRegistro -= 5;
	}
	else
	{
		reg->tamModelo = strlen(reg->modelo);
		reg->tamRegistro += reg->tamModelo;
		registraCodigos(contadorCodigos++, reg, '2');
	}

	char descartSpaces = fgetc(stdin);
	if (descartSpaces == '\r') descartSpaces = fgetc(stdin);

	free(sigla);

	return reg;
}

/**
 * @brief Lê valor da stdin entre aspas
 * @return resultado da leitura
 * @param fptr: arquivo
 */
static char* readStrQuote(FILE* fptr)
{

	char tmpchr;
	char nulo[] = { 'N', 'U', 'L', 'O' };
	int contagemNulo = 0;
	// Lê tudo até o primeiro "
	while ((tmpchr = fgetc(fptr)) != '\"')
	{
		// Se detectar NULO antes da primeira aspas, retorna null
		if (tmpchr == nulo[contagemNulo])
			contagemNulo++;
		else
			contagemNulo = 0;

		if (contagemNulo == 4)
			return NULL;
	};
	tmpchr = fgetc(fptr);

	int i = 0;
	char* palavra = NULL;

	do
	{
		palavra = realloc(palavra, (i + 1) * sizeof(char));
		palavra[i++] = tmpchr;
	} while ((tmpchr = fgetc(fptr)) != '\"');

	palavra = realloc(palavra, (i + 1) * sizeof(char));
	palavra[i] = '\0';

	return palavra;
}
/**
 * @brief Lê filtros que serão utilizados para o update
 */
buscaParams_t* lerFiltrosUpdate()
{
	int numFiltros = readNumberUntil(stdin, ' ');
	buscaParams_t* busca = malloc(sizeof(buscaParams_t));
	inicializaStructBusca(busca, numFiltros, stdin);

	return busca;
}