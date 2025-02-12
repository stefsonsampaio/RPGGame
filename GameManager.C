//comando para executar script no CMD
//cd "c:\vs.code\RPGGame" && gcc GameManager.C -o GameManager && "C:\vs.code\RPGGame\"GameManager 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>

typedef enum {
	VilaInicial,
	Castelo,
	Floresta,
	Caverna,
	CidadeDosGoblins
} Ambiente;

typedef enum {
    Nenhum = -1,
    Defesa,
    Ataque,
    Magia
} TipoItem;

typedef struct {
    char nome[50];
    TipoItem tipo;
    int valor;
} Item;

typedef struct {
    int vida;
    int ataque;
    int defesa;
    int quantidade_cura;
    Item item;
} Player;

typedef struct {
    int vida;
    int ataque;
    int level;
} Inimigo;

Player inicializaPlayer(int vida, int ataque, int defesa, int quantidade_cura) {
    Player p;
    p.vida = vida;
    p.ataque = ataque;
    p.defesa = defesa;
    p.quantidade_cura = quantidade_cura;

    strcpy(p.item.nome, "");
    p.item.tipo = Nenhum;
    p.item.valor = 0;

    return p;
}

void equiparItem(Player *p, const char *nome, TipoItem tipo, int valor) {
    strcpy(p->item.nome, nome);
    p->item.tipo = tipo;
    p->item.valor = valor;
}

Inimigo inicializaInimigo(int nivel) {
    Inimigo i;
    i.vida = (int)ceil(40 * nivel);
    i.ataque = (int)ceil(11 * nivel);
    i.level = nivel;

    return i;
}

void exibePropriedadesDoPlayer(Player *p) {
    printf("\nSuas propriedades:\n");
    printf("- vida: %d\n", p->vida);
    printf("- quantidade de pocoes de cura: %d\n", p->quantidade_cura);
	printf("- pontos de ataque: %d\n", p->ataque);
	printf("- pontos de defesa: %d\n", p->defesa);
    if (strlen(p->item.nome) > 0) {
        printf("- Equipamento: %c\n", p->item.nome);
        if (p->item.tipo == Ataque) {
            printf("- Dano do equipamento: %d\n", p->item.valor);
        }
        else if (p->item.tipo == Defesa) {
            printf("- Defesa do equipamento: %d\n", p->item.valor);
        }
    }
    Sleep(1200);
}

void luta(Player *p, Inimigo *i) {
    srand(time(NULL));

    printf("\nInicio de batalha!\n");
    Sleep(1200);
    
    printf("Voce deu de cara com um goblin level %d, de %d pontos de vida!\n", i->level, i->vida);

    int rodadasComEscudoAtivo;

    while(p->vida > 0 && i->vida > 0) {
        if (rodadasComEscudoAtivo > 0) {
            Sleep(1200);
            printf("Seu escudo ainda esta ativo por %d rodadas!\n", rodadasComEscudoAtivo);
        }

        Sleep(1200);
        int escolha;
        do {
            printf("\n1. Atacar\n2. Defender\n3. Usar Cura\nEscolha: ");
            if (scanf("%d", &escolha) != 1) {
                while (getchar() != '\n'); 
                escolha = 0; 
            }
        } while (escolha < 1 || escolha > 3);

        if (escolha == 1) {
            int danoPlayer = (p->ataque - 5) + (rand() % 6);
            if (p->item.tipo == Ataque) {
                danoPlayer += p->item.valor;
            }
            if (danoPlayer < 1) {
                Sleep(1200);
                printf("\nVoce tentou atacar o inimigo mas ele desviou! Nenhum dano causado.\n");
            }
            else {
                i->vida -= danoPlayer;
                if (i->vida < 0) i->vida = 0;
                Sleep(1200);
                printf("\nVoce atacou causando %d de dano! Vida do inimigo: %d\n", danoPlayer, i->vida);
                if (i->vida <= 0) {
                    Sleep(1200);
                    printf("\nO inimigo foi derrotado!\n");
                    break;
                }
            }
        }
        else if (escolha == 2) {
            Sleep(1200);
            printf("Defesa reforcada por 3 rodadas!\n");
            rodadasComEscudoAtivo = 3;
        }
        else if (escolha == 3) {
            if (p->quantidade_cura > 0) {
                p->vida += 20;
                p->quantidade_cura -= 1;

                Sleep(1200);
                printf("Voce usou uma pocao de cura. Sua vida agora e %d. Quantidade de itens de cura restante: %d\n", p->vida, p->quantidade_cura);
            }
            else {
                Sleep(1200);
                printf("Voce nao tem mais curas! Sua rodada foi desperdicada.");
            }
        }

        int danoInimigo = ((i->ataque - 5) + (rand() % 6)) - p->defesa;
        if (p->item.tipo == Defesa && rodadasComEscudoAtivo > 0) {
            danoInimigo -= p->item.valor;
        }
        
        if (danoInimigo < 1) {
            Sleep(1200);
            printf("O inimigo tentou te atacar mas voce desviou! Nenhum dano sofrido.\n");
        }
        else {
            p->vida -= danoInimigo;
            if (p->vida < 0) p->vida = 0;
            Sleep(1200);
            printf("O inimigo te atacou causando %d de dano! Sua vida: %d\n", danoInimigo, p->vida);
            if (p->vida <= 0) {
                Sleep(1200);
                printf("O jogador foi derrotado!\n");
            }
        }

        if (rodadasComEscudoAtivo > 0) {
            rodadasComEscudoAtivo -= 1;
        }
    }
}

