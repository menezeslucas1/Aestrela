#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define qtdCidade 20

struct cidade
{
	char nome[20];
	int distanciaDestino;
	struct cidade *vizinha[20];
	int distancia[20];
};
typedef struct cidade Cidade;

struct etapa
{
	Cidade *cidade;
	int g;
	int h;
	struct etapa *anterior;
};
typedef struct etapa Etapa;

struct lista
{
	Etapa *etapa;
	struct lista *proximo;
};
typedef struct lista Lista;

void leDistancia(FILE *arquivo, Cidade *cidade, int qtd);
Cidade * procura(char *nome, Cidade *cidade, int qtd);
void leNome(FILE *arquivo, char *nome);
void leMapa(FILE *arquivo, Cidade *mapa, int qtd);
void remover1(Lista **borda);
void expandir(Lista **borda);
void adicionar(Lista **inicio, Etapa *etapa);
void menorCaminho(char *nomeOrigem, Cidade *mapa, int qtd);

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
				cidade[i].distanciaDestino = atoi(aux);
				break;
			}
			else{
				//Lê nome composto da cidade
				strcat(cidade[i].nome," ");
				strcat(cidade[i].nome, aux);
			}
		}while  (s==1);
	}
}

Cidade * procura(char *nome, Cidade *cidade, int qtd)
{
	for (int i=0; i<qtd; i++)
	{
		if (strcmp(nome, cidade[i].nome)==0)
			return &cidade[i];
	}
	printf("cidade não encontrada.\n");
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

void leMapa(FILE *arquivo, Cidade *mapa, int qtd)
{
	char nome[20];
	char vizinha[20];
	int distancia;
	Cidade *cidade;
	Cidade *v;
	int c;
	int i=0;
	for (int j=0; j<qtd; j++)
	{
		//Seleciona uma cidade do mapa
		leNome(arquivo, nome);
		cidade = procura(nome, mapa, qtd);
		if (cidade == NULL)
		{
			printf("Impossível encontrar a cidade: %s\n", nome);
			exit(1);
		}
		i=0;
		//decobre as cidades vizinhas
		do
		{
			//Descobre a distância até a cidade vizinha
			fscanf(arquivo, "%d", &distancia);
			//Descobre a cidade vizinha
			leNome(arquivo, vizinha);
			v = procura(vizinha, mapa, qtd);
			if (v == NULL)
			{
				printf("Impossível encontrar a cidade: %s\n", vizinha);
				exit(1);
			}
			cidade->vizinha[i] = v;
			cidade->distancia[i] = distancia;
			i++;
			c = getc(arquivo);
			if(c!='\n')
				ungetc(c,arquivo);
		}while(c!='\n');
		cidade->vizinha[i]=NULL;
	}
}

void remover1(Lista **borda)
{
	*borda = (*borda)->proximo;
}

//void expandir(Lista **borda, Cidade **cidade, int *distancia)
void expandir(Lista **borda)
{
	int i=0;
	int g = (**borda).etapa->g;
//	int h = borda->etapa->h;
	Etapa *etapa;
	Etapa *anterior = (**borda).etapa;
	Cidade **cidade = (**borda).etapa->cidade->vizinha;
	int *distancia = (**borda).etapa->cidade->distancia;
	remover1(borda);
	while (cidade[i]!=NULL)
	{
		etapa = (Etapa *)malloc(sizeof(etapa));
		etapa->cidade = cidade[i];
		etapa->anterior = anterior;
		etapa->g = g+distancia[i];
		etapa->h = cidade[i]->distanciaDestino;
//		printf("%s\t%d %d %d\n", cidade[i]->nome, etapa->g + etapa->h, etapa->g, etapa->h);
		adicionar(borda, etapa);
		i++;
	}
}

void adicionar(Lista **inicio, Etapa *etapa)
{
	if (*inicio==NULL)
	{
		*inicio = (Lista *)malloc(sizeof(Lista));
		(**inicio).etapa = etapa;
		(**inicio).proximo = NULL;
		return;
	}
	int fnovo = etapa->g + etapa->h;
	int fantigo = (**inicio).etapa->g + (**inicio).etapa->h;
//	printf("%d %d\n", fnovo, fantigo);
	if (fnovo <= fantigo)
	{
		Lista *novo = (Lista *)malloc(sizeof(Lista));
		novo->etapa = etapa;
		novo->proximo = *inicio;
		*inicio = novo;
	}
	else
		adicionar(&(**inicio).proximo,etapa);
}

void menorCaminho(char *nomeOrigem, Cidade *mapa, int qtd)
{
	Cidade *origem = procura(nomeOrigem, mapa, qtd);
	Etapa inicial = {origem,0,origem->distanciaDestino};
	Lista *borda;
	borda=NULL;

	adicionar(&borda, &inicial);

//	printf("%s %d\n",borda->etapa->cidade->nome, borda->etapa->g);
	int i=0;
	while (borda->etapa->cidade->distanciaDestino != 0)
	{
		expandir(&borda);
//	printf("%s %d\n",borda->etapa->cidade->nome, borda->etapa->g);
		if(++i >= 10)
			break;
	}
	Etapa *e;
	e = borda->etapa;
	printf("\nSolução:\n\n");
	do
	{
		printf("%s\n",e->cidade->nome);
		e = e->anterior;
	}while(e!=NULL);
}

int main(int argc, char *argv[])
{
	if (argc!=2)
	{
		printf("modo de usar: $./caminho <nome da cidade>\n");
		return 2;
	}
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
	menorCaminho(argv[1], cidade, qtdCidade);
	printf("Sucesso.\n");
	return 0;
}
