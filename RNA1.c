#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "doublefann.h"

fflush(stdin);
__fpurge(stdin);

//auxiliar exponential
double uo = 0.007;
//fim


int EPOCA = 0;
struct fann *ann;

void imprimemat(int m[2][2]) {
        int i;
       printf("\n      0   1  ");
       printf("\n\n   0");for(i=0;i<2;i++){
                   if(m[i][0]==0)printf("  _ ");
                   else
                   printf("  %d ",m[i][0]);
                   }

       printf("\n\n   1");for(i=0;i<2;i++){
                   if(m[i][1]==0)printf("  _ ");
                   else
                   printf("  %d ",m[i][1]);
                   }

        }

void limpamat(int m[2][2]){
     int i,j;

     for(i=0;i<2;i++){
      for(j=0;j<2;j++){
           m[i][j]=0;
                       }

     }
     }


void marcarmat (int mat[2][2], int pos1, int pos2){

         if(pos1!=pos2)
         mat[pos1][pos2]++;


      }

//EXPONENTIAL DECAY//
void exponential_decay (void) {

fflush(stdin);



float m2;
float x;
float dezM,M;
double e,u,um;

m2 = pow(EPOCA,2);
printf("m2 = %f\n",m2);
M=300;
dezM = 10*M;
printf("10M = %f\n",dezM);
x = -(m2/dezM);
printf("x = %f\n",x);
e=exp(x);
printf("exp(%f)=%.8f\n",x,e);
um=uo*e;
printf("um = %.8f\n",um);
float h;
fann_set_learning_rate(ann,um);
h = fann_get_learning_rate(ann);
printf("LEARNING RATE %f",h);
}


