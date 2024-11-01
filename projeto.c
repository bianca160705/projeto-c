#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

struct Clientes
{
    char nome[50];
    char cpf[12];
    char nomeRua[50];
    char numeroCasa[10];
    char complemento[10];
    char telefone[14];
    int ativo;
};

bool segundoPasso(char cpf[])
{
    int soma = 0, verificador = 0, resto = 0;
    int aux = 11;

    for (int i = 0; i < 10; i++)
    {
        soma += (cpf[i] - '0') * aux;
        aux--;
    }

    resto = soma % 11;

    if (resto < 2)
    {
        verificador = 0;
    }
    else
    {
        verificador = 11 - resto;
    }

    if (verificador == (cpf[10] - '0'))
    {
        printf("\tCPF valido!\n");
        return true;
    }
    else
    {
        printf("\tCPF nao e valido\n");
        return false;
    }
}

bool primeiroPasso(char cpf[])
{
    int soma = 0;
    int aux = 10;

    for (int i = 0; i < 9; i++)
    {
        soma += (cpf[i] - '0') * aux;
        aux--;
    }

    int verificador = 0, resto = 0;
    resto = (soma % 11);

    if (resto < 2)
    {
        verificador = 0;
    }
    else
    {
        verificador = 11 - resto;
    }

    if (verificador == (cpf[9] - '0'))
    {
        return segundoPasso(cpf);
    }
    else
    {
        printf("\tCPF nao e valido! Por favor, digite novamente.\n");
        return false;
    }
}

bool verificaCPF(char cpf[])
{
    bool todosIguais = true;
    int soNumero = 1;

    if (strlen(cpf) != 11)
    {
        printf("\tCPF precisa ter 11 digitos! Por favor, digite novamente.\n");
        return false;
    }
    else
    {
        for (int i = 0; i < 11; i++)
        {
            if (cpf[i] != cpf[0])
            {
                todosIguais = false;
                break;
            }
        }

        for (int i = 0; cpf[i] != '\0'; i++)
        {
            if (!isdigit(cpf[i]))
            {
                soNumero = 0;
                break;
            }
        }

        if (todosIguais)
        {
            printf("\tCPF invalido! Todos os digitos sao iguais. Por favor, digite novamente.\n");
            return false;
        }
        else if (!soNumero)
        {
            printf("\tCPF invalido! Contem caracteres nao numericos. Por favor, digite novamente.\n");
            return false;
        }
        else
        {
            return primeiroPasso(cpf);
        }
    }
}

bool cpfJaCadastrado(struct Clientes *cliente, int quantidadeClientes, const char cpf[])
{
    for (int i = 0; i < quantidadeClientes; i++)
    {
        if (strcmp(cliente[i].cpf, cpf) == 0)
        {
            return true; // CPF já existe
        }
    }
    return false; // CPF não existe
}

