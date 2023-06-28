#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <cstdio>
using namespace std;


// contém nome da criança e a quantidade de vezes que realizou um quesito no total
typedef struct CriancaDestaque
{
  string nome;
  int quantidade;
} criancaDestaque;

// Tipo de dado registro que possui data e todas as açoes realizadas pela criança neste dia
typedef struct Registro
{
  string data;
  map<string, int> acoes; // nome do quesito -> quantidade de vezes
} registro;

// tipo de dados ocorrencia que armazena o nome da criança e todas as vezes que ela fez uma determinada ação
typedef struct Ocorrencia
{
  string nomeCrianca;
  map<string, int> listaQuantidades; // data -> quantidade de vezes (uma lista disso pra cada criança)
} ocorrencia;

// função que recebe o nome do arquivo sem a extensão e retorna a data no formato dd/mm/aaaa
string formataData(string data)
{
  if (data.size() == 8)
  {
    string new_data = data.substr(0, 2) + "/" + data.substr(2, 2) + "/" + data.substr(4, 4);
    return new_data;
  }
  else
  {
    return data;
  }
}

int busca(string nome, vector<vector<string>> &matriz)
{
  for (int i = 0; i < matriz.size(); i++)
  {
    if (nome == matriz[i][0])
    {
      return 1;
    }
  }
  return 0;
}

bool isNumeric(string const &str)
{
  char *p;
  strtol(str.c_str(), &p, 10);
  return *p == 0;
}

void readfile(string nomeArquivo, vector<vector<string>> &matriz)
{
  string line, word;
  vector<string> row;
  fstream file;
  file.open(nomeArquivo, ios::in);
  if (!file)
  {
    cout << "Arquivo nao existe" << endl;
  }
  else
  {
    while (getline(file, line))
    {
      row.clear();
      stringstream str(line);
      while (getline(str, word, ','))
      {
        row.push_back(word);
      }
      matriz.push_back(row);
    }
    cout << "Arquivo " << nomeArquivo << " lido e estruturas atualizadas." << endl
         << endl;
  }
  file.close();
}

int readfile2(string nomeArquivo, vector<vector<string>> &matriz, string &crianca, string &ques)
{
  string line, word;
  vector<string> row;
  fstream file;
  file.open(nomeArquivo, ios::in);
  if (!file)
  {
    cout << "Arquivo " << nomeArquivo << " nao existe" << endl;
    return 0;
  }
  else
  {
    while (getline(file, line))
    {
      row.clear();
      stringstream str(line);
      getline(str, word, ',');
      int repete = busca(word, matriz);
      if (repete == 0)
      {
        row.push_back(word);
        crianca = word;
        while (getline(str, word, ','))
        {
          string quesito = word;
          getline(str, word, ',');
          if (word == "" || (!isNumeric(word)))
          {
            ques = quesito;
            matriz.push_back(row);
            file.close();
            return 3;
          }
          if (stoi(word) <= 0)
          {
            ques = quesito;
            matriz.push_back(row);
            file.close();
            return 3;
          }
          row.push_back(quesito);
          row.push_back(word);
        }
        matriz.push_back(row);
      }
      else
      {
        crianca = word;
        file.close();
        return 2;
      }
    }
  }
  file.close();
  return 1;
}

void show(vector<vector<string>> matriz)
{
  for (int i = 0; i < matriz.size(); i++)
  {
    for (int j = 0; j < matriz[i].size(); j++)
    {
      cout << matriz[i][j] << " ";
    }
    cout << endl;
  }
}

// função que atualiza o map que associa o nome da criança ao seu vector de registros
void atualizarRegistros(vector<vector<string>> info, string data, map<string, vector<registro>> &criancas)
{
  registro reg;
  string nome;

  reg.data = data;
  vector<registro> registros;
  for (int i = 0; i < info.size(); i++)
  {
    (reg.acoes).clear();
    registros.clear();
    nome = info[i][0];
    // Caso a criança ainda não esteja no map criamos um campo dela e seu vector de registros, carregando
    // as informações do arquivo lido em uma variavel registro e inserindo-a no vector
    if (criancas.find(nome) == criancas.end())
    {
      for (int j = 1; j < info[i].size(); j++)
      {
        reg.acoes[info[i][j]] = stoi(info[i][j + 1]);
        j++;
      }
      registros.push_back(reg);
      criancas[nome] = registros;
    }
    // Caso a criança já exista no map, apenas adiciona o registro ao vector de registros
    else
    {
      for (int j = 1; j < info[i].size(); j++)
      {
        reg.acoes[info[i][j]] = stoi(info[i][j + 1]);
        j++;
      }
      criancas[nome].push_back(reg);
    }
  }
}

