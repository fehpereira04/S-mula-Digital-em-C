#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JOGADORES 1000
#define MAX_ARBITROS 50
#define MAX_TIMES 50
#define MAX_PARTIDAS 500

#define MAX_TECNICOS 50
#define MAX_ESTADIOS 50
#define MAX_TRANSMISSOES 50
#define MAX_COMPETICOES 20
#define MAX_VAR_ENTRIES 200
#define MAX_TRANSFERENCIAS 200
#define MAX_RESPONSAVEIS 200

#define MAX_LINEUP 18
#define MAX_SUBS_PER_MATCH 12

#define STRLEN 100
#define MAX_RESP_PER_TRANSM 6

#define MAX_EVENTOS 200

typedef struct {
    int id;
    char nome[STRLEN];
    int idade;
    char nacionalidade[STRLEN];
    int numero_camisa;
    char posicao[30];
    int gols;
    int assistencias;
    int amarelos;
    int vermelhos;
} Jogador;

typedef struct {
    int id;
    char nome[STRLEN];
    int idade;
    char estado[50];
    char federacao[50];
    int partidas;
    int faltas_marcadas;
    int var_intervencoes;
    int cartoes_aplicados;
} Arbitro;

typedef struct {
    int id;
    char nome[STRLEN];
    char estado[50];
    int tecnico_id;
    int titulos;
    int pontuacao;
    int posicao;
    int saldo_gols;
    int vitorias;
    int gols_marcados;
    int gols_sofridos;
    int jogos_casa;
    int vitorias_casa;
    int jogos_fora;
    int vitorias_fora;
    double dividas;
} Time;

typedef struct {
    int id;
    char nome[STRLEN];
    char nacionalidade[STRLEN];
    int idade;
    char esquema_tatico[50];
    double aproveitamento;
} Tecnico;

typedef struct {
    int id;
    char nome[STRLEN];
    char estado[50];
    char cidade[50];
    int lotacao;
    char inauguracao[30];
    char expansao[100];
} Estadio;

typedef struct {
    int id;
    char tipo[50];
    char nome[STRLEN];
    char local[STRLEN];
    char horario[30];
    char permissoes[100];
    int responsaveis_count;
    int responsaveis_ids[MAX_RESP_PER_TRANSM];
} Transmissao;

typedef struct {
    int id;
    char nome[STRLEN];
    char profissao[50];
    char emissora[50];
    int idade;
} ResponsavelTransmissao;

typedef struct {
    int id;
    char tipo[50];
    int qtd_equipes;
    char calendario[200];
    char atual_campeao[STRLEN];
    char maior_campeao[STRLEN];
} Competicao;

typedef struct {
    int id;
    int partida_id;
    int chamadas;
    int interferencias;
    int minuto;
    char tempo_revisao[30];
    char conclusao[200];
    char motivo[200];
} VarEntry;

typedef struct {
    int id;
    int jogador_id;
    char tipo[50];
    double valor;
    double rescisao;
} Transferencia;

typedef struct {
    int minuto;
    int time_id;
    int jogador_out_id;
    int jogador_in_id;
} Substituicao;

typedef struct {
    int id;
    int time_casa_id;
    int time_fora_id;
    int estadio_id;
    int transmissao_id;
    int competicao_id;
    int arbitro_id;
    char clima[50];

    int gols_casa;
    int gols_fora;

    int gols_casa_count;
    int gols_casa_scorers[MAX_EVENTOS];
    int gols_casa_assists[MAX_EVENTOS];

    int gols_fora_count;
    int gols_fora_scorers[MAX_EVENTOS];
    int gols_fora_assists[MAX_EVENTOS];

    int amarelos_casa_count;
    int amarelos_casa_ids[MAX_EVENTOS];
    int amarelos_fora_count;
    int amarelos_fora_ids[MAX_EVENTOS];

    int vermelhos_casa_count;
    int vermelhos_casa_ids[MAX_EVENTOS];
    int vermelhos_fora_count;
    int vermelhos_fora_ids[MAX_EVENTOS];

    int faltas_casa;
    int faltas_fora;

    int cartoes_amarelos_casa;
    int cartoes_amarelos_fora;
    int cartoes_vermelhos_casa;
    int cartoes_vermelhos_fora;

    int esc_casa_count;
    int esc_fora_count;
    int esc_casa[MAX_LINEUP];
    int esc_fora[MAX_LINEUP];

    int subs_count;
    Substituicao subs[MAX_SUBS_PER_MATCH];

    int var_chamadas;

    int teve_prorrogacao;
    int teve_penaltis;
    int penaltis_casa;
    int penaltis_fora;
    int penaltis_casa_scorers_count;
    int penaltis_fora_scorers_count;
    int penaltis_casa_scorers[11];
    int penaltis_fora_scorers[11];
} Partida;

static Jogador jogadores[MAX_JOGADORES];
static Arbitro arbitros[MAX_ARBITROS];
static Time times[MAX_TIMES];
static Partida partidas[MAX_PARTIDAS];

static Tecnico tecnicos[MAX_TECNICOS];
static Estadio estadios[MAX_ESTADIOS];
static Transmissao transmissoes[MAX_TRANSMISSOES];
static Competicao competicoes[MAX_COMPETICOES];
static VarEntry var_entries[MAX_VAR_ENTRIES];
static Transferencia transferencias[MAX_TRANSFERENCIAS];
static ResponsavelTransmissao responsaveis[MAX_RESPONSAVEIS];

static int n_jogadores = 0, n_arbitros = 0, n_times = 0, n_partidas = 0;
static int n_tecnicos = 0, n_estadios = 0, n_transmissoes = 0, n_competicoes = 0;
static int n_var = 0, n_transf = 0, n_responsaveis = 0;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler_string(const char *prompt, char *dest, int tamanho) {
    printf("%s", prompt);
    if (fgets(dest, tamanho, stdin)) {
        size_t len = strlen(dest);
        if (len > 0 && dest[len-1] == '\n') dest[len-1] = '\0';
    } else {
        limpar_buffer();
        dest[0] = '\0';
    }
}

int ler_inteiro(const char *prompt) {
    int v;
    printf("%s", prompt);
    while (scanf("%d", &v) != 1) {
        printf("Entrada inválida. Tente novamente: ");
        limpar_buffer();
    }
    limpar_buffer();
    return v;
}

double ler_double(const char *prompt) {
    double v;
    printf("%s", prompt);
    while (scanf("%lf", &v) != 1) {
        printf("Entrada inválida. Tente novamente: ");
        limpar_buffer();
    }
    limpar_buffer();
    return v;
}

void cadastrar_responsavel() {
    if (n_responsaveis >= MAX_RESPONSAVEIS) { printf("Limite de responsáveis atingido.\n"); return; }
    ResponsavelTransmissao *r = &responsaveis[n_responsaveis];
    r->id = n_responsaveis + 1;
    ler_string("Nome do responsável: ", r->nome, STRLEN);
    ler_string("Profissão: ", r->profissao, 50);
    ler_string("Emissora: ", r->emissora, 50);
    r->idade = ler_inteiro("Idade: ");
    n_responsaveis++;
    printf("Responsável cadastrado com ID %d\n", r->id);
}

void listar_responsaveis() {
    int any = 0;
    for (int i = 0; i < n_responsaveis; ++i) {
        if (responsaveis[i].id <= 0) continue;
        if (!any) { printf("--- Responsáveis na Transmissão ---\n"); any = 1; }
        ResponsavelTransmissao *r = &responsaveis[i];
        printf("ID:%d | %s | %s | %s | Idade:%d\n", r->id, r->nome, r->profissao, r->emissora, r->idade);
    }
    if (!any) printf("Nenhum responsável cadastrado.\n");
}

