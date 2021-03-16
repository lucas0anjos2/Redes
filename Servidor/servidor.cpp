#include "servidor.h"

int Servidor::contarLinhascsv(){
    int retorno = 0; // Retorno da função. Inicializar em zero para evitar lixo de memória.
    ifstream arquivo; // Variável de leitura para ler o arquivo csv.
    string linha; // Variável para armazenar texto de uma linha do arquivo csv.
    arquivo.open("logins.csv");
    if(arquivo.is_open()){ // Se o arquivo for aberto com sucesso.
        while(getline(arquivo, linha)){ // Enquanto existirem linhas no arquivo csv.
            retorno++; // Incrementar o valor do retorno.
        }
    }else{ // Se não for possível abrir o arquivo.
        retorno = -1; // Variável de retorno é marcada com a flag -1.
        cerr << "Não foi possível abrir o arquivo !" << endl;
    }
    return retorno;
}

bool Servidor::quebrarLinha(Login &login, string linha){
    int separador = 0; // Índice do separador. Inicializar em zero para evitar lixo de memória.
    bool achou = false; // Flag para saber se o separador foi encontrado.
    bool separou = false; // Retorno da função.
    for(int i = 0; i < linha.size() && achou != true; i++){ // Percorrer linha.
        if(linha[i] == ':'){ // Se for encontrado o separador.
            separador = i; // O índice é armazenado.
            achou = true; // Flag é marcado com true.
        }
    }
    if(achou){ // Caso o separador tenha sido encontado.
        login.usuario = string(linha, 0, separador); // Armazenar o usuário na estrutura de login.
        login.senha = string(linha, separador + 1, linha.size() - separador - 1); // Armazenar a senha na estrutura de login.
        separou = true; // Marcar o retorno da função como true.
    }
    return separou;
}

void Servidor::preencheBuffer(string info){
    memset(buffer, 0, 500); // Retira qualquer valor contido anteriormente no buffer.
    for(int i = 0; i < info.size(); i++){ // Percorre a string informada.
        buffer[i] = info[i]; // Armazena cada carácter no buffer.
    }
}

void Servidor::quebrarCabecalho(Login &login, string cabecalho){
    int esp1, esp2, esp3, bn1, bn2, bn3; // Variáveis de controle. Armazenarão índices de caracteres como espaços em branco ou quebra de linha.
    int cont; // Contador para saber qual variável de controle estou utilizando.
    cont = 1; // Contar qual espaço em branco (' ') está sendo armazenado o índice.
    for(int i = 0; i < cabecalho.size(); i++){ // Percorrer cabeçalho.
        if(cabecalho[i] == ' '){ // Quando forem encontrados espaços em branco.
            if(cont == 1){ // Se for a primeira ocorrência armazenar o índice na variável de espaço 1 e incrementar o contador.
                esp1 = i;
                cont++;
            }else if(cont == 2){ // Se for a segunda ocorrência armazenar o índice na variável de espaço 2 e incrementar o contador.
                esp2 = i;
                cont++;
            }else if(cont == 3 && cabecalho[0] == 'c'){ // Caso o cabeçalho seja de troca de senha, preencher o terceiro marcador de espaços e incrementar o contador.
                esp3 = i;
                cont++;
            }
        }
    }
    cont = 1; // Contando qual quebra de linha ('\n') está sendo armazenada o índice.
    for(int i = 0; i < cabecalho.size(); i++){ // Percorrer cabeçalho.
        if(cabecalho[i] == '\n'){ // Quando forem encontrados quebras de linha.
            if(cont == 1){ // Se for a primeira ocorrência armazenar o índice na variável de quebra de linha 1 e incrementar o contador.
                bn1 = i;
                cont++;
            }else if(cont == 2){ // Se for a segunda ocorrência armazenar o índice na variável de quebra de linha 2 e incrementar o contador.
                bn2 = i;
                cont++;
            }else if(cont == 3){ // Se for a terceira ocorrência armazenar o índice na variável de quebra de linha 3 e incrementar o contador.
                bn3 = i;
                cont++;
            }
        }
    }
    login.usuario = string(cabecalho, esp1 + 1, bn2 - esp1 - 1); // Obter usuário do cabeçalho.
    login.senha = string(cabecalho, esp2 + 1, bn3 - esp2 - 1); // Obter senha do cabeçalho.
    if(cabecalho[0] == 'c'){ // Caso o cabeçalho seja de troca de senha, preencher o campo nova senha da estrutura de login.
        login.nSenha = string(cabecalho, esp3 + 1, cabecalho.size() - esp3 -2); // Obter a nova senha do cabeçalho.
    }else{ // Caso o cabeçalho não seja de troca de senha.
        login.nSenha = "-1";  // Marcar a nova senha da estrutura de login com a flag -1.
    }
}