// função que busca por uma criança no vector de ocorrencias e retorna o índice no qual ela e suas
// ocorrencias se encontram
int buscaCrianca(string nome, vector<ocorrencia> ocorr)
{
  for (auto it = ocorr.begin(); it != ocorr.end(); it++)
  {
    if (it->nomeCrianca == nome)
    {
      return distance(ocorr.begin(), it);
    }
  }
  return -1;
}

// função que atualiza o map que relaciona o nome do quesito ao seu vector de ocorrências
void atualizarOcorrencias(vector<vector<string>> info, string data, map<string, vector<ocorrencia>> &quesitos)
{
  ocorrencia acontecimento;
  string quesito;
  vector<ocorrencia> ocorrencias;
  for (int i = 0; i < info.size(); i++)
  {
    acontecimento.nomeCrianca = info[i][0];
    for (int j = 1; j < info[i].size(); j++)
    {
      quesito = info[i][j];
      // se o quesito ainda não foi criado no map, criamos ele e seu vector de ocorrencias inserindo a ocorrencia registrada
      // no vector de ocorrências
      if (quesitos.find(quesito) == quesitos.end())
      {
        acontecimento.listaQuantidades[data] = stoi(info[i][j + 1]);
        j++;
        ocorrencias.push_back(acontecimento);
        quesitos[quesito] = ocorrencias;
        ocorrencias.clear();
        (acontecimento.listaQuantidades).clear();
      }
      else
      {
        int ind = buscaCrianca(acontecimento.nomeCrianca, quesitos[quesito]);
        // se a criança não está no vector de ocorrências criamos uma ocorrência que contem seu nome
        // e um map com a data e quantidade de vezes que realizou o quesito
        if (ind == -1)
        {
          acontecimento.listaQuantidades[data] = stoi(info[i][j + 1]);
          j++;
          quesitos[quesito].push_back(acontecimento);
          (acontecimento.listaQuantidades).clear();
          // caso a ocorrência com a criança já exista no vector, apenas adicionamos data->quantidade de vezes
          // na ocorrência
        }
        else
        {
          (quesitos[quesito])[ind].listaQuantidades[data] = stoi(info[i][j + 1]);
          j++;
        }
      }
    }
  }
}

void processamentoLote(string arq, vector<vector<string>> &m, map<string,vector<registro>> &criancas,map<string, vector<ocorrencia>> &quesitos)
{
  string entrada = arq + ".txt", linha;
  fstream aux;
  aux.open(entrada, ios::in);
  if (!aux)
  {
    cout << "O arquivo " << entrada << " nao existe." << endl;
  }
  else
  {
    while (getline(aux, linha))
    {
      string data = formataData(linha);
      linha += ".csv";
      string nome, quesito;
      int check = readfile2(linha, m, nome, quesito);
      if (check == 0)
      {
        cout << "Erro especificado no arquivo log.txt" << endl;
        fstream log;
        log.open("log.txt", ios::app);
        log << "Arquivo: " << linha << endl;
        log << "Linha: 0" << endl;
        log << "O arquivo nao existe ou nao pode ser aberto" << endl
            << endl;
        log.close();
      }
      if (check == 2)
      {
        cout << "Erro especificado no arquivo log.txt" << endl;
        fstream log;
        log.open("log.txt", ios::app);
        log << "Arquivo: " << linha << endl;
        log << "Linha: " << m.size() + 1 << endl;
        log << "A crianca " << nome << " se repete em linhas diferentes no arquivo" << endl
            << endl;
        log.close();
      }
      if (check == 3)
      {
        cout << "Erro especificado no arquivo log.txt" << endl;
        fstream log;
        log.open("log.txt", ios::app);
        log << "Arquivo: " << linha << endl;
        log << "Linha: " << m.size() << endl;
        log << "A quantidade que " << quesito << " possui em " << nome
            << " nao existe ou nao e um inteiro positivo" << endl
            << endl;
        log.close();
      }
      atualizarRegistros(m, data, criancas);
      atualizarOcorrencias(m, data, quesitos);
      show(m);
      cout << endl;
      m.clear();
    }
    aux.close();
  }
}