void cadastrar_jogador() {
    if (n_jogadores >= MAX_JOGADORES) { printf("Limite de jogadores atingido.\n"); return; }
    Jogador *j = &jogadores[n_jogadores];
    j->id = n_jogadores + 1;
    ler_string("Nome: ", j->nome, STRLEN);
    j->idade = ler_inteiro("Idade: ");
    ler_string("Nacionalidade: ", j->nacionalidade, STRLEN);
    j->numero_camisa = ler_inteiro("Numero da camisa: ");
    ler_string("Posição: ", j->posicao, 30);
    j->gols = 0; j->assistencias = 0; j->amarelos = 0; j->vermelhos = 0;
    n_jogadores++;
    printf("Jogador cadastrado com ID %d\n", j->id);
}

void listar_jogadores() {
    int any = 0;
    for (int i = 0; i < n_jogadores; ++i) {
        if (jogadores[i].id <= 0) continue;
        if (!any) { printf("--- Jogadores ---\n"); any = 1; }
        Jogador *j = &jogadores[i];
        printf("ID:%d | %s | %s | Camisa:%d | Pos:%s | Gols:%d | Assts:%d | A:%d | V:%d\n",
               j->id, j->nome, j->nacionalidade, j->numero_camisa, j->posicao,
               j->gols, j->assistencias, j->amarelos, j->vermelhos);
    }
    if (!any) printf("Nenhum jogador cadastrado.\n");
}

void cadastrar_arbitro() {
    if (n_arbitros >= MAX_ARBITROS) { printf("Limite de árbitros atingido.\n"); return; }
    Arbitro *a = &arbitros[n_arbitros];
    a->id = n_arbitros + 1;
    ler_string("Nome: ", a->nome, STRLEN);
    a->idade = ler_inteiro("Idade: ");
    ler_string("Estado: ", a->estado, 50);
    ler_string("Federação: ", a->federacao, 50);
    a->partidas = 0; a->faltas_marcadas = 0; a->var_intervencoes = 0; a->cartoes_aplicados = 0;
    n_arbitros++;
    printf("Árbitro cadastrado com ID %d\n", a->id);
}

void listar_arbitros() {
    int any = 0;
    for (int i = 0; i < n_arbitros; ++i) {
        if (arbitros[i].id <= 0) continue;
        if (!any) { printf("--- Árbitros ---\n"); any = 1; }
        Arbitro *a = &arbitros[i];
        printf("ID:%d | %s | %s | Fed:%s | Partidas:%d | Cartões:%d\n",
               a->id, a->nome, a->estado, a->federacao, a->partidas, a->cartoes_aplicados);
    }
    if (!any) printf("Nenhum árbitro cadastrado.\n");
}

void cadastrar_tecnico() {
    if (n_tecnicos >= MAX_TECNICOS) { printf("Limite de técnicos atingido.\n"); return; }
    Tecnico *t = &tecnicos[n_tecnicos];
    t->id = n_tecnicos + 1;
    ler_string("Nome do tecnico: ", t->nome, STRLEN);
    ler_string("Nacionalidade: ", t->nacionalidade, STRLEN);
    t->idade = ler_inteiro("Idade: ");
    ler_string("Esquema tático: ", t->esquema_tatico, 50);
    t->aproveitamento = 0.0;
    n_tecnicos++;
    printf("Técnico cadastrado com ID %d\n", t->id);
}

void listar_tecnicos() {
    int any = 0;
    for (int i = 0; i < n_tecnicos; ++i) {
        if (tecnicos[i].id <= 0) continue;
        if (!any) { printf("--- Técnicos ---\n"); any = 1; }
        Tecnico *t = &tecnicos[i];
        printf("ID:%d | %s | %s | Idade:%d | Esquema:%s | Apr: %.2f\n",
               t->id, t->nome, t->nacionalidade, t->idade, t->esquema_tatico, t->aproveitamento);
    }
    if (!any) printf("Nenhum técnico cadastrado.\n");
}

void cadastrar_estadio() {
    if (n_estadios >= MAX_ESTADIOS) { printf("Limite de estadios atingido.\n"); return; }
    Estadio *e = &estadios[n_estadios];
    e->id = n_estadios + 1;
    ler_string("Nome do estadio: ", e->nome, STRLEN);
    ler_string("Estado: ", e->estado, 50);
    ler_string("Cidade: ", e->cidade, 50);
    e->lotacao = ler_inteiro("Numero de lugares: ");
    ler_string("Inauguração (data): ", e->inauguracao, 30);
    ler_string("Informações de expansão: ", e->expansao, 100);
    n_estadios++;
    printf("Estádio cadastrado com ID %d\n", e->id);
}

void listar_estadios() {
    int any = 0;
    for (int i = 0; i < n_estadios; ++i) {
        if (estadios[i].id <= 0) continue;
        if (!any) { printf("--- Estádios ---\n"); any = 1; }
        Estadio *e = &estadios[i];
        printf("ID:%d | %s | %s - %s | Lugares:%d | Inaug: %s\n",
               e->id, e->nome, e->cidade, e->estado, e->lotacao, e->inauguracao);
    }
    if (!any) printf("Nenhum estádio cadastrado.\n");
}

void cadastrar_transmissao() {
    if (n_transmissoes >= MAX_TRANSMISSOES) { printf("Limite de transmissões atingido.\n"); return; }
    Transmissao *t = &transmissoes[n_transmissoes];
    t->id = n_transmissoes + 1;
    ler_string("Tipo (TV Aberta/TV a Cabo/Streaming): ", t->tipo, 50);
    ler_string("Nome da transmissão: ", t->nome, STRLEN);
    ler_string("Local: ", t->local, STRLEN);
    ler_string("Horário: ", t->horario, 30);
    ler_string("Permissões: ", t->permissoes, 100);

    t->responsaveis_count = 0;
    int q = ler_inteiro("Quantos responsáveis vincular a essa transmissão? (0-6): ");
    if (q < 0) q = 0; if (q > MAX_RESP_PER_TRANSM) q = MAX_RESP_PER_TRANSM;
    for (int i = 0; i < q; ++i) {
        printf("\nResponsáveis atuais:\n");
        listar_responsaveis();
        printf("Informe ID do responsável (0 para cadastrar novo): ");
        int rid = ler_inteiro("");
        if (rid == 0) {
            cadastrar_responsavel();
            rid = n_responsaveis;
        }
        if (rid > 0 && rid <= n_responsaveis && responsaveis[rid-1].id > 0) {
            t->responsaveis_ids[t->responsaveis_count++] = rid;
        } else {
            printf("ID inválido — pulando.\n");
        }
    }
    n_transmissoes++;
    printf("Transmissão cadastrada com ID %d\n", t->id);
}

void listar_transmissoes() {
    int any = 0;
    for (int i = 0; i < n_transmissoes; ++i) {
        if (transmissoes[i].id <= 0) continue;
        if (!any) { printf("--- Transmissões ---\n"); any = 1; }
        Transmissao *t = &transmissoes[i];
        printf("ID:%d | %s | %s | %s | Horário:%s | Permissões:%s\n",
               t->id, t->tipo, t->nome, t->local, t->horario, t->permissoes);
        if (t->responsaveis_count > 0) {
            printf(" Responsáveis (%d):", t->responsaveis_count);
            for (int r = 0; r < t->responsaveis_count; ++r) {
                int rid = t->responsaveis_ids[r];
                if (rid > 0 && rid <= n_responsaveis && responsaveis[rid-1].id > 0) printf(" %s (ID:%d);", responsaveis[rid-1].nome, rid);
            }
            printf("\n");
        }
    }
    if (!any) printf("Nenhuma transmissão cadastrada.\n");
}

