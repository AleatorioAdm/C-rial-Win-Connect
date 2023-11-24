/*******************************************************
 * Nome do Programa: C-rial Win Connect
 * Descrição: Sistema de Pareamento para Comunicação entre
 *			  Aplicativos programados em C para Windows e
 *			  Dispositivos de Eletrônica Embarcada que
 *			  possuem Comunicação Serial.
 * Data de Criação: 09/11/2023
 * Data da Versão: 17/11/2023
 * Data de Publicação: 23/11/2023
 * Autor: AleatorioAdm (Ademilson Filho)
 * Emails: aleatorioadm.dev@gmail.com /ademilsonfilho.dev@gmail.com
 * Versão: 0.6.1
 *******************************************************/
#include <windows.h>
#include <stdio.h>
#include<string.h>

void led_test_protocol(int j);
void Serial_send(HANDLE *hSerial, char *message);

int main() {
	while(1){
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    char dataExpected[] = "Identity:VIRTUAL_HEART_CONTROLLER_DEVICE";
    char dataAnswered[] = "VHC_DEVICE_NOT_DETECTED_________________";
    char dataAnswered2[] = "VHC_DEVICE_DETECTED";
    char portName[12];
    int *portList;
    portList = (int *)malloc(256 * sizeof(int));
    int portListSize = 0;
    int i = 1;
		
    for (i = 1; i <= 256; ++i) { // Iterar de COM1 a COM256 descobrir portas
        
        sprintf(portName, "\\\\.\\COM%d", i);
        
        //printf("Abrindo porta %s:\n", portName);

        hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        if (hSerial == INVALID_HANDLE_VALUE) {
        	//fprintf(stderr, "Erro ao abrir a porta serial.\n");
        continue;
	    }
	
	    // Configurar parâmetros da porta serial
	    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	    if (!GetCommState(hSerial, &dcbSerialParams)) {
	        fprintf(stderr, "Erro ao obter o estado da porta serial.\n");
	        CloseHandle(hSerial);
	        continue;
	    }
	
	    dcbSerialParams.BaudRate = CBR_9600;
	    dcbSerialParams.ByteSize = 8;
	    dcbSerialParams.StopBits = ONESTOPBIT;
	    dcbSerialParams.Parity = NOPARITY;
	    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
		dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

	    if (!SetCommState(hSerial, &dcbSerialParams)) {
	        fprintf(stderr, "Erro ao configurar o estado da porta serial.\n");
	        CloseHandle(hSerial);
	        continue;
	    }
	    
	    //Serial_send(hSerial, dataAnswered);
	    
	    DWORD bytesWritten2;
		if (!WriteFile(hSerial, dataAnswered, sizeof(dataAnswered), &bytesWritten2, NULL)) {
		    fprintf(stderr, "Erro ao escrever na porta serial.\n");
		} else {
		    //printf("Enviado com sucesso: %s\n", dataAnswered);
		}
		
		Sleep(2000);
        
	    char buffer[40];
	    DWORD bytesRead;
	    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
	        fprintf(stderr, "Erro ao ler da porta serial.\n");
	    } else {
	        buffer[bytesRead] = '\0';  // Adiciona o terminador nulo para criar uma string C
	        //printf("Recebido da porta %s : %s\n", portName, buffer);
	    }
	    
	    Sleep(2000);
	    
	    
	    if (strstr(buffer, dataExpected) != NULL){
	    	DWORD bytesWritten;
		    if (!WriteFile(hSerial, dataAnswered2, sizeof(dataAnswered2), &bytesWritten, NULL)) {
		        fprintf(stderr, "Erro ao escrever na porta serial.\n");
		    } else {
		        //printf("Enviado com sucesso: %s\n", dataAnswered2);
		        
		        	printf("Porta COM%d encontrada!!\n", i);
					portList[portListSize] = i;
					portListSize++;
				
		        
						    
			}
		}
		
		CloseHandle(hSerial);
	    
    }
    
    portListSize;
    portList = (int *)realloc(portList, portListSize * sizeof(int));
    int chosenPort, chosenOp;
    sleep(3);
    while(1){
    	system("cls");
    	printf("Qual Porta voce quer controlar? (numero de referencia)");
    	if (portListSize == 0){
    		printf("\nNENHUMA PORTA ENCONTRADA\n");
		}
    	printf("\n0. Escanear portas novamente");
    	for (i = 0; i < portListSize; i++) {
        	printf("\n%d.COM%d ", i+1, portList[i]);
    	}
    	fflush(stdin);
    	printf("\n");
    	scanf("%i", &chosenPort);
    	
    	if(chosenPort == 0){
    		system("cls");
    		printf("ESCANEANDO PORTAS NOVAMENTE");
    		Sleep(500);
    		printf(".");
    		Sleep(500);
    		printf(".");
    		Sleep(500);
    		printf(".\n");
    		break;
		}
    
    	while(1){
    		if (chosenPort>portListSize || chosenPort<1){
    			printf("Esta opcao nao esta disponivel!!\nEscolha uma das opcoes da lista!!\n\n");
    			sleep(3);
    			break;
			}
			system("cls");
    		printf("PORTA COM%d\n",portList[chosenPort-1]);
    		printf("Qual comando executar?\n");
    		printf("0. Voltar a selecao de portas.\n");
    		printf("1. Protocolo de teste do LED\n");
    		fflush(stdin);
    		scanf("%i", &chosenOp);
    		if (chosenOp == 1){
    			led_test_protocol(portList[chosenPort-1]);
			} else if (chosenOp == 0){
				break;
			} else {
				printf("Comando inexistente!!\n");
			}
		}
    }
	}
    return 0;
}

