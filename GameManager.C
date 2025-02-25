//comando para executar script no CMD
//cd "c:\vs.code\RPGGame" && gcc GameManager.C -o GameManager && "C:\vs.code\RPGGame\"GameManager 

//includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>

//enum utilizado para cada capitulo do jogo
typedef enum {
	VilaInicial,
	Castelo,
	Floresta,
	Caverna,
	CidadeDosGoblins
} Ambiente;

//enum utilizado para definicao de tipo de item equipado pelo player
typedef enum {
    Nenhum = -1,
    Defesa,
    Ataque,
    Magia
} TipoItem;

//declaracao classe de item equipado do player
typedef struct {
    char nome[50];
    TipoItem tipo;
    int valor;
} Item;

//declaracao da classe player
typedef struct {
    int vida;
    int ataque;
    int defesa;
    int quantidade_cura; //o player inicia com 3 potes de cura
	int segunda_chance; //o player inicia com 3 "segundas chances" para caso morra sem concluir o jogo
    Item itens[2]; // O jogador pode equipar até 2 itens
} Player;

//declaracao da classe inimigo
typedef struct {
    int vida;
    int ataque;
    int level;
} Inimigo;

// Inicializa o player
Player inicializaPlayer(int vida, int ataque, int defesa, int quantidade_cura) {
    Player p;
    p.vida = vida;
    p.ataque = ataque;
    p.defesa = defesa;
    p.quantidade_cura = quantidade_cura;
    p.segunda_chance = 3;
    
    for (int i = 0; i < 2; i++) {
        strcpy(p.itens[i].nome, "");
        p.itens[i].tipo = Nenhum;
        p.itens[i].valor = 0;
    }

    return p;
}

// Função para equipar um item
void equiparItem(Player *p, const char *nome, TipoItem tipo, int valor) {
    int slot = -1;
    
    // Verifica se o item já está equipado e se há um slot livre
    for (int i = 0; i < 2; i++) {
        if (p->itens[i].tipo == tipo && tipo == Defesa) {
            // Não pode equipar dois escudos
            printf("\nNao e possivel equipar dois escudos ao mesmo tempo!\n");
            return;
        }
        if (p->itens[i].tipo == Nenhum) {
            slot = i;
        }
    }
    
    // Se nenhum slot livre foi encontrado, substituir o item de menor valor
    if (slot == -1) {
        slot = (p->itens[0].valor < p->itens[1].valor) ? 0 : 1;
    }
    
    // Equipando o novo item
    strcpy(p->itens[slot].nome, nome);
    p->itens[slot].tipo = tipo;
    p->itens[slot].valor = valor;
    printf("\nItem '%s' equipado!\n", nome);
}

//funcao para inicializar inimigo1
Inimigo inicializaInimigo(int nivel) {
    Inimigo i;
    i.vida = (int)ceil(40 * nivel);
    i.ataque = (int)ceil(11 * nivel);
    i.level = nivel;

    return i;
}

// Exibe as propriedades atuais do player
//essa funcao e utilizada repetidamente no decorrer do jogo
void exibePropriedadesDoPlayer(Player *p) {
    printf("\nSuas propriedades:\n");
    printf("- Vida: %d\n", p->vida);
    printf("- Quantidade de pocoes de cura: %d\n", p->quantidade_cura);
    printf("- Pontos de ataque: %d\n", p->ataque);
    printf("- Pontos de defesa: %d\n", p->defesa);
    printf("- Quantidade de segundas chances restantes: %d\n", p->segunda_chance);
    
    for (int i = 0; i < 2; i++) {
        if (p->itens[i].tipo != Nenhum) {
            printf("- Equipamento %d: %s\n", i + 1, p->itens[i].nome);
            if (p->itens[i].tipo == Ataque) {
                printf("  - Dano do equipamento: %d\n", p->itens[i].valor);
            } else if (p->itens[i].tipo == Defesa) {
                printf("  - Defesa do equipamento: %d\n", p->itens[i].valor);
            }
        }
    }
    
    Sleep(1200);
}

