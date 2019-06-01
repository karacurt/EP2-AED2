#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

typedef enum COR {
    BRANCO = 0,
    CINZA = 1,
    PRETO = 2,
} Cor;

typedef struct vertice Vertice;
typedef struct aresta Aresta;

// GRAFO

struct vertice {
    Cor cor;
    int chave, minor, ordem, st_number;
    Vertice *pai;
    Vertice *prox;
    Aresta *vizinhos;
};

struct aresta {
    Vertice *vizinho;
    Aresta *prox;
};

typedef struct {
    Vertice* inicio;
} Grafo;

// LISTA LIGADA RESULTANTE

typedef struct estrutura {
    Vertice* vertice;
    struct estrutura* esq;
    struct estrutura* dir;
} No;

typedef struct {
    No* inicio;
    No* fim;
} ListaLigada;

// Algoritmos da Lista Ligada

No* criar_no(Vertice *v) {
    No* novo = (No*) malloc(sizeof(No));
    novo->vertice = v;
    novo->esq = novo->dir = NULL;
    return novo;
} 

void inicializar_lista(ListaLigada *l) {
    l->inicio = NULL;
    l->fim = NULL;
}

No* buscar_no(ListaLigada l, Vertice *v) {
    No* atual = l.inicio;
    while(atual) {
        if(atual->vertice == v) return atual;
        atual = atual->dir;
    }
    return NULL;
}

void insere_elemento_no_final(ListaLigada *l, Vertice *v) {
    No* novo = criar_no(v);

    if(l->inicio == NULL) l->inicio = novo;
    else {
        No* atual = l->fim;
        atual->dir = novo;
        novo->esq = atual;
    }
    l->fim = novo;
}

void insere_elemento(ListaLigada *l, Vertice *v, No *pai, char* pos) {
    if(l->inicio == NULL || pai == NULL) {
        insere_elemento_no_final(l, v);
        return;
    }

    No* novo = criar_no(v);
    if(pos == "esq") {
        if(pai->esq == NULL) l->inicio = novo;
        else {
            novo->esq = pai->esq;
            pai->esq->dir = novo;
        }
        novo->dir = pai;
        pai->esq = novo;
    } else if(pos == "dir") {
        if(pai->dir == NULL) l->fim = novo;
        else {
            novo->dir = pai->dir;
            pai->dir->esq = novo;
        }
        novo->esq = pai;
        pai->dir = novo;
    } else {
        puts("error");
        return;
    }
}

// Algoritmos do grafo

Vertice* criaVertice(int chave) {
    Vertice *v = (Vertice*) malloc(sizeof(Vertice));
    v->cor = BRANCO;
    v->chave = chave;
    v->ordem = v->minor = v->st_number = -1;
    v->pai = NULL;
    v->prox = NULL;
    v->vizinhos = NULL;
    return v;
}

void inicializar_grafo(Grafo *g, int max) {
    g->inicio = NULL;
    int i;
    for(i = 1; i <= max;) {
        Vertice *v = criaVertice(i++);
        if(g->inicio == NULL) g->inicio = v;
        else {
            Vertice *atual = g->inicio;
            while (atual->prox != NULL) atual = atual->prox;
            atual->prox = v;
        }
    }
}

Vertice* busca_vertice(Grafo g, int chave) {
    Vertice *atual = g.inicio;
    while(atual != NULL) {
        if(atual->chave == chave) return atual;
        atual = atual->prox;
    }
    return NULL;
}

void adiciona_aresta(Grafo g, int vertice, int vizinho) {
    Vertice* v = busca_vertice(g, vertice);
    Vertice* u = busca_vertice(g, vizinho);

    if(v == NULL || u == NULL) return;

    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    nova->vizinho = u;
    nova->prox = NULL;

    if(v->vizinhos == NULL) v->vizinhos = nova;
    else {
        Aresta* atual = v->vizinhos;
        while(atual->prox != NULL) atual = atual->prox;
        atual->prox = nova;
    }
}

void adiciona_aresta_bidirecional(Grafo g, int vertice, int vizinho) {
    adiciona_aresta(g, vertice, vizinho);
    adiciona_aresta(g, vizinho, vertice);
}

