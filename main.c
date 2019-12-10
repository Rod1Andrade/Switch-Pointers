/**
 * @author: Rodrigo Moreira Pires de Andrade
 * 
 * Trabalho final da disciplina de Lógica de Programação II
 * O objetivo desse programa é fazer o controle de pontos
 * de switchs em diferentes prédios.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 255
#define FILENAME "registers/SwitchInfos.dat"

/**
 * Estrutura de Registro de Switch por prédio.
 */
typedef struct
{
    char    PointNumber[STR_LEN];
    char    BuildingName[STR_LEN]; 
    char    Floor[STR_LEN]; 
    char    Room[STR_LEN];
    char    SwitchName[STR_LEN];
    int     PortNumber;
} SwitchRegisters;

/**
 * Função para inserir novos dados no arquivo
 * @throw pode dar algum erro na abertura do aquivo
*/
void NewSwitch()
{
    FILE *file = fopen(FILENAME, "ab+");
    SwitchRegisters switchRegisters;

    if(file == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o arquivo");
        exit(1);
    }

    printf("+ ---------------------------- +\n");
    printf("+    INSERIR NOVO SWITCH       +\n");
    printf("+ ---------------------------- +\n");
    printf("\n"); // Quebra de linha

    setbuf(stdin, NULL);

    printf("Número do ponto: ");
    fgets(switchRegisters.PointNumber, STR_LEN, stdin);
    switchRegisters.PointNumber[strlen(switchRegisters.PointNumber) - 1] = '\0';

    setbuf(stdin, NULL);

    printf("Nome do Prédio: ");
    fgets(switchRegisters.BuildingName, STR_LEN, stdin);
    switchRegisters.BuildingName[strlen(switchRegisters.BuildingName) - 1] = '\0';

    setbuf(stdin, NULL);

    printf("Andar: ");
    fgets(switchRegisters.Floor, STR_LEN, stdin);
    switchRegisters.Floor[strlen(switchRegisters.Floor) - 1] = '\0';

    setbuf(stdin, NULL);

    printf("Sala: ");
    fgets(switchRegisters.Room, STR_LEN, stdin);
    switchRegisters.Room[strlen(switchRegisters.Room) - 1] = '\0';

    setbuf(stdin, NULL);

    printf("Nome do Switch: ");
    fgets(switchRegisters.SwitchName, STR_LEN, stdin);
    switchRegisters.SwitchName[strlen(switchRegisters.SwitchName) - 1] = '\0';

    setbuf(stdin, NULL);

    setbuf(stdin, NULL);
    printf("Número da porta: ");
    scanf("%d", &switchRegisters.PortNumber);

    setbuf(stdin, NULL);

    int qtdRegisters = fwrite(&switchRegisters, sizeof(switchRegisters), 1, file);

    if(qtdRegisters > 0)
        printf("Cadastrado com sucesso\n");

    fclose(file);
}

/**
 * Ordenação do Vetor para que os prédios de mesmo nome fiquem próximos.
 * 
 * Esse algoritimo verifica se o elemento anterior  é diferente do próximo elemento.
 * Caso seja verdade, ele o troca de lugar com o próximo elemento.
 * 
 * É uma troca em forma de bolha, bubble sort.
 * 
 * @param switchRegisters   = O vetor a ser ordenado, vetor de registros
 * @param zie               = O tamanho do vetor
 * 
*/
void OrderByName(SwitchRegisters *switchRegisters, long size)
{
   
    for(int i = 0; i < size; i++)
    {
        int j = i;
        while(j > 0 && strcmp(switchRegisters[j - 1].BuildingName, switchRegisters[j].BuildingName) != 0)
        {
            SwitchRegisters aux = switchRegisters[j - 1];
            switchRegisters[j - 1] = switchRegisters[j];
            switchRegisters[j] = aux;
            j--;
        }
    }

}

/**
 * Função para retonar a quantidade de registros 
 * do arquivo binário;
 * 
 * @throw pode dar algum erro na abertura do aquivo
 * @return LONG 
*/
long size()
{
    FILE *file = fopen(FILENAME, "ab+");
    if(file == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o arquivo");
        exit(1);
    }

    fseek(file, sizeof(SwitchRegisters), SEEK_END);
    return ftell(file) / sizeof(SwitchRegisters);

}

/**
 * Resumo geral de pontos cadastrados
 * 
 * @throw pode dar algum erro na abertura do aquivo
*/
void ReportByBuilding()
{
    
    FILE *file = fopen(FILENAME, "rb");
    rewind(file);
    long Size_T = size();
    SwitchRegisters switchRegisters[Size_T];

    if(file == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o arquivo para leitura");
        exit(1);
    }

    int qtdRead = fread(&switchRegisters, sizeof(SwitchRegisters), Size_T, file);

    if(qtdRead > 0)
    {
        int ControllerNameBuilding = 0;
        int i = ControllerNameBuilding;

        OrderByName(switchRegisters, Size_T - 1);

        while(i < Size_T - 1)
        {

            printf("+ ------------------------------------- +\n");
            printf("        NOME DO PREDIO: %s              \n", switchRegisters[ControllerNameBuilding].BuildingName);
            printf("+ ------------------------------------- +\n");
            printf("| ANDAR | SALA | SWITCH | PORTA | PONTO |\n");
            printf("+ ------------------------------------- +\n");
            
            for(i = ControllerNameBuilding; i < Size_T - 1; i++)
            {
                
                if(strcmp(switchRegisters[i].BuildingName, switchRegisters[ControllerNameBuilding].BuildingName) != 0)
                    break;

                printf(" %s", switchRegisters[i].Floor);
                printf("\t   %s", switchRegisters[i].Room);
                printf("\t %s", switchRegisters[i].SwitchName);
                printf("\t    %d", switchRegisters[i].PortNumber);
                printf("\t  %s", switchRegisters[i].PointNumber);

                printf("\n");

                
            }

            ControllerNameBuilding = i;

            printf("\n");
        }
    }

    fclose(file);
}

