#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>

#define OPCAO_REGISTRAR 1
#define OPCAO_DELETARANTIGAS 2
#define OPCAO_EDITARULTIMO 3
#define OPCAO_VISUALIZARDIA 4
#define NUMERO_GRANDE 70
#define NUMDELETAR 30

/* TODO:
 * Edição
 * Registrar apenas Hora Entrada
 * Registrar apenas Hora Saida;
 * Função de abrir o arquivo (evitar repetir string)
 * Pesquisa e calculo de horas pra tarefa
 */

struct DadosDia {
    int ultimaLinha;
    int horas;
    int minutos;
};

int validaHoras(int horain, int minutoin, int horaout, int minutoout){
    int result = 0;

    if(horain < 5 || horain > 23 || horaout < 5 || horaout > 23 || minutoin > 59 || minutoout > 59){
        printf("\n---Num vem com essas horas malucas nao!---\n\n");
	result = 1;
    }
    if(horaout < horain || (horaout == horain && minutoout < minutoin)){
        printf("\n---Ora ora, um viajante no tempo! Saiu antes de chegar!---\n\n");
        result = 1;
    }
    /*else if(horaout == horain){
        printf("\n---Ah nao. Registra pelo menos uma hora...---\n\n");
        result = 1;
    }*/
    return result;
}


int nextHashTag(char *bugs, int pos){
    int i, max = strlen(bugs);
    
    for(i = pos; i < max; i++){
	if(bugs[i] == '#')
	    return i;
	}
    return -1;
}

int validaBugs(char *bugs){
    int result = 0, count = 0, aux;
    char print[8];
    char resp;

    printf("----Validando Issues! Issues encontradas:\n");
    aux = nextHashTag(bugs, 0);
    while(aux >= 0){
        count ++;
        strncpy(print, &bugs[aux], 7);
        print[7] = '\0';
        printf("\tIssue: %s\n", print);
	aux = nextHashTag(bugs, aux+1);
    }
    if(count){
        printf("-\n%d issue%c, ta certo? [s/n]: ", count, count==1 ? '\0':'s');
    }
    else{
        printf("\nNao encontrei nenhuma issue! Ta certo? [s/n]: ");
    }
    scanf(" %c",&resp);

    if(resp == 's' || resp == 'S')
        result = 0;
    else
        result = 1;
    
    return result;
}

//Extrai as horas trabalhadas da linha e poe em horas e minutos.
void extraiTempoLinha(char *linha, int *horas, int *minutos){
    int i = strlen(linha);
    
    while(linha[i] != '\t')
        i--;
    i++;
    sscanf(&linha[i], "%dh%dmin", horas, minutos);
}

//Ponteiro arquivo deve receber um file ja aberto!
struct DadosDia printHoje(FILE *arquivo, struct tm hoje){
    char *temp = (char *)malloc(256*sizeof(char));
    char hojestr[6];
    int quant=0, line=0, horas=0, minutos=0;
    struct DadosDia retorno;

    retorno.horas = 0;
    retorno.minutos = 0;

    sprintf(hojestr, "%02d/%02d", hoje.tm_mday, hoje.tm_mon+1);

    printf("\nPegando registros de hoje:\n");
    while(fgets(temp,256,arquivo)){             //pega uma linha
        int h, m;
        if(strstr(temp, hojestr)){
            quant++;
            printf("\t-%d: %s",line,temp);
            extraiTempoLinha(temp, &h, &m);
            horas += h;
            minutos += m;
            if(minutos >= 60) {
                minutos -= 60;
                horas++;
            }
        }
        line++;
    }
    if(quant) {
        printf("%d linhas encontradas.\n", quant);
        printf("\n**** Cê já trabalhou %02d:%02d hoje.****\n",horas,minutos);
    }
    else
        printf("\n----Não encontrei nada pra hoje...\n");

    retorno.ultimaLinha = line;
    retorno.horas = horas;
    retorno.minutos = minutos;
    