void cadastrar_competicao() {
    if (n_competicoes >= MAX_COMPETICOES) { printf("Limite de competições atingido.\n"); return; }
    Competicao *c = &competicoes[n_competicoes];
    c->id = n_competicoes + 1;
    ler_string("Tipo de competição: ", c->tipo, 50);
    c->qtd_equipes = ler_inteiro("Quantidade de equipes: ");
    ler_string("Calendário (descrição): ", c->calendario, 200);
    ler_string("Atual campeão: ", c->atual_campeao, STRLEN);
    ler_string("Maior campeão: ", c->maior_campeao, STRLEN);
    n_competicoes++;
    printf("Competição cadastrada com ID %d\n", c->id);
}

void listar_competicoes() {
    int any = 0;
    for (int i = 0; i < n_competicoes; ++i) {
        if (competicoes[i].id <= 0) continue;
        if (!any) { printf("--- Competições ---\n"); any = 1; }
        Competicao *c = &competicoes[i];
        printf("ID:%d | %s | Equipes:%d | Atual:%s | Maior:%s\n",
               c->id, c->tipo, c->qtd_equipes, c->atual_campeao, c->maior_campeao);
    }
    if (!any) printf("Nenhuma competição cadastrada.\n");
}

void cadastrar_var() {
    if (n_var >= MAX_VAR_ENTRIES) { printf("Limite de VAR entries atingido.\n"); return; }
    VarEntry *v = &var_entries[n_var];
    v->id = n_var + 1;
    v->partida_id = ler_inteiro("ID da partida relacionada (0 se geral): ");
    v->chamadas = ler_inteiro("Qtd de chamadas: ");
    v->interferencias = ler_inteiro("Qtd de interferências: ");
    v->minuto = ler_inteiro("Minuto da intervenção: ");
    ler_string("Tempo de revisão: ", v->tempo_revisao, 30);
    ler_string("Conclusão: ", v->conclusao, 200);
    ler_string("Motivo: ", v->motivo, 200);
    n_var++;
    printf("Registro VAR cadastrado com ID %d\n", v->id);
}

void listar_var() {
    if (n_var == 0) { printf("Nenhum VAR registrado.\n"); return; }
    printf("--- VAR Entries ---\n");
    for (int i = 0; i < n_var; ++i) {
        VarEntry *v = &var_entries[i];
        if (v->id <= 0) continue;
        printf("ID:%d | Partida:%d | Chamadas:%d | Interf:%d | Min:%d | Conclusao:%s\n",
               v->id, v->partida_id, v->chamadas, v->interferencias, v->minuto, v->conclusao);
    }
}

void cadastrar_transferencia() {
    if (n_transf >= MAX_TRANSFERENCIAS) { printf("Limite de transferências atingido.\n"); return; }
    Transferencia *tr = &transferencias[n_transf];
    tr->id = n_transf + 1;
    tr->jogador_id = ler_inteiro("ID do jogador: ");
    ler_string("Tipo (compra/emprestimo/etc): ", tr->tipo, 50);
    tr->valor = ler_double("Valor: ");
    tr->rescisao = ler_double("Rescisao: ");
    n_transf++;
    printf("Transferência cadastrada com ID %d\n", tr->id);
}

void listar_transferencias() {
    if (n_transf == 0) { printf("Nenhuma transferência registrada.\n"); return; }
    printf("--- Transferências ---\n");
    for (int i = 0; i < n_transf; ++i) {
        Transferencia *tr = &transferencias[i];
        if (tr->id <= 0) continue;
        printf("ID:%d | JogadorID:%d | Tipo:%s | Valor:%.2f | Rescisao:%.2f\n",
               tr->id, tr->jogador_id, tr->tipo, tr->valor, tr->rescisao);
    }
}

void cadastrar_time() {
    if (n_times >= MAX_TIMES) { printf("Limite de times atingido.\n"); return; }
    Time *t = &times[n_times];
    t->id = n_times + 1;
    ler_string("Nome do time: ", t->nome, STRLEN);
    ler_string("Estado: ", t->estado, 50);

    if (n_tecnicos > 0) {
        printf("Técnicos disponíveis:\n");
        listar_tecnicos();
        int tid = ler_inteiro("Informe ID do técnico (0 para nenhum): ");
        if (tid <= 0 || tid > n_tecnicos) t->tecnico_id = 0;
        else t->tecnico_id = tid;
    } else {
        t->tecnico_id = 0;
    }

    t->titulos = 0; t->pontuacao = 0; t->posicao = 0; t->saldo_gols = 0; t->vitorias = 0;
    t->gols_marcados = 0; t->gols_sofridos = 0; t->jogos_casa = 0; t->vitorias_casa = 0;
    t->jogos_fora = 0; t->vitorias_fora = 0; t->dividas = 0.0;
    n_times++;
    printf("Time cadastrado com ID %d\n", t->id);
}

void listar_times() {
    int any = 0;
    for (int i = 0; i < n_times; ++i) {
        if (times[i].id <= 0) continue;
        if (!any) { printf("--- Times ---\n"); any = 1; }
        Time *t = &times[i];
        char tecn[STRLEN] = "Nenhum";
        if (t->tecnico_id > 0 && t->tecnico_id <= n_tecnicos && tecnicos[t->tecnico_id-1].id > 0) {
            strncpy(tecn, tecnicos[t->tecnico_id - 1].nome, STRLEN-1);
            tecn[STRLEN-1] = '\0';
        }
        printf("ID:%d | %s | Tec:%s | Titulos:%d | Gols:%d | Sofridos:%d | Vitorias:%d | Dívidas:%.2f\n",
               t->id, t->nome, tecn, t->titulos, t->gols_marcados, t->gols_sofridos, t->vitorias, t->dividas);
    }
    if (!any) printf("Nenhum time cadastrado.\n");
}

