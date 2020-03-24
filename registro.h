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
#ifndef REGISTRO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REGISTRO

// É o nome que você quer dar pro csv que armazenará seus registros.
// Tô com pressa mas finalmente pude dar uma melhorada aqui
#define NOME_ARQUIVO "RegistroHoras.csv"

// Numero de linhas a deletar
#define NUMDELETAR 30

/**
* Inicializa o caminho do arquivo com a sua $HOME/Documents e NOME
* Nota: Execute ao iniciar seu programa
*/
void preparaCaminhoArquivo();

/**
 * Abre o arquivo no modo desejado.
 * modo: "r", "r+", "w", etc...
 */
FILE *abreRegistro(char *modo);

// Faz favor de não usar essa função sem ter aberto o arquivo, ok?
void fechaRegistro();

//Deleta as linhas antigas do arquivo
//Ponteiro arquivo deve receber um file ja aberto
void deletarAntigas();

#endif