void Serial_send(HANDLE *hSerial, char *message){
	
    int k = 0;
	for (k=0;k<4;k++){
			Sleep(200);
			DWORD bytesWritten2;
			if (!WriteFile(hSerial, message, sizeof(message), &bytesWritten2, NULL)) {
		    	fprintf(stderr, "Erro ao escrever na porta serial.\n");
			} else {
		    	printf("Enviado com sucesso: %s\n", message);
			}
        	
		}
}

void led_test_protocol(int j){
	char portName[12];
	HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    char dataSent[] = "LED_TEST_PROTOCOL";
    system("cls");
    printf("ENVIANDO COMANDO LED_TEST_PROTOCOL!\n");
	sprintf(portName, "\\\\.\\COM%d", j);
	
		
        //printf("Abrindo porta %s:\n", portName);
        hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        
        if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erro ao abrir a porta serial.\n");
        return;
	    }
	    
	    
	    // Configurar parâmetros da porta serial
	    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	    if (!GetCommState(hSerial, &dcbSerialParams)) {
	        fprintf(stderr, "Erro ao obter o estado da porta serial.\n");
	        CloseHandle(hSerial);
	        return;
	    }
		
	    dcbSerialParams.BaudRate = CBR_9600;
	    dcbSerialParams.ByteSize = 8;
	    dcbSerialParams.StopBits = ONESTOPBIT;
	    dcbSerialParams.Parity = NOPARITY;
	    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
		dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
	    if (!SetCommState(hSerial, &dcbSerialParams)) {
	        fprintf(stderr, "Erro ao configurar o estado da porta serial.\n");
	        CloseHandle(hSerial);
	        return;
	    }
		
		 
	    DWORD bytesWritten;
		if (!WriteFile(hSerial, dataSent, sizeof(dataSent), &bytesWritten, NULL)) {
		    fprintf(stderr, "Erro ao escrever na porta serial.\n");
		} else {
		    printf("Enviado com sucesso!\n");		    
		}
	    
	    char buffer1[29];
	    DWORD bytesRead1;
	    
	    Sleep(100);
	    
		
	    if (!ReadFile(hSerial, buffer1, sizeof(buffer1), &bytesRead1, NULL)) {
	        	
			fprintf(stderr, "Erro ao ler da porta serial.\n");
	    	
		} else {
	        buffer1[bytesRead1] = '\0';  // Adiciona o terminador nulo para criar uma string C
	        
	        printf("PORTA COM%d: %s\n", j, buffer1);
	    }
		
		char buffer2[26];
	    DWORD bytesRead2;
	    
	    Sleep(100);
	    
		
	    if (!ReadFile(hSerial, buffer2, sizeof(buffer2), &bytesRead2, NULL)) {
	        	
			fprintf(stderr, "Erro ao ler da porta serial.\n");
	    	
		} else {
	        buffer2[bytesRead2] = '\0';  // Adiciona o terminador nulo para criar uma string C
	        
	        printf("PORTA COM%d: %s\n", j, buffer2);
	    }
		
		CloseHandle(hSerial);
		
		sleep(5);
}