void registrar_partida() {
    if (n_partidas >= MAX_PARTIDAS) { printf("Limite de partidas atingido.\n"); return; }
    if (n_times < 2) { printf("É necessário cadastrar pelo menos 2 times para registrar partidas.\n"); return; }

    Partida *p = &partidas[n_partidas];
    memset(p, 0, sizeof(Partida));
    p->id = n_partidas + 1;

    printf("Selecionar time casa (ID):\n");
    listar_times();
    p->time_casa_id = ler_inteiro("ID do time casa: ");
    if (p->time_casa_id <= 0 || p->time_casa_id > n_times || times[p->time_casa_id-1].id <= 0) { printf("Time inválido.\n"); return; }

    printf("Selecionar time fora (ID):\n");
    listar_times();
    p->time_fora_id = ler_inteiro("ID do time fora: ");
    if (p->time_fora_id <= 0 || p->time_fora_id > n_times || p->time_fora_id == p->time_casa_id || times[p->time_fora_id-1].id <= 0) {
        printf("Time inválido.\n");
        return;
    }

    if (n_estadios > 0) {
        listar_estadios();
        p->estadio_id = ler_inteiro("ID do estádio (0 para usar local textual): ");
        if (p->estadio_id < 0 || p->estadio_id > n_estadios) { printf("Estádio inválido.\n"); return; }
        if (p->estadio_id > 0 && estadios[p->estadio_id-1].id <= 0) { printf("Estádio inválido.\n"); return; }
    } else p->estadio_id = 0;

    if (n_transmissoes > 0) {
        listar_transmissoes();
        p->transmissao_id = ler_inteiro("ID da transmissão (0 para nenhuma): ");
        if (p->transmissao_id < 0 || p->transmissao_id > n_transmissoes) { printf("Transmissão inválida.\n"); return; }
        if (p->transmissao_id > 0 && transmissoes[p->transmissao_id-1].id <= 0) { printf("Transmissão inválida.\n"); return; }
    } else p->transmissao_id = 0;

    if (n_competicoes > 0) {
        listar_competicoes();
        p->competicao_id = ler_inteiro("ID da competição (0 para nenhuma): ");
        if (p->competicao_id < 0 || p->competicao_id > n_competicoes) { printf("Competição inválida.\n"); return; }
        if (p->competicao_id > 0 && competicoes[p->competicao_id-1].id <= 0) { printf("Competição inválida.\n"); return; }
    } else p->competicao_id = 0;

    if (n_arbitros > 0) {
        listar_arbitros();
        p->arbitro_id = ler_inteiro("ID do árbitro para esta partida (0 para nenhum): ");
        if (p->arbitro_id < 0 || p->arbitro_id > n_arbitros) { printf("Árbitro inválido.\n"); return; }
        if (p->arbitro_id > 0 && arbitros[p->arbitro_id-1].id <= 0) { printf("Árbitro inválido.\n"); return; }
    } else p->arbitro_id = 0;

    ler_string("Clima: ", p->clima, 50);

    p->gols_casa = ler_inteiro("Gols do time casa: ");
    p->gols_fora = ler_inteiro("Gols do time fora: ");
    p->faltas_casa = ler_inteiro("Faltas do time casa: ");
    p->faltas_fora = ler_inteiro("Faltas do time fora: ");

    if (p->gols_casa < 0) p->gols_casa = 0;
    if (p->gols_casa > MAX_EVENTOS) p->gols_casa = MAX_EVENTOS;
    p->gols_casa_count = p->gols_casa;
    for (int i = 0; i < p->gols_casa_count; ++i) {
        listar_jogadores();
        int idg = ler_inteiro("Informe ID do jogador que fez o gol (time casa): ");
        p->gols_casa_scorers[i] = idg;
        if (idg > 0 && idg <= n_jogadores && jogadores[idg-1].id > 0) {
            jogadores[idg-1].gols += 1;
        } else {
            printf("ID inválido para gol; não atualizado no jogador.\n");
        }
        int hasAssist = ler_inteiro("Teve assistência? (1-sim / 0-nao): ");
        if (hasAssist == 1) {
            listar_jogadores();
            int ida = ler_inteiro("ID do assistente: ");
            p->gols_casa_assists[i] = ida;
            if (ida > 0 && ida <= n_jogadores && jogadores[ida-1].id > 0) {
                jogadores[ida-1].assistencias += 1;
            } else {
                printf("ID inválido para assistência; não atualizado.\n");
            }
        } else {
            p->gols_casa_assists[i] = 0;
        }
    }

    if (p->gols_fora < 0) p->gols_fora = 0;
    if (p->gols_fora > MAX_EVENTOS) p->gols_fora = MAX_EVENTOS;
    p->gols_fora_count = p->gols_fora;
    for (int i = 0; i < p->gols_fora_count; ++i) {
        listar_jogadores();
        int idg = ler_inteiro("Informe ID do jogador que fez o gol (time fora): ");
        p->gols_fora_scorers[i] = idg;
        if (idg > 0 && idg <= n_jogadores && jogadores[idg-1].id > 0) {
            jogadores[idg-1].gols += 1;
        } else {
            printf("ID inválido para gol; não atualizado no jogador.\n");
        }
        int hasAssist = ler_inteiro("Teve assistência? (1-sim / 0-nao): ");
        if (hasAssist == 1) {
            listar_jogadores();
            int ida = ler_inteiro("ID do assistente: ");
            p->gols_fora_assists[i] = ida;
            if (ida > 0 && ida <= n_jogadores && jogadores[ida-1].id > 0) {
                jogadores[ida-1].assistencias += 1;
            } else {
                printf("ID inválido para assistência; não atualizado.\n");
            }
        } else {
            p->gols_fora_assists[i] = 0;
        }
    }

    p->amarelos_casa_count = ler_inteiro("Quantidade de cartões amarelos - time casa: ");
    if (p->amarelos_casa_count < 0) p->amarelos_casa_count = 0;
    if (p->amarelos_casa_count > MAX_EVENTOS) p->amarelos_casa_count = MAX_EVENTOS;
    for (int i = 0; i < p->amarelos_casa_count; ++i) {
        listar_jogadores();
        int idc = ler_inteiro("ID do jogador que levou amarelo (casa): ");
        p->amarelos_casa_ids[i] = idc;
        if (idc > 0 && idc <= n_jogadores && jogadores[idc-1].id > 0) {
            jogadores[idc-1].amarelos += 1;
        } else {
            printf("ID inválido; não atualizado.\n");
        }
    }

    p->amarelos_fora_count = ler_inteiro("Quantidade de cartões amarelos - time fora: ");
    if (p->amarelos_fora_count < 0) p->amarelos_fora_count = 0;
    if (p->amarelos_fora_count > MAX_EVENTOS) p->amarelos_fora_count = MAX_EVENTOS;
    for (int i = 0; i < p->amarelos_fora_count; ++i) {
        listar_jogadores();
        int idc = ler_inteiro("ID do jogador que levou amarelo (fora): ");
        p->amarelos_fora_ids[i] = idc;
        if (idc > 0 && idc <= n_jogadores && jogadores[idc-1].id > 0) {
            jogadores[idc-1].amarelos += 1;
        } else {
            printf("ID inválido; não atualizado.\n");
        }
    }

    p->vermelhos_casa_count = ler_inteiro("Quantidade de cartões vermelhos - time casa: ");
    if (p->vermelhos_casa_count < 0) p->vermelhos_casa_count = 0;
    if (p->vermelhos_casa_count > MAX_EVENTOS) p->vermelhos_casa_count = MAX_EVENTOS;
    for (int i = 0; i < p->vermelhos_casa_count; ++i) {
        listar_jogadores();
        int idc = ler_inteiro("ID do jogador que levou vermelho (casa): ");
        p->vermelhos_casa_ids[i] = idc;
        if (idc > 0 && idc <= n_jogadores && jogadores[idc-1].id > 0) {
            jogadores[idc-1].vermelhos += 1;
        } else {
            printf("ID inválido; não atualizado.\n");
        }
    }

    p->vermelhos_fora_count = ler_inteiro("Quantidade de cartões vermelhos - time fora: ");
    if (p->vermelhos_fora_count < 0) p->vermelhos_fora_count = 0;
    if (p->vermelhos_fora_count > MAX_EVENTOS) p->vermelhos_fora_count = MAX_EVENTOS;
    for (int i = 0; i < p->vermelhos_fora_count; ++i) {
        listar_jogadores();
        int idc = ler_inteiro("ID do jogador que levou vermelho (fora): ");
        p->vermelhos_fora_ids[i] = idc;
        if (idc > 0 && idc <= n_jogadores && jogadores[idc-1].id > 0) {
            jogadores[idc-1].vermelhos += 1;
        } else {
            printf("ID inválido; não atualizado.\n");
        }
    }

    Time *tc = &times[p->time_casa_id - 1];
    Time *tf = &times[p->time_fora_id - 1];

    tc->gols_marcados += p->gols_casa;
    tc->gols_sofridos += p->gols_fora;
    tf->gols_marcados += p->gols_fora;
    tf->gols_sofridos += p->gols_casa;

    tc->saldo_gols = tc->gols_marcados - tc->gols_sofridos;
    tf->saldo_gols = tf->gols_marcados - tf->gols_sofridos;

    tc->jogos_casa++;
    tf->jogos_fora++;

    if (p->gols_casa > p->gols_fora) {
        tc->vitorias++;
        tc->vitorias_casa++;
        tc->pontuacao += 3;
    } else if (p->gols_fora > p->gols_casa) {
        tf->vitorias++;
        tf->vitorias_fora++;
        tf->pontuacao += 3;
    } else {
        tc->pontuacao += 1;
        tf->pontuacao += 1;
    }

    printf("\n=== Escalação - Time Casa ===\n");
    p->esc_casa_count = ler_inteiro("Quantos jogadores na escalação (casa) (max 18): ");
    if (p->esc_casa_count < 0) p->esc_casa_count = 0;
    if (p->esc_casa_count > MAX_LINEUP) p->esc_casa_count = MAX_LINEUP;
    for (int i = 0; i < p->esc_casa_count; ++i) {
        listar_jogadores();
        p->esc_casa[i] = ler_inteiro("Informe ID do jogador (casa): ");
    }

    printf("\n=== Escalação - Time Fora ===\n");
    p->esc_fora_count = ler_inteiro("Quantos jogadores na escalação (fora) (max 18): ");
    if (p->esc_fora_count < 0) p->esc_fora_count = 0;
    if (p->esc_fora_count > MAX_LINEUP) p->esc_fora_count = MAX_LINEUP;
    for (int i = 0; i < p->esc_fora_count; ++i) {
        listar_jogadores();
        p->esc_fora[i] = ler_inteiro("Informe ID do jogador (fora): ");
    }

    p->subs_count = ler_inteiro("Quantas substituições registrar? (max 12): ");
    if (p->subs_count < 0) p->subs_count = 0;
    if (p->subs_count > MAX_SUBS_PER_MATCH) p->subs_count = MAX_SUBS_PER_MATCH;
    for (int i = 0; i < p->subs_count; ++i) {
        printf("Substituição #%d\n", i+1);
        p->subs[i].minuto = ler_inteiro("Minuto: ");
        listar_times();
        p->subs[i].time_id = ler_inteiro("ID do time que fez a substituição: ");
        listar_jogadores();
        p->subs[i].jogador_out_id = ler_inteiro("ID do jogador que saiu: ");
        p->subs[i].jogador_in_id = ler_inteiro("ID do jogador que entrou: ");
    }

    p->var_chamadas = ler_inteiro("Quantidade de chamadas VAR nesta partida: ");
    p->teve_prorrogacao = ler_inteiro("Teve prorrogação? (1-sim / 0-nao): ");
    p->teve_penaltis = ler_inteiro("Teve pênaltis? (1-sim / 0-nao): ");
    if (p->teve_penaltis) {
        p->penaltis_casa = ler_inteiro("Gols nos penaltis (casa): ");
        p->penaltis_fora = ler_inteiro("Gols nos penaltis (fora): ");
        p->penaltis_casa_scorers_count = ler_inteiro("Quantos batedores casa listar (0 para pular): ");
        if (p->penaltis_casa_scorers_count > 11) p->penaltis_casa_scorers_count = 11;
        for (int i = 0; i < p->penaltis_casa_scorers_count; ++i) {
            listar_jogadores();
            p->penaltis_casa_scorers[i] = ler_inteiro("ID do batedor casa: ");
        }
        p->penaltis_fora_scorers_count = ler_inteiro("Quantos batedores fora listar (0 para pular): ");
        if (p->penaltis_fora_scorers_count > 11) p->penaltis_fora_scorers_count = 11;
        for (int i = 0; i < p->penaltis_fora_scorers_count; ++i) {
            listar_jogadores();
            p->penaltis_fora_scorers[i] = ler_inteiro("ID do batedor fora: ");
        }
    } else {
        p->penaltis_casa = p->penaltis_fora = 0;
        p->penaltis_casa_scorers_count = p->penaltis_fora_scorers_count = 0;
    }

    n_partidas++;
    printf("\nPartida registrada com sucesso (ID %d)\n", p->id);
}

