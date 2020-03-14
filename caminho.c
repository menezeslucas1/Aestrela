#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define qtdCidade 20

struct vizinha
{
	struct cidade *cidade;
	int distancia;
};

struct cidade
{
	char nome[20];
	int distEstimada;
	struct vizinha vizinha[20];
};

typedef struct cidade Cidade;

void leDistancia(FILE *arquivo, Cidade *cidade, int qtd)
{
	for (int i=0; i<qtd; i++)
	{
		char aux[10];
		//Lê primeiro nome da cidade
		fscanf(arquivo, "%s", cidade[i].nome);
		int s;
		do
		{
			s = fscanf(arquivo, "%s", aux);
			if(isdigit(aux[0]))
			{
				//Lê o valor da distância
				cidade[i].distEstimada = atoi(aux);
				break;
			}
			else{
				//Lê nome composto da cidade
				strcat(cidade[i].nome," ");
				strcat(cidade[i].nome, aux);
			}
		}while  (s==1);
//		printf("%20s\t%d\n", cidade[i].nome, cidade[i].distancia);
	}
}

Cidade * procura(char *nome, Cidade *cidade, int qtd)
{
	for (int i=0; i<qtd; i++)
	{
		if (strcmp(nome, cidade[i].nome)==0)
			return &cidade[i];
	}
	return NULL;
}

void leNome(FILE *arquivo, char *nome)
{
	int c;
	int i=0;
	do
		c = getc(arquivo);
	while(c==' ');
	do{
		while (isalpha(c))
		{
			nome[i] = (char) c;
			c = getc(arquivo);
			i++;
		}
		while(c==' ')
			c = getc(arquivo);
		if (isalpha(c))
		{
			nome[i] = ' ';
			i++;
		}
		else
			nome[i] = 0;
	}while(isalpha(c));
	ungetc(c, arquivo);
}

void leMapa(FILE *arquivo, Cidade *cidade, int qtd)
{


//		printf("aff2\n");
//	}
//	printf("%s\t%d\n", c->vizinha[0].cidade->nome, c->vizinha[0].cidade->distEstimada);
}

int main()
{
	FILE *distancia = fopen("distancia.txt", "r");
	if (distancia == NULL)
	{
		printf("erro ao abrir o arquivo de distâncias.\n");
		return 1;
	}
	Cidade cidade[qtdCidade];
	leDistancia(distancia, cidade, qtdCidade);

	FILE *mapa = fopen("mapa.txt", "r+");
	if (mapa == NULL)
	{
		printf("erro ao abrir o arquivo de mapa.\n");
		return 1;
	}
	
	leMapa(mapa, cidade, qtdCidade);
	printf("Sucesso.\n");
	return 0;
}
