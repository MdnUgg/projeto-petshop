#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 100
#define TAM_TEXTO 100

/* Estruturas principais do atendimento */
typedef struct {
    char descricao[TAM_TEXTO];
    int quantidade;
    float valorUnitario;
    float valorTotal;
} Item;

typedef struct {
    char nome[TAM_TEXTO];
    char telefone[30];
    int cadastrado;
} Cliente;

typedef struct {
    char nome[TAM_TEXTO];
    char especie[30];
    char raca[TAM_TEXTO];
    char porte[20];
    int cadastrado;
} Pet;

void limparEntrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        /* descarta o restante da linha */
    }
}

void lerTexto(const char *mensagem, char texto[], int tamanho) {
    printf("%s", mensagem);
    if (fgets(texto, tamanho, stdin) != NULL) {
        texto[strcspn(texto, "\n")] = '\0';
    }
}

int lerInteiro(const char *mensagem) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            limparEntrada();
            return valor;
        }
        printf("Entrada inválida. Digite um número.\n");
        limparEntrada();
    }
}

void pausar(void) {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void mostrarCabecalho(const char *titulo) {
    printf("\n==============================\n");
    printf("        %s\n", titulo);
    printf("==============================\n");
}

int fazerLogin(char usuarioLogado[]) {
    const char usuarioCorreto[] = "admin";
    const char senhaCorreta[] = "1234";
    char usuario[30];
    char senha[30];
    int tentativa;

    mostrarCabecalho("LOGIN DO FUNCIONÁRIO");
    for (tentativa = 1; tentativa <= 3; tentativa++) {
        lerTexto("Usuário: ", usuario, sizeof(usuario));
        lerTexto("Senha: ", senha, sizeof(senha));

        if (strcmp(usuario, usuarioCorreto) == 0 &&
            strcmp(senha, senhaCorreta) == 0) {
            strcpy(usuarioLogado, usuario);
            printf("\nLogin realizado com sucesso!\n");
            return 1;
        }

        printf("Login inválido. Tentativa %d de 3.\n\n", tentativa);
    }

    printf("Número máximo de tentativas atingido. Programa encerrado.\n");
    return 0;
}

void cadastrarCliente(Cliente *cliente) {
    mostrarCabecalho("CADASTRO DE CLIENTE");
    lerTexto("Nome do cliente: ", cliente->nome, sizeof(cliente->nome));
    lerTexto("Telefone: ", cliente->telefone, sizeof(cliente->telefone));
    cliente->cadastrado = 1;
    printf("Cliente cadastrado com sucesso!\n");
}

void cadastrarPet(const Cliente *cliente, Pet *pet) {
    int opcao;

    if (!cliente->cadastrado) {
        printf("Cadastre o cliente antes de cadastrar o pet.\n");
        return;
    }

    mostrarCabecalho("CADASTRO DE PET");
    printf("Cliente: %s\n", cliente->nome);
    lerTexto("Nome do pet: ", pet->nome, sizeof(pet->nome));

    do {
        printf("\nEspécie:\n");
        printf("1 - Cão\n");
        printf("2 - Gato\n");
        opcao = lerInteiro("Escolha: ");
        switch (opcao) {
            case 1:
                strcpy(pet->especie, "Cão");
                break;
            case 2:
                strcpy(pet->especie, "Gato");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 1 && opcao != 2);

    lerTexto("Raça: ", pet->raca, sizeof(pet->raca));

    do {
        printf("\nPorte:\n");
        printf("1 - Pequeno\n");
        printf("2 - Médio\n");
        printf("3 - Grande\n");
        opcao = lerInteiro("Escolha: ");
        switch (opcao) {
            case 1:
                strcpy(pet->porte, "Pequeno");
                break;
            case 2:
                strcpy(pet->porte, "Médio");
                break;
            case 3:
                strcpy(pet->porte, "Grande");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao < 1 || opcao > 3);

    pet->cadastrado = 1;
    printf("Pet cadastrado com sucesso!\n");
}

void adicionarItem(Item itens[], int *quantidadeItens, const char *descricao,
                   int quantidade, float valorUnitario) {
    if (*quantidadeItens >= MAX_ITENS) {
        printf("Limite de itens do atendimento atingido.\n");
        return;
    }

    strcpy(itens[*quantidadeItens].descricao, descricao);
    itens[*quantidadeItens].quantidade = quantidade;
    itens[*quantidadeItens].valorUnitario = valorUnitario;
    itens[*quantidadeItens].valorTotal = quantidade * valorUnitario;
    (*quantidadeItens)++;

    printf("Item adicionado: %s - R$ %.2f\n", descricao, quantidade * valorUnitario);
}

float precoBanho(const char porte[], char descricao[]) {
    if (strcmp(porte, "Pequeno") == 0) {
        strcpy(descricao, "Banho - porte pequeno");
        return 35.00f;
    }
    if (strcmp(porte, "Médio") == 0) {
        strcpy(descricao, "Banho - porte médio");
        return 45.00f;
    }
    strcpy(descricao, "Banho - porte grande");
    return 60.00f;
}

float precoTosa(int opcao, char descricao[]) {
    switch (opcao) {
        case 1:
            strcpy(descricao, "Tosa higiênica");
            return 30.00f;
        case 2:
            strcpy(descricao, "Tosa completa");
            return 55.00f;
        case 3:
            strcpy(descricao, "Tosa na tesoura");
            return 70.00f;
        default:
            return 0.00f;
    }
}

float precoVeterinaria(int opcao, char descricao[]) {
    switch (opcao) {
        case 1:
            strcpy(descricao, "Consulta veterinária");
            return 100.00f;
        case 2:
            strcpy(descricao, "Vacina");
            return 80.00f;
        case 3:
            strcpy(descricao, "Exame veterinário");
            return 120.00f;
        default:
            return 0.00f;
    }
}

void menuServicos(const Cliente *cliente, const Pet *pet,
                  Item itens[], int *quantidadeItens) {
    int opcao, subtipo;
    float valor;
    char descricao[TAM_TEXTO];

    if (!cliente->cadastrado || !pet->cadastrado) {
        printf("Cadastre o cliente e o pet antes de registrar serviços.\n");
        pausar();
        return;
    }

    do {
        mostrarCabecalho("SERVIÇOS");
        printf("Cliente: %s | Pet: %s\n\n", cliente->nome, pet->nome);
        printf("1 - Banho\n");
        printf("2 - Tosa\n");
        printf("3 - Veterinária\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1:
                valor = precoBanho(pet->porte, descricao);
                adicionarItem(itens, quantidadeItens, descricao, 1, valor);
                pausar();
                break;
            case 2:
                do {
                    printf("\n1 - Tosa higiênica - R$ 30,00\n");
                    printf("2 - Tosa completa - R$ 55,00\n");
                    printf("3 - Tosa na tesoura - R$ 70,00\n");
                    subtipo = lerInteiro("Tipo de tosa: ");
                    valor = precoTosa(subtipo, descricao);
                    if (valor == 0.00f) {
                        printf("Opção inválida.\n");
                    }
                } while (valor == 0.00f);
                adicionarItem(itens, quantidadeItens, descricao, 1, valor);
                pausar();
                break;
            case 3:
                do {
                    printf("\n1 - Consulta - R$ 100,00\n");
                    printf("2 - Vacina - R$ 80,00\n");
                    printf("3 - Exame - R$ 120,00\n");
                    subtipo = lerInteiro("Tipo de atendimento: ");
                    valor = precoVeterinaria(subtipo, descricao);
                    if (valor == 0.00f) {
                        printf("Opção inválida.\n");
                    }
                } while (valor == 0.00f);
                adicionarItem(itens, quantidadeItens, descricao, 1, valor);
                pausar();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
                pausar();
        }
    } while (opcao != 0);
}

void escolherProduto(const char *categoria, const char nomes[][TAM_TEXTO],
                     const float precos[], Item itens[], int *quantidadeItens) {
    int opcao, quantidade;

    do {
        mostrarCabecalho(categoria);
        printf("1 - %-25s R$ %.2f\n", nomes[0], precos[0]);
        printf("2 - %-25s R$ %.2f\n", nomes[1], precos[1]);
        printf("3 - %-25s R$ %.2f\n", nomes[2], precos[2]);
        printf("0 - Voltar\n");
        opcao = lerInteiro("Produto: ");

        if (opcao >= 1 && opcao <= 3) {
            do {
                quantidade = lerInteiro("Quantidade: ");
                if (quantidade <= 0) {
                    printf("A quantidade deve ser maior que zero.\n");
                }
            } while (quantidade <= 0);
            adicionarItem(itens, quantidadeItens, nomes[opcao - 1], quantidade,
                          precos[opcao - 1]);
            pausar();
        } else if (opcao != 0) {
            printf("Opção inválida.\n");
            pausar();
        }
    } while (opcao != 0);
}

void menuProdutos(Item itens[], int *quantidadeItens) {
    const char alimentos[3][TAM_TEXTO] = {
        "Ração cão 10kg", "Ração gato 3kg", "Petisco"
    };
    const float precosAlimentos[3] = {120.00f, 65.00f, 15.00f};
    const char acessorios[3][TAM_TEXTO] = {
        "Coleira", "Brinquedo", "Cama pet"
    };
    const float precosAcessorios[3] = {25.00f, 18.00f, 95.00f};
    const char higiene[3][TAM_TEXTO] = {
        "Shampoo", "Areia sanitária", "Tapete higiênico"
    };
    const float precosHigiene[3] = {22.00f, 30.00f, 40.00f};
    int opcao;

    do {
        mostrarCabecalho("PRODUTOS");
        printf("1 - Alimentos\n");
        printf("2 - Acessórios\n");
        printf("3 - Higiene\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Escolha uma categoria: ");

        switch (opcao) {
            case 1:
                escolherProduto("ALIMENTOS", alimentos, precosAlimentos,
                                itens, quantidadeItens);
                break;
            case 2:
                escolherProduto("ACESSÓRIOS", acessorios, precosAcessorios,
                                itens, quantidadeItens);
                break;
            case 3:
                escolherProduto("HIGIENE", higiene, precosHigiene,
                                itens, quantidadeItens);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
                pausar();
        }
    } while (opcao != 0);
}

void emitirComprovante(const char funcionario[], const Cliente *cliente,
                       const Pet *pet, const Item itens[], int quantidadeItens) {
    int i;
    float total = 0.00f;

    mostrarCabecalho("COMPROVANTE FINAL");
    if (!cliente->cadastrado || !pet->cadastrado) {
        printf("Cadastre o cliente e o pet para emitir o comprovante.\n");
        pausar();
        return;
    }

    printf("Funcionário: %s\n", funcionario);
    printf("Cliente: %s\n", cliente->nome);
    printf("Telefone: %s\n", cliente->telefone);
    printf("Pet: %s | Espécie: %s | Raça: %s | Porte: %s\n",
           pet->nome, pet->especie, pet->raca, pet->porte);
    printf("\nItens do atendimento:\n");
    printf("------------------------------------------------------------\n");

    if (quantidadeItens == 0) {
        printf("Nenhum item registrado.\n");
    }

    for (i = 0; i < quantidadeItens; i++) {
        printf("%2d. %-30s Qtde: %2d  R$ %8.2f\n",
               i + 1, itens[i].descricao, itens[i].quantidade,
               itens[i].valorTotal);
        total += itens[i].valorTotal;
    }

    printf("------------------------------------------------------------\n");
    printf("VALOR TOTAL: R$ %.2f\n", total);
    pausar();
}

void menuCadastros(Cliente *cliente, Pet *pet) {
    int opcao;

    do {
        mostrarCabecalho("CADASTROS");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Cadastrar pet\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1:
                cadastrarCliente(cliente);
                pausar();
                break;
            case 2:
                cadastrarPet(cliente, pet);
                pausar();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
                pausar();
        }
    } while (opcao != 0);
}

void menuPrincipal(const char funcionario[], Cliente *cliente, Pet *pet,
                   Item itens[], int *quantidadeItens) {
    int opcao;

    do {
        mostrarCabecalho("PETSHOP AMIGO FIEL");
        printf("1 - Cadastros\n");
        printf("2 - Serviços\n");
        printf("3 - Produtos\n");
        printf("4 - Comprovante final\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1:
                menuCadastros(cliente, pet);
                break;
            case 2:
                menuServicos(cliente, pet, itens, quantidadeItens);
                break;
            case 3:
                menuProdutos(itens, quantidadeItens);
                break;
            case 4:
                emitirComprovante(funcionario, cliente, pet, itens,
                                   *quantidadeItens);
                break;
            case 0:
                printf("Atendimento encerrado. Até logo!\n");
                break;
            default:
                printf("Opção inválida.\n");
                pausar();
        }
    } while (opcao != 0);
}

int main(void) {
    char funcionario[30];
    Cliente cliente = {"", "", 0};
    Pet pet = {"", "", "", "", 0};
    Item itens[MAX_ITENS];
    int quantidadeItens = 0;

    printf("Sistema de atendimento para petshop\n");

    if (fazerLogin(funcionario)) {
        menuPrincipal(funcionario, &cliente, &pet, itens, &quantidadeItens);
    }

    return 0;
}