void listar_partidas() {
    if (n_partidas == 0) { printf("Nenhuma partida registrada.\n"); return; }
    printf("--- Partidas ---\n");
    for (int i = 0; i < n_partidas; ++i) {
        Partida *p = &partidas[i];
        if (p->id <= 0) continue;
        char estadio_nome[STRLEN] = "";
        if (p->estadio_id > 0 && p->estadio_id <= n_estadios && estadios[p->estadio_id-1].id > 0) {
            strncpy(estadio_nome, estadios[p->estadio_id - 1].nome, STRLEN-1);
        }
        char arbitro_nome[STRLEN] = "Nenhum";
        if (p->arbitro_id > 0 && p->arbitro_id <= n_arbitros && arbitros[p->arbitro_id-1].id > 0) strncpy(arbitro_nome, arbitros[p->arbitro_id - 1].nome, STRLEN-1);

        char *nome_casa = (p->time_casa_id>0 && p->time_casa_id<=n_times && times[p->time_casa_id-1].id>0) ? times[p->time_casa_id-1].nome : "Time removido";
        char *nome_fora = (p->time_fora_id>0 && p->time_fora_id<=n_times && times[p->time_fora_id-1].id>0) ? times[p->time_fora_id-1].nome : "Time removido";

        printf("ID:%d | %s (ID %d) %d x %d %s (ID %d) | Estádio: %s | Clima: %s | Árbitro: %s\n",
               p->id, nome_casa, p->time_casa_id, p->gols_casa, p->gols_fora,
               nome_fora, p->time_fora_id, estadio_nome, p->clima, arbitro_nome);

        if (p->gols_casa_count > 0) {
            printf(" Gols (Casa %d):", p->gols_casa_count);
            for (int k = 0; k < p->gols_casa_count; ++k) {
                int gid = p->gols_casa_scorers[k];
                if (gid > 0 && gid <= n_jogadores && jogadores[gid-1].id > 0)
                    printf(" %s(ID:%d)", jogadores[gid-1].nome, gid);
                else
                    printf(" ID:%d", gid);
                if (p->gols_casa_assists[k] > 0) {
                    int aid = p->gols_casa_assists[k];
                    if (aid > 0 && aid <= n_jogadores && jogadores[aid-1].id > 0)
                        printf(" (assist:%s(ID:%d))", jogadores[aid-1].nome, aid);
                    else
                        printf(" (assist:ID:%d)", aid);
                }
            }
            printf("\n");
        }

        if (p->gols_fora_count > 0) {
            printf(" Gols (Fora %d):", p->gols_fora_count);
            for (int k = 0; k < p->gols_fora_count; ++k) {
                int gid = p->gols_fora_scorers[k];
                if (gid > 0 && gid <= n_jogadores && jogadores[gid-1].id > 0)
                    printf(" %s(ID:%d)", jogadores[gid-1].nome, gid);
                else
                    printf(" ID:%d", gid);
                if (p->gols_fora_assists[k] > 0) {
                    int aid = p->gols_fora_assists[k];
                    if (aid > 0 && aid <= n_jogadores && jogadores[aid-1].id > 0)
                        printf(" (assist:%s(ID:%d))", jogadores[aid-1].nome, aid);
                    else
                        printf(" (assist:ID:%d)", aid);
                }
            }
            printf("\n");
        }

        if (p->amarelos_casa_count > 0) {
            printf(" Amarelos (Casa %d):", p->amarelos_casa_count);
            for (int k = 0; k < p->amarelos_casa_count; ++k) {
                int id = p->amarelos_casa_ids[k];
                if (id > 0 && id <= n_jogadores && jogadores[id-1].id > 0) printf(" %s(ID:%d)", jogadores[id-1].nome, id);
                else printf(" ID:%d", id);
            }
            printf("\n");
        }

        if (p->amarelos_fora_count > 0) {
            printf(" Amarelos (Fora %d):", p->amarelos_fora_count);
            for (int k = 0; k < p->amarelos_fora_count; ++k) {
                int id = p->amarelos_fora_ids[k];
                if (id > 0 && id <= n_jogadores && jogadores[id-1].id > 0) printf(" %s(ID:%d)", jogadores[id-1].nome, id);
                else printf(" ID:%d", id);
            }
            printf("\n");
        }

        if (p->vermelhos_casa_count > 0) {
            printf(" Vermelhos (Casa %d):", p->vermelhos_casa_count);
            for (int k = 0; k < p->vermelhos_casa_count; ++k) {
                int id = p->vermelhos_casa_ids[k];
                if (id > 0 && id <= n_jogadores && jogadores[id-1].id > 0) printf(" %s(ID:%d)", jogadores[id-1].nome, id);
                else printf(" ID:%d", id);
            }
            printf("\n");
        }

        if (p->vermelhos_fora_count > 0) {
            printf(" Vermelhos (Fora %d):", p->vermelhos_fora_count);
            for (int k = 0; k < p->vermelhos_fora_count; ++k) {
                int id = p->vermelhos_fora_ids[k];
                if (id > 0 && id <= n_jogadores && jogadores[id-1].id > 0) printf(" %s(ID:%d)", jogadores[id-1].nome, id);
                else printf(" ID:%d", id);
            }
            printf("\n");
        }

        if (p->subs_count > 0) {
            printf(" Substituições (%d):\n", p->subs_count);
            for (int s = 0; s < p->subs_count; ++s) {
                Substituicao *sub = &p->subs[s];
                printf("  Min:%d | Time:%d | Out:%d | In:%d\n", sub->minuto, sub->time_id, sub->jogador_out_id, sub->jogador_in_id);
            }
        }

        if (p->teve_penaltis) {
            printf(" Pênaltis: %d x %d\n", p->penaltis_casa, p->penaltis_fora);
        }
    }
}