//funcao para luta
//essa funcao tem um comportamento isolado da funcao base que roda o jogo
//essa funcao e chamada na rodada inicial e a cada 4 rodadas do jogo
void luta(Player *p, Inimigo *i) {
    srand(time(NULL));
    printf("\nInicio de batalha!\n");
    Sleep(1200);
    printf("Voce deu de cara com um goblin level %d, de %d pontos de vida!\n", i->level, i->vida);
    int rodadasComEscudoAtivo = 0;
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
            for (int j = 0; j < 2; j++) {
                if (p->itens[j].tipo == Ataque) {
                    danoPlayer += p->itens[j].valor;
                }
            }
            if (danoPlayer < 1) {
                printf("\nVoce tentou atacar o inimigo mas ele desviou! Nenhum dano causado.\n");
            } else {
                i->vida -= danoPlayer;
                if (i->vida < 0) i->vida = 0;
                printf("\nVoce atacou causando %d de dano! Vida do inimigo: %d\n", danoPlayer, i->vida);
                if (i->vida <= 0) {
                    printf("\nO inimigo foi derrotado!\n");
                    break;
                }
            }
        } else if (escolha == 2) {
            printf("Defesa reforcada por 3 rodadas!\n");
            rodadasComEscudoAtivo = 3;
        } else if (escolha == 3) {
            if (p->quantidade_cura > 0) {
                p->vida += 20;
                p->quantidade_cura -= 1;
                printf("Voce usou uma pocao de cura. Sua vida agora e %d. Quantidade de itens de cura restante: %d\n", p->vida, p->quantidade_cura);
            } else {
                printf("Voce nao tem mais curas! Sua rodada foi desperdicada.\n");
            }
        }
        int danoInimigo = ((i->ataque - 5) + (rand() % 6)) - p->defesa;
        for (int j = 0; j < 2; j++) {
            if (p->itens[j].tipo == Defesa) {
                danoInimigo -= p->itens[j].valor; //se equipado com escudo, a defesa é reforçada
                if (rodadasComEscudoAtivo > 0) {
                    danoInimigo -= p->itens[j].valor; //Se está com escudo ativo na rodada, a defesa fica reforçada 2x
                }
            }
        }
        if (danoInimigo < 1) {
            printf("O inimigo tentou te atacar mas voce desviou! Nenhum dano sofrido.\n");
        } else {
            p->vida -= danoInimigo;
            if (p->vida < 0) {
                p->vida = 0; //evitar de ficar com um valor negativo
            }
            printf("O inimigo te atacou causando %d de dano! Sua vida: %d\n", danoInimigo, p->vida);
            if (p->vida <= 0) {
                if (p->segunda_chance > 0){
                    char temp_segunda_chance = 'S';

                    printf("Voce foi derrotado. Deseja usar uma segunda chance? (S/N)\n");
                    char escolha;
                    scanf(" %c", &escolha);
                    if (toupper(escolha) != 'N') {
                        p->segunda_chance -= 1; //mecanica de segunda chance para caso o player morra
                        p->vida = 60;

                        printf("Voce utilizou uma segunda chance. Segundas chances restantes: %d\n", p->segunda_chance);
                    }
                }
                else {
                    printf("Voce foi derrotado!\n");
                }
            }
        }
        if (rodadasComEscudoAtivo > 0) {
            rodadasComEscudoAtivo -= 1;
        }
    }
}

//funcao para facilitar a definicao do ambiente atual do player
Ambiente obterAmbiente(int nivel) {
	if (nivel <= 3) return VilaInicial;
	else if (nivel <= 7) return Castelo;
    else if (nivel <= 11) return Floresta;
    else if (nivel <= 15) return Caverna;
    else return CidadeDosGoblins;
}