Servidor::Servidor(){
    qtdLinhas = 0; // Inicializar contador em zero para evitar lixo de memória.
    qtdLinhas = contarLinhascsv(); // Preencher a variável com o número de linhas do arquivo de logins e senhas.
    socketServ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Inicializando socket do servidor.
    servidor.sin_family = AF_INET; // Definindo padrao ipv4.
    servidor.sin_port = htons(54321); // Definir porta a ser usada.
    servidor.sin_addr.s_addr = htonl(INADDR_ANY); // Vincular com todos os endereços locais.
    if(socketServ == -1 || qtdLinhas == -1){
    	cerr << "Não foi possível criar o socket do servidor !" << endl;
    }else{
        int resultado = bind(socketServ, (sockaddr*)&servidor, sizeof(servidor)); // Variável para checar se o endereço do socket foi definido com sucesso.
        if(resultado == -1){ // Caso ocorra algum erro na função bind().
            cerr << "Não foi possível configurar o socket do servidor !" << endl;
        }else{
            cout << "Socket do servidor inicializado !" << endl;
        }
    }
}

void Servidor::aguardarConexao(){
	int resultado = listen(socketServ, SOMAXCONN); // Armazenar resultado da tentativa de colocar o socket do servidor em modo de escuta.
	if(resultado == -1){
		cerr << "Não foi possível definir o socket para o modo de escuta !" << endl;
	}else{
		cout << "Servidor aguardando conexões !" << endl;
	}
    clienteSize = sizeof(cliente); // Armazenar tamanho do endereço do socket do cliente.
	socketCliente = accept(socketServ, (sockaddr*)&cliente, &clienteSize); // Aceitar a conexão vinda do cliente.
	if(socketCliente == -1){ // Se ocorrer algum erro ao aceitar a conexão do cliente.
        cerr << "O Cliente não pôde ser conectado !" << endl;
    }else{
        cout << "Cliente Conectado !" << endl;
    }
    close(socketServ); // fechar o socket do servidor.
}

/*
bool trocarSenha():
Retorna true se a senha for trocada com sucesso.
Retorna false se ocorrer algum erro.
Observações:
    - Dados[qtdLinhas][2] é uma matriz N x 2, com N sendo o número de linhas do csv.
    - Este arquivo armazenará na primeira coluna os usuários e na segunda as senhas.
    - Esta função só sera chamada depois que o usuário e senha já foram autenticados, então não é necessário
      checar se o usuário e senha informados são validos (estão cadastrados no csv).
*/
bool Servidor::trocarSenha(){
    bool trocou = false; // Retorno da função.
    string dados[qtdLinhas][2]; // Matriz para carregar para a memória os dados do arquivo csv.
    string linha; // String para armazenar a linha de texto do arquivo csv.
    ifstream arquivo; // Variável de leitura para abrir e ler o csv dos usuários e senhas.
    arquivo.open("logins.csv"); // Abrir o arquivo csv.
    if(arquivo.is_open()){ // Se o arquivo for aberto com sucesso.
        for(int i = 0; i < qtdLinhas; i++){ // Percorrer csv trazendo linha a linha.
            getline(arquivo, linha); // Função para capturar a linha do arquivo e armazenar na variável linha.
            Login logLinha; // Estrutura de login auxiliar para armazenar o login armazenado na linha.
            if(quebrarLinha(logLinha, linha)){ // Tentar copiar o usuário e senha da linha e armazenar numa estrutura de login.
                dados[i][0] = logLinha.usuario; // Primeira coluna recebe usuários.
                dados[i][1] = logLinha.senha; // Segunda coluna recebe senhas encriptadas em sha256.
            }else{ // Caso ocorra algum erro durante a obtenção dos valores da linha.
                cerr << "Não foi possível quebrar a linha !" << endl;
            }
        }
    }
    arquivo.close(); // Fecho o arquivo pois ja terminei de ler o que queria.
    // Encontrar em dados[N][2] o usuário fornecido pelo cliente.
    for(int i = 0; i < qtdLinhas; i++){
        if(dados[i][0] == login.usuario){ // Quando o usuário for encontrado.
            dados[i][1].clear(); // limpo na matriz o valor da antiga senha armazenada.
            login.nSenha = base64_decode(login.nSenha); // Decodifico a nova senha (já que ela veio em base64).
            dados[i][1] = sha256(login.nSenha); // Armazeno a nova senha encriptada em sha256.
        }
    }
    // Agora que a senha foi alterada, gravarei a matriz de volta no csv.
    ofstream arquivoS; // Variável de escrita para escrever no csv.
    arquivoS.open("logins.csv", ios::trunc); // Abro o csv de maneira a sobrescrever o conteúdo que existia anteriormente.
        if(arquivoS.is_open()){ // Se o arquivo for aberto com sucesso.
            for(int i = 0; i < qtdLinhas; i++){ // Gravar o mesmo numero de linhas que existiam anteriormente.
                if(i == qtdLinhas - 1){ // Se o loop estiver na última linha.
                    // Não é preciso gravar a quebra de linha.
                    linha.clear(); // limpo o conteúdo da variável linha.
                    // Agora concatenar as informações no formato que o csv armazena (usuário:senha).
                    linha = dados[i][0]; // Usuário.
                    linha.append(":"); // Carácter ":".
                    linha.append(dados[i][1]); // Senha já encriptada em sha256.
                    arquivoS << linha; // Gravar no csv a linha já formatada.
                }else{ // Se o loop não estiver na última linha.
                    linha.clear(); // limpar o conteúdo da variável linha.
                    // Agora concatenar as informações no formato que o csv armazena (usuário:senha).
                    linha = dados[i][0]; // Usuário.
                    linha.append(":"); // Carácter ":".
                    linha.append(dados[i][1]); // Senha já encriptada em sha256.
                    linha.append("\n"); // Para pular uma linha ao ser gravado no csv.
                    arquivoS << linha; // Gravar no csv a linha já formatada.
                }
            }
            trocou = true; // Se tudo ocorreu como deveria marcar o valor do retorno para true;
            arquivoS.close(); // Fechar o csv pois já foi gravado tudo o que era necessário.
        }
    return trocou;
}