int main()
{

 FILE *arqsaida;
 arqsaida = fopen("treinamento_validacao.xls","wa");

float porcent;
float per;
float som =0;
int aa=0;
unsigned int i = 0;
 unsigned int ii= 0;

 int escolha,escolhab;
 float erros=0;
 float msetreino;
 float minimo =1,atual=1;

//para Bold Driver


float u=0.002;
float anterior=0;
float vetor[500];

 int cont=1;
 float h;
//fim


int matriz[2][2];
 limpamat(matriz);

 printf("Escolha a base de dados\n");
 printf("1-Base de Dados normal\n");
 printf("2-Base de Dados com PCA\n");
 scanf("%d",&escolhab);

 printf("Escolha o metodo para acelarar a Taxa de Aprendizagem\n");
 printf("1- Bold Driver\n");
 printf("2- Exponential Decay\n");
 printf("3- Nenhum\n\n");
 scanf("%d",&escolha);


if(escolha == 3)
{
    float uo = 0.7;
}


 const unsigned int layers[4] = {64,15,5,2};  //[3] 2 input,1 camada escondida com(6 neuron),4 output ,... isso para create_standard_array: evita segmentation fault


 const float desired_error = (const float) 0.001;
 const unsigned int max_epochs = 1;
 //const unsigned int epochs_reports = 10;

 // fann_create_standard_array(3, layers);

 struct fann_train_data *data, *validacao_data, *train_data;



 //1 - #########CRIANDO################

//.....criando:

printf("%c[%d;%d;%dmCRIANDO REDE",27,0,37,41);


 ann = fann_create_standard_array(4,layers);

fann_set_learning_rate(ann,uo); //default 0.7 eta


 // Seta funcao de ativacao
 fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
 fann_set_activation_steepness_hidden(ann,0.67); //default 0.5
 fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
 fann_set_activation_steepness_output(ann,0.67);

 //fann_set_training_algorithm(ann, FANN_TRAIN_SARPROP);
 fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
  //fann_set_training_algorithm(ann, FANN_TRAIN_BATCH);
   //fann_set_training_algorithm(ann, FANN_TRAIN_QUICKPROP);
 fann_set_learning_momentum(ann, 0.1);  //acelerar o Backpropagation - usar faixa entre 0.1 e 1.0

 fann_randomize_weights(ann,-0.1,0.1);




 //fann_set_sarprop_temperature(ann,0.015);
printf("learning rate %f",fann_get_learning_rate(ann));
fann_print_parameters(ann); //add


 //2- #############TREINANDO################ (com as 40k treina.txt)

system("read b"); //add

if(escolhab==1)
{
train_data = fann_read_train_from_file("treino_phy.txt");
validacao_data = fann_read_train_from_file("validacao_phy.txt");
}
if(escolhab==2)
{
train_data = fann_read_train_from_file("entrada_treino_PCA95.txt");
validacao_data = fann_read_train_from_file("validacao_cruzada_PCA95.txt");
}

 do
{
printf("%c[%d;%d;%dmTREINANDO RNA.\n",27,0,33,40);

 fann_shuffle_train_data(train_data);
 //fann_train_on_file(ann, "treina.txt", max_epochs, 10, desired_error);
 fann_train_on_data(ann, train_data, 1, 1, desired_error);

  fann_reset_MSE(ann);
	for(ii = 0; ii < fann_length_train_data(train_data); ii++)
	{
		fann_test(ann, train_data->input[ii], train_data->output[ii]);
	}
	msetreino = fann_get_MSE(ann);
	printf("MSE no Treino: %f\n", msetreino);


	fprintf(arqsaida,"%f",msetreino);

 //3- ##########VALIDACAO#################### (verifica convergencia)

 printf("\n%c[%d;%d;%dmVALIDACAO CRUZADA.\n",27,0,35,42);


	fann_reset_MSE(ann);
	for(i = 0; i < fann_length_train_data(validacao_data); i++)
	{
		fann_test(ann, validacao_data->input[i], validacao_data->output[i]);
	}
	atual = fann_get_MSE(ann);
	printf("MSE na Validacao Cruzada: %f\n", atual);

	fprintf(arqsaida,"\t%f",atual);


//SALVA O MINIMO DA REDE
         if(atual < minimo)
         {//salva a rede
            fann_save(ann,"treina.net");
            minimo = atual;
            aa=EPOCA;
            fprintf(arqsaida,"\t\tminimo");
            printf("REDE SALVA\n");
         }

printf("Minimo na EPOCA[%d]= %f\n",aa,minimo);
//FIM
printf("EPOCA[%d]\n",EPOCA);

fprintf(arqsaida,"\t\tEPOCA%i\n",EPOCA);

//MELHORAR A TAXA DE APRENDIZAGEM
if(escolha == 1)
{
printf("%c[%d;%d;%dmBOLD DRIVER.\n",27,0,30,46);

if(EPOCA >1)
{
printf("[%d] anterior =%f\n",EPOCA-1,anterior);
printf("[%d] atual =%f\n",EPOCA,atual);

   if (atual<anterior)
   {
   u = u*1.1;
   printf("MSE diminuiu\n");
   printf("Eta AUMENTOU\n");
   fann_set_learning_rate(ann,u);
   }
   float dif = atual - anterior;
   if( dif > 0.00000000001)
   {
   u = u/2;
   printf("MSE aumentou\n");
   printf("Eta DIMINUI\n");
   fann_set_learning_rate(ann,u);
   }
            h = fann_get_learning_rate(ann);
            printf("LEARNING RATE = %f",h);
            anterior = atual;
}

//FIM
}
//system("read b");

if (escolha == 2)
{
//EXPONENCIAL DECAY
printf("%c[%d;%d;%dmEXPONENTIAL DECAY.\n",27,0,30,46);
exponential_decay();
//
}

EPOCA = EPOCA +1;

//system("read b");
}while (EPOCA<200);
fann_destroy_train(train_data);
fann_destroy_train(validacao_data);

system("read b"); //add
 //6- ##########TESTE###################### (com o ultimo .net e o teste.txt)
printf("%c[%d;%d;%dmTESTE.\n",27,0,30,45);


fann_type *calc_out;

struct fann *ann2 = fann_create_from_file("treina.net");

if(escolhab==1)
{
data = fann_read_train_from_file("teste_phy.txt");
}
if(escolhab==2)
{
    data = fann_read_train_from_file("testes_PCA95.txt");
}

float teste=0;

for (i =0; i<fann_length_train_data(data);i++)
{
calc_out = fann_test(ann2,data->input[i], data->output[i]);

printf("\nout1 %f out2 %f \n, deveria ser sai1 %f sai2 %f \n",calc_out[0],calc_out[1],data->output[i][0],data->output[i][1]);

const float n[2] = {data->output[i][0],data->output[i][1]};


float maxTeste =0;
int iTeste =0;
unsigned x=0;
for(x=0;x<=2;x++)
{
    if(n[x] >= maxTeste)
    {
        maxTeste = n[x];
        iTeste =x;
    }

}

const float m[2]={calc_out[0],calc_out[1]};
float maxRede = 0;
int iRede =0;
unsigned y=0;
for(y=0;y<=2;y++)
{
    if(m[y]>=maxRede)
    {
        maxRede = m[y];
        iRede=y;
    }
}

printf("%d %d",iRede,iTeste);
if (iTeste != iRede)
{
     marcarmat(matriz,iTeste ,iRede);

    erros ++;

}
teste ++;
}


system("read b"); //add

printf("\n%c[%d;%d;%dmMATRIZ DE ERROS (saiu x sair)",27,0,37,44);
 imprimemat(matriz);

printf("\n\n\nNumero de erros = %4.0f",erros);
printf("\nNumero de testes =%5.0f",teste);




porcent = (erros/teste);
per=porcent*100;
som=som+per;

printf("\nPorcentagem de erros =%.2f %c",per,37);



printf("\n\n");
system("read b"); //add

printf("\n\n%c[%d;%d;%dmLIMPANDO.\n",27,0,37,40);
printf("Limpando.\n");
    fann_save(ann,"treina.net");
	//fann_destroy_train(train_data);
	//fann_destroy_train(validacao_data);
	fann_destroy_train(data);

	fann_destroy(ann);
    fann_destroy(ann2);




 return 0;
}
