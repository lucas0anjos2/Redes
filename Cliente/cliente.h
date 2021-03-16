#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED
#include <sys/types.h> // Para uso de constantes e tipos de dados utilizadas na aplicação.
#include <sys/socket.h> // Para uso das estruturas e funções de sockets.
#include <netdb.h> // Para uso de tipos de dados utilizados na aplicação.
#include <arpa/inet.h> //Para uso de funções de conversão de endereços: inet_pton(), inet_ntop(), etc.
#include <iostream> // Biblioteca para operações de I/O.
#include <string.h> // Para uso da função memset() e manipulação de strings.
#include <unistd.h> // Para uso de constantes utilizadas na aplicação e algumas funções.
#include <locale.h> // Para exibição de mensagens com acentuação.
#include "base64.h" // Biblioteca externa para uso de criptografia base64. Disponível em: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp

#define TAM_BUFFER 500 // Constante para definir tamanho do buffer utilizado na comunicação entre o cliente e o servidor.

using namespace std; // Para acesso a recursos da bibliota standard do C++.

struct Login{ // TAD para armazenar estruturas de login.
    string usuario, senha, nSenha;
};

class Cliente{
public:
    // Funções públicas:
    Cliente(int porta, string IP); // Construtor da classe. Cria o socket do cliente e atribiu as devidas configurações.
    bool conectar(); // Manda o cliente conectar com o servidor.
    void rotinaConexao(); // Rotina para quando o cliente estiver conectado.
    void desconectar(); // Fecha o socket do cliente.

private:
    // Variáveis privadas:
    Login login; // Estrutura de login destinada a armazenar as informações para login digitadas pelo usuário.
    int socketCliente; // Socket do cliente.
    sockaddr_in cliente; // Endereço do socket do cliente.
    char buffer[TAM_BUFFER]; // Buffer para troca de informações entre cliente e servidor.
    string usuario, senha; // Variáveis utilizadas para armazenar o usuário e senha informados pelo usuário.
    string comandoUsuario; // Variável utilizada para armazenar o cabeçalho que será enviado ao servidor.
    int bytesRecebidos; // Flag de controle de status para a função recv().

    // Funções privadas:
    bool validarTrocadeSenha(string senha); // Função que verifica se as informações vindas do usuário para a troca de senha estão no formato correto.
    string msgAuth(); // Função que monta o cabeçalho para um requisição de login.
    string msgChgp();// Função que monta o cabeçalho para um requisição de troca de senha.
    // Como a nova senha e a senha atual são passadas pelo usuário em uma só linha quando é requisitado a troca de senha, é preciso separar a informação de senha atual e de nova senha.
    void separarSenhas(string &senha, string &nSenha); // Função para armazenar a nova senha e a senha atual.
    void preencheBuffer(string info); // Preenche o buffer com o valor da string informada como argumento da função.
};

#endif // CLIENTE_H_INCLUDED
