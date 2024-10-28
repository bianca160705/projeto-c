#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

struct Clientes {
    char nome[50];
    char cpf[12];
    char nomeRua[50]; 
    char numeroCasa[10];
    char complemento[10];
    char telefone[14];
    int ativo;
};

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool segundoPasso(char cpf[]){
    int soma = 0, verificador = 0;
    int aux = 11;

    for(int i=0; i < 10; i++){
        soma+= (cpf[i] - '0') * aux;
        aux--;
    }

    soma= soma % 11;

    if(soma < 2){
        verificador = 0;
    }
    else{
        verificador = 11 - soma;
    }

    if(verificador == (cpf[10] - '0')){
        return true;    //CPF válido
    }
    else{
        return false;   //CPF inválido
    }
}

bool primeiroPasso(char cpf[]){
    int soma = 0;
    int aux = 10;

    for(int i = 0; i<9; i++){
        soma += (cpf[i] - '0') * aux;
        aux--;
    }

    int verificador = 0;
    soma = (soma % 11);

    if(soma < 2){
        verificador = 0;
    }
    else{
        verificador = 11 - soma;
    }

    if(verificador == (cpf[9] - '0')){
        return segundoPasso(cpf); //Primeiro dígito válido. Vai para a análise do segundo
    }
    else{
        return false; //CPF inválido
    }
}

int verificarCPFExistente (struct Clientes *cliente, int quantidadeClientes, const char cpf[]) {
    for (int i = 0; i < quantidadeClientes; i++) {
        if (cliente[i].ativo == 1 && strcmp(cliente[i].cpf, cpf) == 0) {
            return 1; //CPF já existe
        }
    }
    return 0; //CPF não existe
}

int validarNome (const char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int validarNumero (const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
               return 0;
        }
        str++;
    }
    return 1;
}