void activedPointsByName()
{

    FILE *file = fopen(FILENAME, "rb");

    rewind(file);

    long Size_T = size();
    SwitchRegisters switchRegisters[Size_T];
    char BuildName[STR_LEN];

    if(file == NULL)
    {
        fprintf(stderr, "Impossível abrir o arquivo para leitura");
        exit(1);
    }

    setbuf(stdin, NULL);

    // Recupera o nome do prédio do usúario
    printf("Nome do Prédio: ");
    fgets(BuildName, STR_LEN, stdin);
    BuildName[strlen(BuildName) - 1] = '\0';

    // Lê o arquivo:
    fread(switchRegisters, sizeof(SwitchRegisters), Size_T - 1, file);

    // Ordena o vetor para acelerar o processo
    OrderByName(switchRegisters, Size_T - 1);

    printf("+ -------------------------------------- +\n");
    printf("| NUMERO | ANDAR | SALA | SWITCH | PORTA |\n");
    printf("+ -------------------------------------- +\n");

    printf("\n");

    int i = 0;
    while(i < Size_T - 1)
    {
        if(strcmp(BuildName, switchRegisters[i].BuildingName) == 0 && switchRegisters[i].PointNumber != 0)
        {       
                printf(" %s", switchRegisters[i].PointNumber);
                printf("\t   %s", switchRegisters[i].Floor);
                printf("\t  %s", switchRegisters[i].Room);
                printf("\t %s", switchRegisters[i].SwitchName);
                printf("\t    %d", switchRegisters[i].PortNumber);

            printf("\n"); // Qubra de linha
        }
        i++;
    }

    fclose(file);
}

void ActivesAndUnctivePoints()
{
    
    FILE *file = fopen(FILENAME, "rb");
    rewind(file);
    long Size_T = size();
    SwitchRegisters switchRegisters[Size_T];

    if(file == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o arquivo para leitura");
        exit(1);
    }

    int qtdRead = fread(switchRegisters, sizeof(SwitchRegisters), Size_T - 1, file);

    OrderByName(switchRegisters, Size_T - 1);

    int ControllerNameBuilding = 0, i = 0;
    int CountActive, CountUnactive;
    if(qtdRead > 0)
    {
        printf("+ ------------------------------------+\n");
        printf("+       Relação Geral de Pontos       +\n");
        printf("+ ----------------------------------- +\n");
        printf(" | PREDIOS   |  ATIVOS  |   INATIVOS  | \n");
        printf("+ ----------------------------------- +\n");
        
        while(i < Size_T -1)
        {
            printf("%s", switchRegisters[ControllerNameBuilding].BuildingName);


            CountActive = CountUnactive = 0;
            while(strcmp(switchRegisters[i].BuildingName, switchRegisters[ControllerNameBuilding].BuildingName) == 0)
            {
                if(switchRegisters[i].PortNumber != 0)
                    CountActive++;
                else
                    CountUnactive++;

                i++; // Incrementa o index
            }
            printf("\t\t%d", CountActive);
            printf("\t\t%d", CountUnactive);
            printf("\n"); // Quebra de linha

            ControllerNameBuilding = i;
        }
        
    }
}

/**
 * Menu de interação com o programa.
*/
void menu()
{

    printf("+ ----------------------------------------------------------- +\n");
    printf("+                    CONTROLE DE PONTOS                       +\n");
    printf("+ ----------------------------------------------------------- +\n");

    printf("\n"); // Quebra de linha

    printf("Bem vindo ao contole de pontos, para iniciar\n");
    printf("o uso deve-se antes cadastrar pelo menos um\n");
    printf("ponto.\n");

    printf("\n"); // Quebra de linha

    printf("Acesse os relatórios para obter as informações triviais.\n");

    printf("\n"); // Quebra de linha

    printf("Para sair basta digitar (-1) e pressionar [ENTER].\n");

    printf("\n"); // Quebra de linha

    printf("+ ----------------------------------------------------------- +\n");
    printf("+   1. Cadastrar pontos                                       +\n");
    printf("+   2. Resumo de pontos                                       +\n");
    printf("+   3. Relação de pontos ativos em um prédio                  +\n");
    printf("+   4. Relação geral de pontos                                +\n");
    printf("+ ----------------------------------------------------------- +\n");

    printf("\n"); // Quebra de linha
}

int main(int argc, char argv)
{


    int Continue = 1;
    int Option;

    while(Continue)
    {
        system("clear");
        menu();
        printf("Opção: ");
        scanf("%d", &Option);

        switch(Option)
        {
            case 1:
                NewSwitch();
            break;

            case 2:
                system("clear");
                ReportByBuilding();  
                setbuf(stdin, NULL);

                printf("Pressione [Enter] para continuar...\n");
                getchar();              
            break;

            case 3:
                activedPointsByName();
                setbuf(stdin, NULL);

                printf("Pressione [Enter] para continuar...\n");
                getchar();  
            break;

            case 4:
                ActivesAndUnctivePoints();
                setbuf(stdin, NULL);

                printf("Pressione [Enter] para continuar...\n");
                getchar();  
            break;

            case -1:
                Continue = 0;
            break;
            
        }
    }

    return 0;
}