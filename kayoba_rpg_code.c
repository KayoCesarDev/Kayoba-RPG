#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

#define Max_Personagens 50

typedef struct {
    int id;
    char nome[50];
    char classe[30];

    int nivel;
    int xp;
    int vitorias;

    int vidaBase;
    int ataque;
    int defesa;
} Player;

Player Personagem[Max_Personagens];
int TotalPersonagens = 0;

/* Auxiliares */
void Pausar();
void LimparTela();  
void LimpaBuffer();
int LimiteDeEscolha(int min, int max);
void Pergunta(char *mensagem, char *texto, int tamanho);

/* Criação Personagem */
void MenuPersonagem();
void CriarPersonagem();
void ListarPersonagens();
void MostrarBarraVida(int vidaAtual, int vidaMax);

/* Menu Principal */
void BatalhaPvP();
void BatalhaPvE();
void VerificarLevelUp(Player *p);
void GlobalRanking();

int main (void) {
    
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    setlocale(LC_ALL, "pt_BR.utf8");

    srand(time(NULL));
    int opcao;
    
    do {
        LimparTela();
        
        printf("|====| |Kayoba / RPG| |====|\n\n");
        
        printf("[1]. Gerenciar Personagens.\n");
        printf("[2]. Batalha 1v1 (PvP).\n");
        printf("[3]. Caçar Monstro (PvE).\n");
        printf("[4]. Ranking.\n");
        printf("[5]. Sair.\n\n");
        
        printf("Escolha uma Opção: ");
        opcao = LimiteDeEscolha(1, 5);
        
        switch (opcao) {

            case 1: MenuPersonagem(); break;
            case 2: BatalhaPvP(); break;
            case 3: BatalhaPvE(); break;
            case 4: GlobalRanking(); break;
            case 5: printf("Jogo Encerrado!\n"); break;
        }
    } while (opcao != 5);
    
    return 0;   
}

/* Funções Auxiliares */

void Pausar() {
    printf("\nAperte o Enter para Continuar...");
    getchar();
}

void LimparTela() {
    
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void LimpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }   
}

int LimiteDeEscolha(int min, int max) {
    int opcao;
    
    while(scanf("%d", &opcao) != 1 || opcao < min || opcao > max) {
        LimpaBuffer();
        printf("Opção Inválida! Digite um Número entre %d e %d\n", min, max);
    }
    LimpaBuffer();
    return opcao;
}