void DFS_visit(Vertice *vertice) {
    vertice->cor = CINZA;

    Aresta *aresta = vertice->vizinhos;
    while(aresta) {
        Vertice *vizinho = aresta->vizinho;
        if(vizinho->cor == BRANCO) DFS_visit(vizinho);
        aresta = aresta->prox;
    }
    vertice->cor = PRETO;
}

void DFS(Grafo g) {
    Vertice *vertice = g.inicio;

    while(vertice) {
        if(vertice->cor == BRANCO) DFS_visit(vertice);
        vertice = vertice->prox;
    }
}

void DFST_visit(Vertice *vertice, int *contador, ListaLigada* l) {
    vertice->cor = CINZA;
    insere_elemento_no_final(l, vertice);
    vertice->minor = vertice->ordem = *contador;
    *contador = *contador + 1;

    Aresta *aresta = vertice->vizinhos;
    while(aresta) {
        Vertice *vizinho = aresta->vizinho;
        if(vizinho->cor == BRANCO) {
            vizinho->pai = vertice;
            DFST_visit(vizinho, contador, l);
        }

        if(vertice->minor > vizinho->minor && vertice->pai != vizinho) {
            vertice->minor = vizinho->minor;
        }
        aresta = aresta->prox;
    }
    vertice->cor = PRETO;
}

bool DFST(Grafo g, ListaLigada *l) {
    if(!g.inicio) return false;
    inicializar_lista(l);
    Vertice *atual = g.inicio;
    atual->pai = NULL;
    int contador = 1;
    
    DFST_visit(atual, &contador, l);

    while(atual) {
        if (
            (atual->cor == BRANCO || atual->minor >= atual->ordem) &&
            atual->chave != 1
        ) return false;
        atual = atual->prox;
    }

    return true;
}

ListaLigada st_numeracao(ListaLigada lista_de_origens) {
    ListaLigada lista_nova;
    inicializar_lista(&lista_nova);

    No* atual = lista_de_origens.inicio;
    Vertice* s = atual->vertice;
    atual = atual->dir;
    Vertice* t = atual->vertice;

    insere_elemento_no_final(&lista_nova, s);
    insere_elemento_no_final(&lista_nova, t);

    atual = atual->dir; // terceiro elemento

    while(atual) {
        Vertice* vertice = atual->vertice;
        Vertice* pai = vertice->pai;
        No* pai_lista = buscar_no(lista_nova, pai);

        if(vertice->minor <= pai_lista->esq->vertice->ordem) 
            insere_elemento(&lista_nova, vertice, pai_lista, "esq");
        else
            insere_elemento(&lista_nova, vertice, pai_lista, "dir");
        atual = atual->dir;
    }

    atual = lista_nova.inicio;
    int contador = 1;
    while(atual) {
      atual->vertice->st_number = contador++;
      atual = atual->dir;
    }

    return lista_nova;
}

int main() {
    Grafo g;
    inicializar_grafo(&g, 6);

    adiciona_aresta(g, 1, 2);
    adiciona_aresta(g, 1, 6);

    adiciona_aresta(g, 2, 3);
    adiciona_aresta(g, 2, 6);
    adiciona_aresta(g, 2, 1);

    adiciona_aresta(g, 3, 4);
    adiciona_aresta(g, 3, 5);
    adiciona_aresta(g, 3, 6);
    adiciona_aresta(g, 3, 2);

    adiciona_aresta(g, 4, 5);
    adiciona_aresta(g, 4, 6);
    adiciona_aresta(g, 4, 3);

    adiciona_aresta(g, 5, 3);
    adiciona_aresta(g, 5, 4);

    adiciona_aresta(g, 6, 1);
    adiciona_aresta(g, 6, 2);
    adiciona_aresta(g, 6, 3);
    adiciona_aresta(g, 6, 4);

    ListaLigada lista_old;
    if (DFST(g, &lista_old)) {
        ListaLigada lista = st_numeracao(lista_old);

        No* atual = lista.inicio;
        while(atual) {
            printf("%i : %i : %i\n", atual->vertice->chave, atual->vertice->minor, atual->vertice->st_number);
            atual = atual->dir;
        }
    }

    return 0;
}