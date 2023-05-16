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

### Descrição do problema:

implementação de um protótipo de sistema de sensoriamento genérico. Na fase de protótipo do projeto será utilizada uma plataforma baseada na NodeMCU para confecção das unidades de sensoriamento. Elas são muito flexíveis e versáteis, sendo ideais para a criação de um ecossistema de Internet das Coisas (IoT). Para simplificar a prova de conceito o sistema deve ser modular, permitindo a substituição na versão de produção.
O sistema será comandado por um Single Board Computer (SBC), e deve ser capaz de controlar o acionamento de um conjunto variável de sensores, assim como monitorar o seu funcionamento, de forma automatizada. Cada operação de leitura ou monitoramento deve ser representada por um código. Dessa forma, o sistema embarcado na NodeMCU deve ser capaz de interpretá-los e realizá-los de maneira adequada, por meio de uma comunicação UART.


### Requisitos da solução:
* O código deverá ser escrito em linguagem C;
* Capacidade de interligação com até 32 unidades de sensoriamento;
* Mecanismo de controle de status de funcionamento das unidades;
* Apenas o SBC será capaz de iniciar uma comunicação.
* Deverá ser capaz de ler e interpretar comandos oriundos do SBC.
* Os comandos serão compostos por palavras de 8 bits (ver tabelas a seguir);
* A informação medida deve ter a maior precisão possível;
* As requisições do SBC podem ser direcionadas para uma unidade específica ou a todas;
* As solicitações e as respostas deverão ser exibidas no display LCD.

Tabela 1: Exemplos de requisição

| Código | Descrição  do comando   |
|:--- |                                   ---: |
| 0x01 |     Solicita a situação atual da unidade de sensoriamento             |
| 0x11 |  Solicita o valor da entrada analógica |
| 0x12 | Solicita o valor de uma das entradas digitais|
| 0x21 | Acendimento do led da NodeMCU        |

Tabela 2: Exemplos de resposta

| Código | Descrição  do comando   |
|:--- |                                   ---: |
| 0x01 |     NodeMCU com problema             |
| 0x11 |  NodeMCU funcionando normalmente |
| 0x12 | Medida de entrada analógica |
| 0x21 | Estado da entrada digital        |


# <a id="hardware_utilizado"></a>
## Hardware utilizado
Existem dois hardwares sendo utilizados.

* Orange PI PC Plus, com 40 pinos GPIO e um processador H3 Quad-core Cortex-A7 H.265/HEVC 4K com arquitetura ARM V7, contendo o sistema operacional Raspbian com Kernel proprietário e em sua versão 5.15.74-sunxi.
   
* NodeMCU (Unidade microcontroladora Node) é um ambiente de desenvolvimento de hardware/software open source feito com base no SoC(System on a Chip) chamado ESP8266. 

# <a id="softwares_utilizados"></a>
## Software utilizado
[GCC](https://gcc.gnu.org/): O GCC, ou GNU Compiler Collection, é um conjunto de compiladores de código aberto desenvolvido pelo Projeto GNU. Ele é amplamente utilizado no desenvolvimento de software para compilar programas escritos em várias linguagens de programação, incluindo C, C++, Fortran, Ada e outros.

O GCC é uma poderosa coleção de compiladores de código aberto amplamente utilizada no desenvolvimento de software. Sua flexibilidade, portabilidade e recursos avançados tornam-no uma escolha popular para compilar programas em várias linguagens de programação e plataformas.

O objetivo principal do GCC é traduzir o código-fonte de um programa em linguagem de programação de alto nível para código de máquina executável. Ele realiza várias etapas durante o processo de compilação, incluindo análise léxica, análise sintática, otimização de código e geração de código.

Além disso é distribuído sob a Licença Pública Geral GNU (GNU GPL), o que significa que seu código-fonte está disponível para modificação e redistribuição livremente. Essa abordagem de código aberto encoraja a colaboração e o aprimoramento contínuo da ferramenta por uma comunidade global de desenvolvedores.

[Arduino IDE](https://www.arduino.cc/en/software): Arduino IDE é um ambiente de desenvolvimento integrado (IDE) que oferece suporte à programação de placas Arduino. É uma plataforma de software de código aberto, fácil de usar e compatível com várias plataformas. Com o Arduino IDE, os usuários podem escrever, compilar e fazer upload de código para suas placas Arduino, permitindo a criação de projetos interativos e dispositivos eletrônicos personalizados de maneira rápida e simples.

# <a id="documentacao"></a>
## Documentação utilizada:
[Datasheet da H3 AllWinner](https://drive.google.com/drive/folders/1JmgtWTlGA-hPv47cLtEYZa-Y3UZPSQNN): Contém todas as informações relacionadas ao funcionamento dos pinos da SBC Orange Pi Pc Plus, bem como seus endereços de memória e informações extras sobre como acessá-las e enviar dados para os pinos relacionados a entrada e saída de propósito geral (GPIO)

[Datasheet ESP8266](https://components101.com/sites/default/files/component_datasheet/ESP8266-NodeMCU-Datasheet.pdf):
Ficha técnica do ESP8266 NodeMCU, uma placa de desenvolvimento baseada no módulo ESP8266. O NodeMCU é um microcontrolador com suporte Wi-Fi embutido, adequado para projetos de IoT. Ele possui um processador de 32 bits, 4 MB de memória Flash e interface USB para programação e comunicação. A placa oferece GPIOs, ADC, PWM e UART, além de suportar protocolos como TCP/IP, UDP, HTTP e MQTT. O NodeMCU é altamente versátil e amplamente utilizado na criação de aplicações conectadas à internet.

[Datasheet do display LCD](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf): Como citado anteriormente, o modelo do display LCD é o Hitachi HD44780U, e sua documentação nos permite descobrir o algoritmo responsável pela inicialização do display bem como o tempo de execução de cada instrução, além da representação de cada caractere em forma de número binário

[Tabela de syscalls do Linux 32 bits para ARM](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI): Documentação contendo tabela de chamadas ao sistema operacional como chamadas de nanoSleep, ou de escrita para serem executadas

[Comunicação UART, material da Universidade de Salento](https://www.unisalento.it/documents/20152/804790/SLIDES+LEZIONE+7-2+INTRODUCTION+TO+UART+COMMUNICATION.pdf/bf04aaf1-3b89-50ee-e1a5-05bda9c59fd3?version=1.0&download=true): Documento contendo introdução sobre comunicações UART, elaborando conceitos como Baud Rate e mostra em detalhes passo a passo de como funciona a transmissão de bits via UART.
