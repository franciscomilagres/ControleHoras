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

/**
 * Abre o arquivo no modo desejado.
 * modo: "r", "r+", "w", etc...
 */
FILE *abreRegistro(char *modo) {
    _registro = fopen("/home/francisco/Documents/RegistroHoras.csv", modo);
    if (!_registro) {
        if (strcmp(modo, "r")) {
            printf("ERRO: Deu pra abrir o arquivo não...\n");
            exit(2);
        } else {
            printf("ERRO: Cade o arquivo RegistroHoras.csv?\nSe continuar vou criar um novo!!!\n");
        }
    }
    return _registro;
}


// Faz favor de não usar essa função sem ter aberto o arquivo, ok?
void fechaRegistro() {
    fclose(_registro);
}


//Deleta as linhas antigas do arquivo
//Ponteiro arquivo deve receber um file ja aberto
void deletarAntigas(){
    FILE *novo;
    FILE *arquivo;
    char *temp = (char *)malloc(256*sizeof(char));
    int i;

    arquivo = abreRegistro("r");
    printf("*------------ Rotina de deleção ----------------\n");
    printf("* Deletar as %d primeiras linhas...\n", NUMDELETAR);

    novo = fopen("/home/francisco/Documents/RegistroHoras_temp.csv", "w");
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
    if (rename("/home/francisco/Documents/RegistroHoras_temp.csv", CAMINHO)) {
        printf("* ERRO: Erro ao renomear aqruivo!!!\n");
    }
    printf("*----------------- Fim Deleção -----------------\n");
}