void rel_artilheiro() {
    if (n_jogadores == 0) { printf("Nenhum jogador cadastrado.\n"); return; }
    int maxg = -1; int idx = -1;
    for (int i = 0; i < n_jogadores; ++i) {
        if (jogadores[i].id <= 0) continue;
        if (jogadores[i].gols > maxg) { maxg = jogadores[i].gols; idx = i; }
    }
    if (idx >= 0) {
        printf("Artilheiro: %s (Gols: %d)\n", jogadores[idx].nome, jogadores[idx].gols);
    } else printf("Nenhum jogador para avaliar.\n");
}

void rel_garcom() {
    if (n_jogadores == 0) { printf("Nenhum jogador cadastrado.\n"); return; }
    int maxa = -1; int idx = -1;
    for (int i = 0; i < n_jogadores; ++i) {
        if (jogadores[i].id <= 0) continue;
        if (jogadores[i].assistencias > maxa) { maxa = jogadores[i].assistencias; idx = i; }
    }
    if (idx >= 0) {
        printf("Garçom: %s (Assistências: %d)\n", jogadores[idx].nome, jogadores[idx].assistencias);
    } else printf("Nenhum jogador para avaliar.\n");
}

void rel_cartoes() {
    if (n_jogadores == 0) { printf("Nenhum jogador cadastrado.\n"); return; }
    int maxAm = -1, maxVe = -1; int idAm=-1, idVe=-1;
    for (int i = 0; i < n_jogadores; ++i) {
        if (jogadores[i].id <= 0) continue;
        if (jogadores[i].amarelos > maxAm) { maxAm = jogadores[i].amarelos; idAm=i; }
        if (jogadores[i].vermelhos > maxVe) { maxVe = jogadores[i].vermelhos; idVe=i; }
    }
    if (idAm >= 0) printf("Mais cartões amarelos: %s (%d)\n", jogadores[idAm].nome, jogadores[idAm].amarelos);
    if (idVe >= 0) printf("Mais cartões vermelhos: %s (%d)\n", jogadores[idVe].nome, jogadores[idVe].vermelhos);
    if (idAm < 0 && idVe < 0) printf("Nenhum jogador para avaliar.\n");
}

void rel_melhor_ataque() {
    if (n_times == 0) { printf("Nenhum time cadastrado.\n"); return; }
    int maxg = -1, idx=-1;
    for (int i = 0; i < n_times; ++i) {
        if (times[i].id <= 0) continue;
        if (times[i].gols_marcados > maxg) { maxg = times[i].gols_marcados; idx=i; }
    }
    if (idx >= 0) printf("Melhor ataque: %s (Gols: %d)\n", times[idx].nome, times[idx].gols_marcados);
    else printf("Nenhum time para avaliar.\n");
}

void rel_melhor_defesa() {
    if (n_times == 0) { printf("Nenhum time cadastrado.\n"); return; }
    int ming = 1<<30, idx=-1;
    for (int i = 0; i < n_times; ++i) {
        if (times[i].id <= 0) continue;
        if (times[i].gols_sofridos < ming) { ming = times[i].gols_sofridos; idx=i; }
    }
    if (idx >= 0) printf("Melhor defesa: %s (Gols sofridos: %d)\n", times[idx].nome, times[idx].gols_sofridos);
    else printf("Nenhum time para avaliar.\n");
}

void rel_melhor_desempenho_casa() {
    if (n_times == 0) { printf("Nenhum time cadastrado.\n"); return; }
    double best = -1.0; int idx=-1;
    for (int i = 0; i < n_times; ++i) {
        if (times[i].id <= 0) continue;
        if (times[i].jogos_casa == 0) continue;
        double taxa = (double)times[i].vitorias_casa / times[i].jogos_casa;
        if (taxa > best) { best = taxa; idx = i; }
    }
    if (idx >= 0) printf("Melhor desempenho em casa: %s (Taxa de vitórias: %.2f)\n", times[idx].nome, best);
    else printf("Nenhum registro de jogos em casa.\n");
}

void rel_melhor_desempenho_fora() {
    if (n_times == 0) { printf("Nenhum time cadastrado.\n"); return; }
    double best = -1.0; int idx=-1;
    for (int i = 0; i < n_times; ++i) {
        if (times[i].id <= 0) continue;
        if (times[i].jogos_fora == 0) continue;
        double taxa = (double)times[i].vitorias_fora / times[i].jogos_fora;
        if (taxa > best) { best = taxa; idx = i; }
    }
    if (idx >= 0) printf("Melhor desempenho fora: %s (Taxa de vitórias: %.2f)\n", times[idx].nome, best);
    else printf("Nenhum registro de jogos fora.\n");
}

void gerar_relatorios() {
    printf("--- Relatórios ---\n");
    rel_artilheiro();
    rel_garcom();
    rel_cartoes();
    rel_melhor_ataque();
    rel_melhor_defesa();
    rel_melhor_desempenho_casa();
    rel_melhor_desempenho_fora();
}

void salvar_dados() {
    FILE *f;
    f = fopen("jogadores.dat", "wb"); if (f) { fwrite(&n_jogadores, sizeof(int), 1, f); fwrite(jogadores, sizeof(Jogador), n_jogadores, f); fclose(f); }
    f = fopen("arbitros.dat", "wb"); if (f) { fwrite(&n_arbitros, sizeof(int), 1, f); fwrite(arbitros, sizeof(Arbitro), n_arbitros, f); fclose(f); }
    f = fopen("times.dat", "wb"); if (f) { fwrite(&n_times, sizeof(int), 1, f); fwrite(times, sizeof(Time), n_times, f); fclose(f); }
    f = fopen("partidas.dat", "wb"); if (f) { fwrite(&n_partidas, sizeof(int), 1, f); fwrite(partidas, sizeof(Partida), n_partidas, f); fclose(f); }

    f = fopen("tecnicos.dat", "wb"); if (f) { fwrite(&n_tecnicos, sizeof(int), 1, f); fwrite(tecnicos, sizeof(Tecnico), n_tecnicos, f); fclose(f); }
    f = fopen("estadios.dat", "wb"); if (f) { fwrite(&n_estadios, sizeof(int), 1, f); fwrite(estadios, sizeof(Estadio), n_estadios, f); fclose(f); }
    f = fopen("transmissoes.dat", "wb"); if (f) { fwrite(&n_transmissoes, sizeof(int), 1, f); fwrite(transmissoes, sizeof(Transmissao), n_transmissoes, f); fclose(f); }
    f = fopen("competicoes.dat", "wb"); if (f) { fwrite(&n_competicoes, sizeof(int), 1, f); fwrite(competicoes, sizeof(Competicao), n_competicoes, f); fclose(f); }
    f = fopen("var_entries.dat", "wb"); if (f) { fwrite(&n_var, sizeof(int), 1, f); fwrite(var_entries, sizeof(VarEntry), n_var, f); fclose(f); }
    f = fopen("transferencias.dat", "wb"); if (f) { fwrite(&n_transf, sizeof(int), 1, f); fwrite(transferencias, sizeof(Transferencia), n_transf, f); fclose(f); }
    f = fopen("responsaveis.dat", "wb"); if (f) { fwrite(&n_responsaveis, sizeof(int), 1, f); fwrite(responsaveis, sizeof(ResponsavelTransmissao), n_responsaveis, f); fclose(f); }

    printf("Dados salvos nos ficheiros.\n");
}

