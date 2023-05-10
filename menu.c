#include <stdio.h>
#include <wiringPi.h>
#include <lcd.h>



//USE WIRINGPI PIN NUMBERS
#define LCD_RS  13               //Register select pin
#define LCD_E   18               //Enable Pin
#define LCD_D4  21               //Data pin 4
#define LCD_D5  24               //Data pin 5
#define LCD_D6  26               //Data pin 6
#define LCD_D7  27               //Data pin 7
#define BUTTON_MAIS  23          // PA07
#define BUTTON_MENOS  25         // PA10
#define BUTTON_ENTER  19         // PA20

#define UART_3 "/dev/ttyS3"
#define BAUD_RATE 115200


int lcd;
int fd;


int qtdItensMenu01 = 33;
int qtdItensMenu02 = 8;


int unidadeSelecionada = 0;
int escolhaFazerNaNode = 0;



void imprimir_menu_lcd(char opcoes_menu[][30], int posicaoAtual) {
    // Limpa o display
    lcdClear(lcd);    
    // imprimindo as strings do vetor no LCD
    //lcdPosition(lcd, 0, 0);
    lcdPrintf(lcd, ("%s\n", opcoes_menu[posicaoAtual]));
}



void mostrar_menu_01(int vetor_menu01[][30], int posicaoAtual){
    // atribuindo valores às strings do vetor
    imprimir_menu_lcd(vetor_menu01, posicaoAtual);
}



void mostrar_menu_02(int vetor_menu02[][30],int posicaoAtual){
    // atribuindo valores às strings do vetor
    imprimir_menu_lcd(vetor_menu02, posicaoAtual);
}

// Envia a informacao com base no vetor e posicao passadas
void sendData(int fd, unsigned char* array, unsigned char pos) {
    serialPutchar(fd, array[pos]);
    delay(2);
}

// Implementa o timeout
unsigned char recvDigitalData(int fd) {
    if (serialDataAvail(fd) > 0) return serialGetchar(fd);
    delay(8);
    if (serialDataAvail(fd) > 0) return serialGetchar(fd);
    return 0; // Nao existe nenhum dado a ser lido no buffer
}

// Move os dados do buffer para o vetor de
// acordo com as regras definidas no protocolo
void getAnalogData(int fd, unsigned char* analogBytes) {
    if (serialDataAvail(fd) > 0) {
        analogBytes[0] = serialGetchar(fd);
        delay(3);
        analogBytes[1] = serialGetchar(fd);
        return bytes2short(analogBytes);
    }
}

unsigned short int recvAnalogData(int fd, unsigned char* analogBytes) {
    // Tenta ler o buffer
    getAnalogData(fd, analogBytes);
    // Tempo de timeout
    delay(8);
    // Tenta ler o buffer
    getAnalogData(fd, analogBytes);
    return 0; // Nao existe nenhum dado a ser lido no buffer
}

unsigned short int bytes2short(unsigned char* analogBytes) {
    unsigned short int analogData = 0;
    analogData = (analogData | analogBytes[1]) << 8;
    analogData = (analogData | analogBytes[0])
    return analogData;
}

unsigned short int recvData(int fd, unsigned char* analogBytes, int pos) {
    // Sensor Analogico
    if (pos == 1) return recvAnalogData(fd, analogBytes);
    // Sensor digital
    return (unsigned short int) recvDigitalData(fd);
}

