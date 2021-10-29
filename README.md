# ControleHoras
O ControleHoras é um nome criativo e revolucionário escolhido para um programa tão criativo e revolucionário quanto que nada mais faz do que
registrar suas horas de trabalho no formato que ele quer!

Para controlar as horas, o ControleHoras conta com uma feature presente no cérebro do usuário (controle), sendo este o encarregado do
real controle. Sem esta feature, o ControleHoras é apenas um registrador com umas coisinhas a mais que, de acordo com a paciência, vão
surgindo.

### Que que tem aí?
- Registro de hora de entrada, hora de saída e a tarefa realizada nesse periodo (opcao 1)
- Exclusão dos registros antigos, apaga os 30 registros mais antigos (opcao 2)
- Suporte a mais de um registro por dia (revolucionário, não?)
- Edição do último registro (opção 3)
- Cálculo das horas trabalhadas no dia (opção 4)
- Suporte a cálculo de horas gasta com uma determinada tarefa (opção 5) (necessária feature no usuário para padronizar com um label os
registros de uma mesma tarefa)
    - Como assim? Suponha que você trabalhou de 10h às 12h e de 16h às 18h numa mesma tarefa, de numero 000123. Ao registrar no ControleHoras
    basta adicionar, por exemplo #000123, no seu resumo. Pesquisando depois por #000123, ele te retornará todas as horas gastas com a tarefa.

Escolhendo uma opção diferente (ex.: 6), você recebe um insulto acerca de uma feature do usuário que pode não estar funcionando corretamente.

### Não espere encontrar
Um código organizado, limpo, de funcionamento perfeito, com boas práticas.

Este repositório não foi construído para a Crítica da Convenção de Programadores. Foi escrito em ultra-velocidad enquanto realizava build/deploy
de outras aplicações.


## Pré-configuração necessária
Até agora você precisa do
- GCC
- Make
- e da lib `libreadline`.

No presente momento, ela parece estar na versão 8. Aí basta

### Instalar a Libreadline
**Ubuntu**
```bash
sudo apt install libreadline8 libreadline-dev
```
**OpenSuse**
```bash
sudo zypper install libreadline8 libreadline-dev
```

**Outros Sistemas Operacionais**

Algo semelhante a isso aí

**Windows**

Não ouse utilizar o ControleHoras nisto.


## Compilar
```bash
mkdir out
make
```

## Executar
```bash
./out/controlehoras
```

O resto, de uma forma muito educada, ele vai te ensinar.
Seja feliz!


