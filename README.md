# Redes
Trabalho Final da disciplina de Redes de Computadores. 

## Descrição
O objetivo deste trabalho é implementar um pequeno programa em C++, usando um projeto no Code::Blocks. O trabalho deve ser desenvolvido de acordo com a Seção 2. Será implementado em grupos de 4 pessoas, podendo haver, no máximo, um único grupo com 5 integrantes.
O trabalho vale 30 (trinta) pontos da média final. Todos os membros do grupo devem trabalhar colaborativamente, discutindo amplamente todas as decisões tomadas na implementação.
Trabalhos que não compilam não possuem condições de avaliação e valem automaticamente 0 (zero). Também serão anulados trabalhos cuja cópia da internet for comprovada em qualquer momento, bem como cópia de trabalhos de outros grupos (sendo anuladas todas as cópias).
A compilação e os testes serão efetuados em Linux, portanto, é altamente recomendável que o trabalho seja testado, pelo menos uma vez, nessa plataforma.
Além dos testes de caixa preta (resolução do problema), serão avaliados no teste de caixa clara:
- clareza e corretude do código;
- conformidade às especificações;
- comentários e endentação correta.
Os trabalhos devem ser entregues de acordo com as instruções da seção Prazo de Entrega.

## Especificação
A meta da implementação é a construção do programa seguindo os seguintes critérios:
- O programa deve ser corretamente modularizado, usando POO (dividido em classes, métodos, etc.);
- O projeto deve ser criado da seguinte forma: nome1-nome2-nome3-nome4, onde nome1, nome2 e nome3 e nome4 devem ser substituídos pelo nome dos membros do grupo, por exemplo: chicoanisio-josoares-carlosalberto-tiririca.
- No cabeçalho do arquivo, deve constar um comentário com a identificação completa dos membros do grupo, incluindo número de matrícula.
- Ao término do trabalho, cada grupo deverá:
  - Selecionar a opção Clean do menu Build, no Code::Blocks;
  - Compactar a pasta do projeto com todo seu conteúdo, usando o mesmo nome da pasta como nome do arquivo (ex.: nome1-nome2-nome3-nome4.zip);
 
Deve ser implementada uma aplicação cliente-servidor que simule uma fechadura eletrônica. O usuário deverá digitar seu login (apenas números) e, em seguida, sua senha (também numérica).
O cliente enviará a requisição de autenticação conforme a Figura 1. Observar que o cabeçalho da requisição termina sempre com uma linha em branco. A senha será sempre enviada com codificação base64, que deverá ser decodificado no servidor e validado junto ao arquivo com usuários e senhas.

### Formato da Mensagem de autenticação
auth

user: 123456

pass: NjUOMzIxCg==

O servidor verificará um arquivo ASCII contendo os pares de login e senha, separados por dois pontos (:). A senha deve ser armazenada em algum formato de hash seguro (SHA1, SHA256, etc.).
O usuário poderá trocar a própria senha, se iniciar seu login com o caracter *, como por exemplo, *123456 e, na senha, digitar a senha atual, seguida por *,  a nova senha e o caracter # (654321*615243#). Será gerada uma requisição com o formato apresentado na Figura 2 (observar linha em branco no final). A senha só poderá ser alterada se a senha atual corresponder ao cadastrado no  arquivo.

### Formato da mensagem de alteração de senha
changepass

user: 123456

pass: NjUOMzIxCg==

newpass: NjE1MjQzCg==

O servidor deverá responder com um cabeçalho contendo uma mensagem de status, conforme Tabela 1, seguida por uma linha em branco. Após estabelecer a conexão, o servidor deve passar o atendimento a uma thread da biblioteca padrão da linguagem, para continuar a receber outras requisições. A ação de liberar a fechadura será representada pela impressão de uma mensagem informando que a fechadura foi destravada. Nos demais casos, deve voltar ao  prompt de login do usuário.
A comunicação entre cliente e servidor deve ser feita usando sockets, usando a porta TCP 54321. Para verificar a aderência ao protocolo, pares de clientes
e servidores de grupos distintos poderão ser testados aleatoriamente.
Deverão ser entregues, além do código-fonte, o arquivo com os dados dos usuários, uma versão do arquivo com as senhas em claro (sem hash) e, no caso de uso de TLS, os certificados gerados para a aplicação.

### Mensagens de status/erro
|Mensagem               |Ação                               |
|-----------------------|-----------------------------------|
|200 Auth OK            |`Liberar a fechadura`              |
|250 Pass changed       |`Voltar para solicitação de login` |
|401 Not authorized     |`Voltar para solicitação de login` |
|450 User doesn't exists|`Voltar para solicitação de login` |

## Prazo de Entrega
Os trabalhos devem ser entregues, impreterivelmente, até a data estipulada abaixo, através do upload dos arquivos através desta tarefa. Serão aceitos trabalhos até às 23:55h dessa data. A partir das 23:56h, a nota do grupo é automaticamente zero e o envio de arquivos será suspenso.
Sob nenhuma hipótese serão aceitos trabalhos enviados por e-mail, mídia  removível ou impressos.
Serão avaliados apenas os arquivos enviados através do Moodle, dentro do prazo estipulado.