int main() {
    // Guarda o indice da node selecionada
    unsigned char selectedNode;
    // Guarda os bytes provenientes do sensor analogico da Node
    unsigned char analogBytes[2];
    unsigned char followCommands[] = { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6 };
    unsigned char ConsultCommands[] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6 };
    unsigned char select_node[] = {
                                        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                                        0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0x10
                                    };
    unsigned char deselect_node[] = {   
                                        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                                        0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F
                                    };
    // Menus
    char vetor_menu01[qtdItensMenu01][30];
    char vetor_menu02[qtdItensMenu02][30];

    // Inicializar biblioteca
    if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        return 1;
    }

    // Setar botões como entrada
    pinMode(BUTTON_MAIS, INPUT);
    pinMode(BUTTON_MENOS, INPUT);
    pinMode(BUTTON_ENTER, INPUT);
    
    int buttonMaisState;
    int buttonMenosState;
    int buttonEnterState;


    // Fazer os menus
    // Menu 01
    sprintf(vetor_menu01[0], "Selecionar Todas as Unidades");
    for (int i = 1; i < qtdItensMenu01 - 1 ; i++) {
        sprintf(vetor_menu01[i], "Selecionar Unidade %d", i);
    }
    sprintf(vetor_menu01[qtdItensMenu01 - 1], "Sair");

    // Menu 02
    sprintf(vetor_menu02[0], "Acender Led");
    sprintf(vetor_menu02[1], "Sensor Analogico A0");
    sprintf(vetor_menu02[2], "Monitorar Analogico A0");
    sprintf(vetor_menu02[3], "Sensor Digital D0");
    sprintf(vetor_menu02[4], "Monitorar Digital D0");
    sprintf(vetor_menu02[5], "Sensor Digital D0");
    sprintf(vetor_menu02[6], "Monitorar Digital D1");
    sprintf(vetor_menu02[7], "Voltar");
    

    // Configurar UART
    if ((fd = serialOpen (UART_3, BAUD_RATE)) < 0){
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        return 1 ;
    }

    // Iniciar LCD
    if ((lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0) > 0))
    {
        printf("Erro ao inicializar o LCD");
        return 1;
    }

    lcdClear(lcd);
    lcdPuts(lcd, "BEM-VINDO");
    
    
    // Menu ativo no momento
    int menu01 = 1;
    int menu02 = 0;

    //
    int escolhaMenu01;
    int escolhaMenu02;

    // Guarda os dados provenientes da Node
    unsigned short int recvdData = 0;

    // Variavel para controlar o que vai ser mostrado no Menu
    int posicao = 0;
    
    // Mostrar a primeira opção do primeiro Menu
    delay(2000);
    mostrar_menu_01(vetor_menu01, posicao);
    
    while(1){
        // Verificar se algum botão foi pressionado
        buttonMaisState = digitalRead(BUTTON_MAIS);
        buttonMenosState = digitalRead(BUTTON_MENOS);
        buttonEnterState = digitalRead(BUTTON_ENTER);
        
        // Entender esse Dalay, se precisa ou não
        delay(200);
        
        if (!buttonMaisState) {
            // O botão foi pressionado
            posicao++;
            
            // Verificar se o que vai ser mostrado é o menu 1 ou 2
            if (menu01 == 1){
                // Se posição passar da quantidade de itens do menu, levar para a posição zero do menu
                if (posicao == qtdItensMenu01){
                    posicao = 0;
                }
                mostrar_menu_01(vetor_menu01, posicao);
            
            }else if (menu02 == 1){
                // Se posição passar da quantidade de itens do menu, levar para a posição zero do menu
                if (posicao == qtdItensMenu02){
                    posicao = 0;
                }
                mostrar_menu_02(vetor_menu02, posicao);
            }
        
        }
        else if (!buttonMenosState){
            // O botão foi pressionado
            posicao--;

            // Verificar se o que vai ser mostrado é o menu 1 ou 2
            if (menu01 == 1){
                // Se posição for menor que a quantidade de itens do menu, levar para a posição 32 do menu
                if (posicao == -1){
                    posicao = qtdItensMenu01 - 1;
                }
                mostrar_menu_01(vetor_menu01, posicao);
            
            }else if (menu02 == 1){
                // Se posição for menor que a quantidade de itens do menu, levar para a posição 6 do menu
                if (posicao == -1){
                    posicao = qtdItensMenu02 - 1;
                }
                mostrar_menu_02(vetor_menu02, posicao);
            }
        
        }
        else if (!buttonEnterState){
            // O botão foi pressionado
            if (menu01 == 1){
                // Verificar se apertou enter na posição Sair
                escolhaMenu01 = posicao;
                if (escolhaMenu01 == qtdItensMenu01 - 1){
                    lcdPuts(lcd, "TCHAU...");
                    delay(2000);
                    break;
                }else{
                    // Seleciona a Node com o ID escolhido
                    sendData(fd, select_node, posicao);
                    // Salva a posicao da node selecionada no vetor de ID das Nodes
                    selectedNode = posicao;
                    //serialPutchar(fd, select_node[posicao - 1]);
                    // Desabilita menu 1
                    menu01 = 0;
                    // Habilita menu 2
                    menu02 = 1;
                    // Reseta o contador de posicoes
                    posicao = 0;
                    mostrar_menu_02(vetor_menu02, posicao);
                }

            }else if (menu02 == 1){
                escolhaMenu02 = posicao;
                // Verificar se apertou enter na posição Voltar
                if (escolhaMenu02 == qtdItensMenu02 - 1){
                    // Desseleciona a Node previamente selecionada
                    sendData(fd, deselect_node, selectedNode);
                    //serialPutchar(fd, deselect_node[posicao - 1]);
                    menu01 = 1;
                    menu02 = 0;
                    posicao = escolhaMenu01;
                    mostrar_menu_01(vetor_menu01, posicao);
                }else{
                    // Mandar mensagem para a node e pegar o dado para exibir no LCD
                    lcdPuts(lcd, "ENVIANDO COMANDO...");
                    // Enviando comando a Node selecionada
                    // Logica para implementar a consulta ou monitoramento
                    sendData(fd, followCommands, posicao);
                    delay(500);
                    lcdClear(lcd);
                    lcdPuts(lcd, "COMANDO ENVIADO!");
                    delay(500);
                    recvdData = recvdData(fd, analogBytes, posicao);
                    lcdClear(lcd);
                    if(recvdData) {
                        lcdPrintf("DADO DO SENSOR -> %hu", recvdData);
                        delay(2000);
                    }
                    else {
                        lcdPuts("NODE INALCANCAVEL :/");
                        delay(2000);
                    }
                    //serialPutchar(fd, array_command[menu_counter]);
                    // Mostrar dado que foi recebido
                    // Mostrar Menu 02
                    mostrar_menu_02(vetor_menu02, posicao);
                }   
            }
        }
    }
    return 0;
}