// função auxiliar para tratar o buffer de entrada
string trim(const string &str)
{
  size_t first = str.find_first_not_of(' ');
  if (string::npos == first)
  {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

// função que recebe imprime na tela o relatório de uma criança (data e ações realizadas + quantidade
// de vezes que elas foram feitas em cada dia)
void relatorioCrianca(map<string, vector<registro>> dados)
{
  string nomeBusca;
  cout << "Escreva o nome de uma crianca a qual deseje ver o relatorio: ";
  cin.ignore();
  getline(cin, nomeBusca);
  nomeBusca = trim(nomeBusca);
  auto iterator = dados.find(nomeBusca);
  if (iterator != dados.end())
  {
    for (auto &registro : iterator->second)
    {
      cout << endl
           << " " << iterator->first;
      cout << " em " << registro.data << ":" << endl;
      for (auto &variavel : registro.acoes)
      {
        cout << " - " << variavel.first << ": " << variavel.second << endl;
      }
    }
  }
  else
  {
    cout << "Elemento nao encontrado." << endl;
  }
}

// função que retorna a maior quantidade de vezes que um quesito foi realizado
int maior_elemento(vector<criancaDestaque> criancas)
{
  int max = 0;
  for (int i = 0; i < criancas.size(); i++)
  {
    if (criancas[i].quantidade > max)
      max = criancas[i].quantidade;
  }
  return max;
}

// função que verifica e imprime quais crianças se destacaram mais em um determinado quesito
void destaque(string quesito, map<string, vector<ocorrencia>> quesitos)
{
  auto it = quesitos.find(quesito);
  vector<criancaDestaque> destaques;
  if (it != quesitos.end())
  {
    criancaDestaque crianca;
    for (auto &ocorr : it->second)
    {
      crianca.nome = ocorr.nomeCrianca;
      crianca.quantidade = 0;
      for (auto &valor : ocorr.listaQuantidades)
      {
        crianca.quantidade += valor.second;
      }
      destaques.push_back(crianca);
    }
  }
  else
  {
    cout << "Este quesito nao foi encontrado. Nao ha destaques" << endl;
  }

  int maximoValor = maior_elemento(destaques);
  cout << endl
       << "Destaques do quesito " << quesito << ":" << endl
       << endl;
  for (int i = 0; i < destaques.size(); i++)
  {
    if (destaques[i].quantidade == maximoValor)
    {
      cout << destaques[i].nome << " com total de " << destaques[i].quantidade << endl;
    }
  }
}

// função que imprime o relatório de um determinado quesito, incluindo destaques, data,pessoa e quantas vezes foi realizado
// em certo dia por determinada pessoa
void relatorioQuesito(map<string, vector<ocorrencia>> dadosQuesito)
{
  string quesitoBusca;
  cout << "Escreva o nome de um quesito qual deseje ver o relatorio: ";
  cin.ignore();
  getline(cin, quesitoBusca);
  quesitoBusca = trim(quesitoBusca);
  destaque(quesitoBusca, dadosQuesito);
  auto iterator = dadosQuesito.find(quesitoBusca);

  if (iterator != dadosQuesito.end())
  {

    cout << endl
         << "Quesito " << iterator->first << ":" << endl
         << endl;

    for (auto &ocorrencia : iterator->second)
    {

      for (auto &quesitos : ocorrencia.listaQuantidades)
      {

        cout << "- " << ocorrencia.nomeCrianca << " em";
        cout << " " << quesitos.first << ": " << quesitos.second << endl;
      }
    }
  }
  else
    cout << "Elemento nao encontrado." << endl;
}

void salvarDadosBinario(const map<string, vector<registro>>& criancas, const map<string, vector<ocorrencia>>& quesitos, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    // Salvar dados das crianÃ§as
    size_t numCriancas = criancas.size();
    arquivo.write(reinterpret_cast<const char*>(&numCriancas), sizeof(size_t));
    for (const auto& crianca : criancas) {
        size_t nomeSize = crianca.first.size();
        arquivo.write(reinterpret_cast<const char*>(&nomeSize), sizeof(size_t));
        arquivo.write(crianca.first.c_str(), nomeSize);

        size_t numRegistros = crianca.second.size();
        arquivo.write(reinterpret_cast<const char*>(&numRegistros), sizeof(size_t));
        for (const auto& registro : crianca.second) {
            size_t dataSize = registro.data.size();
            arquivo.write(reinterpret_cast<const char*>(&dataSize), sizeof(size_t));
            arquivo.write(registro.data.c_str(), dataSize);

            size_t numAcoes = registro.acoes.size();
            arquivo.write(reinterpret_cast<const char*>(&numAcoes), sizeof(size_t));
            for (const auto& acao : registro.acoes) {
                size_t acaoSize = acao.first.size();
                arquivo.write(reinterpret_cast<const char*>(&acaoSize), sizeof(size_t));
                arquivo.write(acao.first.c_str(), acaoSize);
                arquivo.write(reinterpret_cast<const char*>(&acao.second), sizeof(int));
            }
        }
    }

    // Salvar dados dos quesitos
    size_t numQuesitos = quesitos.size();
    arquivo.write(reinterpret_cast<const char*>(&numQuesitos), sizeof(size_t));
    for (const auto& quesito : quesitos) {
        size_t quesitoSize = quesito.first.size();
        arquivo.write(reinterpret_cast<const char*>(&quesitoSize), sizeof(size_t));
        arquivo.write(quesito.first.c_str(), quesitoSize);

        size_t numOcorrencias = quesito.second.size();
        arquivo.write(reinterpret_cast<const char*>(&numOcorrencias), sizeof(size_t));
        for (const auto& ocorrencia : quesito.second) {
            size_t nomeCriancaSize = ocorrencia.nomeCrianca.size();
            arquivo.write(reinterpret_cast<const char*>(&nomeCriancaSize), sizeof(size_t));
            arquivo.write(ocorrencia.nomeCrianca.c_str(), nomeCriancaSize);

            size_t numQuantidades = ocorrencia.listaQuantidades.size();
            arquivo.write(reinterpret_cast<const char*>(&numQuantidades), sizeof(size_t));
            for (const auto& quantidade : ocorrencia.listaQuantidades) {
                size_t dataSize = quantidade.first.size();
                arquivo.write(reinterpret_cast<const char*>(&dataSize), sizeof(size_t));
                arquivo.write(quantidade.first.c_str(), dataSize);
                arquivo.write(reinterpret_cast<const char*>(&quantidade.second), sizeof(int));
            }
        }
    }

    arquivo.close();
    cout << "Dados salvos em " << nomeArquivo << " com sucesso." << endl;
}

void recuperarDadosBinario(map<string, vector<registro>>& criancas, map<string, vector<ocorrencia>>& quesitos, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }

    // Limpar as estruturas existentes
    criancas.clear();
    quesitos.clear();

    // Recuperar dados das crianÃ§as
    size_t numCriancas;
    arquivo.read(reinterpret_cast<char*>(&numCriancas), sizeof(size_t));
    for (size_t i = 0; i < numCriancas; i++) {
        size_t nomeSize;
        arquivo.read(reinterpret_cast<char*>(&nomeSize), sizeof(size_t));
        string nomeCrianca(nomeSize, ' ');
        arquivo.read(&nomeCrianca[0], nomeSize);

        size_t numRegistros;
        arquivo.read(reinterpret_cast<char*>(&numRegistros), sizeof(size_t));
        vector<registro> registros(numRegistros);
        for (size_t j = 0; j < numRegistros; j++) {
            size_t dataSize;
            arquivo.read(reinterpret_cast<char*>(&dataSize), sizeof(size_t));
            string data(dataSize, ' ');
            arquivo.read(&data[0], dataSize);

            size_t numAcoes;
            arquivo.read(reinterpret_cast<char*>(&numAcoes), sizeof(size_t));
            map<string, int> acoes;
            for (size_t k = 0; k < numAcoes; k++) {
                size_t acaoSize;
                arquivo.read(reinterpret_cast<char*>(&acaoSize), sizeof(size_t));
                string acao(acaoSize, ' ');
                arquivo.read(&acao[0], acaoSize);
                int valor;
                arquivo.read(reinterpret_cast<char*>(&valor), sizeof(int));
                acoes[acao] = valor;
            }

            registros[j] = { data, acoes };
        }

        criancas[nomeCrianca] = registros;
    }

    // Recuperar dados dos quesitos
    size_t numQuesitos;
    arquivo.read(reinterpret_cast<char*>(&numQuesitos), sizeof(size_t));
    for (size_t i = 0; i < numQuesitos; i++) {
        size_t quesitoSize;
        arquivo.read(reinterpret_cast<char*>(&quesitoSize), sizeof(size_t));
        string nomeQuesito(quesitoSize, ' ');
        arquivo.read(&nomeQuesito[0], quesitoSize);

        size_t numOcorrencias;
        arquivo.read(reinterpret_cast<char*>(&numOcorrencias), sizeof(size_t));
        vector<ocorrencia> ocorrencias(numOcorrencias);
        for (size_t j = 0; j < numOcorrencias; j++) {
            size_t nomeCriancaSize;
            arquivo.read(reinterpret_cast<char*>(&nomeCriancaSize), sizeof(size_t));
            string nomeCrianca(nomeCriancaSize, ' ');
            arquivo.read(&nomeCrianca[0], nomeCriancaSize);

            size_t numQuantidades;
            arquivo.read(reinterpret_cast<char*>(&numQuantidades), sizeof(size_t));
            map<string, int> quantidades;
            for (size_t k = 0; k < numQuantidades; k++) {
                size_t dataSize;
                arquivo.read(reinterpret_cast<char*>(&dataSize), sizeof(size_t));
                string data(dataSize, ' ');
                arquivo.read(&data[0], dataSize);
                int valor;
                arquivo.read(reinterpret_cast<char*>(&valor), sizeof(int));
                quantidades[data] = valor;
            }

            ocorrencias[j] = { nomeCrianca, quantidades };
        }

        quesitos[nomeQuesito] = ocorrencias;
    }

    arquivo.close();
    cout << "Dados recuperados do arquivo " << nomeArquivo << " com sucesso." << endl;
}

int main()
{
  cout << "1 - Ler novo arquivo CSV" << endl;
  cout << "2 - Gerar relatorio por crianca" << endl;
  cout << "3 - Gerar relatorio por quesito" << endl;
  cout << "4 - Processamento em lote" << endl;
  cout << "5 - Salvar as estruturas em arquivo binario" << endl;
  cout << "6 - Resgatar as informacoes das estruturas em arquivo binario" << endl;
  cout << "7 - Finalizar o programa" << endl << endl;
  int entrada;
  cin >> entrada;

  map<string, vector<registro>> criancas;   // map que associa cada criança ao seu vector de registros
  map<string, vector<ocorrencia>> quesitos; // map que associa cada quesito ao seu vector de ocorrências

  while (entrada != 7)
  {
    // Lê arquivo e atualiza as estruturas
    if (entrada == 1)
    {
      string filename;
      cout << "Entre como o nome do arquivo: ";
      cin >> filename;
      string data = formataData(filename);
      filename += ".csv";
      vector<vector<string>> matriz;
      readfile(filename, matriz);
      atualizarRegistros(matriz, data, criancas);
      atualizarOcorrencias(matriz, data, quesitos);
    }
    // Imprime relatório de uma criança
    if (entrada == 2)
    {
      relatorioCrianca(criancas);
      cout << endl;
    }
    // Imprime relatório de um quesito
    if (entrada == 3)
    {
      relatorioQuesito(quesitos);
      cout << endl;
    }
    if (entrada == 4)
    {
      string arquivo;
      cout << "Entre com o nome do arquivo txt" << endl;
      cin >> arquivo;
      vector<vector<string>> matriz;
      processamentoLote(arquivo, matriz,criancas,quesitos);
    }
    if (entrada == 5)
    {
      salvarDadosBinario(criancas, quesitos, "indice.dat");
    }
    if (entrada == 6)
    {
      recuperarDadosBinario(criancas, quesitos, "indice.dat");
    } 
    cout << "1 - Ler novo arquivo CSV" << endl;
    cout << "2 - Gerar relatorio por crianca" << endl;
    cout << "3 - Gerar relatorio por quesito" << endl;
    cout << "4 - Processamento em lote" << endl;
    cout << "5 - Salvar as estruturas em arquivo binario" << endl;
    cout << "6 - Resgatar as informacoes das estruturas em arquivo binario" << endl;
    cout << "7 - Finalizar o programa" << endl << endl;
    cin >> entrada;
    if (entrada == 7) {
      remove("indice.dat");
    }
  }
  return 0;
}