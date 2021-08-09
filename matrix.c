#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"


int matrix_insert_element(Matrix **m, int linha, int coluna, float info){ // Criei essa função para inserir elementos nas matrizes

	int i;
	Matrix *novo = (Matrix*)malloc(sizeof(Matrix));
	if (!novo){return 1;}
	Matrix *aux = *m;
	Matrix *auxCol = NULL;
	Matrix *auxLin = NULL;
	Matrix *ant = NULL;
	
	if (info == 0){ // Para inserir células cabeça
		if (linha == -1){
			while (aux->right != *m){ 
				aux = aux->right;
			}
			aux->right = novo;
			novo->right = *m;
			novo->below = novo;
		}
		if (coluna == -1){
			while (aux->below != *m){ 
				aux = aux->below;
			}
			aux->below = novo;
			novo->right = novo;
			novo->below = *m;
		}
		
	}else{ // Para inserir elementos da matriz
	
		// Inserir elemento na lista circular da coluna
		for (i = 1; i <= coluna; i++){
			aux = aux->right;
		}
		auxCol = aux;
		
		while (aux->below != auxCol && aux->line < linha){
			ant = aux;
			aux = aux->below;
		}
		
		if (aux->line == linha){ // Para o caso da célula já existir
			aux->info = info;
		}else{		
			if (aux->line > linha){ // Um elemento no meio
				ant->below = novo;
				novo->below = aux;
			}else{ // Inserir o último elemento da coluna até o momento
				aux->below = novo;
				novo->below = auxCol;
			}
		}
		
		// Inserir elemento na lista circular da linha
		aux = *m;
		for (i = 1; i <= linha; i++){
			aux = aux->below;
		}
		auxLin = aux;
		while (aux->right != auxLin && aux->column < coluna){
			ant = aux;
			aux = aux->right;
		}

		if (aux->column == coluna){ // Para o caso da celula ja existir
			aux->info = info;
		}else{		
			if (aux->column > coluna){ // Um elemento no meio
				ant->right = novo;
				novo->right = aux;
			}else{ // Inserir o ultimo elemento da coluna ate o momento
				aux->right = novo;
				novo->right = auxLin;
			}
		}
		
	}
	
	/* Apesar do TAD não utilizar os campos linha, coluna e info das celulas cabeça, eu optei por gravar estes valores
	possibilitando o uso de uma única função para a criação de células cabeça e também para inserção de elementos na matriz */
	novo->line = linha;
	novo->column = coluna;
	novo->info = info;
	//printf("Elemento %f adicionado na linha %d, coluna %d\n", novo->info, novo->line, novo->column);
	
	return 0;
}

int matrix_line_size (const Matrix* m){ // Função simples que retorna o número de linhas de uma matriz qualquer
	int cont = 0;
	Matrix *aux = m;
	
	while (aux->below != m){
		aux = aux->below;	
		cont++;
	}
	
	return cont;
}

int matrix_column_size (const Matrix* m){ // Função simples que retorna o número de colunas de uma matriz qualquer
	int cont = 0;
	Matrix *aux = m;
	
	while (aux->right != m){
		aux = aux->right;	
		cont++;
	}
	
	return cont;
}

int matrix_create(Matrix** m){
	int nLinhas, nColunas, linha, coluna, i;
	float info;
	
	printf("\nEntre com o numero de linhas da matriz : ");
	scanf("%d", &nLinhas);
	printf("\nEntre com o numero de colunas da matriz : ");
	scanf("%d", &nColunas);
	
	if (nLinhas < 1 || nColunas < 1){
		printf ("\nNumero de linhas e colunas da matriz deve ser positivo.");
		return 1;
	}
	
	*m = (Matrix*)malloc(sizeof(Matrix));
	if (!*m){return 1;}
	(*m)->right = *m;
	(*m)->below = *m;
	(*m)->line = -1;
	(*m)->column = -1;
	(*m)->info = 0; // OBS -> "(*m)->info = 0;" é a mesma coisa que "(**m).info = 0;"
	
	// Criando células cabeça
	for (i = 1; i <= nColunas; i++){
		matrix_insert_element (m, -1, i, 0);
	}
	for (i = 1; i <= nLinhas; i++){
		matrix_insert_element (m, i, -1, 0);
	}
	
	// Lendo e inserindo os elementos da matriz
	printf("\nEntre com os elementos da matriz a seguir :\n");
	while (1){
		printf("\nLinha : ");
		scanf("%d", &linha);
		if (linha == 0) break;
		printf("Coluna : ");
		scanf("%d", &coluna);
		printf("Valor : ");
		scanf("%f", &info);
		
		if (linha < 1 || linha > nLinhas){ // Verificando se a linha digitada está nos limites da matriz
			printf("Linha invalida. Insira o elemento novamente.\n");
		}else{
			if (coluna < 1 || coluna > nColunas){ // Verificando se a coluna digitada está nos limites da matriz
				printf("Coluna invalida. Insira o elemento novamente.\n");
			}else{
				matrix_insert_element (m, linha, coluna, info);
			}
		}
		
	}
	
	return 0;
}

void matrix_line_destroy(Matrix *m){
	if (m->right->column != -1){
		matrix_line_destroy(m->right);
	}
	if (m->column != -1){
		free(m);
	}	
}

int matrix_destroy(Matrix* m){
	int i;
	int linhas = matrix_column_size(m);
	
	Matrix *aux = NULL;
	
	for (i = 0; i <= linhas; i++){
		matrix_line_destroy(m);
		m = m->below;
	}
	
	for (i = 0; i <= linhas; i++){
		aux = m;
		m = m->below;
		free(aux);
	}
	
	return 0;
}