void descricaoAmbiente(Ambiente ambiente, int rodada) {
    printf("\n--- Explorando o ambiente ---\n");
    Sleep(1200);

    switch (ambiente) {
        case VilaInicial:
            switch (rodada) {
                case 1:
                    printf("Voce mal saiu de casa e ja encontrou seu primeiro inimigo. Seu coracao acelera, mas voce sabe que nao pode desistir agora.\n");
                    Sleep(1200);
                    printf("A vila esta um caos desde que a princesa sumiu. Pessoas murmuram em cantos, preocupadas com o futuro do reino.\n");
                    Sleep(1200);
                    printf("Um velho comenta sobre um castelo abandonado ao leste. Talvez la haja alguma pista.\n");
                    Sleep(1200);
                    break;
                case 2:
                    printf("Os moradores falam cada vez mais sobre o sumico da princesa. Alguns acreditam que goblins estao envolvidos.\n");
                    Sleep(1200);
                    printf("Uma senhora lhe oferece um amuleto. 'Vai precisar de sorte, garoto', ela diz.\n");
                    Sleep(1200);
                    printf("No fundo da vila, um ferreiro trabalha sem parar. Talvez suas armas sejam uteis para a jornada.\n");
                    Sleep(1200);
                    break;
                case 3:
                    printf("Uma crianca puxa sua manga. 'Voce vai salvar a princesa? Eu acredito em voce!'\n");
                    Sleep(1200);
                    printf("O lider da vila lhe entrega um pequeno mapa. 'Se for mesmo se arriscar, ao menos va preparado'.\n");
                    Sleep(1200);
                    break;
                case 4:
                    printf("Voce ja se sente mais reconhecido. Esta mais motivado do que nunca a trazer a princesa de volta!\n");
                    Sleep(1200);
                    printf("Voce vai chegando nos limites da vila, a caminho do castelo. La voce deve conseguir mais informacoes e itens mais fortes para a sua viagem.\n");
                    Sleep(1200);
                    break;
            }
            break;

        case Castelo:
            switch (rodada) {
                case 6:
                    printf("Voce atravessa os portoes do castelo e sente um arrepio na espinha. O local esta abandonado ha anos.\n");
                    Sleep(1200);
                    printf("Tapestrias antigas retratam reis e batalhas esquecidas. Algo aqui parece esconder um segredo.\n");
                    Sleep(1200);
                    break;
                case 7:
                    printf("Nos corredores frios, voce ouve ecos estranhos. Seria o vento ou algo mais?\n");
                    Sleep(1200);
                    printf("Uma biblioteca poeirenta se estende diante de voce. Pergaminhos falam de um rei goblin... e de sua obsessao por humanos.\n");
                    Sleep(1200);
                    break;
                case 8:
                    printf("Escadas em espiral levam a uma torre desmoronada. Voce encontra um bau trancado, mas nao tem a chave.\n");
                    Sleep(1200);
                    printf("A lua brilha pela janela quebrada, revelando simbolos ocultos nas paredes.\n");
                    Sleep(1200);
                    break;
                case 9:
                    printf("Uma sala secreta se revela atras de uma estante falsa. No centro, um trono vazio coberto de poeira.\n");
                    Sleep(1200);
                    printf("Uma sensacao estranha toma conta de voce. Algo ou alguem ainda habita este castelo...\n");
                    Sleep(1200);
                    break;
            }
            break;

        case Floresta:
            switch (rodada) {
                case 11:
                    printf("A floresta eh densa e cheia de ruidos estranhos. Criaturas ocultas te observam.\n");
                    Sleep(1200);
                    printf("Marcas nas arvores indicam que outros passaram por aqui antes. Mas sera que voltaram?\n");
                    Sleep(1200);
                    break;
                case 12:
                    printf("Um riacho corta a trilha. Pegadas estranhas seguem adiante. Voce hesita, mas continua.\n");
                    Sleep(1200);
                    printf("O vento sussurra entre as arvores, como se alertasse sobre um perigo oculto.\n");
                    Sleep(1200);
                    break;
                case 13:
                    printf("Vultos passam rapidamente entre as sombras. Estarao te seguindo?\n");
                    Sleep(1200);
                    printf("Uma clareira escondida contem ruinas antigas. Inscricoes antigas falam de um antigo ritual.\n");
                    Sleep(1200);
                    break;
                case 14:
                    printf("O ar se torna mais pesado conforme voce avanca. Algo poderoso se esconde nesta floresta.\n");
                    Sleep(1200);
                    printf("De repente, um rugido ecoa entre as arvores. A prova final desta jornada esta proxima.\n");
                    Sleep(1200);
                    break;
            }
            break;

        case Caverna:
            switch (rodada) {
                case 16:
                    printf("A escuridao da caverna parece quase solida. Apenas sua tocha ilumina o caminho.\n");
                    Sleep(1200);
                    printf("Cristais brilhantes pontuam as paredes, refletindo sombras distorcidas.\n");
                    Sleep(1200);
                    break;
                case 17:
                    printf("Voce tropeca em algo rigido. Um esqueleto segura um velho pergaminho.\n");
                    Sleep(1200);
                    printf("Ao desenrolar o pergaminho, voce descobre um mapa. Ele aponta para um tesouro escondido mais a frente.\n");
                    Sleep(1200);
                    break;
                case 18:
                    printf("O som de gotejamento ecoa pelas paredes, mas ha algo mais... passos?\n");
                    Sleep(1200);
                    printf("Uma estranha inscricao na pedra sugere um enigma a ser resolvido.\n");
                    Sleep(1200);
                    break;
                case 19:
                    printf("O ar fica pesado, e um brilho avermelhado surge no fundo da caverna.\n");
                    Sleep(1200);
                    printf("Um guardiao se ergue diante do que parece ser uma valiosa relíquia.\n");
                    Sleep(1200);
                    break;
            }
            break;

        case CidadeDosGoblins:
            switch (rodada) {
                case 21:
                    printf("A cidade goblin eh uma bagunca caotica. Criaturas pequenas negociam objetos duvidosos.\n");
                    Sleep(1200);
                    printf("Voce escuta goblins cochichando sobre uma prisioneira humana... a princesa!\n");
                    Sleep(1200);
                    break;
                case 22:
                    printf("O cheiro de metal derretido preenche o ar. Goblins forjam armas em chamas alaranjadas.\n");
                    Sleep(1200);
                    printf("Um goblin encapuzado te encara de um beco escuro. 'Talvez eu tenha informacoes... por um preco'.\n");
                    Sleep(1200);
                    break;
                case 23:
                    printf("Voce encontrou a princesa! Agora precisa fugir antes que o alarme soe.\n");
                    Sleep(1200);
                    printf("Mas sera que fugir sera o bastante? Ou o rei goblin ira persegui-los ate o fim do mundo?\n");
                    Sleep(1200);
                    break;
                case 24:
                    printf("O rei goblin aguarda em seu trono. Este eh o momento decisivo.\n");
                    Sleep(1200);
                    printf("O castelo goblin treme com os gritos dos servos. Apenas um de voces saira vivo desta batalha.\n");
                    Sleep(1200);
                    break;
            }
            break;

        default:
            printf("Voce esta em um lugar desconhecido... Algo nao parece certo aqui.\n");
            Sleep(1200);
            break;
    }

    printf("\nO que deseja fazer?\n");
}


