/*******************************************************************************
 |                       USP - Universidade de São Paulo                       |
 |            ICMC - Instituto de Ciências Matemáticas e de Computação         |
 *******************************************************************************
 |                    Bacharelado em Ciências de Computação                    |
 |                                   2022/1                                    |
 |                                                                             |
 |              Autores: Maria Júlia Soares De Grandi (12542501)     100%      |
 |                       Théo Bruno Frey Riffel (12547812)           100%      |
 |                            Disciplina: SCC0215                              |
 *******************************************************************************
  > Terceiro Trabalho Prático
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../includes/funcionalidades.h"

int main()
{
	int funcionalidade;
	scanf("%d ", &funcionalidade);
	switch(funcionalidade)
	{
	case 1:
		createTable();
		break;
	case 2:
		selectSemWhere();
		break;
	case 3:
		selectCWhere();
		break;
	case 4:
		recuperarRegistro();
		break;	
	case 5:
		createIndex();
		break;
	case 6:
		delete();
		break;
	case 7:
		insertInto();
		break;
	case 8:
		update();
		break;
	case 9:
		createBTreeIndex();
		break;
	case 10:
		busca();
		break;
	case 11:
		insertFile();
		break;
	default:
		break;
	}

	return 0;
}