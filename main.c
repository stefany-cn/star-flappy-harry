#include "raylib.h"
#include <stdio.h>
#include <string.h>

/// TAMANHO JANELA
#define MAX_WIDTH 1200
#define MAX_HEIGHT 800
/// ALTURAS TORRES
#define MAX_ALT_TORRE 450
#define MIN_ALT_TORRE 250
/// QUANTIDADE OBSTACULOS
#define MAX_QTD_OBSTAC 200
/// POSICAO INICIAL JOGADOR
#define POS_INIC_JOG_X 150
#define POS_INIC_JOG_Y 400
/// DISTANCIA E POSICAO INICIAL ENTRE OBSTACULOS EM X
#define DIST_X_OBST 250
#define POS_INIC_OBST_X 500
/// ESCALAS DE TEXTURAS
#define ESCALA_TEXT_TORRE 0.4
#define ESCALA_TEXT_JOG 0.5
#define ESCALA_TEXT_DEMENT 0.2
#define ESCALA_SOLDADO 2.3
/// AJUSTES DE POSICAO E TAMANHO TEXTURAS E SUAS RESPECTIVAS FORMAS
#define AJUSTE_REC_TORRE 70
#define AJUSTE_POS_Y_DEMENT 80
#define AJUSTE_POS_Y_ADVERS 90

/// CONSTANTES DA DIFICULDADE PADRAO
// score
#define SCORE_THRESHOLD 600
// gap minimo
#define GAP_MIN 330
// incremento do gap entre cima e baixo
#define INC_GAP 30
// velocidade inicial
#define VEL_OBSTACULOS 2
// incremento da velocidade
#define INC_VEL_OBSTACULOS 1
// diferenca de altura minima
#define DIF_ALT 100
// incremento diferenca de altura
#define INC_DIF_MAX_ALT 100
// aceleracao de subida inicial do jogador
#define ACEL_JOG_SUB_INIC 1
// aceleracao de subida maxima do jogador ap�s chegar no score threshold
#define ACEL_JOG_SUB_MAX 2
// aceleracao de caida do jogador
#define ACEL_JOG_CAIDA 1
/// TAMANHOS DE FONTES
// fonte padrao
#define TAM_TEXTO_PADRAO 40
#define TAM_TEXTO_RANKING 50
/// TRUE ou FALSE
#define TRUE 1
#define FALSE 0
///RANKING
#define MAX_RANKING 5
//tamanho caixa ranking
#define TAM_CAIXA_RANKING 1000
///INCREMENTO SCORE
#define INC_SCORE 100
///VELOCIDADE MAXIMA
#define VEL_MAX 8

typedef struct Torre_str
{
    Rectangle recTorre;
    Texture2D textTorreSW;
    Texture2D textTorreHP;
    Vector2 posicao;
    int passou;

} TORRE;

typedef struct Jogador_str
{
    Rectangle recJogador;
    Texture2D textJogSW;
    Texture2D textJogHP;
    Vector2 posicao;
    float rotationJogador;
    int score;

} JOGADOR;

typedef struct obstaculo_str
{
    Rectangle recSoldado;
    Texture2D textSoldadoSW;
    Texture2D textSoldadoHP;
    Vector2 posicao;
    int passou;

} OBSTACULO;
typedef struct Usuario_str{
    char nome[40];
    int score;
} USUARIO;

//DECLARACAO DE VARIAVEIS GLOBAIS
static int letterCount = 0;
static int tela_atual = 0;