void eventoAleatorio(Player *p) {
    int chance = rand() % 100;
    if (chance < 55) {
        int itemChance = rand() % 100;
        TipoItem tipoNovoItem;
        const char *itemNome;
        int valor = 0;
        if (itemChance <= 5) {
            itemNome = "Espada Rara (+25 de dano)";
            tipoNovoItem = Ataque;
            valor = 25;
        } else if (itemChance <= 10) {
            itemNome = "Escudo Raro (+25 de defesa)";
            tipoNovoItem = Defesa;
            valor = 25;
        } else if (itemChance <= 30) {
            p->quantidade_cura++;
            printf("\nVoce encontrou um pote de cura! Agora tem %d itens de cura.\n", p->quantidade_cura);
            return;
        } else if (itemChance <= 65) {
            itemNome = "Espada Comum (+10 de dano)";
            tipoNovoItem = Ataque;
            valor = 10;
        } else {
            itemNome = "Escudo Comum (+10 de defesa)";
            tipoNovoItem = Defesa;
            valor = 10;
        }
        printf("\nVoce encontrou um(a) %s!\n", itemNome);
        printf("Deseja equipar o novo item? (S/N)\n");
        char escolha;
        scanf(" %c", &escolha);
        if (toupper(escolha) == 'S') {
            equiparItem(p, itemNome, tipoNovoItem, valor);
            printf("Item equipado!\n");
        } else {
            printf("Voce escolheu nao equipar o item!\n");
        }
    } else if (chance < 95) {
        p->quantidade_cura++;
        printf("\nVoce encontrou uma pocao de cura! Agora tem %d itens de cura.\n", p->quantidade_cura);
    } else {
        printf("\nVoce foi emboscado por um inimigo oculto!\n");
        Inimigo meuInimigo = inicializaInimigo(1);
        luta(p, &meuInimigo);
    }
    Sleep(1200);
}


