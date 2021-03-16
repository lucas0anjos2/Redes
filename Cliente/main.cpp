/* Identificação dos integrantes do grupo:
	Lucas dos Anjos de Castro. RA: 0026970.
	Emanuel Elias Ferreira. RA: 0022115.
	Railson Martins da Mata. RA: 0022030.
	Rafael Camargo de Freitas. RA: 0011524. */

#include "cliente.h"

using namespace std;

int main(){

	setlocale(LC_ALL, "Portuguese"); // Configuração para poder exibir mensagens com acentuação.
	Cliente *clt = new Cliente(54321, "127.0.0.1"); // Instanciar objeto de cliente informando a porta a ser utilizada e o host a se conectar.
	if(clt->conectar()){ // Tentar conectar e caso for obtido êxito.
		clt->rotinaConexao(); // Executar rotina de conexão.
		clt->desconectar(); // Ao fim da execução rotina fechar socket do cliente.
	}
    return 0;
}