Ambiente obterAmbiente(int nivel) {
	if (nivel <= 3) return VilaInicial;
	else if (nivel <= 7) return Castelo;
    else if (nivel <= 11) return Floresta;
    else if (nivel <= 15) return Caverna;
    else return CidadeDosGoblins;
}

void explorar(Player *p, int rodada) {
    Ambiente ambiente = obterAmbiente(rodada);
    int opcao;

    printf("\n=== Exploração no ambiente: \n");
	
	Sleep(1200);
	
	switch (ambiente) {
		case VilaInicial:
			printf("1 - Conversar com os aldeões\n");
            printf("2 - Procurar um item no mercado\n");
            printf("3 - Descansar na pousada (+vida)\n");
            break;
        case Castelo:
            printf("1 - Investigar a biblioteca real\n");
            printf("2 - Explorar as catacumbas do castelo\n");
            printf("3 - Conversar com os guardas\n");
            break;
        case Floresta:
            printf("1 - Procurar ervas medicinais (+vida)\n");
            printf("2 - Caçar uma criatura\n");
            printf("3 - Encontrar um esconderijo\n");
            break;
        case Caverna:
            printf("1 - Procurar um tesouro escondido\n");
            printf("2 - Encontrar uma saída alternativa\n");
            printf("3 - Se esconder de criaturas sombrias\n");
            break;
        case CidadeDosGoblins:
            printf("1 - Espionar os goblins\n");
            printf("2 - Roubar suprimentos\n");
            printf("3 - Desafiar um goblin para um duelo\n");
            break;
	}
	
	printf("\nEscolha sua ação: ");
    scanf("%d", &opcao);
	
	switch (opcao) {
        case 1:
            printf("Você toma uma decisão interessante...\n");
            Sleep(1200);
            break;
        case 2:
            printf("Algo inesperado acontece...\n");
            Sleep(1200);
            break;
        case 3:
            printf("Você se prepara para os desafios futuros...\n");
            Sleep(1200);
            break;
        default:
            printf("Escolha inválida! Você hesita e perde tempo...\n");
    }
}

int main()
{
    //APRESENTAÇÃO DO JOGO
    printf("**************************************\n");
    printf("======================================\n");
    printf("**************************************\n");
    printf("\nBEM VINDO AO MEU JOGO RPG DE CONSOLE\n\n");
    printf("**************************************\n");
    printf("======================================\n");
    printf("**************************************\n");

    printf("\nExplicacoes iniciais:\n");
    Sleep(1200);
    printf("Seu objetivo e chegar ao final dos 20 niveis do jogo.\n");
    printf("No primeiro nivel vc enfrentara seu primeiro inimigo.\nA partir dai, voce enfrentara um inimigo a cada 4 leveis.\nOs demais leveis sao de exploracao e historia.\n");
    printf("O jogo permite multiplas decisoes, entrega diferentes possibiidades a cada vez que voce joga, e tem diferentes finais.\n");
    printf("Ao morrer, voce tem um total de 3 segundas chances. Se perder todas, o seu jogo acaba e voce precisara reiniciar, entao, tome cuidado!\n");
    printf("Durante as fases de exploracao, e possivel encontrar itens com raridades diferentes ou encontrar armadilhas. Entao, fique sempre atento.\n");
    printf("Durante a batalha, voce tem as opcoes de SE CURAR, ATACAR, DEFENDER. Cada funcionalidade tem seu funcionamento.\n");
    printf("Atacar voce apenas ataca.\nDefender voce aumenta a defesa do personagem durante 3 rodadas, mas isso so e valido se voce estiver equipado com um item do tipo ESCUDO.\nUsar cura voce usa uma de suas pocoes de cura e encerra a rodada.\n");
    printf("Por ultimo, vale ressaltar que tanto voce quanto o inimigo tem um sistema de level e upgrade, e ambos ficam mais fortes de acordo com o level.\n");
    printf("\nDito isso, valos comecar o jogo!\n");

    char temp;
    printf("\nPressione qualquer tecla para continuar...\n");
    scanf("%c", temp);

    Player meuPlayer = inicializaPlayer(100, 20, 4, 3);

    for (int i = 0; i < 5; i++) {
        printf("\nInicio da rodada %d!\n", i + 1);
        Sleep(1200);

        char nextLevel;

        if (meuPlayer.vida <= 0) {
            Sleep(1200);
            printf("O jogador morreu! Fim de jogo.\n");
            return 0;
        }

        Sleep(1200);
        exibePropriedadesDoPlayer(&meuPlayer);


        printf("\nPronto para a proxima etapa? (S/N)\n");
        fflush(stdin);
        scanf(" %c", &nextLevel);

        if (toupper(nextLevel) == 'N') {
            printf("\nJogo encerrado!");
            return 0;
        }

        if (((i + 1) % 4 == 0) || i == 0) {
            Inimigo meuInimigo = inicializaInimigo(i > 0 ? (i + 1) / 4 : 1);
            luta(&meuPlayer, &meuInimigo);
        }
        else {
            explorar(&meuPlayer, i + 1);
        }
    }

    if (meuPlayer.vida > 0) {
        Sleep(1200);
        printf("\nParabens! Voce venceu o jogo!\n");
    }


    return 0;
}