/*
autenticarL():
retorno = 1: autenticacao bem sucedida
retorno = 0: usuario nao encontrado
retorno = -1: usuario encontrado porem senha errada
*/
int Servidor::autenticarL(){
    int retorno = 0;//retorno da funcao
    bool achouU = false; //flag para controle da busca de usuario
    ifstream arquivo;
    string linha; //string para armazenar linha do arquivo
    arquivo.open("logins.csv");
    if(arquivo.is_open()){ // se o arquivo for aberto com sucesso
        for(int i = 0; i < qtdLinhas && achouU != true; i++){ // percorrer arquivo buscando usuario
            getline(arquivo, linha);
            //checar se minha linha contem o usuario fornecido pelo cliente
            Login loginLinha; // estrutura de login auxiliar para armazenar linha em que estou
            quebrarLinha(loginLinha, linha); // preencho loginLinha com o usuario e senha encontrado na linha do arquivo
            if(loginLinha.usuario == login.usuario){ // se encontrar o usuario informado pelo cliente no arquivo csv
                achouU = true;
                string decoded64_senha = base64_decode(login.senha); // decodificar a senha que chega em base 64
                //codificar em sha256 para a comparacao com o arquivo csv
                string encodedsha256_senha = sha256(decoded64_senha);
                if(loginLinha.senha == encodedsha256_senha){ // checar senha
                    retorno = 1;
                    decoded64_senha.clear();
                    encodedsha256_senha.clear();
                }else{
                    retorno = -1;
                    cerr << "Usuário encontrado porém senha errada !" << endl;
                }
            }
            loginLinha.usuario.clear();
            loginLinha.senha.clear();
        }
    }else{
        cerr << "Não foi possível abrir o arquivo !" << endl;
    }
    return retorno;
}

void Servidor::rotinaConexao(){
	while(1){ // Loop infinito para interagir com clientes.
		memset(buffer, 0, TAM_BUFFER); // Limpar o buffer para garantir que não existão lixos de memória ou qualquer informação indesejada.
		bytesRecebidos = recv(socketCliente, buffer, TAM_BUFFER, 0); // Receber cabeçalhos do Cliente.
        if(bytesRecebidos == -1){ // Caso ocorra erro na função recv().
            cerr << "Ocorreu um erro na recepção dos dados !" << endl;
            break;
        }
        if(bytesRecebidos == 0){ // Caso o cliente seja desconectado.
            cout << "O cliente foi desconectado !" << endl;
            break;
        }
        cout << "\nRecebido:\n" << buffer; // Mostrar o conteúdo vindo do cliente.
        string buff(buffer); // Converter o buffer para string para uso dos métodos da classe string.
        quebrarCabecalho(login, buff); // Armazenar as informações do cabeçalho.
        int autenticacao = autenticarL(); // Tentar logar com o usuário e senha informados pelo cliente.
        switch(autenticacao){ // Checar possibilidades de autenticação.
            case 1: // Se o login deu certo.
                // Verificar se o cliente quer fazer login ou trocar senha.
                if(login.nSenha == "-1"){ // Caso o cliente não tenha requisitado trocar senha.
                    preencheBuffer("200 Auth OK\n\nA fechadura foi liberada !\n\n"); // Preencher buffer com mensagem de autenticação ok.
                }else{ // Caso o cliente tenha requisitado trocar senha.
                    if(trocarSenha()){ // Tentar realizar a troca da senha pela nova senha informada.
                        preencheBuffer("250 Pass changed\n\n"); // Preencher buffer com mensagem de senha trocada com sucesso.
                    }else{
                        cerr << "Ocorreu algum erro na troca da senha !" << endl;
                    }
                }
                break;
            case 0: // Se o usuário não for encontrado.
                preencheBuffer("450 User doesn't exists\n\n"); // Preencher buffer com mensagem de usuário inexistente.
                break;
            case -1: // Se o usuário for encontrado porém a senha estiver errada.
                preencheBuffer("401 Not authorized\n\n"); // Preencher buffer com mensagem de falha na autenticaçao.
            break;
        }
        autenticacao = -2;
        send(socketCliente, buffer, TAM_BUFFER, 0); // Ao fim das checagens enviar o buffer ao cliente.
	}
}

void Servidor::desconectar(){
	close(socketCliente); // Fechar socket do cliente.
}