    free(temp);
    return retorno;
}

//Deleta as linhas antigas do arquivo
//Ponteiro arquivo deve receber um file ja aberto
void deletarAntigas(){
    FILE *novo;
    FILE *arquivo;
    char *temp = (char *)malloc(256*sizeof(char));
    int i;

    arquivo = fopen("/home/francisco/Documents/RegistroHoras.csv", "r");
    printf("*------------ Rotina de deleção ----------------\n");
    printf("* Deletar as %d primeiras linhas...\n", NUMDELETAR);

    novo = fopen("/home/francisco/Documents/RegistroHoras_temp.csv", "w");
    //ir pegando linha ate atingir o fim do arquivo
    for (i = 0; feof(arquivo) == 0 && fgets(temp, 256, arquivo) != NULL; i++) {
        if(i < NUMDELETAR)
            continue;                           //Pular as NUMERO_GRANDE primeiras linhas
        printf("%d ",i);
        fprintf(novo, "%s", temp);              //copiar as restantes
    }
    fclose(novo);
    fclose(arquivo);
    printf("\n* %d linhas no novo arquivo!\n", i-NUMDELETAR);
    if(rename("/home/francisco/Documents/RegistroHoras_temp.csv","/home/francisco/Documents/RegistroHoras.csv")) {
        printf("* ERRO: Erro ao renomear aqruivo!!!\n");
    }
    printf("*----------------- Fim Deleção -----------------\n");
}

//Resposta em *resposta
void *calculaTempo(struct DadosDia *resposta, int horain, int horaout, int minutoin, int minutoout){
    if (minutoout < minutoin) {     //pra não dar negativo na subtraçao, principio da matematica
        horaout --;
        minutoout += 60;
    }
    resposta->horas = horaout-horain;
    resposta->minutos = minutoout-minutoin;
}

void daLinha(int length, char c, char fim) {
    int i = 0;

    while(i < length) {
        printf("%c",c);
        i++;
    }
    printf("%c", fim);
}

void registrarHoras(struct tm hoje, struct DadosDia dadoshj) {
    int horain, minutoin;
    int horaout, minutoout, aux;
    struct DadosDia tempoRegistro;
    char bugs[90];
    FILE *arquivo;
    
    do{
        printf("Hora de Entrada: ");
        scanf("%d:%d", &horain, &minutoin);
        printf("Hora de Saida: ");
        scanf("%d:%d", &horaout, &minutoout);
    } while (validaHoras(horain, minutoin, horaout, minutoout));
    if(horaout > hoje.tm_hour)
        printf("Vejo que és profeta...\n");

    //Pegar o tempo trabalhado
    calculaTempo(&tempoRegistro, horain, horaout, minutoin, minutoout);
	
    do{
        printf("Trabalhou com o que? Resposta: ");
        scanf(" %[^\n]s",bugs);
        printf("\nReconhecendo \"%s\"\n", bugs);
    } while(validaBugs(bugs));

    daLinha(90, '*', '\n');
    printf("Registrando...\n");

    arquivo = fopen("/home/francisco/Documents/RegistroHoras.csv","a");
    if(!arquivo){
        printf("Deu pra abrir o arquivo nao...\nFlw!\n");
        exit(2);
    }
    
    aux = fprintf(arquivo, "%02d/%02d\t%02d:%02d\t%02d:%02d\t%s\t%02dh%02dmin\n",
            hoje.tm_mday, hoje.tm_mon + 1, horain, minutoin, horaout, minutoout, bugs, tempoRegistro.horas, tempoRegistro.minutos);
    if(aux > 0){
        printf("Ok. Registrei... Confere Ae: ");
        printf("%02d/%02d\t%02d:%02d\t%02d:%02d\t%s\n",hoje.tm_mday, hoje.tm_mon + 1, horain, minutoin, horaout, minutoout, bugs);
        daLinha(90, '-', '\n');
        daLinha(20, '-', ' ');
        printf("%02dh%02dmin de trabalho nesse registro... ", tempoRegistro.horas, tempoRegistro.minutos);
        daLinha(20, '-', '\n');
        dadoshj.horas += tempoRegistro.horas;
        dadoshj.minutos += tempoRegistro.minutos;
        if(dadoshj.minutos > 59) {
            dadoshj.minutos -= 60;
            dadoshj.horas++;
        }
        daLinha(20, '#', ' ');
        printf("%01dh%02dmin trabalhados hoje até agora ", dadoshj.horas, dadoshj.minutos);
        daLinha(20, '#', '\n');
    }
    else {
        printf("ERRO: Consegui escrever nao...\n");
    }
    
    fclose(arquivo);

}

