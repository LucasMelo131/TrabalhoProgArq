/*
Integrantes:
- Erick Junqueira Ramos
- Lucas Melo de Carvalho
- Pedro Gabriel Nunes
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>


using namespace std;


//estrutura usada somente para exibição dos destaques de um quesito
//contém nome da criança e a quantidade de vezes que realizou um quesito no total
typedef struct CriancaDestaque {
    string nome;
    int quantidade;
}criancaDestaque;

//Tipo de dado registro que possui data e todas as açoes realizadas pela criança neste dia
typedef struct Registro
{

    string data;
    map<string,int> acoes; // nome do quesito -> quantidade de vezes 

}
registro;

//tipo de dados ocorrencia que armazena o nome da criança e todas as vezes que ela fez uma determinada ação
typedef struct Ocorrencia {

    string nomeCrianca;
    map<string,int> listaQuantidades; // data -> quantidade de vezes (uma lista disso pra cada criança)

}
ocorrencia;

//função que recebe o nome do arquivo sem a extensão e retorna a data no formato dd/mm/aaaa
string formataData (string data) {
  if(data.size() == 8 ) {
    string new_data = data.substr(0,2) + "/" + data.substr(2,2) + "/" + data.substr(4,4);
    return new_data;
  } else {
    return data;
  }
}


//função que lê um arquivo csv e armazena o conteúdo numa matriz 
void readfile (string nomeArquivo, vector<vector<string>> &matriz) {
    
    string line,word; 
    vector<string> row;
    fstream file;
    file.open(nomeArquivo,ios::in);
    if (!file) {
        cout << "Arquivo nao existe" << endl;
    }
    else {
        while(getline(file,line)) {
            row.clear();
            stringstream str(line);
            while(getline(str,word,',')) {
                row.push_back(word);
            }
            matriz.push_back(row);
        }
        cout << "Arquivo " << nomeArquivo << " lido e estruturas atualizadas." << endl << endl;
    }
    file.close();
}

//função que atualiza o map que associa o nome da criança ao seu vector de registros
void atualizarRegistros (vector<vector<string>> info, string data, map<string,vector<registro>> &criancas) 
{
    registro reg;
    string nome;

    reg.data = data;
    vector<registro> registros;
    for (int i = 0; i < info.size(); i++) {
        (reg.acoes).clear();
        registros.clear();
        nome = info[i][0];
        //Caso a criança ainda não esteja no map criamos um campo dela e seu vector de registros, carregando
        //as informações do arquivo lido em uma variavel registro e inserindo-a no vector
        if (criancas.find(nome) == criancas.end()) {
            for (int j = 1; j < info[i].size(); j++) {
                reg.acoes[info[i][j]] = stoi(info[i][j+1]);
                j++;
            }
            registros.push_back(reg);
            criancas[nome] = registros;
        }
        //Caso a criança já exista no map, apenas adiciona o registro ao vector de registros
        else {
            for (int j = 1; j < info[i].size(); j++) {
                reg.acoes[info[i][j]] = stoi(info[i][j+1]);
                j++;
            }
            criancas[nome].push_back(reg);
        }
    }
}

//função que busca por uma criança no vector de ocorrencias e retorna o índice no qual ela e suas 
//ocorrencias se encontram
int buscaCrianca (string nome,vector<ocorrencia> ocorr) {
    for (auto it = ocorr.begin(); it != ocorr.end(); it++) {
        if(it->nomeCrianca == nome) {
            return distance(ocorr.begin(),it);
        }
    }
    return -1;
}

//função que atualiza o map que relaciona o nome do quesito ao seu vector de ocorrências
void atualizarOcorrencias (vector<vector<string>> info, string data, map<string,vector<ocorrencia>> &quesitos) {
    ocorrencia acontecimento;
    string quesito;
    vector<ocorrencia> ocorrencias;
    for (int i = 0; i < info.size(); i++) {
        acontecimento.nomeCrianca = info[i][0];
        for (int j = 1; j < info[i].size(); j++) {
            quesito = info[i][j];
            //se o quesito ainda não foi criado no map, criamos ele e seu vector de ocorrencias inserindo a ocorrencia registrada
            //no vector de ocorrências
            if (quesitos.find(quesito) == quesitos.end()) {
                acontecimento.listaQuantidades[data] = stoi(info[i][j+1]);
                j++;
                ocorrencias.push_back(acontecimento);
                quesitos[quesito] = ocorrencias;
                ocorrencias.clear();
                (acontecimento.listaQuantidades).clear();
            }
            else {
                int ind = buscaCrianca(acontecimento.nomeCrianca,quesitos[quesito]);
                //se a criança não está no vector de ocorrências criamos uma ocorrência que contem seu nome
                //e um map com a data e quantidade de vezes que realizou o quesito
                if  (ind == -1) {
                    acontecimento.listaQuantidades[data] = stoi(info[i][j+1]);
                    j++;
                    quesitos[quesito].push_back(acontecimento);
                    (acontecimento.listaQuantidades).clear();
                //caso a ocorrência com a criança já exista no vector, apenas adicionamos data->quantidade de vezes
                //na ocorrência
                } else {
                    (quesitos[quesito])[ind].listaQuantidades[data] = stoi(info[i][j+1]);
                    j++;
                }       
            }
        }
    }

}

//função auxiliar para tratar o buffer de entrada
string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

//função que recebe imprime na tela o relatório de uma criança (data e ações realizadas + quantidade
//de vezes que elas foram feitas em cada dia)
void relatorioCrianca(map<string,vector<registro>> dados){
    string nomeBusca;
    cout<<"Escreva o nome de uma crianca a qual deseje ver o relatorio: ";
    cin.ignore();
    getline(cin,nomeBusca);
    nomeBusca = trim(nomeBusca);
    auto iterator = dados.find(nomeBusca);    
    if (iterator != dados.end())
    {
        for (auto& registro : iterator->second)
        {
            cout <<endl<< " "<< iterator->first;
            cout << " em " << registro.data << ":"<< endl;
            for (auto& variavel : registro.acoes)
            {
                cout << " - " << variavel.first << ": " << variavel.second<<endl;
            }
        }
    }
    else
    {
        cout << "Elemento nao encontrado." << endl;
    }
}

//função que retorna a maior quantidade de vezes que um quesito foi realizado
int maior_elemento (vector<criancaDestaque> criancas) {
    int max = 0;
    for (int i = 0; i < criancas.size() ; i++) {
        if (criancas[i].quantidade > max) max = criancas[i].quantidade;
    }
    return max;
}

//função que verifica e imprime quais crianças se destacaram mais em um determinado quesito
void destaque (string quesito,map<string,vector<ocorrencia>> quesitos ) {
    auto it = quesitos.find(quesito);
    vector<criancaDestaque> destaques;
    if (it != quesitos.end() ) {
        criancaDestaque crianca;
        for (auto& ocorr : it->second ) {
            crianca.nome = ocorr.nomeCrianca;
            crianca.quantidade = 0;
            for (auto& valor : ocorr.listaQuantidades) {
                crianca.quantidade += valor.second;
            }
            destaques.push_back(crianca);
        }
    }
    else {
        cout << "Este quesito nao foi encontrado. Nao ha destaques" << endl;
    }


    int maximoValor = maior_elemento(destaques);
    cout << endl <<"Destaques do quesito " << quesito << ":" <<endl<<endl;
    for (int i = 0; i < destaques.size() ; i++) {
        if (destaques[i].quantidade == maximoValor) {
            cout << destaques[i].nome << " com total de " << destaques[i].quantidade << endl;
        }
    }
    
} 

//função que imprime o relatório de um determinado quesito, incluindo destaques, data,pessoa e quantas vezes foi realizado
//em certo dia por determinada pessoa
void relatorioQuesito(map<string,vector<ocorrencia>> dadosQuesito){
    string quesitoBusca;
    cout<<"Escreva o nome de um quesito qual deseje ver o relatorio: ";
    cin.ignore();
    getline(cin,quesitoBusca);
    quesitoBusca = trim(quesitoBusca);
    destaque(quesitoBusca,dadosQuesito);
    auto iterator = dadosQuesito.find(quesitoBusca);    
    
    if (iterator != dadosQuesito.end()){

        cout <<endl<< "Quesito "<< iterator->first<<":"<<endl<<endl;
        
        for (auto& ocorrencia : iterator->second){

            for (auto& quesitos : ocorrencia.listaQuantidades){

                cout << "- " << ocorrencia.nomeCrianca << " em";
                cout << " " << quesitos.first <<": " << quesitos.second<<endl;
                
            }
        }
    
    }
    else 
      cout << "Elemento nao encontrado." << endl;
    
}

int main() {
    cout << "1 - Ler novo arquivo CSV" << endl;
    cout << "2 - Gerar relatorio por crianca" << endl;
    cout << "3 - Gerar relatorio por quesito" << endl;
    cout << "4 - Finalizar o programa" << endl;
    int entrada; cin >> entrada;

    map<string,vector<registro>> criancas; //map que associa cada criança ao seu vector de registros
    map <string,vector<ocorrencia>> quesitos; //map que associa cada quesito ao seu vector de ocorrências

    while (entrada != 4) {
        //Lê arquivo e atualiza as estruturas
        if (entrada == 1) {
            string filename;
	        cout<<"Entre como o nome do arquivo: ";
	        cin>>filename;
            string data = formataData(filename);
            filename += ".csv";
            vector<vector<string>> matriz;
            readfile(filename,matriz);
            atualizarRegistros(matriz,data,criancas);
            atualizarOcorrencias(matriz,data,quesitos);

        }
        //Imprime relatório de uma criança
        if (entrada == 2) {
            relatorioCrianca(criancas);
            cout << endl;
        }
        //Imprime relatório de um quesito
        if (entrada == 3) {
            relatorioQuesito(quesitos);
            cout << endl;
        }
        cout << "1 - Ler novo arquivo CSV" << endl;
        cout << "2 - Gerar relatorio por crianca" << endl;
        cout << "3 - Gerar relatorio por quesito" << endl;
        cout << "4 - Finalizar o programa" << endl;
        cin >> entrada;
    }
    return 0;
}