void cadastrarCliente (struct Clientes *cliente, int quantidadeClientes, FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    for (int i = 0; i < quantidadeClientes; i++) {
        limparBuffer();

        do {
            printf("\t==> Digite o nome do cliente %d: ", i + 1);
            fgets(cliente[i].nome, sizeof(cliente[i].nome), stdin);
            cliente[i].nome[strcspn(cliente[i].nome, "\n")] = '\0';

            if (!validarNome(cliente[i].nome)) {
                printf("\tNome inválido! Por favor, insira apenas letras.\n");
            }
        } while (!validarNome(cliente[i].nome));

        //validação do CPF
        int cpfValido = 0;
        do {
            printf("\t==> Digite o CPF (somente os números) do cliente %d: ", i + 1);
            fgets(cliente[i].cpf, sizeof(cliente[i].cpf), stdin);
            cliente[i].cpf[strcspn(cliente[i].cpf, "\n")] = '\0';

            if(strlen(cliente[i].cpf) != 11){
                printf("\tCPF inválido! O CPF deve ter 11 números.\n");
            } else if (verificarCPFExistente(cliente, quantidadeClientes, cliente[i].cpf)) {
                printf("\tCPF já cadastrado! Por favor, insira um CPF diferente.\n");
            } else if (!primeiroPasso(cliente[i].cpf)) {
                printf("\tCPF inválido! Por favor, insira um CPF válido.\n");
                limparBuffer();
            } else {
                cpfValido = 1; // CPF é válido
            }
        } while (!cpfValido);

        printf("\tCPF válido!\n");

        do {
            printf("\t==> Digite o nome da rua do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[i].nomeRua, sizeof(cliente[i].nomeRua), stdin);
            cliente[i].nomeRua[strcspn(cliente[i].nomeRua, "\n")] = '\0';

            if (!validarNome(cliente[i].nomeRua)) {
                printf("\tNome inválido! Por favor, insira apenas letras.\n");
            }
        } while (!validarNome(cliente[i].nomeRua));

        do {
            printf("\t==> Digite o número da casa do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[i].numeroCasa, sizeof(cliente[i].numeroCasa), stdin);
            cliente[i].numeroCasa[strcspn(cliente[i].numeroCasa, "\n")] = '\0';

            if (!validarNumero(cliente[i].numeroCasa)) {
                printf("\tEntrada inválida! Por favor, insira apenas números.\n");
            }
        } while (!validarNumero(cliente[i].numeroCasa));

        do {
            printf("\t==> Digite o complemento (número da casa/apartamento) do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[i].complemento, sizeof(cliente[i].complemento), stdin);
            cliente[i].complemento[strcspn(cliente[i].complemento, "\n")] = '\0';

            if (!validarNumero(cliente[i].complemento)) {
                printf("\tEntrada inválida! Por favor, insira apenas números.\n");
            }
        } while (!validarNumero(cliente[i].complemento));

        do {
            printf("\t==> Digite seu telefone (somente os números) do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[i].telefone, sizeof(cliente[i].telefone), stdin);
            cliente[i].telefone[strcspn(cliente[i].telefone, "\n")] = '\0';

            if (!validarNumero(cliente[i].telefone)) {
                printf("\tTelefone inválido! Por favor, insira apenas números.\n");
            }
        } while (!validarNumero(cliente[i].telefone));

        cliente[i].ativo = 1;

        printf("\n\tCliente cadastrado com sucesso!\n");

        fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n", cliente[i].nome, 
        cliente[i].cpf, cliente[i].nomeRua, cliente[i].numeroCasa, cliente[i].complemento, cliente[i].telefone, cliente[i].ativo);
    }
}

void lerClientesDoArquivo(struct Clientes *cliente, int *quantidadeClientes, FILE *arquivo) {
    rewind(arquivo);
    *quantidadeClientes = 0;

    while (fscanf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
    cliente[*quantidadeClientes].nome, cliente[*quantidadeClientes].cpf, cliente[*quantidadeClientes].nomeRua, cliente[*quantidadeClientes].numeroCasa, 
    cliente[*quantidadeClientes].complemento, cliente[*quantidadeClientes].telefone, &cliente[*quantidadeClientes].ativo) == 7) {

        if (cliente[*quantidadeClientes].ativo == 1) {
            (*quantidadeClientes)++;
        }

    }
}

void quickSortAlfabetica (struct Clientes *cliente, int inicio, int fim) {
        if (inicio < fim) {
            int pivo = inicio;
            int i = inicio;
            int j = fim;
            struct Clientes temp;

            while (i < j) {
                while (strcmp(cliente[i].nome, cliente[pivo].nome) <= 0 && i < fim) {
                    i++;
                }

                while (strcmp(cliente[j].nome, cliente[pivo].nome) > 0) {
                    j--;
                }

                if (i < j) {
                    temp = cliente[i];
                    cliente[i] = cliente[j];
                    cliente[j] = temp;
                }
            }

            temp = cliente[inicio];
            cliente[inicio] = cliente[j];
            cliente[j] = temp;

            quickSortAlfabetica(cliente, inicio, j - 1);
            quickSortAlfabetica(cliente, j + 1, fim);
    }
}

void quickSortNumerica(struct Clientes *cliente, int inicio, int fim) {
    if (inicio < fim) {
        int i = inicio;
        int j = fim;
        struct Clientes temp;

        int pivor = (inicio + fim) / 2;
        struct Clientes pivotElement = cliente[pivor];

        while (i <= j) {
            while (cliente[i].ativo == 1 && atoi(cliente[i].numeroCasa) < atoi(pivotElement.numeroCasa)) {
                i++;
            }

            while (cliente[j].ativo == 1 && atoi(cliente[j].numeroCasa) > atoi(pivotElement.numeroCasa)) {
                j--;
            }

            if (i <= j) {
                temp = cliente[i];
                cliente[i] = cliente[j];
                cliente[j] = temp;
                i++;
                j--;
            }
        }

        if (inicio < j) {
            quickSortNumerica(cliente, inicio, j);
        }

        if (i < fim) {
            quickSortNumerica(cliente, i, fim);
        }
    }
}

void desativarCliente(struct Clientes *cliente, int quantidadeClientes, int idDesativar, FILE *arquivo) {
    int encontrado = 0;

    for (int i = 0; i < quantidadeClientes; i++) {
        if (idDesativar == i) {
            cliente[i].ativo = 0;
            encontrado = 1;
            printf("\tCliente desativado com sucesso.\n");

            //reabre o arquivo em modo de escrita, truncando-o
            fclose(arquivo);
            arquivo = fopen("pizzaria.txt", "w");
            if (arquivo == NULL) {
                printf("\tErro ao abrir o arquivo para escrita.\n");
                return;
            }

            //reescreve todos os clientes (ativos) no arquivo
            for (int j = 0; j < quantidadeClientes; j++) {
                fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                    cliente[j].nome, cliente[j].cpf, cliente[j].nomeRua, cliente[j].numeroCasa, cliente[j].complemento, cliente[j].telefone,
                    cliente[j].ativo);
            }

            break;
        }
    }

    if (!encontrado) {
        printf("\tEste cliente não existe.\n");
    }
}

void listarCliente (struct Clientes *cliente, int quantidadeClientes, int opcaoOrdenacao) {
    if (quantidadeClientes == 0) {
        printf("\n\tNenhum cliente cadastrado.\n");
        return;
    }
    
    if (opcaoOrdenacao == 1) {
        quickSortAlfabetica(cliente, 0, quantidadeClientes - 1);
    } else if (opcaoOrdenacao == 2) {
        quickSortNumerica(cliente, 0, quantidadeClientes - 1);
    } else {
        printf("\n\tOpção inválida.\n");
        return;
    }

    printf("\n\tLista de clientes:\n");
    printf("\t************************************************\n");

    for (int i = 0; i < quantidadeClientes; i++) {
        if (cliente[i].ativo == 1) {
            printf("\tCliente %d\n", i + 1);
            printf("\tNome: %s\n", cliente[i].nome);
            printf("\tCPF: %s\n", cliente[i].cpf);
            printf("\tNome da rua: %s\n", cliente[i].nomeRua);
            printf("\tNúmero da rua: %s\n", cliente[i].numeroCasa);
            printf("\tComplemento: %s\n", cliente[i].complemento);
            printf("\tTelefone: %s\n", cliente[i].telefone);
            printf("\t************************************************\n");
        }
    }
}

int compareClientes (const void *a, const void *b) {
    return strcmp(((struct Clientes*)a)->cpf, ((struct Clientes*)b)->cpf);
}

void consultarCliente (struct Clientes *cliente, int quantidadeClientes, char cpfConsultar[]) {
    qsort(cliente, quantidadeClientes, sizeof(struct Clientes), compareClientes);

    int encontrado = 0;
    int inicio = 0, fim = quantidadeClientes - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int comparar = strcmp(cpfConsultar, cliente[meio].cpf);
    
        if (comparar == 0) {
            printf("\n\t************************************************\n");
            printf("\tNome: %s\n", cliente[meio].nome);
            printf("\tCPF: %s\n", cliente[meio].cpf);
            printf("\tEndereço: %s\n", cliente[meio].nomeRua);
            printf("\tNúmero da rua: %s\n", cliente[meio].numeroCasa);
            printf("\tComplemento: %s\n", cliente[meio].complemento);
            printf("\tTelefone: %s\n", cliente[meio].telefone);

            encontrado = 1;
            break;
        } else if (comparar < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
        
    if (!encontrado) {
        printf("\tCPF não cadastrado.\n");
    }
}

void excluirCliente (struct Clientes *cliente, int *quantidadeClientes, FILE *arquivo) {
    int idExcluir;

    printf("\t==> Informe o número do cliente que deseja excluir: ");
    scanf("%d", &idExcluir);
    idExcluir--;

    if (idExcluir >= 0 && idExcluir < *quantidadeClientes) {
        cliente[idExcluir].ativo = 0; // Marcar como inativo
        printf("\tCliente excluído com sucesso.\n");

        // Atualizar o arquivo com a nova situação dos clientes
        fclose(arquivo);
        arquivo = fopen("pizzaria.txt", "w");
        if (arquivo == NULL) {
            printf("\tErro ao abrir o arquivo para escrita.\n");
            return;
        }

        for (int i = 0; i < *quantidadeClientes; i++) {
            if (cliente[i].ativo == 1) {
                fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                        cliente[i].nome, cliente[i].cpf, cliente[i].nomeRua, cliente[i].numeroCasa, cliente[i].complemento, cliente[i].telefone, cliente[i].ativo);
            }
        }
    } else {
        printf("\tNúmero inválido.\n");
    }
}

void realizarPedido (int *pedido, FILE *arquivo) {
    char resposta;

    do {
        printf("\n");
        printf("\t************************************************\n");
        printf("\t*                     Menu                     *\n");
        printf("\t************************************************\n");
        printf("\t*    1 - Calabresa (8 pedaços)                *\n");
        printf("\t*    2 - Calabresa (12 pedaços)               *\n");
        printf("\t*    3 - Frango com catupiry (8 pedaços)      *\n");
        printf("\t*    4 - Frango com catupiry (12 pedaços)     *\n");
        printf("\t*    5 - 4 queijos (8 pedaços)                *\n");
        printf("\t*    6 - 4 queijos (12 pedaços)               *\n");
        printf("\t************************************************\n");

        printf("\n\t==> Escolha uma opção: ");
        scanf("%d", pedido);

        fseek(arquivo, 0, SEEK_END);

        fprintf(arquivo, "Pedido: %d\n", *pedido);

        printf("\t==> Deseja continuar pedindo (S/N)?: ");
        scanf(" %c", &resposta);

    } while (resposta != 'N' && resposta != 'n');

    printf("\tObrigado por pedir conosco.\n");
}

int main() {

    FILE *arquivo;
    int opcao, quantidadeClientes = 0, pedido, idDesativar, opcaoOrdenacao;
    char cpfConsultar[12];
    
    arquivo = fopen("pizzaria.txt", "a+");
    if (arquivo == NULL) {
        printf("\tProblema ao abrir o arquivo");
        return 1;
    }

    struct Clientes *cliente;
    cliente = malloc(100 * sizeof(struct Clientes));
    if (cliente == NULL){
        printf("\nErro na alocação de memória.");
        return 1;
    }

    lerClientesDoArquivo(cliente, &quantidadeClientes, arquivo);

    do {
        printf("\n");
        printf("\t************************************************\n");
        printf("\t*         Bem-vindo a nossa pizzaria           *\n");
        printf("\t************************************************\n");
        printf("\t*    1 - Cadastrar um cliente                  *\n");
        printf("\t*    2 - Listar cliente                        *\n");
        printf("\t*    3 - Consultar um cliente a partir do CPF  *\n");
        printf("\t*    4 - Desativar um cliente                  *\n");
        printf("\t*    5 - Excluir um cliente                    *\n");
        printf("\t*    6 - Realizar um pedido                    *\n");
        printf("\t*    7 - Sair                                  *\n");
        printf("\t************************************************\n");
        printf("\n\t==> Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\t==> Quantos clientes você deseja cadastrar? ");
                scanf("%d", &quantidadeClientes);
                
                cadastrarCliente(cliente, quantidadeClientes, arquivo);
                break;
            case 2:
                printf("\t==> Deseja listar os clientes em ordem alfabética (1) ou númerica (2)? ");
                scanf("%d", &opcaoOrdenacao);
                
                listarCliente(cliente, quantidadeClientes, opcaoOrdenacao);
                break;
            case 3:
                printf("\t==> Informe o CPF do cliente que você deseja consultar: ");
                scanf("%s", cpfConsultar);

                consultarCliente(cliente, quantidadeClientes, cpfConsultar);
                break;
            case 4:
                printf("\t==> Informe o número do cliente que deseja desativar: ");
                scanf("%d", &idDesativar);
                idDesativar--;

                desativarCliente(cliente, quantidadeClientes, idDesativar, arquivo);
                break;
            case 5:
                excluirCliente(cliente, &quantidadeClientes, arquivo);
                break;
            case 6:
                realizarPedido(&pedido, arquivo);
                break;
            case 7:
                printf("\tSaindo do programa.\n");
                break;
            default:
                printf("\tOpção inválida.\n");
                break;
        }
    } while (opcao != 7);

    free(cliente);

    if (fclose(arquivo) == 0) {
        printf("\n\tArquivo fechado corretamente.");
        return 0;
    } else {
        printf("\n\tProblema ao fechar o arquivo.");
        return 1;
    }

    return 0;
}