/* Identificação dos integrantes do grupo:
	Lucas dos Anjos de Castro. RA: 0026970.
	Emanuel Elias Ferreira. RA: 0022115.
	Railson Martins da Mata. RA: 0022030.
	Rafael Camargo de Freitas. RA: 0011524. */

#include "servidor.h"

using namespace std;

int main(){

    setlocale(LC_ALL, "Portuguese"); // Configuração para poder exibir mensagens com acentuação.
    Servidor *srv = new Servidor(); // Instanciar objeto de servidor.
    srv->aguardarConexao(); // Aguardar a conexão de um cliente.
    srv->rotinaConexao(); // Executar rotina de conexão.
    srv->desconectar(); // desconectar cliente.

    return 0;
}
