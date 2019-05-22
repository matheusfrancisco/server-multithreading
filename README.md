# Server Multithreading 


O sistema deve permitir que arquivos locais sejam acessados por usuários remotos simultaneamente.
As operações permitidas pelo sistema devem incluir:

## Etapa 1

Desenvolver a estrutura de acesso do servidor de arquivos. Ele deverá será acessado via socket TCP. Cada conexão deverá ser gerida por uma thread. Condições de corrida deverão ser tratadas por meio de semáforos ou mutexes. Nesta etapa você não precisa implementar as operações sobre arquivos listadas acima. Ao invés disso, use as operações diretamente do sistema de arquivos do seu sistema operacional. Recomenda-se que o servidor imprima mensagens na tela para demonstrar o funcionamento ao professor.


* [x] criar (sub)diretório
* [x] remover (sub)diretório
* [x] entrar em (sub)diretório
* [x] mostrar conteúdo do diretório
* [x] criar arquivo 
* [x] remover arquivo
* [x] escrever um sequência de caracteres em um arquivo
* [x] mostrar conteúdo do arquivo





## Compile
```
git clone   https://github.com/matheusfrancisco/server-SO

cd server-SO

make
```






