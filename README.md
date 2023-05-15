# PBL 2 - SISTEMAS DIGITAIS

* * *

## Autores: 
  * Carlos Valadão [@arlosValadao](https://github.com/arlosValadao)
  * Fernando Mota [@f3rn4nd0000](https://github.com/f3rn4nd0000)
  * Silas Silva [@silas-silva](https://github.com/silas-silva)

* * *

## Seções 

&nbsp;&nbsp;&nbsp;[**1.** Introdução](#introducao)

&nbsp;&nbsp;&nbsp;[**2.** Hardware Utilizado](#hardware_utilizado)

&nbsp;&nbsp;&nbsp;[**3.** Softwares Utilizados](#softwares_utilizados)

&nbsp;&nbsp;&nbsp;[**4.** Descrição da Solução](#descricao_solucao)

&nbsp;&nbsp;&nbsp;[**5.** Testes realizados](#testes_realizados)

&nbsp;&nbsp;&nbsp;[**6.** Limitações da solução desenvolvida](#limitacoes)

&nbsp;&nbsp;&nbsp;[**7.** Documentação Utilizada](#documentacao)

&nbsp;&nbsp;&nbsp;[**8.** Execução do Projeto](#execucao_projeto)


# <a id="introducao"></a>
## Introdução

Este documento apresenta em detalhes como foi feito o desenvolvimento de um sistema de comunicação entre
uma placa SBC Orange Pi e uma unidade NodeMCU com o objetivo de realizar a consulta e monitoramento de
diversos sensores embutidos nessa unidade.

# <a id="hardware_utilizado"></a>
## Hardware utilizado

# <a id="softwares_utilizados"></a>
## Software utilizado
[GCC](https://gcc.gnu.org/): O GCC, ou GNU Compiler Collection, é um conjunto de compiladores de código aberto desenvolvido pelo Projeto GNU. Ele é amplamente utilizado no desenvolvimento de software para compilar programas escritos em várias linguagens de programação, incluindo C, C++, Fortran, Ada e outros.

O GCC é uma poderosa coleção de compiladores de código aberto amplamente utilizada no desenvolvimento de software. Sua flexibilidade, portabilidade e recursos avançados tornam-no uma escolha popular para compilar programas em várias linguagens de programação e plataformas.

O objetivo principal do GCC é traduzir o código-fonte de um programa em linguagem de programação de alto nível para código de máquina executável. Ele realiza várias etapas durante o processo de compilação, incluindo análise léxica, análise sintática, otimização de código e geração de código.

Além disso é distribuído sob a Licença Pública Geral GNU (GNU GPL), o que significa que seu código-fonte está disponível para modificação e redistribuição livremente. Essa abordagem de código aberto encoraja a colaboração e o aprimoramento contínuo da ferramenta por uma comunidade global de desenvolvedores.


# <a id="documentacao"></a>
## Documentação usada:
[Datasheet da H3 AllWinner](https://drive.google.com/drive/folders/1JmgtWTlGA-hPv47cLtEYZa-Y3UZPSQNN): Contém todas as informações relacionadas ao funcionamento dos pinos da SBC Orange Pi Pc Plus, bem como seus endereços de memória e informações extras sobre como acessá-las e enviar dados para os pinos relacionados a entrada e saída de propósito geral (GPIO)

[Datasheet do display LCD](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf): Como citado anteriormente, o modelo do display LCD é o Hitachi HD44780U, e sua documentação nos permite descobrir o algoritmo responsável pela inicialização do display bem como o tempo de execução de cada instrução, além da representação de cada caractere em forma de número binário

[Tabela de syscalls do Linux 32 bits para ARM](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI): Documentação contendo tabela de chamadas ao sistema operacional como chamadas de nanoSleep, ou de escrita para serem executadas


[Comunicação UART, material da Universidade de Salento](https://www.unisalento.it/documents/20152/804790/SLIDES+LEZIONE+7-2+INTRODUCTION+TO+UART+COMMUNICATION.pdf/bf04aaf1-3b89-50ee-e1a5-05bda9c59fd3?version=1.0&download=true): Documento contendo introdução sobre comunicações UART, elaborando conceitos como Baud Rate e mostra em detalhes passo a passo de como funciona a transmissão de bits via UART.