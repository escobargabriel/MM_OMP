/*

Multiplicação de matrizes convencional, em bloco, convencional com POSIX threads e em blocos com POSIX threads.

                                            ATENÇÃO!!!

            Para entender onde e como fazer alterações no código leia o arquivo "README".
Nele constam detalhes de em qual código fonte e quais variáveis são necessárias para fazer a divisão da matriz de blocos.
*/

/*Matrizes Globais*/
int **mat_a;
int **mat_b;
int **mat_cOmp;
//int **mat_cOmpIKJ; 
int **mat_cOmpBlock; 
//int **mat_cOmpBlockIKJ;
    

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv2.h"
#include "matriz.h"
#include "matriz-operacoes.h"
#include "matriz-operacoes-omp.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// DECLARAÇÃO de VARIÁVEIS
	FILE *fmat_a, *fmat_b, *fmat_c,  *fmatomp_c,  *fmatbloco_c2,  *fmatblockomp_c; 
    // *fmatbloco_c, , *fmatblockompIKJ_c *fmatblocoIKJ_c2, *fmatompIKJ_c, *fmatikj_c,
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time, total=0;
	//matriz_bloco_t **Vsubmat_a = NULL;
	//matriz_bloco_t **Vsubmat_b = NULL;
	//matriz_bloco_t **Vsubmat_c = NULL;
	//int nro_submatrizes = 2;
	//int **mat_bloco_final = NULL;
    int **mat_bloco2 = NULL;
    //int **mat_bloco2IKJ = NULL;	
    //int tid, threads;
	//pthread_t *pool;
	//param_t *args;

    /*Definindo numero de threads e de repeticoes dos testes*/
    int nt; //recebe numero de threads
    int NR = 10; //numero de repeticoes de cada caso
    

    /*Variaveis para armazenar medias e speedup*/
    double mediaSeq = 0.0, mediaOmp = 0.0, mediaBloco2 = 0.0, mediaOmpBlock = 0.0;
    double speedupOmp = 0.0, speedupBlock = 0.0; 
    //mediaBloco = 0.0 , mediaOmpBlockIKJ = 0.0 , speedupBlockIKJ = 0.0
    mat_a = NULL;
    mat_b = NULL;
    int **mat_c = NULL;
    //int **matikj_c = NULL;
    mat_cOmp = NULL;
    //mat_cOmpIKJ = NULL; 
    mat_cOmpBlock = NULL;
    //mat_cOmpBlockIKJ = NULL;


    //Dimensao de blocos    
    //Para garantir funcionamento (N/nBlocos % 2 == 0)
    int nBlocos = 16; //blocos de nBlocos x nBlocos


	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	if (argc != 4){
		printf ("ERRO: Numero de parametros \n");
		exit (1);
	}

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	fmat_a = fopen(argv[1],"r");
	fmat_b = fopen(argv[2],"r");
	nt = atoi(argv[3]);