void carregar_dados() {
    FILE *f;
    f = fopen("jogadores.dat", "rb"); if (f) { fread(&n_jogadores, sizeof(int), 1, f); fread(jogadores, sizeof(Jogador), n_jogadores, f); fclose(f); }
    f = fopen("arbitros.dat", "rb"); if (f) { fread(&n_arbitros, sizeof(int), 1, f); fread(arbitros, sizeof(Arbitro), n_arbitros, f); fclose(f); }
    f = fopen("times.dat", "rb"); if (f) { fread(&n_times, sizeof(int), 1, f); fread(times, sizeof(Time), n_times, f); fclose(f); }
    f = fopen("partidas.dat", "rb"); if (f) { fread(&n_partidas, sizeof(int), 1, f); fread(partidas, sizeof(Partida), n_partidas, f); fclose(f); }

    f = fopen("tecnicos.dat", "rb"); if (f) { fread(&n_tecnicos, sizeof(int), 1, f); fread(tecnicos, sizeof(Tecnico), n_tecnicos, f); fclose(f); }
    f = fopen("estadios.dat", "rb"); if (f) { fread(&n_estadios, sizeof(int), 1, f); fread(estadios, sizeof(Estadio), n_estadios, f); fclose(f); }
    f = fopen("transmissoes.dat", "rb"); if (f) { fread(&n_transmissoes, sizeof(int), 1, f); fread(transmissoes, sizeof(Transmissao), n_transmissoes, f); fclose(f); }
    f = fopen("competicoes.dat", "rb"); if (f) { fread(&n_competicoes, sizeof(int), 1, f); fread(competicoes, sizeof(Competicao), n_competicoes, f); fclose(f); }
    f = fopen("var_entries.dat", "rb"); if (f) { fread(&n_var, sizeof(int), 1, f); fread(var_entries, sizeof(VarEntry), n_var, f); fclose(f); }
    f = fopen("transferencias.dat", "rb"); if (f) { fread(&n_transf, sizeof(int), 1, f); fread(transferencias, sizeof(Transferencia), n_transf, f); fclose(f); }
    f = fopen("responsaveis.dat", "rb"); if (f) { fread(&n_responsaveis, sizeof(int), 1, f); fread(responsaveis, sizeof(ResponsavelTransmissao), n_responsaveis, f); fclose(f); }

    printf("Dados carregados (se existiam ficheiros).\n");
}

