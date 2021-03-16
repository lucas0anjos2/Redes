#include <iostream>
#include <fstream>
#include "sha256.h" // Licenciado por zedwood.com. Disponível em: http://www.zedwood.com/article/cpp-sha256-function

using namespace std;

int contarLinhascsv(string nomeArquivo){
    int retorno = 1;
    ifstream arquivo;
    string aux;
    arquivo.open(nomeArquivo.c_str());
    if(arquivo.is_open()){
        while(getline(arquivo, aux)){
            retorno++;
        }
    }else{
        retorno = -1;
    }
    return retorno;
}

int main()
{
    int linhas = contarLinhascsv("senhas.txt");
    if(linhas == -1){
        cout << "nao foi possivel abrir o arquivo" << endl;
    }else{
        string senhas[linhas];
        ifstream arquivoL;
        arquivoL.open("senhas.txt");
        if(arquivoL.is_open()){
            string linha;
            for(int i = 0; i < linhas; i++){
                getline(arquivoL, linha);
                senhas[i] = linha;
            }
        }
        arquivoL.close();
            ofstream arquivoE;
        arquivoE.open("senhasEncriptadas.txt", ios::trunc);
        if(arquivoE.is_open()){
            string linha;
            for(int i = 0; i < linhas - 1; i++){
                linha = sha256(senhas[i]);
                linha.append("\n");
                cout << "decoded: " << senhas[i] << endl;
                cout << "encoded: " << linha << endl;
                arquivoE << linha;
                linha.clear();
            }
        }else{
            cerr << "erro ao abrir arquivo\n";
        }
        arquivoE.close();
    }



    return 0;
}