void editarLinha(int linha) {
    FILE *arquivo = fopen("/home/francisco/Documents/RegistroHoras.csv", "r+");
    char *temp = (char *)malloc(256*sizeof(char));
    char *entrada = (char *)malloc(13*sizeof(char));
    char *saida = (char *)malloc(13*sizeof(char));
    struct DadosDia tempoRegistro;
    char bugs[90];

    int horain, minutoin, horaout, minutoout, dia, mes;
    int horainArq, minutoinArq, horaoutArq, minutooutArq;
    int i = 0;

    while(fgets(temp,256,arquivo) && --linha);
    
    if(linha == 0) {
        daLinha(20, '-', '\n');
        printf("Seguinte: %s\n", temp);
        // parse na linha
        sscanf(temp, "%d/%d %d:%d %d:%d %s ", &dia, &mes, &horainArq, &minutoinArq, &horaoutArq, &minutooutArq, bugs);
        printf("Os bug: %s\n",bugs);

        sprintf(entrada, "%02d:%02d\b\b\b\b\b", horainArq, minutoinArq);
        sprintf(saida, "%02d:%02d\b\b\b\b\b", horaoutArq, minutooutArq);
        
        horain = horainArq;
        minutoin = minutoinArq;
        horaout = horaoutArq;
        minutoout = minutooutArq;

        do{
            printf("Hora de Entrada: %02d:%02d\b\b\b\b\b", horainArq, minutoinArq);
            sscanf(readline(entrada), "%d:%d", &horain, &minutoin);
            printf("Hora de Saida: %02d:%02d\b\b\b\b\b", horaoutArq, minutooutArq);
            sscanf(readline(saida), "%d:%d", &horaout, &minutoout);
        } while(validaHoras(horain, minutoin, horaout, minutoout));
        
        calculaTempo(&tempoRegistro, horain, horaout, minutoin, minutoout);

        do{
            printf("Trabalhou com o que? Resposta: ");
            scanf(" %[^\n]s",bugs);
            printf("\nReconhecendo \"%s\"\n", bugs);
        } while(validaBugs(bugs));

        fseek(arquivo, -1*strlen(temp), SEEK_CUR);

        //reaproveitando variável. linha aqui é o tamanho de temp
        linha = strlen(temp);
        while(i < linha)
            temp[i++] = 0;      //limpando string

        sprintf(temp, "%02d/%02d\t%02d:%02d\t%02d:%02d\t%s\t%02dh%02dmin\n",
                dia, mes, horain, minutoin, horaout, minutoout, bugs, tempoRegistro.horas, tempoRegistro.minutos);

        if (fputs(temp, arquivo)) {
            printf("Editei, ficou assim: \n\t%s\n", temp);
        } else {
            printf("ERRO: Num deu pra escrever no arquivo não...\n");
        }

    } else {
        printf("ERRO: Cê passou um número de linha além da quantidade, vacilão!");
    }
    
    daLinha(20, '-', '\n');
    free(temp);
    free(entrada);
    free(saida);
    fclose(arquivo);
}