void editar_jogador() {
    listar_jogadores();
    int id = ler_inteiro("ID do jogador a editar: ");
    if (id <= 0 || id > n_jogadores || jogadores[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Jogador *j = &jogadores[id - 1];
    ler_string("Novo nome: ", j->nome, STRLEN);
    j->idade = ler_inteiro("Nova idade: ");
    ler_string("Nova nacionalidade: ", j->nacionalidade, STRLEN);
    j->numero_camisa = ler_inteiro("Novo número da camisa: ");
    ler_string("Nova posição: ", j->posicao, 30);

    printf("Jogador atualizado com sucesso!\n");
}

void remover_jogador() {
    listar_jogadores();
    int id = ler_inteiro("ID do jogador para remover: ");
    if (id <= 0 || id > n_jogadores || jogadores[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    jogadores[id - 1].id = -1;
    printf("Jogador removido.\n");
}

void editar_tecnico() {
    listar_tecnicos();
    int id = ler_inteiro("ID do técnico a editar: ");
    if (id <= 0 || id > n_tecnicos || tecnicos[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Tecnico *t = &tecnicos[id - 1];
    ler_string("Novo nome: ", t->nome, STRLEN);
    ler_string("Nova nacionalidade: ", t->nacionalidade, STRLEN);
    t->idade = ler_inteiro("Nova idade: ");
    ler_string("Novo esquema tático: ", t->esquema_tatico, 50);

    printf("Técnico atualizado com sucesso!\n");
}

void remover_tecnico() {
    listar_tecnicos();
    int id = ler_inteiro("ID do técnico para remover: ");
    if (id <= 0 || id > n_tecnicos || tecnicos[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    tecnicos[id - 1].id = -1;

    for (int i = 0; i < n_times; i++) {
        if (times[i].tecnico_id == id) times[i].tecnico_id = 0;
    }

    printf("Técnico removido.\n");
}

void editar_responsavel() {
    listar_responsaveis();
    int id = ler_inteiro("ID do responsável a editar: ");
    if (id <= 0 || id > n_responsaveis || responsaveis[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    ResponsavelTransmissao *r = &responsaveis[id - 1];
    ler_string("Novo nome: ", r->nome, STRLEN);
    ler_string("Nova profissão: ", r->profissao, 50);
    ler_string("Nova emissora: ", r->emissora, 50);
    r->idade = ler_inteiro("Nova idade: ");

    printf("Responsável atualizado com sucesso!\n");
}

void remover_responsavel() {
    listar_responsaveis();
    int id = ler_inteiro("ID do responsável para remover: ");
    if (id <= 0 || id > n_responsaveis || responsaveis[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    responsaveis[id - 1].id = -1;

    for (int i = 0; i < n_transmissoes; i++) {
        for (int j = 0; j < transmissoes[i].responsaveis_count; j++) {
            if (transmissoes[i].responsaveis_ids[j] == id)
                transmissoes[i].responsaveis_ids[j] = 0;
        }
    }

    printf("Responsável removido.\n");
}

void editar_time() {
    listar_times();
    int id = ler_inteiro("ID do time a editar: ");
    if (id <= 0 || id > n_times || times[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Time *t = &times[id - 1];
    ler_string("Novo nome do time: ", t->nome, STRLEN);
    ler_string("Novo estado: ", t->estado, 50);

    if (n_tecnicos > 0) {
        printf("Técnicos disponíveis:\n");
        listar_tecnicos();
        int tid = ler_inteiro("Novo técnico (0 para remover): ");
        if (tid >= 0 && tid <= n_tecnicos) t->tecnico_id = tid;
    } else {
        t->tecnico_id = 0;
    }

    printf("Time atualizado com sucesso!\n");
}

void remover_time() {
    listar_times();
    int id = ler_inteiro("ID do time para remover: ");
    if (id <= 0 || id > n_times || times[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    times[id - 1].id = -1;

    for (int i = 0; i < n_partidas; ++i) {
        Partida *p = &partidas[i];
        if (p->id <= 0) continue;
        if (p->time_casa_id == id) p->time_casa_id = 0;
        if (p->time_fora_id == id) p->time_fora_id = 0;
    }

    printf("Time removido.\n");
}

void editar_transmissao() {
    listar_transmissoes();
    int id = ler_inteiro("ID da transmissão a editar: ");
    if (id <= 0 || id > n_transmissoes || transmissoes[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Transmissao *t = &transmissoes[id - 1];
    ler_string("Novo tipo: ", t->tipo, 50);
    ler_string("Novo nome: ", t->nome, STRLEN);
    ler_string("Novo local: ", t->local, STRLEN);
    ler_string("Novo horário: ", t->horario, 30);
    ler_string("Novas permissões: ", t->permissoes, 100);

    t->responsaveis_count = 0;
    int q = ler_inteiro("Quantos responsáveis vincular a essa transmissão agora? (0-6): ");
    if (q < 0) q = 0; if (q > MAX_RESP_PER_TRANSM) q = MAX_RESP_PER_TRANSM;
    for (int i = 0; i < q; ++i) {
        listar_responsaveis();
        int rid = ler_inteiro("Informe ID do responsável (0 para pular): ");
        if (rid > 0 && rid <= n_responsaveis && responsaveis[rid-1].id > 0) {
            t->responsaveis_ids[t->responsaveis_count++] = rid;
        } else {
            printf("ID inválido — pulando.\n");
        }
    }

    printf("Transmissão atualizada.\n");
}

void remover_transmissao() {
    listar_transmissoes();
    int id = ler_inteiro("ID da transmissão para remover: ");
    if (id <= 0 || id > n_transmissoes || transmissoes[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    transmissoes[id - 1].id = -1;
    for (int i = 0; i < n_partidas; ++i) {
        if (partidas[i].transmissao_id == id) partidas[i].transmissao_id = 0;
    }
    printf("Transmissão removida.\n");
}

void editar_arbitro() {
    listar_arbitros();
    int id = ler_inteiro("ID do árbitro a editar: ");
    if (id <= 0 || id > n_arbitros || arbitros[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Arbitro *a = &arbitros[id - 1];
    ler_string("Novo nome: ", a->nome, STRLEN);
    a->idade = ler_inteiro("Nova idade: ");
    ler_string("Novo estado: ", a->estado, 50);
    ler_string("Nova federação: ", a->federacao, 50);

    printf("Árbitro atualizado com sucesso!\n");
}

void remover_arbitro() {
    listar_arbitros();
    int id = ler_inteiro("ID do árbitro para remover: ");
    if (id <= 0 || id > n_arbitros || arbitros[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    arbitros[id - 1].id = -1;

    for (int i = 0; i < n_partidas; ++i) {
        if (partidas[i].arbitro_id == id) partidas[i].arbitro_id = 0;
    }

    printf("Árbitro removido.\n");
}

void editar_estadio() {
    listar_estadios();
    int id = ler_inteiro("ID do estádio a editar: ");
    if (id <= 0 || id > n_estadios || estadios[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Estadio *e = &estadios[id - 1];
    ler_string("Novo nome: ", e->nome, STRLEN);
    ler_string("Novo estado: ", e->estado, 50);
    ler_string("Nova cidade: ", e->cidade, 50);
    e->lotacao = ler_inteiro("Nova lotação: ");
    ler_string("Nova data de inauguração: ", e->inauguracao, 30);
    ler_string("Novas informações de expansão: ", e->expansao, 100);

    printf("Estádio atualizado.\n");
}

void remover_estadio() {
    listar_estadios();
    int id = ler_inteiro("ID do estádio para remover: ");
    if (id <= 0 || id > n_estadios || estadios[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    estadios[id - 1].id = -1;

    for (int i = 0; i < n_partidas; ++i) {
        if (partidas[i].estadio_id == id) partidas[i].estadio_id = 0;
    }

    printf("Estádio removido.\n");
}

void editar_competicao() {
    listar_competicoes();
    int id = ler_inteiro("ID da competição a editar: ");
    if (id <= 0 || id > n_competicoes || competicoes[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    Competicao *c = &competicoes[id - 1];
    ler_string("Novo tipo de competição: ", c->tipo, 50);
    c->qtd_equipes = ler_inteiro("Nova quantidade de equipes: ");
    ler_string("Novo calendário (descrição): ", c->calendario, 200);
    ler_string("Novo atual campeão: ", c->atual_campeao, STRLEN);
    ler_string("Novo maior campeão: ", c->maior_campeao, STRLEN);

    printf("Competição atualizada.\n");
}

void remover_competicao() {
    listar_competicoes();
    int id = ler_inteiro("ID da competição para remover: ");
    if (id <= 0 || id > n_competicoes || competicoes[id-1].id <= 0) { printf("ID inválido.\n"); return; }

    competicoes[id - 1].id = -1;

    for (int i = 0; i < n_partidas; ++i) {
        if (partidas[i].competicao_id == id) partidas[i].competicao_id = 0;
    }

    printf("Competição removida.\n");
}

void menu_jogadores() {
    int op;
    do {
        printf("\n--- Jogadores ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_jogador(); break;
            case 2: listar_jogadores(); break;
            case 3: editar_jogador(); break;
            case 4: remover_jogador(); break;
            case 5: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 5);
}

void menu_arbitros() {
    int op;
    do {
        printf("\n--- Árbitros ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_arbitro(); break;
            case 2: listar_arbitros(); break;
            case 3: editar_arbitro(); break;
            case 4: remover_arbitro(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

void menu_tecnicos() {
    int op;
    do {
        printf("\n--- Técnicos ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_tecnico(); break;
            case 2: listar_tecnicos(); break;
            case 3: editar_tecnico(); break;
            case 4: remover_tecnico(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

void menu_estadios() {
    int op;
    do {
        printf("\n--- Estádios ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_estadio(); break;
            case 2: listar_estadios(); break;
            case 3: editar_estadio(); break;
            case 4: remover_estadio(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

void menu_transmissoes() {
    int op;
    do {
        printf("\n--- Transmissões ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_transmissao(); break;
            case 2: listar_transmissoes(); break;
            case 3: editar_transmissao(); break;
            case 4: remover_transmissao(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

void menu_competicoes() {
    int op;
    do {
        printf("\n--- Competições ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_competicao(); break;
            case 2: listar_competicoes(); break;
            case 3: editar_competicao(); break;
            case 4: remover_competicao(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

void menu_var() {
    int op;
    do {
        printf("\n--- VAR ---\n1.Cadastrar 2.Listar 3.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_var(); break;
            case 2: listar_var(); break;
            case 3: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 3);
}

void menu_transferencias() {
    int op;
    do {
        printf("\n--- Transferências ---\n1.Cadastrar 2.Listar 3.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_transferencia(); break;
            case 2: listar_transferencias(); break;
            case 3: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 3);
}

void menu_times() {
    int op;
    do {
        printf("\n--- Times ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_time(); break;
            case 2: listar_times(); break;
            case 3: editar_time(); break;
            case 4: remover_time(); break;
            case 5: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 5);
}

void menu_partidas() {
    int op;
    do {
        printf("\n--- Partidas ---\n1.Registrar partida 2.Listar partidas 3.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: registrar_partida(); break;
            case 2: listar_partidas(); break;
            case 3: break;
            default: printf("Opção invalida.\n");
        }
    } while (op != 3);
}

void menu_responsaveis() {
    int op;
    do {
        printf("\n--- Responsáveis na Transmissão ---\n1.Cadastrar 2.Listar 3.Editar 4.Remover 5.Voltar\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: cadastrar_responsavel(); break;
            case 2: listar_responsaveis(); break;
            case 3: editar_responsavel(); break;
            case 4: remover_responsavel(); break;
            case 5: break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 5);
}

int main() {
    carregar_dados();
    int op;
    do {
        printf("\n==== Súmula Digital em C ===\n");
        printf("1.Jogadores 2.Árbitros 3.Técnicos 4.Times 5.Partidas\n");
        printf("6.Estadios 7.Transmissões 8.Competições 9.VAR 10.Transferências\n");
        printf("11.Responsáveis 12.Relatorios 13.Salvar 14.Sair\nEscolha: ");
        op = ler_inteiro("");
        switch(op) {
            case 1: menu_jogadores(); break;
            case 2: menu_arbitros(); break;
            case 3: menu_tecnicos(); break;
            case 4: menu_times(); break;
            case 5: menu_partidas(); break;
            case 6: menu_estadios(); break;
            case 7: menu_transmissoes(); break;
            case 8: menu_competicoes(); break;
            case 9: menu_var(); break;
            case 10: menu_transferencias(); break;
            case 11: menu_responsaveis(); break;
            case 12: gerar_relatorios(); break;
            case 13: salvar_dados(); break;
            case 14: salvar_dados(); printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (op != 14);

    return 0;
}