int validarNome(const char *nome)
{
    for (int i = 0; nome[i] != '\0'; i++)
    {
        if (!isalpha(nome[i]) && nome[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

int validarNumero(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

void cadastrarCliente(struct Clientes *cliente, int *quantidadeClientes, FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END);

    int quantidade;
    printf("\t==> Quantos clientes deseja cadastrar? ");
    scanf("%d", &quantidade);
    getchar(); // tirar \n

    for (int i = 0; i < quantidade; i++)
    {
        char cpf[12];
        while (true)
        {
            printf("\t==> Digite o CPF do cliente: ");
            scanf("%11s", cpf);

            if (cpfJaCadastrado(cliente, *quantidadeClientes, cpf))
            {
                printf("\tCPF já cadastrado. Tente novamente.\n");
            }
            else
            {
                if (verificaCPF(cpf))
                {
                    strcpy(cliente[*quantidadeClientes].cpf, cpf);
                    cliente[*quantidadeClientes].ativo = 1;
                    (*quantidadeClientes)++; // Incrementa a quantidade de clientes
                    break;
                }
            }
        }

        do
        {
            printf("\t==> Digite o nome do cliente: ");
            scanf(" %[^\n]", cliente[*quantidadeClientes - 1].nome);

            if (!validarNome(cliente[*quantidadeClientes - 1].nome))
            {
                printf("\tNome inválido! Por favor, insira apenas letras.\n");
            }
        } while (!validarNome(cliente[*quantidadeClientes - 1].nome));

        // Coleta informações do cliente
        do
        {
            printf("\t==> Digite o nome da rua do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[*quantidadeClientes - 1].nomeRua, sizeof(cliente[*quantidadeClientes - 1].nomeRua), stdin);
            cliente[*quantidadeClientes - 1].nomeRua[strcspn(cliente[*quantidadeClientes - 1].nomeRua, "\n")] = '\0';

            if (!validarNome(cliente[*quantidadeClientes - 1].nomeRua))
            {
                printf("\tNome inválido! Por favor, insira apenas letras.\n");
            }
        } while (!validarNome(cliente[*quantidadeClientes - 1].nomeRua));

        do
        {
            printf("\t==> Digite o número da casa do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[*quantidadeClientes - 1].numeroCasa, sizeof(cliente[*quantidadeClientes - 1].numeroCasa), stdin);
            cliente[*quantidadeClientes - 1].numeroCasa[strcspn(cliente[*quantidadeClientes - 1].numeroCasa, "\n")] = '\0';
        } while (!validarNumero(cliente[*quantidadeClientes - 1].numeroCasa));

        do
        {
            printf("\t==> Digite o complemento (número da casa/apartamento) do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[*quantidadeClientes - 1].complemento, sizeof(cliente[*quantidadeClientes - 1].complemento), stdin);
            cliente[*quantidadeClientes - 1].complemento[strcspn(cliente[*quantidadeClientes - 1].complemento, "\n")] = '\0';
        } while (!validarNumero(cliente[*quantidadeClientes - 1].complemento));

        do
        {
            printf("\t==> Digite seu telefone (somente os números) do cliente %d: ", i + 1);
            fflush(stdin);
            fgets(cliente[*quantidadeClientes - 1].telefone, sizeof(cliente[*quantidadeClientes - 1].telefone), stdin);
            cliente[*quantidadeClientes - 1].telefone[strcspn(cliente[*quantidadeClientes - 1].telefone, "\n")] = '\0';
        } while (!validarNumero(cliente[*quantidadeClientes - 1].telefone));

        fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                cliente[*quantidadeClientes - 1].nome, cliente[*quantidadeClientes - 1].cpf,
                cliente[*quantidadeClientes - 1].nomeRua, cliente[*quantidadeClientes - 1].numeroCasa,
                cliente[*quantidadeClientes - 1].complemento, cliente[*quantidadeClientes - 1].telefone,
                cliente[*quantidadeClientes - 1].ativo);

        printf("\n\tCliente cadastrado com sucesso!\n");
    }
}

void lerClientesDoArquivo(struct Clientes *cliente, int *quantidadeClientes, FILE *arquivo)
{
    rewind(arquivo);
    *quantidadeClientes = 0;

    while (fscanf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                  cliente[*quantidadeClientes].nome, cliente[*quantidadeClientes].cpf, cliente[*quantidadeClientes].nomeRua, cliente[*quantidadeClientes].numeroCasa,
                  cliente[*quantidadeClientes].complemento, cliente[*quantidadeClientes].telefone, &cliente[*quantidadeClientes].ativo) == 7)
    {

        if (cliente[*quantidadeClientes].ativo == 1 || cliente[*quantidadeClientes].ativo == 0)
        {
            (*quantidadeClientes)++;
        }
    }
}

void quickSortAlfabetica(struct Clientes *cliente, int inicio, int fim)
{
    if (inicio < fim)
    {
        int pivo = inicio;
        int i = inicio;
        int j = fim;
        struct Clientes temp;

        while (i < j)
        {
            while (strcmp(cliente[i].nome, cliente[pivo].nome) <= 0 && i < fim)
            {
                i++;
            }

            while (strcmp(cliente[j].nome, cliente[pivo].nome) > 0)
            {
                j--;
            }

            if (i < j)
            {
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

void quickSortNumerica(struct Clientes *cliente, int inicio, int fim)
{
    if (inicio < fim)
    {
        int i = inicio;
        int j = fim;
        struct Clientes temp;

        int pivor = (inicio + fim) / 2;
        struct Clientes pivotElement = cliente[pivor];

        while (i <= j)
        {
            while (cliente[i].ativo == 1 && atoi(cliente[i].numeroCasa) < atoi(pivotElement.numeroCasa))
            {
                i++;
            }

            while (cliente[j].ativo == 1 && atoi(cliente[j].numeroCasa) > atoi(pivotElement.numeroCasa))
            {
                j--;
            }

            if (i <= j)
            {
                temp = cliente[i];
                cliente[i] = cliente[j];
                cliente[j] = temp;
                i++;
                j--;
            }
        }

        if (inicio < j)
        {
            quickSortNumerica(cliente, inicio, j);
        }

        if (i < fim)
        {
            quickSortNumerica(cliente, i, fim);
        }
    }
}

void desativarCliente(struct Clientes *cliente, int quantidadeClientes, int idDesativar, FILE *arquivo)
{
    int encontrado = 0;

    for (int i = 0; i < quantidadeClientes; i++)
    {
        if (idDesativar == i)
        {
            cliente[i].ativo = 0;
            encontrado = 1;
            printf("\tCliente desativado com sucesso.\n");

            // reabre o arquivo em modo de escrita, truncando-o
            fclose(arquivo);
            arquivo = fopen("clientes.txt", "w");
            if (arquivo == NULL)
            {
                printf("\tErro ao abrir o arquivo para escrita.\n");
                return;
            }

            // reescreve todos os clientes (ativos) no arquivo
            for (int j = 0; j < quantidadeClientes; j++)
            {
                fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                        cliente[j].nome, cliente[j].cpf, cliente[j].nomeRua, cliente[j].numeroCasa, cliente[j].complemento, cliente[j].telefone,
                        cliente[j].ativo);
            }

            break;
        }
    }

    if (!encontrado)
    {
        printf("\tEste cliente não existe.\n");
    }
}

void listarCliente(struct Clientes *cliente, int quantidadeClientes, int opcaoOrdenacao)
{
    if (quantidadeClientes == 0)
    {
        printf("\n\tNenhum cliente cadastrado.\n");
        return;
    }

    if (opcaoOrdenacao == 1)
    {
        quickSortAlfabetica(cliente, 0, quantidadeClientes - 1);
    }
    else if (opcaoOrdenacao == 2)
    {
        quickSortNumerica(cliente, 0, quantidadeClientes - 1);
    }
    else
    {
        printf("\n\tOpção inválida.\n");
        return;
    }

    printf("\n\tLista de clientes:\n");
    printf("\t************************************************\n");

    for (int i = 0; i < quantidadeClientes; i++)
    {
        if (cliente[i].ativo == 1)
        {
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

int compareClientes(const void *a, const void *b)
{
    return strcmp(((struct Clientes *)a)->cpf, ((struct Clientes *)b)->cpf);
}

void consultarCliente(struct Clientes *cliente, int quantidadeClientes, char cpfConsultar[])
{
    qsort(cliente, quantidadeClientes, sizeof(struct Clientes), compareClientes);

    int encontrado = 0;
    int inicio = 0, fim = quantidadeClientes - 1;

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;
        int comparar = strcmp(cpfConsultar, cliente[meio].cpf);

        if (comparar == 0)
        {
            printf("\n\t************************************************\n");
            printf("\tNome: %s\n", cliente[meio].nome);
            printf("\tCPF: %s\n", cliente[meio].cpf);
            printf("\tEndereço: %s\n", cliente[meio].nomeRua);
            printf("\tNúmero da rua: %s\n", cliente[meio].numeroCasa);
            printf("\tComplemento: %s\n", cliente[meio].complemento);
            printf("\tTelefone: %s\n", cliente[meio].telefone);

            encontrado = 1;
            break;
        }
        else if (comparar < 0)
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
    }

    if (!encontrado)
    {
        printf("\tCPF não cadastrado.\n");
    }
}

void excluirCliente(struct Clientes *cliente, int *quantidadeClientes, FILE *arquivo)
{
    int idExcluir;

    printf("\t==> Informe o número do cliente que deseja excluir: ");
    scanf("%d", &idExcluir);
    idExcluir--;

    if (idExcluir >= 0 && idExcluir < *quantidadeClientes)
    {
        cliente[idExcluir].ativo = 0; // Marcar como inativo
        printf("\tCliente excluído com sucesso.\n");

        // Atualizar o arquivo com a nova situação dos clientes
        fclose(arquivo);
        arquivo = fopen("clientes.txt", "w");
        if (arquivo == NULL)
        {
            printf("\tErro ao abrir o arquivo para escrita.\n");
            return;
        }

        for (int i = 0; i < *quantidadeClientes; i++)
        {
            if (cliente[i].ativo == 1)
            {
                fprintf(arquivo, "Nome: %s\nCPF: %s\nNome da rua: %s\nNumero da rua: %s\nComplemento: %s\nTelefone: %s\nAtivo: %d\n",
                        cliente[i].nome, cliente[i].cpf, cliente[i].nomeRua, cliente[i].numeroCasa, cliente[i].complemento, cliente[i].telefone, cliente[i].ativo);
            }
        }
    }
    else
    {
        printf("\tNúmero inválido.\n");
    }
}

struct Clientes *buscarCliente(struct Clientes *clientes, int quantidadeClientes, const char *cpf)
{
    for (int i = 0; i < quantidadeClientes; i++)
    {
        if (clientes[i].ativo && strcmp(clientes[i].cpf, cpf) == 0)
        {
            return &clientes[i];
        }
    }
    return NULL; // null se o cliente não for encontrado
}

void realizarPedido(struct Clientes *clientes, int quantidadeClientes)
{
    static int idPedido = 1;
    char cpf[12];
    int sabor, tamanho, quantidade;
    float preco = 0.0, totalPedido = 0.0;
    int totalQuantidade = 0;
    char adicionarOutroSabor, novoCliente;

    FILE *arquivoPedidos;

    do
    {
        printf("\t==> Digite o CPF do cliente para realizar o pedido: ");
        scanf("%s", cpf);

        struct Clientes *cliente = buscarCliente(clientes, quantidadeClientes, cpf);
        if (cliente == NULL)
        {
            printf("\t Cliente não encontrado ou inativo.\n");
            return;
        }

        arquivoPedidos = fopen("pedidos.txt", "a");

        if (arquivoPedidos == NULL)
        {
            printf("Erro ao abrir o arquivo de pedidos.\n");
            return;
        }

        fprintf(arquivoPedidos, "Pedido %d:\n", idPedido);

        do
        {
            printf("\n");
            printf("\t************************************************\n");
            printf("\t*                    Menu                      *\n");
            printf("\t************************************************\n");
            printf("\t*     Escolha o sabor:                         *\n");
            printf("\t*     1 - Calabresa                            *\n");
            printf("\t*     2 - Quatro Queijos                       *\n");
            printf("\t*     3 - Frango com Catupiry                  *\n");
            printf("\t************************************************\n");
            printf("\t==> Digite o número do sabor desejado: ");
            fflush(stdin);
            scanf("%d", &sabor);

            char saborEscolhido[20];

            switch (sabor)
            {
            case 1:
                strcpy(saborEscolhido, "Calabresa");
                printf("\n");
                printf("\t************************************************\n");
                printf("\t*            Você escolheu Calabresa.          *\n");
                printf("\t************************************************\n");
                printf("\t*            Escolha o tamanho:                *\n");
                printf("\t*        1 - Pequena (4 fatias) - R$20.00      *\n");
                printf("\t*        2 - Média (8 fatias) - R$30.00        *\n");
                printf("\t*        3 - Grande (12 fatias) - R$40.00      *\n");
                printf("\t************************************************\n");
                printf("\t==> Digite o número do tamanho desejado: ");
                fflush(stdin);
                scanf("%d", &tamanho);
                if (tamanho == 1)
                    preco = 20.0;
                else if (tamanho == 2)
                    preco = 30.0;
                else if (tamanho == 3)
                    preco = 40.0;
                else
                    printf("Tamanho inválido.\n");
                break;
            case 2:
                strcpy(saborEscolhido, "Quatro Queijos");
                printf("\n");
                printf("\t************************************************\n");
                printf("\t*         Você escolheu Quatro Queijos.        *\n");
                printf("\t************************************************\n");
                printf("\t*            Escolha o tamanho:                *\n");
                printf("\t*         1 - Pequena (4 fatias) - R$25.00     *\n");
                printf("\t*         2 - Média (8 fatias) - R$35.00       *\n");
                printf("\t*         3 - Grande (12 fatias) - R$45.00     *\n");
                printf("\t************************************************\n");
                printf("\t==> Digite o número do tamanho desejado: ");
                fflush(stdin);
                scanf("%d", &tamanho);
                if (tamanho == 1)
                    preco = 25.0;
                else if (tamanho == 2)
                    preco = 35.0;
                else if (tamanho == 3)
                    preco = 45.0;
                else
                    printf("Tamanho inválido.\n");
                break;
            case 3:
                strcpy(saborEscolhido, "Frango com Catupiry");
                printf("\n");
                printf("\t************************************************\n");
                printf("\t*       Você escolheu Frango com Catupiry.     *\n");
                printf("\t************************************************\n");
                printf("\t*            Escolha o tamanho:                *\n");
                printf("\t*       1 - Pequena (4 fatias) - R$22.00       *\n");
                printf("\t*       2 - Média (8 fatias) - R$32.00         *\n");
                printf("\t*       3 - Grande (12 fatias) - R$42.00       *\n");
                printf("\t************************************************\n");
                printf("\t==> Digite o número do tamanho desejado: ");
                fflush(stdin);
                scanf("%d", &tamanho);
                if (tamanho == 1)
                    preco = 22.0;
                else if (tamanho == 2)
                    preco = 32.0;
                else if (tamanho == 3)
                    preco = 42.0;
                else
                    printf("Tamanho inválido.\n");
                break;
            default:
                printf("Sabor inválido.\n");
                fclose(arquivoPedidos);
                return;
            }

            if (preco > 0)
            {
                printf("\t==> Quantas pizzas deste tipo você deseja? ");
                fflush(stdin);
                scanf("%d", &quantidade);

                totalPedido += preco * quantidade;
                totalQuantidade += quantidade;

                fprintf(arquivoPedidos, "%d pizza %s %s\n", quantidade, saborEscolhido, (tamanho == 1) ? "pequena" : (tamanho == 2) ? "media"
                                                                                                                                    : "grande");
            }

            printf("\t==> Deseja adicionar outra pizza ao pedido? (s/n): ");
            fflush(stdin);
            scanf(" %c", &adicionarOutroSabor);

        } while (adicionarOutroSabor == 's' || adicionarOutroSabor == 'S');

        printf("\n");
        printf("\t****************************************************************************\n");
        printf("\t          Resumo do Pedido #%d:\n", idPedido);
        printf("\t          Cliente: %s\n", cliente->nome);
        printf("\t          CPF: %s\n", cliente->cpf);
        printf("\t          Endereço de Entrega: Rua %s, Nº %s, Complemento: %s\n",
               cliente->nomeRua, cliente->numeroCasa, cliente->complemento);
        printf("\t          Quantidade Total de Pizzas: %d\n", totalQuantidade);
        printf("\t          Total do Pedido: R$%.2f\n", totalPedido);
        printf("\t****************************************************************************\n");
        printf("\n");

        fprintf(arquivoPedidos, "Endereco: Rua %s, Nº %s, Complemento: %s\n\n",
                cliente->nomeRua, cliente->numeroCasa, cliente->complemento);

        fclose(arquivoPedidos);

        idPedido++;
        totalPedido = 0.0;
        totalQuantidade = 0;

        printf("\t==> Deseja realizar um novo pedido para outro cliente? (s/n): ");
        fflush(stdin);
        scanf(" %c", &novoCliente);

    } while (novoCliente == 's' || novoCliente == 'S');
}

int main()
{

    FILE *arquivo;
    int opcao, quantidadeClientes = 0, idDesativar, opcaoOrdenacao;
    char cpfConsultar[12];

    arquivo = fopen("clientes.txt", "a+");
    if (arquivo == NULL)
    {
        printf("\tProblema ao abrir o arquivo");
        return 1;
    }

    struct Clientes *cliente;
    cliente = malloc(100 * sizeof(struct Clientes));
    if (cliente == NULL)
    {
        printf("\nErro na alocação de memória.");
        return 1;
    }

    lerClientesDoArquivo(cliente, &quantidadeClientes, arquivo);

    do
    {
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

        switch (opcao)
        {
        case 1:
            cadastrarCliente(cliente, &quantidadeClientes, arquivo);
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
            realizarPedido(cliente, quantidadeClientes);
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

    if (fclose(arquivo) == 0)
    {
        printf("\n\tArquivo fechado corretamente.");
        return 0;
    }
    else
    {
        printf("\n\tProblema ao fechar o arquivo.");
        return 1;
    }

    return 0;
}