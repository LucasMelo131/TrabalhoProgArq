#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

/* Trab arquivo 2 parte o que fazer :

- Primeira funcionalidade: processar CSVs em lote
i)  O arquivo não existir ou não puder ser aberto (x)
ii) Uma criança aparecer mais de uma vez (em linhas diferentes) no mesmo arquivo
iii) A quantidade de vezes que uma criança executou um quesito não existir ou não
     for um número inteiro positivo

- Segunda funcionalidade: armazenar as estruturas vector<registro> e vector<ocorrencia> num arquivo binário (.dat)
- Terceira funcionalidade: recuperar as informações salvas em um arquivo binário
  as estruturas existentes naquele momento deve ser limpas/esvaziadas
  e devem ser construídas a partir das informações vindas do arquivo binário.

- Opcional: modularizar o programa (fazer os .h)

- Manter o programa original e criar as novas funcionalidades.

*/

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

int readfile2 (string nomeArquivo, vector<vector<string>> &matriz, string &crianca)
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
        while (getline(str, word, ','))
        {
          row.push_back(word);
        }
        matriz.push_back(row);
      }
      else
      {
        crianca = word;
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

void processamentoLote(string arq, vector<vector<string>> &m)
{
  string name = arq + ".txt", linha;
  fstream aux;
  aux.open(name, ios::in);
  if (!aux)
  {
    cout << "O arquivo " << name << " nao existe." << endl;
  }
  else
  {
    while (getline(aux, linha))
    {
      linha += ".csv";
      string kid;
      int check = readfile2(linha, m, kid);
      if (check == 0)
      {
        fstream log;
        log.open("log.txt", ios::app);
        log << "Arquivo: " << linha << endl;
        log << "Linha: 0" << endl;
        log << "O arquivo nao existe ou nao pode ser aberto" << endl
            << endl;
      }
      if (check == 2)
      {
        fstream log;
        log.open("log.txt", ios::app);
        log << "Arquivo: " << linha << endl;
        log << "Linha: " << m.size() + 1 << endl;
        log << "A crianca " << kid << " se repete em linhas diferentes no arquivo" << endl
            << endl;
      }
      show(m);
      cout << endl;
      m.clear();
    }
  }
}

int main()
{
  vector<vector<string>> m;
  processamentoLote("entrada", m);
  return 0;
}