void Pergunta(char *mensagem, char *texto, int tamanho) {
    printf("%s", mensagem);
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

/* Funções do Menu Personagem */

void MenuPersonagem() {
    int op;

    do {
        LimparTela();
        printf("\n--- MENU PERSONAGEM ---\n");
        printf("[1] Criar\n");
        printf("[2] Listar\n");
        printf("[3] Voltar\n");

        printf("Escolha uma Opção: ");
        op = LimiteDeEscolha(1,3);

        switch(op) {
            case 1: CriarPersonagem(); break;
            case 2: ListarPersonagens(); break;
            case 3: break;
        }

    } while(op != 3);
}

void CriarPersonagem() {
    LimparTela();
    if (TotalPersonagens >= Max_Personagens) {
        printf("Limite de Personagens Atingido!\n");
        Pausar();
        return;
    }

    Player novo;
    novo.id = TotalPersonagens + 1;
    novo.nivel = 1;
    novo.xp = 0;
    novo.vitorias = 0;
    
    printf("\n");
    Pergunta("Nome: ", novo.nome, 50);

    printf("\nEscolha a classe:\n");
    printf("[1] Guerreiro\n");
    printf("[2] Mago\n");
    printf("[3] Arqueiro\n");

    printf("Escolha uma Opção: ");
    int classe = LimiteDeEscolha(1,3);

    if (classe == 1) {
        strcpy(novo.classe, "Guerreiro");
        novo.vidaBase = 120;
        novo.ataque = 12;
        novo.defesa = 8;
    }
    else if (classe == 2) {
        strcpy(novo.classe, "Mago");
        novo.vidaBase = 80;
        novo.ataque = 18;
        novo.defesa = 3;
    }
    else {
        strcpy(novo.classe, "Arqueiro");
        novo.vidaBase = 100;
        novo.ataque = 14;
        novo.defesa = 5;
    }

    Personagem[TotalPersonagens++] = novo;

    printf("\nPersonagem Criado Com Sucesso!\n");
    Pausar();
}

void ListarPersonagens() {
    LimparTela();

    if (TotalPersonagens == 0) {
        printf("Nenhum Personagem Criado Ainda!\n");
        Pausar();
        return;
    }

    printf("\n==== |LISTA DE PERSONAGENS| ====\n");

    for (int k = 0; k < TotalPersonagens; k++) {

        printf("\nID: %d\n", Personagem[k].id);
        printf("Nome: %s\n", Personagem[k].nome);
        printf("Classe: %s\n", Personagem[k].classe);
        
        printf("Nivel: %d\n", Personagem[k].nivel);
        printf("XP: %d / %d\n", Personagem[k].xp, Personagem[k].nivel * 100);
        printf("Vitórias: %d\n", Personagem[k].vitorias);
        
        printf("Vida Base: %d\n", Personagem[k].vidaBase);
        printf("Ataque: %d\n", Personagem[k].ataque);
        printf("Defesa: %d\n", Personagem[k].defesa);

        printf("-----------------------------\n");
    }

    Pausar();
}

void MostrarBarraVida(int vidaAtual, int vidaMax) {
    
    if (vidaMax <= 0) return; // Evita divisão por 0;
    if (vidaAtual < 0) vidaAtual = 0; // Só para quando tomar muito dano e morrer, mostrar 0 na vida!
    if (vidaAtual > vidaMax) vidaAtual = vidaMax; // Mesma coisa, se curar demais, mostra a vida máxima!
    
    int totalBarras = 20;
    int barrasCheias = (vidaAtual * totalBarras) / vidaMax; // Calcula as barras cheias com uma regra de 3 simples.

    printf("["); // Só pra abrir uma caixa visualmente!

    for (int k = 0; k < totalBarras; k++) {
        if (k < barrasCheias)
            printf("█"); // Foi o que deu certo primeiro kkkkkkk
        else
            printf("░"); // Esse tbm...
    }

    printf("] %d/%d\n", vidaAtual, vidaMax);
}

/* Funções do Menu Principal */

void BatalhaPvE() {
    LimparTela();

    if (TotalPersonagens == 0) {
        printf("Nenhum personagem criado!\n");
        Pausar();
        return;
    }
    
    int confirme;
    printf("Quer mesmo lutar contra monstros agora?\n");
    printf("[1] - SIM | [2] - NÃO: ");
    scanf("%d", &confirme);
    printf("\n");

    if(confirme == 2) {
        return;
    } else {
        printf("Escolha o personagem:\n");
        for(int k = 0; k < TotalPersonagens; k++) {
            printf("[%d] %s (Nv %d)\n", k, Personagem[k].nome, Personagem[k].nivel);
        }
        
        int escolha = LimiteDeEscolha(0, TotalPersonagens-1);
        Player *p = &Personagem[escolha];
        
        int nivelMonstro = p->nivel + (rand() % 3 - 1);
        
        if (nivelMonstro < 1) {
            nivelMonstro = 1;
        } 
            
        int vidaMonstro = 60 + (nivelMonstro * 12);
        int vidaMonstroMax = vidaMonstro;
        
        int ataqueMonstro = 8 + (nivelMonstro * 3);
        int defesaMonstro = 6 + nivelMonstro;
        
        int vidaPlayer = p->vidaBase;
        int vidaPlayerMax = p->vidaBase;
        
        int turno = rand() % 2;
        
        printf("\nUm Monstro Nível %d apareceu!\n\n", nivelMonstro);
    
        // Mostrar quem começa
        if (turno == 0) {
            printf("%s Começa Atacando!\n\n", p->nome);
        } else {
            printf("O Monstro Começa Atacando!\n\n");
        }

        Sleep(1000);
    
        while (vidaPlayer > 0 && vidaMonstro > 0) {
            
            if (turno == 0) {
                
                printf("%s está atacando...\n", p->nome);
                Sleep(1000);
    
                int dano = (p->ataque + rand()%5) - defesaMonstro;
                if (dano < 1) dano = 1;
                
                if (rand() % 100 < 15) {
                    dano *= 2;
                    printf("CRÍTICO!\n");
                    Sleep(1000);
                }
                
                vidaMonstro -= dano;
                
                printf("%s causou %d de dano!\n", p->nome, dano);
                Sleep(1000);
                turno = 1;
                LimparTela();
            } else {
                
                printf("O monstro está atacando...\n");
                Sleep(1000);
    
                int dano = (ataqueMonstro + rand()%5) - p->defesa;
                if (dano < 1) dano = 1;
                
                if (rand() % 100 < 15) {
                    dano *= 2;
                    printf("CRÍTICO DO MONSTRO!\n");
                    Sleep(1000);
                }
                
                vidaPlayer -= dano;
                
                printf("Monstro causou %d de dano!\n", dano);
                Sleep(1000);
                
                turno = 0;
                LimparTela();
            }
    
            // Mostrar as duas barras sempre
            printf("\n%s ", p->nome);
            MostrarBarraVida(vidaPlayer, vidaPlayerMax);
    
            printf("Monstro ");
            MostrarBarraVida(vidaMonstro, vidaMonstroMax);
    
            printf("\n");
            Sleep(1000);
        }
        
        if (vidaPlayer > 0) {
    
            printf("\n|=|CONGRATULATIONS|=|\n");
            Pausar();
    
            int xpGanho = 50 + (nivelMonstro * 10);
            printf("\nVocê Ganhou %d XP!\n", xpGanho);
            Pausar();
    
            p->xp += xpGanho;
            VerificarLevelUp(p);
    
        } else {
            printf("\nVocê Perdeu...\n");
            Pausar();
        }

    }
}

void BatalhaPvP() {
    LimparTela();

    if (TotalPersonagens < 2) {
        printf("Precisa de pelo menos 2 personagens!\n");
        Pausar();
        return;
    }

    int confirme;
    printf("Quer mesmo lutar contra monstros agora?\n");
    printf("[1] - SIM | [2] - NÃO: ");
    scanf("%d", &confirme);

    if (confirme == 2) {
        return;
    } else {
        printf("Escolha o primeiro jogador:\n");
        for(int k = 0; k < TotalPersonagens; k++)
            printf("[%d] %s (Nv %d)\n", k, Personagem[k].nome, Personagem[k].nivel);
        
        int p1 = LimiteDeEscolha(0, TotalPersonagens-1);
        int p2;
        
        do {
            printf("Escolha o segundo jogador:\n");
            p2 = LimiteDeEscolha(0, TotalPersonagens-1);
        } while(p1 == p2);
        
        Player *a = &Personagem[p1];
        Player *b = &Personagem[p2];
        
        int vidaA = a->vidaBase;
        int vidaB = b->vidaBase;
        
        int vidaAMax = a->vidaBase;
        int vidaBMax = b->vidaBase;
        
        int turno = rand() % 2;
        
        printf("\nBatalha começou!\n\n");
    
        // Mostrar quem começa
        if (turno == 0)
            printf("%s começa atacando!\n\n", a->nome);
        else
            printf("%s começa atacando!\n\n", b->nome);
    
        Sleep(1000);
        
        while (vidaA > 0 && vidaB > 0) {
            
            if (turno == 0) {
                
                printf("%s está atacando...\n", a->nome);
                Sleep(700);
    
                int dano = (a->ataque + rand()%5) - b->defesa;
                if (dano < 1) dano = 1;
                
                if (rand()%100 < 15) {
                    dano *= 2;
                    printf("CRÍTICO!\n");
                }
                
                vidaB -= dano;
                
                printf("%s causou %d de dano!\n", a->nome, dano);
                
                turno = 1;
                
            } else {
                
                printf("%s está atacando...\n", b->nome);
                Sleep(700);
    
                int dano = (b->ataque + rand() % 5) - a->defesa;
                if (dano < 1) dano = 1;
                
                if (rand() % 100 < 15) {
                    dano *= 2;
                    printf("CRÍTICO!\n");
                }
                
                vidaA -= dano;
                
                printf("%s causou %d de dano!\n", b->nome, dano);
                
                turno = 0;
            }
    
            // Mostrar as duas barras sempre
            printf("\n%s ", a->nome);
            MostrarBarraVida(vidaA, vidaAMax);
    
            printf("%s ", b->nome);
            MostrarBarraVida(vidaB, vidaBMax);
    
            printf("\n");
            Sleep(1000);
        }
        
        if (vidaA > 0) {
            
            printf("%s venceu!\n", a->nome);
            
            int xpGanho = 100;
            printf("%s ganhou %d XP!\n", a->nome, xpGanho);
            
            a->xp += xpGanho;
            a->vitorias++;
            VerificarLevelUp(a);
            
        } else {
            
            printf("%s venceu!\n", b->nome);
            
            int xpGanho = 100;
            printf("%s ganhou %d XP!\n", b->nome, xpGanho);
            
            b->xp += xpGanho;
            b->vitorias++;
            VerificarLevelUp(b);
        }
        
        Pausar();
    }
    
}

void VerificarLevelUp(Player *p) {
    LimparTela();

    while (p->xp >= p->nivel * 100) {

        p->xp -= p->nivel * 100;
        p->nivel++;
        p->vidaBase += 10;
        p->ataque += 2;
        p->defesa += 1;

        printf("\n%s SUBIU PARA O NÍVEL %d!\n", p->nome, p->nivel);
        printf("Vida +10 | Ataque +2 | Defesa +1\n\n");
    }
}

void GlobalRanking() {
    LimparTela();
    if (TotalPersonagens == 0) {
        printf("Sem jogadores!\n");
        Pausar();
        return;
    }

    Player copia[Max_Personagens];

    // Copia os personagens
    for(int k = 0; k < TotalPersonagens; k++)
        copia[k] = Personagem[k];

    // Ordenação por Nível e depois por Vitórias
    for(int k = 0; k < TotalPersonagens - 1; k++) {
        for(int j = 0; j < TotalPersonagens - 1 - k; j++) {

            if (copia[j].nivel < copia[j+1].nivel ||
               (copia[j].nivel == copia[j+1].nivel &&
                copia[j].vitorias < copia[j+1].vitorias)) {

                Player temp = copia[j];
                copia[j] = copia[j+1];
                copia[j+1] = temp;
            }
        }
    }

   printf("\n--- TOP 10 RANKING ---\n");

   int limite = TotalPersonagens < 10 ? TotalPersonagens : 10;

   for(int k = 0; k < limite; k++) {
       
       printf("%dº - %s (Nv %d | Vitórias %d)\n",
       k+1,
       copia[k].nome,
       copia[k].nivel,
       copia[k].vitorias);  
    } 
    
    Pausar();
}