//	args  = (param_t *)   malloc(threads * sizeof(param_t));
//	pool =  (pthread_t *) malloc(threads * sizeof(pthread_t));

  if ((fmat_a == NULL) || (fmat_b == NULL)) {
		printf("Error: Na abertura dos arquivos.\n");
		exit(1);
  }

  // matriz_a N (linha) x La (coluna)
	extrai_parametros_matriz(fmat_a, &N, &La, &vet_line, &nr_line);
	mat_a = alocar_matriz(N, La);
	filein_matriz (mat_a, N, La, fmat_a, vet_line, nr_line);

	// matriz_b Lb (linha) x M (coluna)
	extrai_parametros_matriz(fmat_b, &Lb, &M, &vet_line, &nr_line);
	mat_b = alocar_matriz(Lb, M);
	filein_matriz (mat_b, Lb, M, fmat_b, vet_line, nr_line);

	// matriz_c (resultante) = N (linha) x M(coluna)
	mat_c = alocar_matriz(N, M);
	zerar_matriz(mat_c, N, M);

	if ((mat_c == NULL) || (mat_b == NULL) || (mat_a == NULL)) {
		printf("ERROR: Out of memory\n");
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    if(La != Lb){
		printf("\nMatrizes imcompativeis!\n");
		printf("Encerrando execucao\n\n");
		exit(1);
	}
	

	if(N % nBlocos != 0){
		nBlocos = 4;
	}
    if(N == 10){
        nBlocos = 2;
    }

	printf("\nDimensao dos blocos %dx%d\n", nBlocos, nBlocos);

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação IJK
    printf("\n----------------------------------------------------------------------------\n");
    printf("\nIniciando execucao da Multiplicao de matrizes sequencial convencional IJK\n");
    
	zerar_matriz(mat_c, N, M);
    for(int i = 0; i < NR; i++){	
        start_time = wtime();
	    multiplicarIJK(mat_a,mat_b,mat_c, N, N, N);
	    end_time = wtime();
        total = end_time - start_time;
        printf("Execucao %d: %lf\n", i+1, total);
        mediaSeq += (end_time - start_time);     
    }
    mediaSeq = mediaSeq/NR;
 
	//printf("\n ##### Multiplicação de Matrizes (IJK) #####\n");
	//printf("\tRuntime: %f\n", end_time - start_time);
	fmat_c= fopen("outIJK.map-result","w");
	fileout_matriz(mat_c, N, M, fmat_c);
	printf("\nFim da execucao da Multiplicao de matrizes sequencial convencional IJK\n");
    printf("\n----------------------------------------------------------------------------\n");
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%




// Multiplicação ikj
//    printf("\n----------------------------------------------------------------------------\n");
 //   printf("\nIniciando execucao da Multiplicao de matrizes sequencial convencional IKJ\n");
  //  double mediaSeqikj = 0.0;
   // matikj_c = alocar_matriz(N, M);
//	zerar_matriz(matikj_c, N, M);
//    for(int i = 0; i < NR; i++){	
  //      start_time = wtime();
//	    multiplicarIKJ(mat_a,mat_b,matikj_c, N, N, N);
//	    end_time = wtime();
  //      total = end_time - start_time;
   //     printf("Execucao %d: %lf\n", i+1, total);
    //    mediaSeqikj += (end_time - start_time);     
    //}
    //mediaSeqikj = mediaSeqikj/NR;
 
	//printf("\n ##### Multiplicação de Matrizes (ikj) #####\n");
	//printf("\tRuntime: %f\n", end_time - start_time);
	//fmatikj_c= fopen("outikj.map-result","w");
	//fileout_matriz(matikj_c, N, M, fmatikj_c);
	//printf("\nFim da execucao da Multiplicao de matrizes sequencial convencional IKJ\n");
    //printf("\n----------------------------------------------------------------------------\n");





	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação em Bloco v2
    /*    
    printf("\n----------------------------------------------------------------------------\n");
    printf("\nIniciando execução da Multiplicação de Matrizes com struct\n");
    for(int i = 0; i < 10; i++){
	start_time = wtime();	
    Vsubmat_a = particionar_matriz (mat_a, N, La, 1, nro_submatrizes);
	Vsubmat_b = particionar_matriz (mat_b, Lb, M, 0, nro_submatrizes);
	Vsubmat_c = constroi_submatriz (N, M, nro_submatrizes);
	mat_bloco_final = alocar_matriz(N, M);
	zerar_matriz(mat_bloco_final, N, M);
    
	multiplicar_submatriz (Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
	multiplicar_submatriz (Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]);
	somarIJ(Vsubmat_c[0]->matriz,Vsubmat_c[1]->matriz,mat_bloco_final, N, N, N, N);
    
	end_time = wtime();
    mediaBloco += (end_time - start_time);
    }
    mediaBloco = mediaBloco / 10;

	//printf("\tRuntime: %f\n\n", end_time - start_time);
	fmatbloco_c = fopen("outBlocov2.map-result","w");
	fileout_matriz(mat_bloco_final, N, M, fmatbloco_c);
	
    comparar_matriz (mat_c, mat_bloco_final, N, M);
	//printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    printf("\nFim da execucao da Multiplicao de matrizes com struct\n");
    printf("\n----------------------------------------------------------------------------\n");
    */
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    /*     Multiblocks                         */
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    printf("\n----------------------------------------------------------------------------\n");
    printf("\nIniciando Multiplicação de matrizes em blocos IJK\n");
    for(int i = 0; i < NR; i++){
	    start_time = wtime();
        mat_bloco2 = alocar_matriz(N, M);
    	zerar_matriz(mat_bloco2, N, M);    	
        multiplicaBloco(N, N, mat_a, mat_b, mat_bloco2, nBlocos);
	    end_time = wtime();
        total = end_time - start_time;
        printf("Execucao %d: %lf\n", i+1, total);
        mediaBloco2 += (end_time - start_time);
    }

    mediaBloco2 = mediaBloco2 / NR;

    

	fmatbloco_c2 = fopen("outBloco2.map-result","w");
	fileout_matriz(mat_bloco2, N, M, fmatbloco_c2);
	printf("\ncomparando sequencial com sequencial em blocos.\n");
    comparar_matriz (mat_c, mat_bloco2, N, M);
	//printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    printf("\nFim da execucao da Multiplicao de matrizes em blocos IJK\n");
    printf("\n----------------------------------------------------------------------------\n");

        // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //printf("\n----------------------------------------------------------------------------\n");
    //printf("\nIniciando Multiplicação de matrizes em blocos IKJ\n");
    //double mediaBloco2IKJ = 0.0;

    //for(int i = 0; i < NR; i++){
    //    start_time = wtime();
    //    mat_bloco2IKJ = alocar_matriz(N, M);
    //    zerar_matriz(mat_bloco2IKJ, N, M);     
    //    multiplicaBlocoIKJ(N, N, mat_a, mat_b, mat_bloco2IKJ, nBlocos);
    //    end_time = wtime();
    //    total = end_time - start_time;
    //   printf("Execucao %d: %lf\n", i+1, total);
    //    mediaBloco2IKJ += (end_time - start_time);
    //}

    //mediaBloco2IKJ = mediaBloco2 / NR;

    

    //fmatblocoIKJ_c2 = fopen("outBloco2.map-result","w");
    //fileout_matriz(mat_bloco2IKJ, N, M, fmatblocoIKJ_c2);
    //printf("\ncomparando sequencial com sequencial em blocos IKJ.\n");
    //comparar_matriz (mat_c, mat_bloco2IKJ, N, M);
    //printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    //printf("\nFim da execucao da Multiplicao de matrizes em blocos IKJ\n");
    //printf("\n----------------------------------------------------------------------------\n");
    //              %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //---------------------------FIM DOS SEQUENCIAIS---------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //---------------------------INICIO PARALELOS------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //              %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    /*          MM_thread - exec                       */
    printf("\n----------------------------------------------------------------------------\n");
    printf("\nIniciando execucao multi-thread OpenMP da multiplicacao de matrizes convencional IJK\n");
    mat_cOmp = alocar_matriz(N, M);
	zerar_matriz(mat_cOmp, N, M);
    for(int i = 0; i < NR; i++){
    start_time = wtime();
    multiplicarOmp (mat_a,mat_b,mat_cOmp,N,N,N,nt);
    end_time = wtime();
    total = end_time - start_time;
    printf("Execucao %d: %lf\n", i+1, total);	
    mediaOmp += (end_time - start_time);     
    }
    mediaOmp = mediaOmp / NR;
    speedupOmp = mediaSeq/mediaOmp;    

   	fmatomp_c = fopen("outOMPIJK.map-result","w");
	fileout_matriz(mat_cOmp, N, M, fmatomp_c);
    printf("\ncomparando sequencial com sequencial com multi-thread IJK.\n");
   	comparar_matriz (mat_c, mat_cOmp, N, M);
	//printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    printf("\nFim da execucao da Multiplicao de matrizes multi-thread OpenMP convencional IJK\n");
    printf("\n----------------------------------------------------------------------------\n");
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%


    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    /*          MM_thread - exec                       */
    //double mediaOmpIKJ = 0.0;
    //double speedupOmpIKJ = 0.0;
    //printf("\n----------------------------------------------------------------------------\n");
    //printf("\nIniciando execucao multi-thread OpenMP da multiplicacao de matrizes convencional IKJ\n");
    //mat_cOmpIKJ = alocar_matriz(N, M);
    //zerar_matriz(mat_cOmpIKJ, N, M);
    //for(int i = 0; i < NR; i++){
    //	start_time = wtime();
    //	multiplicarOmp (mat_a,mat_b,mat_cOmpIKJ,N,N,N,nt);
    //	end_time = wtime();
    //	total = end_time - start_time;
    //	printf("Execucao %d: %lf\n", i+1, total);   
    //	mediaOmpIKJ += (end_time - start_time);     
    //}
    //mediaOmpIKJ = mediaOmpIKJ / NR;
    //speedupOmpIKJ = mediaSeqikj/mediaOmpIKJ;    

    //fmatompIKJ_c = fopen("outOMPIKJ.map-result","w");
    //fileout_matriz(mat_cOmpIKJ, N, M, fmatompIKJ_c);
    //printf("\ncomparando sequencial com sequencial com multi-thread IKJ.\n");
    //comparar_matriz (mat_c, mat_cOmpIKJ, N, M);
    //printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    //printf("\nFim da execucao da Multiplicao de matrizes multi-thread OpenMP convencional IKJ\n");
    //printf("\n----------------------------------------------------------------------------\n");


// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    /*          MM_thread - execBlock                       */
   // double mediaOmpBlockIKJ = 0.0;
   // printf("\n----------------------------------------------------------------------------\n");    
   // printf("\nIniciando execucao multi-thread OpenMP da multiplicação em blocos ikj\n");
   // mat_cOmpBlockIKJ = alocar_matriz(N, M);
	//zerar_matriz(mat_cOmpBlockIKJ, N, M);
    //for(int i = 0; i < NR; i++){
    //zerar_matriz(mat_cOmpBlockIKJ, N, M);
    //start_time = wtime();
    //multiplicaBlocoOMPIKJ(N, N, mat_a, mat_b, mat_cOmpBlockIKJ, nBlocos ,nt);	
    //end_time = wtime();
    //total = end_time - start_time;
    //printf("Execucao %d: %lf\n", i+1, total);	
    //mediaOmpBlockIKJ += (end_time - start_time);     
    //}
    //mediaOmpBlockIKJ = mediaOmpBlockIKJ / NR;
    //speedupBlockIKJ = mediaBloco2IKJ/mediaOmpBlockIKJ;    

   	//fmatblockompIKJ_c = fopen("outOMPBlockIKJ.map-result","w");
	//fileout_matriz(mat_cOmpBlockIKJ, N, M, fmatblockompIKJ_c);
    //printf("\ncomparando sequencial em blocos com multi-thread em blocos.\n");
	//comparar_matriz (mat_c, mat_cOmpBlockIKJ, N, M);
	//printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    //printf("\nFim da execucao multi-thread OpenMP da Multiplicao de matrizes em blocos ikj\n");
    //printf("\n----------------------------------------------------------------------------\n");
    





    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    /*          MM_thread - execBlock                       */
    printf("\n----------------------------------------------------------------------------\n");    
    printf("\nIniciando execucao multi-thread OpenMP da multiplicação em blocos IJK\n");
    mat_cOmpBlock = alocar_matriz(N, M);
	zerar_matriz(mat_cOmpBlock, N, M);
    for(int i = 0; i < NR; i++){
    zerar_matriz(mat_cOmpBlock, N, M);
    start_time = wtime();
    multiplicaBlocoOMP(N, N, mat_a, mat_b, mat_cOmpBlock, nBlocos ,nt);	
    end_time = wtime();
    total = end_time - start_time;
    printf("Execucao %d: %lf\n", i+1, total);	
    mediaOmpBlock += (end_time - start_time);     
    }
    mediaOmpBlock = mediaOmpBlock / NR;
    speedupBlock = mediaBloco2/mediaOmpBlock;    

   	fmatblockomp_c = fopen("outOMPBlock.map-result","w");
	fileout_matriz(mat_cOmpBlock, N, M, fmatblockomp_c);
    printf("\ncomparando sequencial em blocos com multi-thread em blocos.\n");
	comparar_matriz (mat_c, mat_cOmpBlock, N, M);
	//printf("Resultados individuais encontram-se nos arquivos <out*.map-result>.\n");
    printf("\nFim da execucao multi-thread OpenMP da Multiplicao de matrizes em blocos IJK\n");
    printf("\n----------------------------------------------------------------------------\n");
    /*                     END                             */
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    printf("\n----------------------------------------------------------------------------\n");
    printf("\n-   Sequenciais:                                                           -");
    printf("\n-   Media Sequencial IJK:                                 %f         -", mediaSeq);
    //printf("\n-   Media Sequencial IKJ:                                 %lf         -", mediaSeqikj);    
    //printf("\n-   Media Sequencial em Blocos 1 - (Structs):             %f         -", mediaBloco);
    printf("\n-   Media Sequencial em Blocos IJK:                       %f         -", mediaBloco2);
    //printf("\n-   Media Sequencial em Blocos IKJ:                       %f         -", mediaBloco2IKJ);
    printf("\n----------------------------------------------------------------------------\n");
    printf("\n-   Paralelos:                                                             -");
    printf("\n-   Media Openmp convenciaonal IJK:                           %f     -", mediaOmp);
    //printf("\n-   Media Openmp convenciaonal IKJ:                           %f     -", mediaOmpIKJ);
    printf("\n----------------------------------------------------------------------------\n");
    printf("\n-   Media Openmp em Blocos IJK:                               %f     -", mediaOmpBlock);
    //printf("\n-   Media Openmp em Blocos IKJ:                               %f     -", mediaOmpBlockIKJ);
    printf("\n----------------------------------------------------------------------------\n");    
    printf("\n-   Speedup OpenMP convencional IJK:                           %f    -", speedupOmp);
    //printf("\n-   Speedup OpenMP convencional IKJ:                           %f    -", speedupOmpIKJ);
    printf("\n----------------------------------------------------------------------------\n");
    printf("\n-   Speedup OpenMP em blocos IJK:                              %f    -", speedupBlock);
    //printf("\n-   Speedup OpenMP em blocos IKJ:                              %f    -", speedupBlockIKJ);
    printf("\n----------------------------------------------------------------------------\n");
  
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// LIBERAR MEMÓRIA
	//Vsubmat_a = liberar_submatriz (Vsubmat_a,nro_submatrizes);
	//Vsubmat_b = liberar_submatriz (Vsubmat_b,nro_submatrizes);
	//Vsubmat_c= liberar_submatriz (Vsubmat_c,nro_submatrizes);

	
    liberar_matriz(mat_a,N,La);
	liberar_matriz(mat_b,Lb,M);
	liberar_matriz(mat_c,N,M);
    //liberar_matriz(matikj_c,N,M);
    	//liberar_matriz(mat_bloco_final,N,M);
    liberar_matriz(mat_bloco2,N,M);
    //liberar_matriz(mat_bloco2IKJ,N,M);
    liberar_matriz(mat_cOmp,N,M);
    //liberar_matriz(mat_cOmpIKJ,N,M);
    //liberar_matriz(mat_cOmpblockIKJ,N,M);
//    liberar_matriz(mat_cOmpblock,N,M);    

	fclose(fmat_a);
	fclose(fmat_b);
    fclose(fmat_c);
   // fclose(fmatikj_c);
    //fclose(fmatbloco_c);
    //fclose(fmatblocoIKJ_c);
    //fclose(fmatblockompIKJ_c);    
    fclose(fmatbloco_c2);
    //fclose(fmatblocoIKJ_c2);	
    fclose(fmatomp_c);
    //fclose(fmatompIKJ_c);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    return 0;
}

    
