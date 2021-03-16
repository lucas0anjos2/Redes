#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED
#include <sys/socket.h> // // Para uso das estruturas e funções de sockets.
#include <arpa/inet.h> // Para uso de funções de conversão de endereços: inet_pton(), inet_ntop(), etc.
#include <iostream> // Biblioteca para operações de I/O.
#include <string.h> // Para uso da função memset() e manipulação de strings.
#include <unistd.h> // Para uso de constantes utilizadas na aplicação e algumas funções.
#include <fstream> // Biblioteca para fluxo de I/O em arquivos externos. Ex.: .csv, .txt.
#include <locale.h> // Para exibição de mensagens com acentuação.
#include "base64.h" // Biblioteca externa para uso de criptografia base64. Disponível em: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
#include "sha256.h" // Biblioteca externa para uso de criptografia sha256. Disponível em: http://www.zedwood.com/article/cpp-sha256-function

#define TAM_BUFFER 500 // Constante para definir tamanho do buffer utilizado na comunicação entre o servidor e o cliente.

using namespace std; // Para acesso a recursos da bibliota standard do C++.

struct Login{ // TAD para armazenar estruturas de login.
    string usuario, senha, nSenha;
};

class Servidor{
public:
    // Funções públicas:
    Servidor(); //Construtor da classe. Cria o socket do servidor e atribiu as devidas configurações.
    void aguardarConexao(); // Coloca o socket do servidor em modo de escuta e aguarda a conexao de algum cliente.
    void rotinaConexao(); // Rotina para quando existirem clientes conectados.
    void desconectar(); // Fecha o socket do cliente.

private:
    // Variáveis privadas:
    int socketServ; // Socket do servidor;
    sockaddr_in servidor; // Endereço do socket do servidor.
    int socketCliente; // Soclet do cliente.
    sockaddr_in cliente; // Endereço do socket do cliente.
    socklen_t clienteSize; // Tamanho do endereço do socket do cliente.
    char buffer[TAM_BUFFER]; // Buffer para troca de informações entre servidor e cliente.
    int bytesRecebidos; // Flag de controle de status para a função recv().
    Login login; // Estrutura de login destinada a armazenar as informações para login vindas do cliente.
    int qtdLinhas; // Contador para armazenar a quantidades de linhas do arquivo de usuários e senhas.

    // Funções privadas:
    int contarLinhascsv(); // Percore o arquivo de usuário e senhas e retorna quantas linhas ele tem.
    bool quebrarLinha(Login &login, string linha); // Separa usuário e senha contidos numa linha e armazena em estrutura de login.
    int autenticarL(); // Verifica usuário e senha armazenados na variável login.
    bool trocarSenha(); // Redefine a senha do usuário informado pelo cliente no arquivo csv.
    void quebrarCabecalho(Login &login, string cabecalho); // Armazena as informações do cabeçalho vindo do cliente em suas variáveis de destino.
    void preencheBuffer(string info); // Preenche o buffer com o valor da string informada como argumento da função.
};


#endif // SERVIDOR_H_INCLUDED
