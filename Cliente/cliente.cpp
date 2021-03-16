#include "cliente.h"

Cliente::Cliente(int porta, string IP){
	socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Inicializando socket do cliente.
	cliente.sin_family = AF_INET; // Definir padrao ipv4.
	cliente.sin_port = htons(porta); // Definir porta a ser usada.
	cliente.sin_addr.s_addr = inet_addr(IP.c_str()); // Vincular com o IP do host.
    if(socketCliente == -1){
    	cerr << "Não foi possível criar o socket do cliente !" << endl;
    }else{
    	cout << "Socket do cliente inicializado !" << endl;
    }
}

bool Cliente::conectar(){
	int resultado = connect(socketCliente, (sockaddr*)&cliente, sizeof(cliente)); // Tenta conectar o cliente com o servidor.
	if(resultado == -1){ // Caso ocorra erro na execução da função de conexão.
		cerr << "Não foi possível conectar-se ao servidor !" << endl;
		close(socketCliente); // Fechar o socket do cliente.
		return false;
	}else{
		cout << "Cliente conectado !" << endl;
		return true;
	}
}

void Cliente::separarSenhas(string &senha, string &newSenha){
	int aste; // Variável para armazenar o índice em que se encontra o carácter '*'.
	for(int i = 0; i < senha.size(); i++){ // percorrer senha
		if(senha[i] == '*'){ // buscar indice do "*"
			aste = i; // Armazenar índice.
		}
	}
	newSenha = string(senha, aste + 1, senha.size() - aste - 2); // Obter a nova senha.
	senha = string(senha, 0, aste); // Obter a senha atual.
}

void Cliente::preencheBuffer(string info){
    memset(buffer, 0, 500); // Retira qualquer valor contido anteriormente no buffer.
    for(int i = 0; i < info.size(); i++){ // Percorre a string informada.
        buffer[i] = info[i]; // Armazena cada carácter no buffer.
    }
}

bool Cliente::validarTrocadeSenha(string senha){
	int ocorrenciasAste = 0; // Contador para ocorrências do carácter '*'.
	int ocorrenciashHashtag = 0; // Contador para ocorrências do carácter '#'.
	for(int i = 0; i < senha.size(); i++){ // Percorrer parâmetro senha.
		if(senha[i] == '*'){ // Verificar se existe o separador das senhas e quantas ocorrências existem.
			ocorrenciasAste++; // Incrementar contador.
		}
		if(senha[senha.size() - 1] == '#'){ // Verificar se o parâmetro termina com o carácter '#'.
			if(senha[i] == '#'){ // Verificar ocorrências do carácter '#'.
				ocorrenciashHashtag++; // Incrementar contador.
			}
		}
	}
	if(ocorrenciasAste == 1 && ocorrenciashHashtag == 1){ // Caso existam apenas uma ocorrência de cada e o parâmetro senha terminar com o carácter '#'.
		return true;
	}else{ // Caso o parâmetro senha não passe na validação.
		return false;
	}
}