///FUNCAO QUE DESENHA O MENU INICIAL
int menuInicial(void)
{
    int tela;
    if(tela_atual == 0){
        ClearBackground(BLACK); //desenha fundo
        BeginDrawing();
        DrawText("STAR-FLAPPY-HARRY", MAX_WIDTH/2 - MeasureText("STAR-FLAPPY-HARRY", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 - 100, TAM_TEXTO_PADRAO, WHITE);
        DrawText("NOVO JOGO (N)", MAX_WIDTH/2 - MeasureText("NOVO JOGO (N)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 + 100, TAM_TEXTO_PADRAO, WHITE);
        DrawText("RANKING (R)", MAX_WIDTH/2 - MeasureText("RANKING (R)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 + 300, TAM_TEXTO_PADRAO, WHITE);
        DrawText("DIFICULDADE (D)", MAX_WIDTH/2 - MeasureText("DIFICULDADE (D)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 + 200, TAM_TEXTO_PADRAO, WHITE);
        DrawText("FECHAR JOGO (E)", MAX_WIDTH/2 - MeasureText("FECHAR JOGO (E)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 + 400, TAM_TEXTO_PADRAO, WHITE);

        if(IsKeyPressed(KEY_N)){ //novo jogo
            tela_atual = 1;
        }else if (IsKeyPressed(KEY_E)){ //fechar sem salvar
            tela_atual = 6;
        } else if (IsKeyPressed(KEY_D)){ //dificuldade
            tela_atual = 4;
        } else if (IsKeyPressed(KEY_R)){ //ranking
            tela_atual = 5;
        }

        EndDrawing();
    }
    return tela = tela_atual;
}

///FUNCAO PARA CARREGAR A TEXTURA DOS JOGADORES
void carregaTextJogador(JOGADOR *jogador){

    ///Textura tema Star Wars ------------------------------------------
    jogador->textJogSW = LoadTexture("sprites/grogu.png");
    ///Textura tema Harry Potter ------------------------------------------
    jogador->textJogHP = LoadTexture("sprites/harry-broomstick-1.png");
}
///FUNCAO PARA INICIALIZAR A POSICAO DO JOGADOR
void jogadorPosicao(JOGADOR *jogador)
{
    // posição do inicial do jogador
    jogador->posicao = (Vector2){POS_INIC_JOG_X, POS_INIC_JOG_Y};
    jogador->recJogador.x = jogador->posicao.x;
    jogador->recJogador.y = jogador->posicao.y;
    jogador->recJogador.height = 2 * (jogador->textJogSW.height);
    jogador->recJogador.width = 2 * (jogador->textJogSW.width);
}
///FUNCAO PARA CARREGAR A TEXTURA DOS OBSTACULOS
void carregaTextObstaculos(TORRE torre[], TORRE torreCima[], OBSTACULO soldados[], Texture2D textSabres[4], Texture2D nave, Texture2D soldado, Texture2D textTorre[4], Texture2D balaco, Texture2D dementador){
    //DECLARACAO DE VARIAVEIS AUXILIARES
    int i = 0, j = 0;
    //colocando as texturas nas variaveis das estruturas
    for (i = 0, j = 0; i < MAX_QTD_OBSTAC; i++, j++)
    {
        if (j > 3)
        {
            j = 0;
        }
        torre[i].textTorreSW = textSabres[j];
        torre[i].textTorreHP = textTorre[j];
        torreCima[i].textTorreSW = soldado;
        torreCima[i].textTorreHP = dementador;
        soldados[i].textSoldadoSW = nave;
        soldados[i].textSoldadoHP = balaco;
    }
}
///FUNCAO PARA INICIALIZAR A POSICAO DOS OBSTACULOS
void posicaoInicialObstaculos(int gap, int inc_gap, int score_threshold, int dif_alt_obstaculos, TORRE torre[], TORRE torreCima[], Vector2 torresPos[], OBSTACULO soldados[], Vector2 soldadoPos[])
{
    //DECLARACAO DE VARIAVEIS AUXILIARES
    int i = 0, j = 0;
    int gapObst = gap;

    //cria as posicoes dos obstaculos
    for (i = 0; i < MAX_QTD_OBSTAC; i++)
    {
        torresPos[i].x = POS_INIC_OBST_X + DIST_X_OBST * i;
        torresPos[i].y = GetRandomValue(MIN_ALT_TORRE, MAX_ALT_TORRE);
        if (gapObst >= GAP_MIN)
        {
            // DIFICULDADE AUMENTANDO DE ACORDO COM O score_threshold
            if (i >= (score_threshold/100) && i % (score_threshold/100) == 0)
            {
                gapObst -= inc_gap;
                torresPos[i].y = torresPos[i].y + gapObst/2;
            }
            else
            {
                torresPos[i].y = torresPos[i].y + gapObst/2;
            }
        }
        else
        {
            torresPos[i].y = torresPos[i].y + gapObst/2;
        }
        torreCima[i].posicao.y = torresPos[i].y - gapObst;
    }
    //coloca as posicoes nas estruturas dos obstaculos (torres)
    for (i = 0; i < MAX_QTD_OBSTAC; i++)
    {
        torre[i].posicao.x = torresPos[i].x;
        torre[i].posicao.y = torresPos[i].y;
        torre[i].recTorre.y = torre[i].posicao.y;
        torre[i].recTorre.x = torre[i].posicao.x;
        torre[i].recTorre.height = 4 * torre[i].textTorreSW.height;
        torre[i].recTorre.width = 4 * torre[i].textTorreSW.width;
        torre[i].passou = FALSE;

        torreCima[i].posicao.x = torresPos[i].x;
        torreCima[i].recTorre.y = torreCima[i].posicao.y;
        torreCima[i].recTorre.x = torreCima[i].posicao.x;
        torreCima[i].recTorre.height = ESCALA_TEXT_DEMENT * (torreCima[i].textTorreHP.height);
        torreCima[i].recTorre.width = ESCALA_TEXT_DEMENT * (torreCima[i].textTorreHP.width);

    }
    //coloca as posicoes nas estruturas dos soldados de acordo com as torres de cima
    for (i = 0; i < MAX_QTD_OBSTAC; i++)
    {
        //cria as posicoes
        soldadoPos[i].x = POS_INIC_OBST_X + DIST_X_OBST * i;
        soldadoPos[i].y = torreCima[i].posicao.y - AJUSTE_POS_Y_DEMENT;
        //coloca as posicoes na estrutura
        soldados[i].posicao.x = soldadoPos[i].x;
        soldados[i].posicao.y = soldadoPos[i].y;
        soldados[i].recSoldado.x = soldadoPos[i].x;
        soldados[i].recSoldado.y = soldadoPos[i].y;
        soldados[i].recSoldado.height = ESCALA_SOLDADO * (soldados[i].textSoldadoSW.height);
        soldados[i].recSoldado.width = ESCALA_SOLDADO * (soldados[i].textSoldadoSW.width);
    }
}

///FUNCAO PARA CRIAR NOVAS POSICOES PARA OS OBSTACULOS
void novaPosicao(int score_threshold, int vel_obstaculos, int inc_vel_obstaculos, TORRE torre[], TORRE torreCima[], Vector2 torresPos[], OBSTACULO soldados[], Vector2 soldadoPos[])
{
    //DECLARACAO DE VARIAVEL AUXILIAR
    int i = 0;

    //cria nova posicao dos obstaculos de acordo com a velocidade e incremento da velocidade lidos no txt
    for (i = 0; i < MAX_QTD_OBSTAC; i++)
    {
        if (vel_obstaculos < VEL_MAX)
        {
            if (i < (score_threshold/100) && i % (score_threshold/100) == 0)
            {
                vel_obstaculos += inc_vel_obstaculos;
                torresPos[i].x -= vel_obstaculos;
                soldadoPos[i].x -= vel_obstaculos * 1.5;
            }
            else
            {
                torresPos[i].x -= vel_obstaculos;
                soldadoPos[i].x -= vel_obstaculos * 1.5;
            }
        }
    }

    //coloca as novas posicoes nas estruturas
    for (i = 0; i < MAX_QTD_OBSTAC; i++)
    {
        torre[i].posicao.x = torresPos[i].x;
        torre[i].recTorre.x = torresPos[i].x;

        torreCima[i].posicao.x = torresPos[i].x;
        torreCima[i].recTorre.x = torresPos[i].x;

        soldados[i].posicao.x = soldadoPos[i].x;
        soldados[i].recSoldado.x = soldadoPos[i].x;
    }
}
///FUNCAO PARA ESCREVER O RANKING NO ARQUIVO BINARIO
int escreveRanking(USUARIO top5[]){
    //DECLARACAO DE VARIAVEL AUXILIAR
    int i = 0;
    //DECLARACAO DO ARQUIVO
    FILE *rankingBin;
    //DECLARACAO DA VARIAVEL DO TIPO FLAG
    int escreveu_arq = FALSE;

    rankingBin = fopen("ranking.bin","rb+");

    if(fwrite(top5, sizeof(USUARIO), MAX_RANKING, rankingBin) != MAX_RANKING){
        printf("Erro na leitura!");
        escreveu_arq = FALSE;
    } else{
        escreveu_arq = TRUE;
        printf("Arquivo escrito com sucesso!\n");
    }
    fclose(rankingBin);

    return escreveu_arq;

}
///FUNCAO PARA LER O ARQUIVO BINARIO
int leRanking(USUARIO top5[]){
    //DECLARACAO DE VARIAVEL AUXILIAR
    int i = 0;
    //DECLARACAO DO ARQUIVO
    FILE *rankingBin;
    //DECLARACAO DA VARIAVEL DO TIPO FLAG
    int leu_arq = FALSE;

    rankingBin = fopen("ranking.bin","rb");
    rewind(rankingBin);
    if(!rankingBin){
        printf("Erro ao abrir arquivo para a leitura");
    } else {
        if(fread(top5, sizeof(USUARIO), MAX_RANKING, rankingBin) != MAX_RANKING){
            printf("Erro na leitura!");
            leu_arq = FALSE;
        } else {
            leu_arq = TRUE;
        }
    }

    fclose(rankingBin);
    return leu_arq;
}

///FUNCAO QUE DESENHA A TELA DE RANKING
void desenhaRanking (USUARIO top5[]){
     //DECLARACAO DE VARIAVEIS AUXILIARES
    int nUsuario = 0;
    //DECLARACAO DE VARIAVEIS DO TIPO FLAG
    int leu_arq = FALSE;

    if(leu_arq = leRanking(top5)){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("TOP 5", MAX_WIDTH / 2 - MeasureText("TOP 5", TAM_TEXTO_PADRAO) / 2, TAM_TEXTO_RANKING, TAM_TEXTO_PADRAO, WHITE);
        for(nUsuario = 0; nUsuario < MAX_RANKING; nUsuario++){
            DrawRectangle((MAX_WIDTH - TAM_CAIXA_RANKING)/2, (100 * nUsuario) + 100, TAM_CAIXA_RANKING, 45, LIGHTGRAY);
            DrawText(top5[nUsuario].nome, (MAX_WIDTH - TAM_CAIXA_RANKING)/2 + 100, (100 * nUsuario) + 100, TAM_TEXTO_RANKING, BLACK);
            DrawText(TextFormat("%04i", top5[nUsuario].score), 900, (100 * nUsuario) + 100, TAM_TEXTO_RANKING, BLACK);
            DrawRectangleLines((MAX_WIDTH - TAM_CAIXA_RANKING)/2, (100 * nUsuario) + 100,  TAM_CAIXA_RANKING, 45, SKYBLUE);
        }
        DrawText("VOLTAR (V)", MAX_WIDTH / 2 - MeasureText("VOLTAR (V)", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT - 100, TAM_TEXTO_PADRAO, WHITE);
        if(IsKeyPressed(KEY_V)){
            tela_atual = 0;
        }
    }

    EndDrawing();
}

///FUNCAO PARA VERIFICAR SE O NOVO SCORE VAI PARA O ARQUIVO BINARIO
int verificaScore(USUARIO *novoUsuario, USUARIO top5[]){
    //DECLARACAO DE VARIAVEIS AUXILIARES
    int i = 0;
    int nUsuario = 0;
    USUARIO temp1 = {0};
    USUARIO temp2 = {0};
    //DECLARACAO DE VARIAVEIS DO TIPO FLAG
    int leu_arq = FALSE;
    int achou = FALSE;
    int novoRecord = FALSE;

    if(leu_arq = leRanking(top5)){
        do{
            if(top5[nUsuario].score <= novoUsuario->score){
                if(top5[0].score <= novoUsuario->score){
                    novoRecord = TRUE;
                }
                achou = TRUE;
            }
            nUsuario++;
        } while(!achou && nUsuario < MAX_RANKING);
        if(achou){
            nUsuario--;
            if(nUsuario < (MAX_RANKING-1)){
                temp1 = top5[nUsuario];
                top5[nUsuario] = *novoUsuario;
            } else {
                top5[nUsuario] = *novoUsuario;
            }
            if(nUsuario < (MAX_RANKING-1)){
                for(i = nUsuario+1; i < (MAX_RANKING-1); i++){
                    temp2 = top5[i];
                    top5[i] = temp1;
                    temp1 = top5[i+1];
                }
            }
        }
    }
    return novoRecord;
}

///FUNCAO QUE DESENHA A CAIXA QUE PEDE O NOME DO USUARIO
void desenhaCaixaNome(void){
    Rectangle textBox = {MAX_WIDTH/2 - (300/2), 180, 300, TAM_TEXTO_RANKING};

    ClearBackground(BLACK);
    DrawText("DIGITE SEU NOME", MAX_WIDTH/2 - MeasureText("DIGITE SEU NOME", TAM_TEXTO_PADRAO)/2, 100, TAM_TEXTO_PADRAO, WHITE);
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, SKYBLUE);

}

///FUNCAO QUE PEGA O NOME DO USUARIO
int pegaNomeUsuario (USUARIO *novoUsuario){
    int terminou = FALSE;
    Rectangle textBox = {MAX_WIDTH/2.0f - (300/2) + 10, 190, 300, TAM_TEXTO_RANKING };
    int tecla = 0;
    BeginDrawing();
    desenhaCaixaNome();
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    tecla = GetCharPressed();
    while (tecla > 0)
    {
        if ((tecla > 32) && (tecla <= 125) && (letterCount < 10))
        {
            novoUsuario->nome[letterCount] = (char) tecla;
            novoUsuario->nome[letterCount+1] = '\0';
            letterCount++;
        }

        tecla = GetCharPressed();  // verfica proxima caractere

    }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0){
                letterCount = 0;
            }
            novoUsuario->nome[letterCount] = '\0';
        }
        if(IsKeyPressed(KEY_ENTER)){
            terminou = TRUE;
        }
        DrawText(novoUsuario->nome, textBox.x + 2 , textBox.y + 2, TAM_TEXTO_PADRAO, DARKBLUE);
        DrawText(TextFormat("NOME: %i/%i", letterCount, 10), 315, 250, 20, DARKGRAY);
        DrawText("APERTE ENTER PARA CONTINUAR", MAX_WIDTH / 2 - TAM_TEXTO_PADRAO - MeasureText("APERTE ENTER PARA CONTINUAR", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT / 2 - 100, TAM_TEXTO_PADRAO, WHITE);

    EndDrawing();
    return terminou;
}

///FUNCAO QUE DESENHA A TELA DE GAME OVER
void desenhaGameOver(void){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("GAME OVER!", MAX_WIDTH / 2 - MeasureText("GAME OVER!", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT / 2 - TAM_TEXTO_PADRAO, TAM_TEXTO_PADRAO, BLACK);
        DrawText("APERTE ENTER PARA VOLTAR", MAX_WIDTH / 2 - TAM_TEXTO_PADRAO - MeasureText("APERTE ENTER PARA VOLTAR", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT / 2 - 100, TAM_TEXTO_PADRAO, BLACK);
    EndDrawing();
}

///FUNCOA PARA DESENHAR TELA DE NOVO RECORD
void desenhaNovoRecord (USUARIO *novoUsuario){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("NOVO RECORD!", MAX_WIDTH / 2 - MeasureText("NOVO RECORD!", 60) / 2, MAX_HEIGHT / 2 - 200, 60, BLACK);
        DrawText(TextFormat("%04i", novoUsuario->score), MAX_WIDTH / 2 - MeasureText(TextFormat("%04i", novoUsuario->score), 60) / 2, MAX_HEIGHT / 2 - 100 , 60, BLACK);
        DrawText("APERTE ENTER PARA VOLTAR", MAX_WIDTH / 2 - MeasureText("APERTE ENTER PARA VOLTAR", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT / 2 + 100, TAM_TEXTO_PADRAO, BLACK);
    EndDrawing();
}

///FUNCAO PARA INICIAR O JOGO (CHAMA AS FUNCOES QUE CRIAM AS POSICOES NOVAS E ZERA O ESTADO DO JOGO)
void iniciaJogo(int gap, int inc_gap, int score_threshold, int dif_alt_obstaculos, USUARIO *novoUsuario, JOGADOR *jogador, TORRE torre[], TORRE torreCima[], Vector2 torresPos[], OBSTACULO soldados[], Vector2 soldadoPos[], int *game_over)
{
    posicaoInicialObstaculos(gap, inc_gap, score_threshold, dif_alt_obstaculos, torre, torreCima, torresPos, soldados, soldadoPos);
    jogadorPosicao(jogador);
    novoUsuario->score = 0;
    novoUsuario->nome[0] = '\0';
    letterCount = 0;
    *game_over = FALSE;
}

///FUNCAO PARA VERIFICAR SE HOUVE COLISAO
int verificaColisao(int score_threshold, int gap, int inc_gap, int vel_obstaculos, int inc_vel_obstaculos, int dif_max_altura, int inc_dif_max_altura, USUARIO *novoUsuario, USUARIO top5[5], JOGADOR *jogador, TORRE torre[], TORRE torreCima[], Vector2 torresPos[], OBSTACULO soldados[], Vector2 soldadoPos[], int *game_over, int tela_atual)
{
    //DECLARACAO DE VARIAVEIS AUXILIARES
    int i = 0;
    //DECLARACAO DE VARIAVEIS TIPO FLAG
    int novoRecord = FALSE;
    int terminou = FALSE;
    int escreveu_arq = FALSE;
    int inicio = FALSE;
    int tela = tela_atual;

    for (i = 0; i < MAX_QTD_OBSTAC; i++)                            //para a quantidade maxima de obstaculos
    {
        if (CheckCollisionRecs(jogador->recJogador, torre[i].recTorre) ||
            CheckCollisionRecs(jogador->recJogador, torreCima[i].recTorre) ||
            CheckCollisionRecs(jogador->recJogador, soldados[i].recSoldado))
        {                                                                        //se houver colisao do jogador com qualquer um dos obstaculos
            *game_over = TRUE;                                                  //game over
            while(!terminou){
                terminou = pegaNomeUsuario(novoUsuario);  //pega o nome do usuario
            }
            novoRecord = verificaScore(novoUsuario, top5); //verifica se teve novo record
            escreveu_arq = escreveRanking(top5);           //escreve o arquivo ranking
            while(!inicio){                                //enquanto a tecla enter nao eh pressionada
                if(IsKeyPressed(KEY_ENTER)){
                    inicio = TRUE;
                    tela = 0;
                } else {
                    if(novoRecord){
                        desenhaNovoRecord(novoUsuario);   //se tiver um novo record, desenha a tela de novo record
                    } else {
                        desenhaGameOver();                //senao, desenha a tela comum de game over
                    }
                }
            }
        }
        else if ((torresPos[i].x < jogador->posicao.x) && !torre[i].passou && !(*game_over)) //se nao houve colisao e o jogador passou pelo obstaculo inferior
        {
            novoUsuario->score += INC_SCORE;                                                 //aumenta a pontuacao em 100
            torre[i].passou = TRUE;
            tela = tela_atual;                                                               //mantem na tela do jogo
        }
    }
    return tela;
}

///FUNCAO QUE ATUALIZA O FRAME DO JOGO
int atualizaFrame(int score_threshold, int gap, int inc_gap, int vel_obstaculos, int inc_vel_obstaculos, int dif_max_altura, int inc_dif_max_altura, USUARIO *novoUsuario, USUARIO top5[5], JOGADOR *jogador, TORRE torre[], TORRE torreCima[], Vector2 torresPos[], OBSTACULO soldados[], Vector2 soldadoPos[], int *game_over, int tela_atual)
{
    //DECLARACAO DE VARIAVEIS DO TIPO FLAG
    int escreveu_arq = FALSE;
    int cria_novo = FALSE;
    int terminou = FALSE;
    int inicio = FALSE;
    int novoRecord = FALSE;
    int tela = tela_atual;

    //se não deu game over
    if (!(*game_over))
    {
        novaPosicao(score_threshold, vel_obstaculos, inc_vel_obstaculos, torre, torreCima, torresPos, soldados, soldadoPos); //cria nova posicao para os obstaculos
        if (IsKeyDown(KEY_SPACE) && !(*game_over)) //se a tecla espaco esta sendo pressionada e nao deu game over
        {
            jogador->posicao.y -= 1;               //posicao do jogador eh do seu retangulo sobe 1px
            jogador->recJogador.y -= 1;
        }

        else if (jogador->posicao.y <= 0)          //se a posicao do jogador eh menor ou igual a zero
        {
            *game_over = TRUE;                     //game over
            while(!terminou){
                terminou = pegaNomeUsuario(novoUsuario);  //pega o nome do usuario
            }
            novoRecord = verificaScore(novoUsuario, top5); //verifica se teve novo record
            escreveu_arq = escreveRanking(top5);           //escreve o arquivo ranking
            while(!inicio){                                //enquanto a tecla enter nao eh pressionada
                if(IsKeyPressed(KEY_ENTER)){
                    inicio = TRUE;
                    tela = 0;
                } else {
                    if(novoRecord){
                        desenhaNovoRecord(novoUsuario);   //se tiver um novo record, desenha a tela de novo record
                    } else {
                        desenhaGameOver();                //senao, desenha a tela comum de game over
                    }
                }
            }
        }
        else                                            //se a tecla espaco nao esta sendo pressionada
        {
            jogador->posicao.y += 1;                    //posicao do jogador e seu retangulo aumenta em 1px
            jogador->recJogador.y += 1;
        }
        //verifica se houve colisao
        tela = verificaColisao(score_threshold, gap, inc_gap, vel_obstaculos, inc_vel_obstaculos, dif_max_altura, inc_dif_max_altura, novoUsuario, top5, jogador, torre, torreCima, torresPos, soldados, soldadoPos, game_over, tela_atual);
    }
    return tela;
}

///FUNCAO QUE DESEJA O JOGO COM O TEMA DE STAR WARS
void desenhaJogoSW(Texture2D backgroundStarWars, USUARIO *novoUsuario, JOGADOR *jogador, TORRE torre[], TORRE torreCima[], OBSTACULO soldados[], int *game_over)
{
    //DECLARACAO DE VARIAVEL AUXILIAR
    int i = 0;

    BeginDrawing();

    // ClearBackground(RAYWHITE);

    if (!(*game_over))
    {

        DrawTexture(backgroundStarWars, 0, 0, WHITE);
        DrawTextureEx(jogador->textJogSW, jogador->posicao, 0, 2, WHITE);

        for (i = 0; i < MAX_QTD_OBSTAC; i++)
        {
            DrawTextureEx(torre[i].textTorreSW, torre[i].posicao, 0, 4, WHITE);
            DrawTextureEx(soldados[i].textSoldadoSW, soldados[i].posicao, 0, 2.5, WHITE);
            DrawTextureEx(torreCima[i].textTorreSW, torreCima[i].posicao, 0, 4, WHITE);
        }
    }

    DrawText(TextFormat("%04i", novoUsuario->score), 20, 20, TAM_TEXTO_PADRAO, WHITE);

    EndDrawing();
}

///FUNCAO QUE DESEJA O JOGO COM O TEMA DE HARRY POTTER
void desenhaJogoHP(Texture2D backgroundHP, USUARIO *novoUsuario, JOGADOR *jogador, TORRE torre[], TORRE torreCima[], OBSTACULO soldados[], int *game_over)
{
    //DECLARACAO DE VARIAVEL AUXILIAR
    int i = 0;

    BeginDrawing();
    if (!(*game_over))
    {

        DrawTexture(backgroundHP, 0, 0, WHITE);
        DrawTextureEx(jogador->textJogHP, jogador->posicao, 0, ESCALA_TEXT_JOG, WHITE);

        for (i = 0; i < MAX_QTD_OBSTAC; i++)
        {
            DrawTextureEx(torre[i].textTorreHP, torre[i].posicao, 0, ESCALA_TEXT_TORRE, WHITE);
            DrawTextureEx(soldados[i].textSoldadoHP, soldados[i].posicao, 0, 1, WHITE);
            DrawTextureEx(torreCima[i].textTorreHP, torreCima[i].posicao, 0, ESCALA_TEXT_DEMENT, WHITE);
        }
    }

    DrawText(TextFormat("%04i", novoUsuario->score), 20, 20, TAM_TEXTO_PADRAO, WHITE);
    EndDrawing();
}


///FUNCAO QUE DESENHA O MENU PARA ESCOLHA DO TEMA
void desenhaMenuTema (Texture2D starW, Texture2D harryP){
    Vector2 posStar = {MAX_WIDTH/4 + starW.width - MeasureText("STAR FLAPPY (S)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/4 - starW.height/2};
    Vector2 posHarry = {MAX_WIDTH/3 + harryP.width + MeasureText("FLAPPY HARRY (H)", TAM_TEXTO_PADRAO)/2 + 100, MAX_HEIGHT/4 - harryP.height/2};
    ClearBackground(BLACK); //desenha menu
    BeginDrawing();
    DrawTextureEx(starW,posStar, 0, 2, WHITE);
    DrawText("STAR FLAPPY (S)", MAX_WIDTH/4 - MeasureText("STAR FLAPPY (S)", TAM_TEXTO_PADRAO)/2, MAX_HEIGHT/2, TAM_TEXTO_PADRAO, WHITE);
    DrawTextureEx(harryP,posHarry, 0, 2, WHITE);
    DrawText("FLAPPY HARRY (H)", MAX_WIDTH/3 + MeasureText("FLAPPY HARRY (H)", TAM_TEXTO_PADRAO)/2 + 80, MAX_HEIGHT/2, TAM_TEXTO_PADRAO, WHITE);
    DrawText("VOLTAR (V)", MAX_WIDTH / 2 - MeasureText("VOLTAR (V)", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT - 100, TAM_TEXTO_PADRAO, WHITE);

    if(IsKeyPressed(KEY_V)){
            tela_atual = 0;
    }
    if(IsKeyPressed(KEY_S)){ //novo jogo
        tela_atual = 2;
    } else if(IsKeyPressed(KEY_H)){
        tela_atual = 3;
    }
    EndDrawing();
}

///FUNCAO QUE DESENHA A TELA PARA A ESCOLHA DE DIFICULDADE E CARREGA O ARQUIVO DE ACORDO
void desenhaMenuDificuldade(int *score_threshold, int *gap, int *inc_gap, int *dif_max_altura, int *inc_dif_max_altura, int *vel_obstaculos, int *inc_vel_obstaculos){

    int comeca = FALSE;
    FILE *arq_dificil;
    FILE *arq_facil;

    //ABRE OS ARQUIVOS PRA LEITURA
    arq_dificil = fopen("dificuldade2.txt","r");
    arq_facil = fopen("dificuldade1.txt", "r");

    //SE UM DOS DOIS FOR DIFIRENTE ERRO
    if(!arq_dificil ||  !arq_facil){
        printf("Erro ao abrir arquivo");
    } else {
        ClearBackground(BLACK); //desenha menu
        BeginDrawing();
        if(IsKeyPressed(KEY_I)){
            DrawText("DIFICIL (I) [OK]", MAX_WIDTH/2 - MeasureText("DIFICIL (I) [OK]", TAM_TEXTO_PADRAO)/2 , MAX_HEIGHT/4 + 200, TAM_TEXTO_PADRAO, WHITE);
            while(!feof(arq_dificil)){
                fscanf(arq_dificil, "%d %d %d %d %d %d %d", score_threshold, gap, inc_gap, dif_max_altura, inc_dif_max_altura, vel_obstaculos, inc_vel_obstaculos);
            }
            tela_atual = 1;
        } else {
            DrawText("DIFICIL (I)", MAX_WIDTH/2 - MeasureText("DIFICIL (I)", TAM_TEXTO_PADRAO)/2 , MAX_HEIGHT/4 + 200, TAM_TEXTO_PADRAO, WHITE);
        }
        if(IsKeyPressed(KEY_F)){
            DrawText("FACIL (F) [OK]", MAX_WIDTH/2 - MeasureText("FACIL (F) [OK]", TAM_TEXTO_PADRAO)/2 , MAX_HEIGHT/4 + 100, TAM_TEXTO_PADRAO, WHITE);
            while(!feof(arq_facil)){
                fscanf(arq_facil, "%d %d %d %d %d %d %d", score_threshold, gap, inc_gap, dif_max_altura, inc_dif_max_altura, vel_obstaculos, inc_vel_obstaculos);
            }
            tela_atual = 1;
        } else {
            DrawText("FACIL (F)", MAX_WIDTH/2 - MeasureText("FACIL (F)", TAM_TEXTO_PADRAO)/2 , MAX_HEIGHT/4 + 100, TAM_TEXTO_PADRAO, WHITE);
        }
        DrawText("VOLTAR (V)", MAX_WIDTH / 2 - MeasureText("VOLTAR (V)", TAM_TEXTO_PADRAO) / 2, MAX_HEIGHT - 100, TAM_TEXTO_PADRAO, WHITE);
        if(IsKeyPressed(KEY_V)){
            tela_atual = 0;
        }
        EndDrawing();
    }
    fclose(arq_dificil);
    fclose(arq_facil);

}
///FUNCAO PARA DESCARREGAR AS TEXTURAS
void unloadGame(JOGADOR *jogador, Texture2D sabre, Texture2D sabre1, Texture2D sabre2, Texture2D sabre3, Texture2D nave, Texture2D soldado, Texture2D torre, Texture2D torre1, Texture2D torre2, Texture2D torre3, Texture2D balaco, Texture2D dementador, Texture2D fundoSW, Texture2D fundoHP, Texture2D starW, Texture2D harryP){

    UnloadTexture(jogador->textJogHP);
    UnloadTexture(jogador->textJogSW);

    UnloadTexture(sabre);
    UnloadTexture(sabre1);
    UnloadTexture(sabre2);
    UnloadTexture(sabre3);

    UnloadTexture(torre);
    UnloadTexture(torre1);
    UnloadTexture(torre2);
    UnloadTexture(torre3);

    UnloadTexture(soldado);
    UnloadTexture(nave);
    UnloadTexture(balaco);
    UnloadTexture(dementador);

    UnloadTexture(fundoSW);
    UnloadTexture(fundoHP);

    UnloadTexture(starW);
    UnloadTexture(harryP);
}

int main(void)
{
    int tela;
    //DECLARACAO DE VARIAVEIS DE DIFICULDADE
    int score_threshold = SCORE_THRESHOLD;
    int gap = GAP_MIN, inc_gap = INC_GAP;
    int dif_max_altura = DIF_ALT, inc_dif_max_altura = INC_DIF_MAX_ALT;
    int vel_obstaculos = VEL_OBSTACULOS, inc_vel_obstaculos = INC_VEL_OBSTACULOS;
    //DECLARACAO DE VARIAVEIS DO TIPO FLAG
    int comeca = FALSE;
    int terminou = FALSE;
    int sair = FALSE;
    int game_over = FALSE;
    //DECLARACAO DE ESTRUTURAS
    USUARIO top5[MAX_RANKING] = {0};
    USUARIO novoUsuario = {0};
    JOGADOR jogador = {0};
    TORRE torre[MAX_QTD_OBSTAC] = {0};
    TORRE torreCima[MAX_QTD_OBSTAC] = {0};
    OBSTACULO soldados[MAX_QTD_OBSTAC] = {0};
    //DECLARACAO VETOR POSICAO DAS ESTRUTURAS
    Vector2 torresPos[MAX_QTD_OBSTAC] = {0};
    Vector2 soldadoPos[MAX_QTD_OBSTAC] = {0};
    //DECLARACAO DE TEXTURAS
    ///Texturas tema Star Wars ------------------------------------------
    //textura do fundo
    Texture2D backgroundStarWars;
    //textura dos sabres
    Texture2D sabre;
    Texture2D sabre1;
    Texture2D sabre2;
    Texture2D sabre3;

    Texture2D textSabres[4] = {0};
    //textura nave
    Texture2D nave;
    //textura soldado
    Texture2D soldado;
    //textura titulo
    Texture2D starW;

    ///Texturas tema Harry Potter ------------------------------------------
    //textura do fundo
    Texture2D backgroundHP;
    //textura torres
    Texture2D torrelufa;
    Texture2D torrecorvinal;
    Texture2D torregrifinoria;
    Texture2D torresonserina;

    Texture2D textTorre[4] = {0};
    //textura balaco
    Texture2D balaco;
    //textura dementador
    Texture2D dementador;
    //textura titulo
    Texture2D harryP;


    InitWindow(MAX_WIDTH, MAX_HEIGHT, "Star-flappy-Harry");                                       //Abre a janela do jogo

    SetTargetFPS(60);  //Coloca o fps do jogo em 60
    //Carregando texturas
    ///Texturas tema Star Wars ------------------------------------------
    //textura dos sabres
    sabre = LoadTexture("sprites/sabre.png");
    sabre1 = LoadTexture("sprites/sabre1.png");
    sabre2 = LoadTexture("sprites/sabre2.png");
    sabre3 = LoadTexture("sprites/sabre3.png");

    textSabres[0] = sabre;
    textSabres[1] = sabre1;
    textSabres[2] = sabre2;
    textSabres[3] = sabre3;
    //textura nave
    nave = LoadTexture("sprites/nave.png");
    //textura soldado
    soldado = LoadTexture("sprites/soldado.png");
    //textura do fundo do tema Star Wars
    backgroundStarWars = LoadTexture("sprites/fundo.png");
    //textura titulo
    starW = LoadTexture("sprites/star-w.png");
    ///Texturas tema Harry Potter ------------------------------------------
    //textura torres
    torrelufa = LoadTexture("sprites/torre-lufalufa.png");
    torrecorvinal = LoadTexture("sprites/torre-corvinal.png");
    torregrifinoria = LoadTexture("sprites/torre-grifinoria.png");
    torresonserina = LoadTexture("sprites/torre-sonserina.png");

    textTorre[0] = torrelufa;
    textTorre[1] = torrecorvinal;
    textTorre[2] = torregrifinoria;
    textTorre[3] = torresonserina;
    //textura balaco
    balaco = LoadTexture("sprites/balaco.png");
    //textura dementador
    dementador = LoadTexture("sprites/dementador.png");
    //textura do fundo do tema Harry Potter
    backgroundHP = LoadTexture("sprites/hogwarts1.png");
    //textura titulo
    harryP = LoadTexture("sprites/harry-p.png");

    carregaTextObstaculos(torre, torreCima, soldados, textSabres, nave, soldado, textTorre, balaco, dementador);    //Carrega as texturas dos obstaculos
    carregaTextJogador(&jogador);                                                                                   //Carrega as texturas dos jogadores

    //----------------------------------------------------------------
    // LOOP PRINCIPAL DO JOGO
    while (!WindowShouldClose())
    {
        while(tela_atual != 6){
            //verifica escolha no menu
           tela = menuInicial();
            switch(tela_atual){
                case 0: iniciaJogo(gap, inc_gap, score_threshold, dif_max_altura, &novoUsuario, &jogador, torre, torreCima, torresPos, soldados, soldadoPos, &game_over);
                        tela = menuInicial();
                        if(tela == 6){
                            unloadGame(&jogador,sabre1,sabre2,sabre3,sabre,nave,soldado,torrecorvinal,torregrifinoria,torrelufa,torresonserina,balaco,dementador,backgroundHP,backgroundStarWars, starW, harryP);
                            CloseWindow();
                        }
                        break;
                case 1: desenhaMenuTema(starW, harryP);
                        break;
                case 2: desenhaJogoSW(backgroundStarWars, &novoUsuario, &jogador, torre, torreCima, soldados, &game_over);
                        tela_atual = atualizaFrame(score_threshold, gap, inc_gap, vel_obstaculos, inc_vel_obstaculos, dif_max_altura, inc_dif_max_altura, &novoUsuario, top5, &jogador, torre, torreCima, torresPos, soldados, soldadoPos, &game_over, tela_atual);
                        break;
                case 3: desenhaJogoHP(backgroundHP, &novoUsuario, &jogador, torre, torreCima, soldados, &game_over);
                        tela_atual = atualizaFrame(score_threshold, gap, inc_gap, vel_obstaculos, inc_vel_obstaculos, dif_max_altura, inc_dif_max_altura, &novoUsuario, top5, &jogador, torre, torreCima, torresPos, soldados, soldadoPos, &game_over, tela_atual);
                        break;
                case 4: desenhaMenuDificuldade(&score_threshold, &gap, &inc_gap, &dif_max_altura, &inc_dif_max_altura, &vel_obstaculos, &inc_vel_obstaculos);
                        break;
                case 5: desenhaRanking(top5);
                        break;
                case 6: unloadGame(&jogador,sabre1,sabre2,sabre3,sabre,nave,soldado,torrecorvinal,torregrifinoria,torrelufa,torresonserina,balaco,dementador,backgroundHP,backgroundStarWars, starW, harryP);
                        CloseWindow();
                        printf("Sair!");
                        break;
            }
        }
    }
    //------------------------------------------------------------------------------


    return 0;
}