// Funcao para explorar o ambiente
void explorar(Player *p, int rodada) {
    Ambiente ambiente = obterAmbiente(rodada);
    int opcao;

    printf("\n=== Level de exploracao no ambiente: ===\n");
    Sleep(1200);

    descricaoAmbiente(ambiente, rodada);

    printf("1 - Explorar o local\n");
    printf("2 - Interagir com um objeto\n");
    printf("3 - Descansar e recuperar energia (+vida)\n");

    printf("\nEscolha sua acao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("\nVoce decide explorar mais profundamente...\n");
            Sleep(1200);
            eventoAleatorio(p); // Chama o evento aleatorio
            break;

        case 2:
            printf("\nVoce encontra um objeto interessante...\n");
            Sleep(1200);
            if (ambiente == VilaInicial) {
                printf("Um velho livro de magia. Voce sente um conhecimento antigo fluindo em voce...\n");
                printf("Isso te deixa mais forte e mais experiente. Goblins sao seres magicos. Voce ganha mais defesa contra eles.\n");
                p->defesa += 1;
            }
            else if (ambiente == Castelo) {
                printf("Uma armadura enferrujada, mas ainda pode ser util como protecao!\n");
                p->defesa += 1;
            }
            else if (ambiente == Floresta) {
                printf("Uma fonte mágica! Voce sente sua vida sendo restaurada!\n");
                p->vida += 10;
            }
            else if (ambiente == Caverna) {
                printf("Uma tocha misteriosa. Agora voce enxerga melhor na escuridao.\n");
                printf("Isso faz com que voce tenha mais precisao nos golpes. Seu dano aumentou.\n");
                p->ataque += 5;
            }
            else if (ambiente == CidadeDosGoblins) {
                printf("Uma pedra com inscricões estranhas. Talvez um enigma...\n");
                printf("Voce le tudo e descobre mais informacoes sobre a raca dos goblins.\n");
                printf("Isso aumenta o seu dano contra os goblins.\n");
                p->ataque += 3;
            }
            break;

        case 3:
            printf("\nVoce decide descansar um pouco...\n");
            Sleep(1200);
            p->vida += 10; // Recupera um pouco de vida
            printf("Sua vida aumentou um pouco!\n");
            break;

        default:
            printf("\nEscolha inválida! Voce hesita e perde tempo...\n");
    }
}