void Cliente::rotinaConexao(){
	while(1){ // Loop infinito para interagir com o servidor e com o usuário.
		cout << "Digite o usuário e tecle Enter. Digite a senha e tecle Enter." << endl;
		cin >> usuario; // Armazenar usuário informado pelo usuário.
		cin >> senha;	// Armazenar senha informada pelo usuário.
		if(usuario == "sair" || senha == "sair"){ // Caso o usuário digite sair em qualquer campo.
			desconectar(); // Desconectar do servidor.
			break; // Sair do loop infinito.
		}
		/*Como estou dentro de uma rotina, as variáveis que armazenam dados vindos do usuário precisam ser limpas a cada iteração */
		comandoUsuario.clear();
		login.usuario.clear();
		login.senha.clear();
		login.nSenha.clear();
		if(usuario[0] == '*'){ // Se o usuário quiser redefinir sua senha.
			// Checar se a redefinição de senha foi digitada no formato correto (senha*novasenha#).
			if(validarTrocadeSenha(senha)){
				// Separar nova senha de senha atual, armazenar e montar cabeçalho.
				login.usuario = string(usuario, 1, usuario.size() - 1); // Armazenar a senha ignorando a primeira posição, já que corresponde ao carácter '*'.
				// Como a variável senha é armazenada no formato: senha*novaSenha#, é preciso separar a senha atual da nova senha
				string novaSenha; // Variável auxiliar para ober a nova senha.
				separarSenhas(senha, novaSenha); // Obtenho a nova senha e armazeno na variável auxiliar.
				login.senha = base64_encode(reinterpret_cast<const unsigned char*>(senha.c_str()), senha.size()); // Armazenar senha encriptada em base64.
				login.nSenha = base64_encode(reinterpret_cast<const unsigned char*>(novaSenha.c_str()), novaSenha.size()); // Armazenar nova senha encriptada em base64.
				novaSenha.clear(); // Limpar variável que continha a nova senha.
				comandoUsuario = string(msgChgp()); // Gerar o cabeçalho que será enviado ao servidor.
			}else{
				comandoUsuario = "-1";
				cerr << "Informações digitadas em um formato inválido !" << endl;
			}
		}else{ // Se o usuário quiser logar.
			// Armazenar usuário e senha para montar cabeçalho.
			login.usuario = usuario; // Armazenar usuário.
			login.senha = base64_encode(reinterpret_cast<const unsigned char*>(senha.c_str()), senha.size()); // Armazenar senha encriptada em base64.
			login.nSenha = string("-1"); // Marcar variável da nova senha com flag -1 já que não será utilizada.
			comandoUsuario = string(msgAuth()); // Gerar o cabeçalho que será enviado ao servidor.
		}

		if(comandoUsuario != "-1"){
			preencheBuffer(comandoUsuario);
			int resultado = send(socketCliente, buffer, TAM_BUFFER, 0); // Tentar enviar o buffer para o servidor.
			if(resultado > 0){ // Caso não ocorra nenhum erro.
				memset(buffer, 0, TAM_BUFFER); // Limpar o buffer.
				bytesRecebidos = recv(socketCliente, buffer, TAM_BUFFER, 0); // Tentar receber resposta do servidor.
				if(bytesRecebidos > 0){ // Caso não ocorra nenhum erro.
					cout << buffer; // Exibir o que foi recebido do servidor.
				}
			}
		}


	}
}

void Cliente::desconectar(){
	close(socketCliente); // Fechar socket do cliente.
}

/* msgAuth():
	Observação:
		- A senha utilizada para montar o cabeçalho já é inserida codificada em base 64.
		- A nova senha não será utilizada, portanto não entra no cabeçalho. */
string Cliente::msgAuth(){
    string retorno; // Variável para armazenar o retorno da função.
	retorno.append("auth\n"); // Inserir título do cabeçalho seguido de quebra de linha.
    retorno.append("user: "); // Inserir identificação do usuário.
    retorno.append(login.usuario); // Inserir usuário.
    retorno.append("\n"); // Inserir quebra de linha.
    retorno.append("pass: "); // Inserir identificação da senha.
    retorno.append(login.senha); // Inserir a senha.
    retorno.append("\n"); // Inserir quebra de linha.
    return retorno; // Retornar cabeçalho montado.
}

/* msgChgp():
	Observação:
		- A senha utilizada para montar o cabeçalho já é inserida codificada em base 64.
		- A nova senha utilizada para montar o cabeçalho já é inserida codificada em base 64. */
string Cliente::msgChgp(){
    string retorno;
	retorno.append("changepass\n"); // Inserir título do cabeçalho seguido de quebra de linha.
    retorno.append("user: "); // Inserir identificação do usuário.
    retorno.append(login.usuario); // Inserir usuário.
    retorno.append("\n"); // Inserir quebra de linha.
    retorno.append("pass: "); // Inserir identificação da senha.
    retorno.append(login.senha); // Inserir a senha.
    retorno.append("\n"); // Inserir quebra de linha.
    retorno.append("newpass: "); // Inserir identificação da nova senha.
    retorno.append(login.nSenha); // Inserir a nova senha.
    retorno.append("\n"); // Inserir quebra de linha.
    return retorno; // Retornar cabeçalho montado.
}