int matrix_print(const Matrix* m){
	if (!m){return 1;} // Condição para erro
	Matrix *aux = m->below;
	
	printf ("\n%d  %d\n", matrix_line_size(m), matrix_column_size(m));
	while (aux != m){
		while (aux->right->column != -1){
			aux = aux->right;
			printf("%d  %d  %.1f\n", aux->line, aux->column, aux->info);
		}
		aux = aux->right->below;
	}
	
	return 0;
}

int matrix_add(const Matrix* m, const Matrix* n, Matrix** r){
	int i, j;
	float info, elemA, elemB;
	
	if (matrix_line_size(m) != matrix_line_size(n) || matrix_column_size(m) != matrix_column_size(n)){ // Tamanhos incompativeis, soma não pode ser realizada
		printf ("Matrizes não podem ser somadas (tamanhos incompativeis)");
		return 1;
	}else{
		// Destruindo qualquer valor que esteja dentro da matriz r que conterá a solução
		// matrix_destroy(r);
		
		// Alocando a matriz r
		*r = (Matrix*)malloc(sizeof(Matrix));
		if (!*r){return 1;}
		(*r)->right = *r;
		(*r)->below = *r;
		(*r)->line = -1;
		(*r)->column = -1;
		(*r)->info = 0;
		
		// Criando as células cabeça de r
		for (i = 1; i <= matrix_column_size(m); i++){
			matrix_insert_element (r, -1, i, 0);
		}
		for (i = 1; i <= matrix_line_size(m); i++){
			matrix_insert_element (r, i, -1, 0);
		}		
		
		// Precorrendo as matrizes m e n e realizando a soma em r
		for (i = 1; i <= matrix_line_size(m); i++){
			for (j = 1; j <= matrix_column_size(m); j++){
				matrix_getelem(m, i, j, &elemA);
				matrix_getelem(n, i, j, &elemB);
				info = elemA + elemB;
				if (info != 0){
					matrix_insert_element (r, i, j, info);
				}
			}
		}
		
		return 0;
	}
}

int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r){
	int i, j, k;
	float info, elemA, elemB;
	
	if (matrix_column_size(m) != matrix_line_size(n)){ // Tamanhos incompativeis, soma não pode ser realizada
		printf ("Matrizes não podem ser somadas (tamanhos incompativeis)");
		return 1;
	}else{
		// Destruindo qualquer valor que esteja dentro da matriz r que conterá a solução
		// matrix_destroy(r);
		
		// Alocando a matriz r
		*r = (Matrix*)malloc(sizeof(Matrix));
		if (!*r){return 1;}
		(*r)->right = *r;
		(*r)->below = *r;
		(*r)->line = -1;
		(*r)->column = -1;
		(*r)->info = 0;
		
		// Criando as células cabeça de r
		for (i = 1; i <= matrix_column_size(n); i++){
			matrix_insert_element (r, -1, i, 0);
		}
		for (i = 1; i <= matrix_line_size(m); i++){
			matrix_insert_element (r, i, -1, 0);
		}		
		
		// Precorrendo as matrizes m e n e realizando a soma em r
		for (i = 1; i <= matrix_line_size(m); i++){
			for (j = 1; j <= matrix_column_size(n); j++){
				info = 0;
				for (k = 1; k <= matrix_column_size(m); k++){
					matrix_getelem(m, i, k, &elemA);
					matrix_getelem(n, k, j, &elemB);
					info += elemA*elemB;
				}
				if (info != 0){
					matrix_insert_element (r, i, j, info);
				}
			}
		}
		
		return 0;
	}
}

int matrix_transpose(const Matrix* m, Matrix** r){
	int i, j;
	float info, elem;
	
	// Destruindo qualquer valor que esteja dentro da matriz r que conterá a solução
	// matrix_destroy(r);
	
	// Alocando a matriz r
	*r = (Matrix*)malloc(sizeof(Matrix));
	if (!*r){return 1;}
	(*r)->right = *r;
	(*r)->below = *r;
	(*r)->line = -1;
	(*r)->column = -1;
	(*r)->info = 0;
	
	// Criando as células cabeça de r
	for (i = 1; i <= matrix_line_size(m); i++){
		matrix_insert_element (r, -1, i, 0);
	}
	for (i = 1; i <= matrix_column_size(m); i++){
		matrix_insert_element (r, i, -1, 0);
	}		
	
	for (i = 1; i <= matrix_line_size(m); i++){
		for (j = 1; j <= matrix_column_size(m); j++){
			matrix_getelem(m, i, j, &elem);
			if (elem != 0){
				matrix_insert_element (r, j, i, elem);	
			}
		}
	}
		
	return 0;

}

int matrix_getelem(const Matrix* m, int x, int y, float *elem){ // x = linha, y = coluna
	int i;
	Matrix *aux = m;
	Matrix * auxCol = NULL;
	
	for (i = 1; i <= y; i++){
		aux = aux->right;
	}
	auxCol = aux;
	
	while (aux->below != auxCol && aux->line != x){
		aux = aux->below;
	}
	
	if (aux->line == x){ // Para o caso de valores diferentes de zero
		*elem = aux->info;
	}else{ // Para o caso do valor da posição pedida ser zero
		*elem = 0;
	}
	free(aux);
	free(auxCol);
	
	return 0;
}

int matrix_setelem(Matrix* m, int x, int y, float elem){ // x = linha, y = coluna 
	if (matrix_insert_element (&m, x, y, elem)){return 1;}
	return 0;
}