int main()
{
    //APRESENTAcaO DO JOGO
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
    printf("No primeiro nivel vc enfrentara seu primeiro inimigo.\nA partir dai, voce enfrentara um inimigo a cada 5 leveis.\nOs demais leveis sao de exploracao e historia.\n");
    printf("Nos leveis de exploracao, voce conseguira escolher como prosseguir em cada etapa, conforme sera exibido na tela.\n");
    printf("O jogo permite multiplas decisoes, entrega diferentes possibiidades a cada vez que voce joga, e tem diferentes finais.\n");
    printf("Ao morrer, voce tem um total de 3 segundas chances. Se perder todas, o seu jogo acaba e voce precisara reiniciar, entao, tome cuidado!\n");
    printf("Durante as fases de exploracao, e possivel encontrar itens com raridades diferentes ou encontrar armadilhas. Entao, fique sempre atento.\n");
    printf("Durante a batalha, voce tem as opcoes de SE CURAR, ATACAR, DEFENDER. Cada funcionalidade tem seu funcionamento.\n");
    printf("Atacar voce apenas ataca.\nDefender voce aumenta a defesa do personagem durante 3 rodadas, mas isso so e valido se voce estiver equipado com um item do tipo ESCUDO.\nUsar cura voce usa uma de suas pocoes de cura e encerra a rodada.\n");
    printf("Por ultimo, vale ressaltar que tanto voce quanto o inimigo tem um sistema de level e upgrade, e ambos ficam mais fortes de acordo com o level.\n");
    printf("\nDito isso, valos comecar o jogo!\n");

	//variável usada somente para dar uma pausa antes de iniciar de fato o jogo
    char temp;
    printf("\nPressione qualquer tecla para continuar...\n");
    scanf(" %c", &temp);

    printf("\nVoce e um simples ferreiro morador da uma vila proxima ao castelo onde fica a familia real.\n");
    printf("Sua vida sempre foi muito pacata, mas voce nao e como as pessoas dali.\n");
    printf("Tudo que voce sempre quis foi uma chance de provar como voce e corajoso e destemido, e que merece respeito de todos!\n");
    printf("Num dia voce acorda um um alvoroco em toda a vila. A grande npoticia: a princesa foi sequestrada!\n");
    printf("Nao se fala em outra coisa. Todos estao nervosos e aflitos.\n"); 
    printf("Os goblins estao invadindo a vila e fazendo o que querem. Alguem precisa para-los\n");
    printf("O que sera dessa vila sem a amada princesa para cuidar de todos?\n");
    printf("Voce nao pensa duas vezes. Essa e sua chance para ser o heroi de todos.\n");    
    printf("E aqui que sua historia comeca. Voce pega suas coisas e sai de casa para descobrir onde esta a princesa e traze-la de volta.\n");

    //variável usada somente para dar uma pausa antes de iniciar de fato o jogo
    printf("\nPressione qualquer tecla para continuar...\n");
    scanf(" %c", &temp);

	//inicializacao do player
    Player meuPlayer = inicializaPlayer(100, 20, 4, 3);

	//loop inicial do game. O jogo tem 25 rodadas
    for (int i = 0; i < 26; i++) {
        printf("\nInicio da rodada %d!\n", i + 1);
        Sleep(1200);

        char nextLevel;

        if (meuPlayer.vida <= 0) {
            Sleep(1200);
            printf("O jogador morreu! Fim de jogo.\n");
            return 0;
        }

        Sleep(1200);
		//exibindo propeirdades atuais do player
        exibePropriedadesDoPlayer(&meuPlayer);


        printf("\nPronto para a proxima etapa? (S/N)\n");
        fflush(stdin); //evitar bugs ao capturar resposta do player
        scanf(" %c", &nextLevel);

        if (toupper(nextLevel) == 'N') {
            printf("\nJogo encerrado!");
            return 0;
        }

        if (((i + 1) % 5 == 0) || i == 0) {
            if (i==0) {
                printf("\nVoce mal saiu de casa e ja deu de cara com seu primeiro inimmigo! Prepare-se, sua aventura comecou mais cedo do que voce esperava!\n");
            }
            if (i==1){
                printf("\n===Voce esta na vila!===\n");
            }
            if (i==6){
                printf("\n===Voce chegou ao Castelo!===\n");
            }
            if (i==11){
                printf("\n===Voce chegou a Floresta Amaldiçoada!===\n");
            }
            if (i==16){
                printf("\n===Voce chegou a Caverna!===\n");
            }
            if (i==21){
                printf("\n===Voce chegou a tao temida Cidade Dos Goblins!===\n===Bem-vindo ao level final!===\n");
            }

			//o inimigo e inicializado a cada 4 rodadas e suas propriedades aumentam de acordo com o level atual
            Inimigo meuInimigo = inicializaInimigo(i > 0 ? (i + 1) / 5 : 1);
            luta(&meuPlayer, &meuInimigo);
        }
        else {
            explorar(&meuPlayer, i);
        }
    }

	//Se chegar ao fim do jogo vivo, significa que venceu o jogo
    if (meuPlayer.vida > 0) {
        Sleep(1200);
        printf("\nParabens! Voce venceu o jogo!\n");
    }


    return 0;
}