// Printa o dia especifico do arquivo. Se encontra-lo.
// O dia e mes devem ser validos e o arquivo deve estar aberto ja.
void printDia(FILE *arquivo, int dia, int mes) {
    
    char *temp = (char *)malloc(256*sizeof(char));
    char diastr[6];
    int horas = 0, minutos = 0, quant = 0, line = 0;

    sprintf(diastr, "%02d/%02d", dia, mes);
    
    while(fgets(temp,256,arquivo)){             //pega uma linha
        int h, m;
        if(strstr(temp, diastr)){
            quant++;
            printf("\t-%d: %s",line,temp);
            extraiTempoLinha(temp, &h, &m);
            horas += h;
            minutos += m;
            if(minutos >= 60) {
                minutos -= 60;
                horas++;
            }
        }
        line++;
    }
    if(quant) {
        printf("%d linhas encontradas.\n", quant);
        printf("\n**** Cê já trabalhou %02d:%02d em %d/%d.****\n",horas,minutos, dia, mes);
    }
    else
        printf("\n---- %d/%d???? Ce num registrou esse dia não...\n", dia, mes);

    free(temp);

}

void visualizarDia() {
    int dia, mes;

    FILE *arquivo = fopen("/home/francisco/Documents/RegistroHoras.csv", "r");

    daLinha(40, '.', '\n');
    
    do {
        printf("Que dia? [dd/MM]: ");
        scanf("%d/%d", &dia, &mes);
    
        if (dia < 32 && mes < 13) {
            printDia(arquivo, dia, mes);
        } else {
            printf("Dia %d/%d não existe, seu zé...\n", dia, mes);
        }
    } while(dia > 31 || mes > 12);

    daLinha(40, '.', '\n');
    fclose(arquivo);
}

int main(int argc, char **argv){
    int aux, opcao;
    time_t t = time(NULL);
    struct tm hoje = *localtime(&t);
    FILE *arquivo;
    struct DadosDia dadoshj;

    daLinha(90, '*', '\n');
    printf("Bem vindo ao First Francisco Registrador de Horas!!!***\n");
    printf("Hoje eu deleto linhas antigas, calculo o tanto do dia, o tanto do registro que ce trabalhou!\nE agora até visualizo e edito registro! Então fica de boa aí! \n");
    printf("\nHoje é: %d/%d/%d\n", hoje.tm_year +1900, hoje.tm_mon+1, hoje.tm_mday);
    daLinha(90, '-', '\n');

    //verificar as entradas e as de hoje, pra futura edicao.

    arquivo = fopen("/home/francisco/Documents/RegistroHoras.csv", "r");
    if(!arquivo){
        printf("ERRO: Cade o arquivo RegistroHoras.csv?\nSe continuar vou criar um novo!\n");
    }
    else{
        dadoshj = printHoje(arquivo, hoje);
        fclose(arquivo);
        daLinha(90, '*', '\n');
        printf("* Tem %d linhas no arquivo, ta? Ta.\nO que ce quer fazer?\n", dadoshj.ultimaLinha);
        printf("*\t1 - Registar Horas.\n");
        printf("*\t2 - Deletar as %d mais antigas.\n", NUMDELETAR);
        printf("*\t3 - Editar último registro.\n");
        printf("*\t4 - Visualizar horas do dia.\n");
        printf("* Opção: ");
        scanf("%d",&opcao);
        switch(opcao) {
            case OPCAO_REGISTRAR:
                daLinha(50, '-', '\n');
                printf("Vamos registrar!\n");
                registrarHoras(hoje, dadoshj);
                break;
            case OPCAO_DELETARANTIGAS:
                daLinha(50, '*', '\n');
                deletarAntigas();
                break;
            case OPCAO_EDITARULTIMO:
                editarLinha(dadoshj.ultimaLinha);
                break;
            case OPCAO_VISUALIZARDIA:
                visualizarDia();
                break;
            default:
                printf(" Nao sabe ler o número das opções???? ");
                break;
        }
    }

    printf("\nFlw!\n");
    daLinha(90, '*', '\n');

    return 0;
}
