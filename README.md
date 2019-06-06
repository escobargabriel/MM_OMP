# Descrição

> Este programa em sua execução realiza operacoes basicas e a multiplicao de estruturas matriciais de quatro diferentes maneiras: convencional, em blocos, multi-thread convencional e multi-thread em blocos paralelizadas com a interface de programação paralela OpenMP. 

> Estão presentes nos códigos funções para alocação de memória, geração e preenchimento de todas as posições das matrizes, liberação da memória, etc.

## Compilação

> __Importante!__

> __Este código e demais arquivos auxiliares foram desenvolvidos e executados em sistema operacional linux__

> Para compilar os códigos é necessário executar o arquivo "Makefile", neste arquivo estão todas as flags e comandos necessários para a compilação da aplicação e bibliotecas auxiliares. Para executar este arquivo é necessário navegar através do terminal no sistema operacional linux até o diretório onde se encontram os arquivos ".c", ".h", além do "Makefile" (exemplo: $ cd /home/MM_Mthread).

> Estando dentro do diretório correto basta executar o *Makefile*.

> $ make

> Após execução deste comando os códigos seram compilados e gerados os executáveis e binarios.

	
## Execução

### Execução isolada

> O arquivo *Makefile* é responsável pela compilação e geração dos executáveis da aplicação.

> Após compilação para gerar os arquivos de entrada basta executar o comando abaixo:

> $./geraMatriz2 128 128

> O primeiro parâmetro corresponde ao executável gerado na compilação, ja o segundo e o terceiro parâmetros correspondem as dimensões da matriz N x M (128 x 128).

> O nome do arquivo segue o padrao adotado pelo professor da disciplina.

> São necessários dois arquivos de entrada para execução da aplicação principal *main_matriz.c*.

> O arquivo gerado pela aplicação *geraMatriz2.c* possui por padrão o nome 128x128-mat.map, onde 128x128 são as dimensões NxM.

> Para executar a aplicação principal é preciso gerar um arquivo com as dimensões NxM, onde N e M devem ser iguais (matrizes quadradas), renomear o arquivo manualmente, e gerar novo arquivo com as mesmas dimensões do primeiro para garantir a compatibilidade dos arquivos de entrada.

###EXECUÇÕES EM SÉRIE

> Foi criado um script para facilitar as execuções, nele são criados arquivos para utilizar nas execuções onde o primeiro arquivo é criado, renomeado e outro arquivo gerado em seguida.

> Para executar o script basta digita navegar via até o diretório onde os arquivos se encontram e executar o comando:

> $ bash executa.sh

> Serão executadas 4 variações dos testes e os resultados estarão impressos em arquivos de texto com nome indicando a dimensão das matrizes quadradas e o número de threads.

###RESTRIÇÕES

> __IMPORTANTE__

> __As implementações da multiplicação sequencial e paralela em blocos tem como parâmetro a variável nBlocos que corresponde a dimensão dos blocos.__

> __Caso seja atribuído o valor 2, serão criados blocos de dimensões 2x2, se o valor for 4, serão criados blocos de dimensões 4x4 e assim por diante.__

> __Dependendo do valor escolhido para as dimensões dos blocos a aplicação pode ter ganho de desempenho ou degradação.__

> __O valor 16 foi atribuído para a dimensão dos blocos, mas foram colocadas verificações condicionais para garantir o funcionamento da aplicação.__

> __Para os casos testados o escalonamento dinâmico foi o que obteve os melhores resultados, mas para alterar para os outros basta alterar no pragma o schedule (escalonador) para guided, static, etc__


> Para executar a aplicação principal é necessário que os códigos tenham sido compilados anteriormente através da execução do arquivo "Makefile". Uma vez compilados os códigos das aplicaçoes, e gerados os arquivos de entrada basta executar o seguinte comando:

> $ ./main_matriz 128x128-mat.map 128x128-mat1.map 2

> O primeiro parâmetro corresponde ao executável gerado na compilação atraves do arquivo Makefile, o segundo e o terceiro parametros corresponde aos arquivos de entrada onde as matrizes estao armazenadas, já o último parâmetro correspode ao número de threads que a aplicação executará. 


> O código do arquivo "main_matriz.c" foi desenvolvido utilizando como base o código dado pelo professor. 


## Remoção dos arquivos

> Para remover os arquivos criados na execução do *makefile* basta executar o comando abaixo

> $ make clean 


## Contato

> Quaisquer dúvidas ou problemas favor entrar em contato pelo endereço escobarvasques@gmail.com.

## Autor

> Gabriel Escobar.

## Data
> 25/05/2019.
