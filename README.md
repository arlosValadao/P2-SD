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

(&nbsp;&nbsp;&nbsp;[**4.** Descrição da Solução](#descricao_solucao))

<!-- &nbsp;&nbsp;&nbsp;[**5.** Testes realizados](#testes_realizados) -->

<!-- &nbsp;&nbsp;&nbsp;[**6.** Limitações da solução desenvolvida](#limitacoes) -->

&nbsp;&nbsp;&nbsp;[**5.** Documentação Utilizada](#documentacao)

<!-- &nbsp;&nbsp;&nbsp;[**5.** Execução do Projeto](#execucao_projeto) -->


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
## Hardwares utilizados


* Orange PI PC Plus, com 40 pinos GPIO e um processador H3 Quad-core Cortex-A7 H.265/HEVC 4K com arquitetura ARM V7, contendo o sistema operacional Raspbian com Kernel proprietário e em sua versão 5.15.74-sunxi.

![Orange_Pi_H3_Pinout jpg 08b35c4e663edbc02a044174a5a154cc](https://github.com/arlosValadao/P2-SD/assets/42982873/c32a02e5-c406-4518-98eb-0bfc1137b4b8)

Especificações técnicas:

| CPU | H3 Quad-core Cortex-A7 H.265/HEVC 4K   |
|:--- |                                   ---: |
| GPU |     Mali400MP2 GPU @600MHz             |
| Memória (SDRAM) |  1GB DDR3 (shared with GPU)|
| Armazenamento interno | Cartão MicroSD (32 GB); 8GB eMMC Flash|
| Rede embarcada | 10/100 Ethernet RJ45        |
| Fonte de alimentação | Entrada DC,<br>entradas USB e OTG não servem como fonte de alimentação | 
| Portas USB | 3 Portas USB 2.0, uma porta OTG USB 2.0 |
| Periféricos de baixo nível | 40 pinos        |


* NodeMCU (Unidade microcontroladora Node) é um ambiente de desenvolvimento de hardware/software open source feito com base no SoC(System on a Chip) chamado ESP8266. 

![pinout-NodeMCU](https://github.com/arlosValadao/P2-SD/assets/42982873/6e4ff7be-0855-499e-b0e0-9da7034c6909)

Especificações técnicas:

* Memória flash: 4 MB
* Tensão de operação:
* Pinos Digitais: 3,3 V
* Pino Analógico: 1,0 V
* Wireless padrão 802.11 b/g/n
* Antena embutida
* Conector micro-usb para programação e alimentação
* Modos de operação: STA/AP/STA+AP
* Suporta 5 conexões TCP/IP
* Portas GPIO: 13
* D0 (GPIO16) só pode ser usado como entrada ou saída, não suporta outras funções (interrupção, PWM, I2C, etc)
* GPIO com funções de PWM, I2C, SPI, etc
* Resolução do PWM: 10 bits (valores de 0 a 1023)
* 01x Conversor analógico digital (ADC)
* Dimensões: 49 x 26 x 7 mm (sem considerar os pinos)


# <a id="softwares_utilizados"></a>
## Softwares utilizados
[GCC](https://gcc.gnu.org/): O GCC, ou GNU Compiler Collection, é um conjunto de compiladores de código aberto desenvolvido pelo Projeto GNU. Ele é amplamente utilizado no desenvolvimento de software para compilar programas escritos em várias linguagens de programação, incluindo C, C++, Fortran, Ada e outros.

O GCC é uma poderosa coleção de compiladores de código aberto amplamente utilizada no desenvolvimento de software. Sua flexibilidade, portabilidade e recursos avançados tornam-no uma escolha popular para compilar programas em várias linguagens de programação e plataformas.

O objetivo principal do GCC é traduzir o código-fonte de um programa em linguagem de programação de alto nível para código de máquina executável. Ele realiza várias etapas durante o processo de compilação, incluindo análise léxica, análise sintática, otimização de código e geração de código.

Além disso é distribuído sob a Licença Pública Geral GNU (GNU GPL), o que significa que seu código-fonte está disponível para modificação e redistribuição livremente. Essa abordagem de código aberto encoraja a colaboração e o aprimoramento contínuo da ferramenta por uma comunidade global de desenvolvedores.

[Arduino IDE](https://www.arduino.cc/en/software): Arduino IDE é um ambiente de desenvolvimento integrado (IDE) que oferece suporte à programação de placas Arduino. É uma plataforma de software de código aberto, fácil de usar e compatível com várias plataformas. Com o Arduino IDE, os usuários podem escrever, compilar e fazer upload de código para suas placas Arduino, permitindo a criação de projetos interativos e dispositivos eletrônicos personalizados de maneira rápida e simples.

## Descrição da solução:

Os conceitos principais que envolvem a solução foram:

1) Realizar comunicação entre a placa SBC e a Node-MCU usando protocolo UART

Antes de mais nada é necessário habilitar e configurar os pinos da Orange PI que estavam diretamente conectados a placa NodeMCU, para isso recorremos ao manual disponibilizado em [link oficial](https://drive.google.com/drive/folders/1tOkewb8F1kN7q0qqOmISZmUP7ZwhRRLB) para ativar os pinos TXD0 e RXD0 da placa conforme esquematizado abaixo.
![uarttx3](https://github.com/arlosValadao/P2-SD/assets/42982873/8fc647e7-e0dc-4036-b808-21395c561e47)

Para isso é necessário alterar o arquivo `/boot/orangepiEnv.txt` e inserir e digitar:
`overlays=uart1 uart2 uart3`
Após isso, reiniciar o sistema.

2) Desenvolver um protocolo capaz de cumprir os requisitos 

<b>Tabela para seleção de unidades:</b>
| Código | Descrição do Comando |
|:--- |                                   ---: |
| 0x1 | Selecionar Unidade 1 |
| 0x2 | Selecionar Unidade 2 |
| 0x3 | Selecionar Unidade 3 |
| 0x4 | Selecionar Unidade 4 |
| 0x5 | Selecionar Unidade 5 |
| 0x6 | Selecionar Unidade 6 |
| 0x7 | Selecionar Unidade 7 |
| 0x8 | Selecionar Unidade 8 |
| 0x9 | Selecionar Unidade 9 |
| 0xA | Selecionar Unidade 10 |
| 0xB | Selecionar Unidade 11 |
| 0xC | Selecionar Unidade 12 |
| 0xD | Selecionar Unidade 13 |
| 0xE | Selecionar Unidade 14 |
| 0xF | Selecionar Unidade 15 |
| 0x10 | Selecionar Unidade 16 |
| ... | ... |
| 0x1F | Selecionar Unidade 31 |

<b>Tabela para desselecionar unidades</b>
| Código | Descrição do Comando |
|:--- |                                   ---: |
| 0x81 | Desselecionar Unidade 1 |
| 0x82 | Desselecionar Unidade 2 |
| 0x83 | Desselecionar Unidade 3 |
| 0x84 | Desselecionar Unidade 4 |
| 0x85 | Desselecionar Unidade 5 |
| 0x86 | Desselecionar Unidade 6 |
| 0x87 | Desselecionar Unidade 7 |
| 0x88 | Desselecionar Unidade 8 |
| 0x89 | Desselecionar Unidade 9 |
| 0x8A | Desselecionar Unidade 10 |
| 0x8B | Desselecionar Unidade 11 |
| 0x8C | Desselecionar Unidade 12 |
| 0x8D | Desselecionar Unidade 13 |
| 0x8E | Desselecionar Unidade 14 |
| 0x8F | Desselecionar Unidade 15 |
| ... | ... |
| 0x9F | Desselecionar Unidade 31 |

<b>Tabela contendo respostas para seleção de unidade</b>

| Código | Descrição do Comando |
|:--- |                                   ---: |
| 0x1 | Selecionada Unidade 1 |
| 0x2 | Selecionada Unidade 2 |
| 0x3 | Selecionada Unidade 3 |
| 0x4 | Selecionada Unidade 4 |
| 0x5 | Selecionada Unidade 5 |
| 0x6 | Selecionada Unidade 6 |
| 0x7 | Selecionada Unidade 7 |
| 0x8 | Selecionada Unidade 8 |
| 0x9 | Selecionada Unidade 9 |
| 0xA | Selecionada Unidade 10 |
| 0xB | Selecionada Unidade 11 |
| 0xC | Selecionada Unidade 12 |
| 0xD | Selecionada Unidade 13 |
| 0xE | Selecionada Unidade 14 |
| 0xF | Selecionada Unidade 15 |
| 0x10| Selecionada Unidade 16 |
| ... | ... |
| 0x1F| Selecionada Unidade 31 |

<b> Tabela contendo respostas para desseleção de unidade </b>

| Código | Descrição do Comando |
|:--- |                                   ---: |
| 0x80 | Desselecionada Unidade 0 |
| 0x81 | Desselecionada Unidade 1 |
| 0x82 | Desselecionada Unidade 2 |
| 0x83 | Desselecionada Unidade 3 |
| 0x84 | Desselecionada Unidade 4 |
| 0x85 | Desselecionada Unidade 5 |
| 0x86 | Desselecionada Unidade 6 |
| 0x87 | Desselecionada Unidade 7 |
| 0x88 | Desselecionada Unidade 8 |
| 0x89 | Desselecionada Unidade 9 |
| 0x8A | Desselecionada Unidade 10 |
| 0x8B | Desselecionada Unidade 11 |
| 0x8C | Desselecionada Unidade 12 |
| 0x8D | Desselecionada Unidade 13 |
| 0x8E | Desselecionada Unidade 14 |
| 0x8F | Desselecionada Unidade 15 |
| ... | ... |
| 0x9F| Desselecionada Unidade 31 |

Abaixo alguns trechos de código que podem auxiliar no entendimento da solução:

  * Integração entre protocolo e SBC usando protoboard:

Tudo isso devia ser feito de modo interativo através dos botões disponibilizados na protoboard, sendo assim foram criadas variáveis nas formas de vetores que pudessem representar as diversas requisições/respostas possíveis:

```
char monitoringLabels[3][10] = { {"D0"}, {"D1"}, {"A0"} };
char monitoringLabels2[3][10] = { {"A0"}, {"D0"}, {"D1"} };
unsigned char monitoringArray[] = { 0xC3, 0xC5, 0xC1 };
unsigned char consultCommands[] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7 };
unsigned char selectNode[MAX_UNITS] = {
  0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
  0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF ,0x10, 0x11,
  0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
  0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
  };
unsigned char deselectNode[MAX_UNITS] = {   
  0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0
  };
```

  * Usando a biblioteca [wiringpi](http://wiringpi.com/), podemos ainda declarar o modo dos botões na protoboard como entrada:

```
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_ENTER, INPUT);
```

  * PARSING E ENVIO DOS DADOS ANALÓGICOS:
Como o dado a ser lido é de 10 bits, e o núm. máximo de bits a ser enviado é de 8, então foi feita uma divisão por 10 do valor lido pelo pino analógico (A0) e após isso enviado em dois pacotes os valores quociente e resto, após serem enviados os valores são unidos novamente na placa SBC.

```
case(0xC1):
  analogData = analogRead(A0);
  quocient = analogData / 10;
  rest = analogData % 10;
  Serial.write(quocient);
  delay(2);
  Serial.write(rest);
  break;
```
Trecho de código de parsing e envio na NodeMCU


# <a id="documentacao"></a>
## Documentação utilizada:
[Datasheet da H3 AllWinner](https://drive.google.com/drive/folders/1JmgtWTlGA-hPv47cLtEYZa-Y3UZPSQNN): Contém todas as informações relacionadas ao funcionamento dos pinos da SBC Orange Pi Pc Plus, bem como seus endereços de memória e informações extras sobre como acessá-las e enviar dados para os pinos relacionados a entrada e saída de propósito geral (GPIO)

[Datasheet ESP8266](https://components101.com/sites/default/files/component_datasheet/ESP8266-NodeMCU-Datasheet.pdf):
Ficha técnica do ESP8266 NodeMCU, uma placa de desenvolvimento baseada no módulo ESP8266. O NodeMCU é um microcontrolador com suporte Wi-Fi embutido, adequado para projetos de IoT. Ele possui um processador de 32 bits, 4 MB de memória Flash e interface USB para programação e comunicação. A placa oferece GPIOs, ADC, PWM e UART, além de suportar protocolos como TCP/IP, UDP, HTTP e MQTT. O NodeMCU é altamente versátil e amplamente utilizado na criação de aplicações conectadas à internet.

[Datasheet do display LCD](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf): Como citado anteriormente, o modelo do display LCD é o Hitachi HD44780U, e sua documentação nos permite descobrir o algoritmo responsável pela inicialização do display bem como o tempo de execução de cada instrução, além da representação de cada caractere em forma de número binário

[Comunicação UART, material da Universidade de Salento](https://www.unisalento.it/documents/20152/804790/SLIDES+LEZIONE+7-2+INTRODUCTION+TO+UART+COMMUNICATION.pdf/bf04aaf1-3b89-50ee-e1a5-05bda9c59fd3?version=1.0&download=true): Documento contendo introdução sobre comunicações UART, elaborando conceitos como Baud Rate e mostra em detalhes passo a passo de como funciona a transmissão de bits via UART.

