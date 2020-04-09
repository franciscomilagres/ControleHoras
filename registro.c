/** Projeto Controle Horas -- Francisco Milagres                                *
 * Autor: Francisco Milagres.                                                   *
 * -----------------------------------------------------------------------------*
 * Biblioteca dedicada a controlar as operações realizadas no arquivo CSV que é *
 * utilizado para registrar as horas trabalhadas. Inicialmente, basta configurar*
 * o caminho do arquivo e a intenção é que ela tome conta de tudo.              *
 * --------------------SOFTWARE LIVRE------------------------------------------ *
 *  O uso desta biblioteca é completamente livre, bem como o de todos os        *
 *  componentes do projeto. O autor fica isento de qualquer responsabilidade    *
 *  acerca de problemas provocados pelo uso deste arquivo.                      *
 *  Resumindo, use por sua conta e risco e não reclama comigo. Flw!             *
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include "registro.h"

// Global dedicado a evitar o uso excessivo de ponteiros nos parâmetros das funções
// Aponta para o arquivo que é manipulado.
// VOCÊ NÃO PRECISA ACESSAR ELE DIRETAMENTE! AS FUNÇÕES VÃO DAR ELE PRA VOCÊ
FILE *_registro;

// Global dedicado a armazenar o caminho do seu arquivo.
// Afinal, pra que um festival de ponteiros?
// MESMA COISA, SO QUE AS FUNÇÕES É QUE VÃO USAR
char *_caminho;

/**
* Configura o *_caminho com sua $HOME/Documents e o nome
* definido no .h.
*/
void preparaCaminhoArquivo() {
    _caminho = (char *)malloc(100*sizeof(char));
    if (_caminho != NULL) {
        strcat( strcpy(_caminho, getenv("HOME")), "/Documents/");
        strcat(_caminho, NOME_ARQUIVO);
    } else {
        printf("ERRO: Falha ao alocar memória!");
        exit(1);
    }
}



/**
 * Abre o arquivo no modo desejado.
 * modo: "r", "r+", "w", etc...
 */
FILE *abreRegistro(char *modo) {
    _registro = fopen(_caminho, modo);
    if (!_registro) {
        if (strcmp(modo, "r")) {
            printf("ERRO: Deu pra abrir o arquivo não...\n");
            exit(2);
        } else {
            if (_caminho == NULL) {
                printf("ERRO: Parece que você não rodou o preparaCaminhoArquivo, zé! Tem que rodar!\n");
                printf("LOG: _caminho = %s\n", _caminho ? _caminho : "NULL");
                exit(2);
            }
            else {
                printf("\n(((OPA))): Cade o arquivo %s ?\nSe continuar vou criar um novo!!!\n\n", _caminho);
            }
        }
    }
    return _registro;
}


// Faz favor de não usar essa função sem ter aberto o arquivo, ok?
void fechaRegistro() {
    fclose(_registro);
}


//Deleta as linhas antigas do arquivo
void deletarAntigas(){
    FILE *novo;
    FILE *arquivo;
    char *temp = (char *)malloc(256*sizeof(char));
    char *nometmp = (char *)malloc(100*sizeof(char));
    int i;

    strcat( strcpy(nometmp, getenv("HOME")), "/Documents/dctmp.csv" );

    arquivo = abreRegistro("r");
    printf("*------------ Rotina de deleção ----------------\n");
    printf("* Deletar as %d primeiras linhas...\n", NUMDELETAR);

    novo = fopen(nometmp, "w");
    //ir pegando linha ate atingir o fim do arquivo
    for (i = 0; feof(arquivo) == 0 && fgets(temp, 256, arquivo) != NULL; i++) {
        if(i < NUMDELETAR)
            continue;                           //Pular as NUMDELETAR primeiras linhas
        printf("%d ",i);
        fprintf(novo, "%s", temp);              //copiar as restantes
    }
    fclose(novo);
    fechaRegistro();
    printf("\n* %d linhas no novo arquivo!\n", (i > NUMDELETAR) ? i-NUMDELETAR : 0);
    
    if (rename(nometmp, _caminho)) {
        printf("* ERRO: Erro ao renomear aqruivo!!!\n");
    }
    printf("*----------------- Fim Deleção -----------------\n");

    free(temp);
    free(nometmp);
}
