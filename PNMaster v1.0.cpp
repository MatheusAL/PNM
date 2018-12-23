// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos         (criado em 16/06/14)
//          Andre Gustavo dos Santos         (modificado em 22/05/18)
//			Gabriel de Paula Felix			 (modificado em 09/06/18)
//			Matheus Antônio dos Santos Lima	 (modificado em 09/06/18)
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main() {
	int largura, altura;							//Dimensoes da imagem
	char tipo[4];									//Tipo da imagem
	ifstream arqentrada;							//Arquivo que contem a imagem original
	ofstream arqsaida;								//Arquivo que contera a imagem gerada
	char comentario[200], u;						//Auxiliares
	int i, j,k, valor;			
	char nome[100]; //								//Variável para ler o nome do arquivo.

//*** LEITURA DA IMAGEM ***//
	
	cout<<endl<<"/**** Bem-vindo ao Editor de Imagens PNMaster! ****/"<<endl<<endl;
	cout<<"Digite o nome do arquivo de foto a ser utilizado: ";
	cin>>nome;
	//*** Abertura do arquivo ***//
	arqentrada.open(nome,ios::in); //Abre arquivo para leitura
	if (!arqentrada) {
		cout << "Erro, verifique se o nome do arquivo esta correto e se possui formato .pnm"<<endl;
		return 0;
	}
	//***************************//

	//*** Leitura do cabecalho ***//
	arqentrada >> tipo;	//Le o tipo de arquivo
	arqentrada.get();	//Le e descarta o \n do final da 1a. linha
	if (strcmp(tipo,"P2")==0) {
		cout << "Imagem em tons de cinza carregada com sucesso.\n";
	}else if (strcmp(tipo,"P3")==0) {
		cout << "Imagem colorida carregada com sucesso.\n";
	}
	else if (strcmp(tipo,"P5")==0 || strcmp(tipo,"P6")==0) {
		cout << "Imagem no formato RAW\n";
		cout << "Desculpe, mas nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}


	while((u = arqentrada.get()) == '#')	//Enquanto for comentario
		arqentrada.getline(comentario,200);	//Le e descarta a linha "inteira"

	arqentrada.putback(u);	//Devolve o caractere lido para a entrada, pois como
							//nao era comentario, era o primeiro digito da largura

	arqentrada >> largura >> altura;	//Le o numero de pixels da horizontal e vertical
	cout << "Tamanho: " << largura << " x " << altura << endl;
	
	unsigned char m[altura][largura][3];	//Matriz de forma LxHx3 para imagens RGB e Tons de cinza.
	unsigned char mx[altura][largura][3];	//Matriz da forma LxHx3 para aplicação filtros.
	
	arqentrada >> valor;	//Valor maximo do pixel (temos que ler, mas nao sera usado)
	//*** Leitura dos pixels da imagem ***//
	if (strcmp(tipo,"P3")==0) { //Leitura da imagem colorida (Gabriel)
		
	for(i=0;i<altura;i++)
		for(j=0;j<largura;j++) {
			for(k=0;k<3;k++){
				arqentrada >> valor;
				m[i][j][k] = (unsigned char)valor;
			}
		}
	}else if (strcmp(tipo,"P2")==0) { //Leitura da imagem preto e branca (Gabriel)
				for(i=0;i<altura;i++)
						for(j=0;j<largura;j++) {
						arqentrada >> valor;
						m[i][j][1]= (unsigned char)valor;
						}
	}
	//************************************//
	arqentrada.close();
//*** FIM DA LEITURA DA IMAGEM ***//

//*** TRATAMENTO DA IMAGEM ***//
	int opcao;
	cout<<endl;
	cout<<"Digite o filtro desejado para aplicar: "<<endl<<endl;
	cout<<"Lista de filtros disponiveis: "<<endl<<endl;
	cout<<"1- Escurecer imagem "<<endl;
	cout<<"2- Clarear imagem "<<endl;
	cout<<"3- Inverter imagem horizontalmente"<<endl;
	cout<<"4- Negativar a imagem"<<endl;
	cout<<"5- Transformar imagem colorida em tons de cinza"<<endl;
	cout<<"6- Efeito Sobel"<<endl;
	cout<<"7- Efeito Sharpening"<<endl;
	cout<<"8- Efeito Extra"<<endl;
	cout<<"0- Encerrar o Imagic "<<endl<<endl;
	cout<<"Digite aqui o comando desejado:  ";
	cin>>opcao;

	int fator;
	if(opcao==1){
		cout << "Qual o fator de escurecimento (1-100) ? ";
		cin >> fator;

		//[FILTRO] Escurecer imagens coloridas:
		if (strcmp(tipo,"P3")==0) {
		for(i=0;i<altura;i++)
			for(j=0;j<largura;j++) {
				for(k=0;k<3;k++){
					valor = (int)m[i][j][k];			//Seleciona valor do pixel
					valor -= fator;						//Escurece-o
					if (valor<0)						//Se o valor der negativo, assume valor preto(0).
						valor = 0;
					m[i][j][k] = (unsigned char)valor;	//Matriz original recebe a modificacao.
				}
			}
		} else if (strcmp(tipo,"P2")==0) {
		
		//[FILTRO] Escurecer imagens em tons de cinza:
	  		 		for(i=0;i<altura;i++)
						for(j=0;j<largura;j++) {
							valor = (int)m[i][j][1];			//Seleciona valor do pixel
							valor -= fator;						//Escurece-o
							if (valor<0)						//Se o valor der negativo, assume valor preto(0).
								valor = 0;
							m[i][j][1] = (unsigned char)valor;	//Matriz original recebe a modificacao.
						}	
					}
	}	
	
	if(opcao==2){
		// [FILTRO] Clarear imagens coloridas:
		cout << "Qual o fator de clareamento (1-100) ? ";
		cin >> fator;
			if (strcmp(tipo,"P3")==0) {
				for(i=0;i<altura;i++)
					for(j=0;j<largura;j++) {
						for(k=0;k<3;k++){
							valor = (int)m[i][j][k];			//Seleciona o pixel
							valor += fator;						//O clareia
							if (valor>=255)						//Se o novo valor for >255, deixa branco(255).
								valor = 255;
							m[i][j][k] = (unsigned char)valor;	//Matriz original recebe a modificacao.
						}
					}
			}
	else if (strcmp(tipo,"P2")==0) {
			// [FILTRO] Clarear imagens tons de cinza:
			for(i=0;i<altura;i++)
				for(j=0;j<largura;j++) {
					valor = (int)m[i][j][1];			//Seleciona o pixel
					valor += fator;						//O clareia
					if (valor>=255)						//Se o novo valor for >255, deixa branco(255).
						valor = 255;
					m[i][j][1] = (unsigned char)valor;	//Matriz original recebe a modificacao.
				}
	
		}
	}

	if(opcao==3){
		if (strcmp(tipo,"P2")==0) {
			//[FILTRO] Inverter imagens tons de cinza horizontalmente:
			int k,aux;
			for(i=0;i<altura;i++)
				for(j=0, k=largura-1; j<largura/2;j++, k--) { //Processo de inverter matrizes usando uma variavel auxiliar
					aux = (int)m[i][j][1];
					m[i][j][1]= m[i][k][1];
					m[i][k][1]=aux;
				}
	}else if (strcmp(tipo,"P3")==0) {
	
				//[FILTRO] Inverter imagens coloridas horizontalmente:
				int aux,k,x;
				for(i=0;i<altura;i++)
					for(j=0, x=largura-1; j<largura/2;j++, x--) {
						for(k=0;k<3;k++){
							aux = m[i][j][k];			//Processo de inverter matriz usando uma variavel auxiliar
							m[i][j][k]= m[i][x][k];
							m[i][x][k]=aux;
						}
					}
		}
	}
	if(opcao==4){
		//[FILTRO] Negativar imagens coloridas:
		if (strcmp(tipo,"P3")==0) {
			for(i=0;i<altura;i++)			
				for(j=0;j<largura;j++) {	//Aplicando a subtracao(255-x), sendo x o valor anterior de 0 a 255 do pixel
					for(k=0;k<3;k++){		//Com essa subtracao, a cor do pixel sera invertida
						valor = (255-(int)m[i][j][k]);			
						m[i][j][k] = (unsigned char)valor;	//Armazena a modificacao na matriz principal
					}
				}
		} else if (strcmp(tipo,"P2")==0) {
		//[FILTRO] Negativar imagens em tons de cinza:
	   			for(i=0;i<altura;i++)
					for(j=0;j<largura;j++) {			//Aplicando a subtracao(255-x), sendo x o valor anterior de 0 a 255 do pixel
						valor = (255-(int)m[i][j][1]);	//Com essa subtracao, a cor do pixel sera invertida
						m[i][j][1] = (unsigned char)valor;	//Armazena a modificacao na matriz principal
					}
		}
	}
	
	if(opcao==5){  //[FILTRO] Transformar Imagem Colorida em Imagens Tons de Cinza
		for (i = 0; i < altura; i++){
			for (j = 0; j < largura; j++){
				for(k=0;k<3;k++){ 	//Calcula a média entre os pixels R G B da imagem colorida:
					valor = (((int)m[i][j][k] + (int)m[i][j][k+1] + (int)m[i][j][k+2])/3);   
					m[i][j][k]= (unsigned char)valor;  //Armazena a modificacao na matriz principal
		
					}
				}
		}
	}
	
	if(opcao==6){
		if (strcmp(tipo,"P2")==0) { //[FILTRO]  SOBEL (Dectectar bordas) - Imagens em Tons de Cinza
			int temp=0;				//Aplicar o Efeito definido pela Matriz do Sobel em uma matriz MX
			for(i=0;i<altura;i++){	
				for(j=0;j<largura;j++){	//Como para aplicar a matriz existem limitacoes em diferentes pixels
					if(i==0){			//utilizamos casos diferentes para grupos de 1 ou mais pixels da tabela.
						if(j==0){ //LOCAL DO PIXEL: Extremo superior esquerdo
							temp=(-2*m[i+1][j][1])+(-1*m[i+1][j+1][1]);	//Aplicando as multiplicacoes e somas no pixel.
							if (temp>255){		
								temp=255;		//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp; //Armazena o calculo na matriz de filtros MX
						}else if (j==largura-1){ //LOCAL DO PIXEL: extremo superior direito
							temp=(-1*m[i+1][j-1][1])+(-2*m[i+1][j][1]);
							if (temp>255){
								temp=255; 
							}else if (temp<0){  //O processo descrito anteriormente é análogo para os demais pixels da matriz.
								temp=0;	
							}
							mx[i][j][1]= temp;	
						}
						if ((j!=0 )&& (j!=largura-1)){
							//LOCAL DO PIXEL: Borda superior
							temp= (-1*m[i+1][j-1][1])+(-2*m[i+1][j][1])+(-1*m[i+1][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}
					}else{
						if(i==altura-1){
							if(j==0){ //LOCAL DO PIXEL: Extremo inferior esquerdo
								temp= (2*(m[i-1][j][1]))+ (1*(m[i-1][j+1][1]));
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp;
							}else if (j==largura-1){ 	//LOCAL DO PIXEL: Extremo inferior direito
								temp= (1*(m[i-1][j-1][1]))+ (2*(m[i-1][j][1]));
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp;
							}
							if ((j!=0)&& (j!=largura-1)){	//LOCAL DO PIXEL:Borda inferior
								temp= (1*(m[i-1][j-1][1]))+ (2*(m[i-1][j][1]))+ (1*(m[i-1][j+1][1]));
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp;
							}
						}	
					}
					if ((j==0) && ((i!=0) && (i!=altura-1))){
						//LOCAL DO PIXEL: borda esquerda
						temp= (2*(m[i-1][j][1]))+ (1*(m[i-1][j+1][1]))+(-2*m[i+1][j][1])+(-1*m[i+1][j+1][1]);
						if (temp>255){
							temp=255;
						}else if (temp<0){
							temp=0;	
						}
						mx[i][j][1]= temp;
					}else if ((j==largura-1) && ((i!=0) && (i!=altura-1) )){
						//LOCAL DO PIXEL: borda direita
						temp= (1*(m[i-1][j-1][1]))+ (2*(m[i-1][j][1]))+ (-1*m[i+1][j-1][1])+(-2*m[i+1][j][1]);
						if (temp>255){
							temp=255;
						}else if (temp<0){
							temp=0;	
						}
						mx[i][j][1]= temp;
					}
					
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da matriz (sem limitacoes)
						temp= (1*(m[i-1][j-1][1]))+ (2*(m[i-1][j][1]))+ (1*(m[i-1][j+1][1]))+ (-1*m[i+1][j-1][1])+(-2*m[i+1][j][1])+(-1*m[i+1][j+1][1]);
						if (temp>255){
							temp=255;
						}else if (temp<0){
							temp=0;	
						}
						mx[i][j][1]= temp;
					}	
			}	
		}
			//Calculando a segunda etapa (matriz) do Efeito Sobel e realizando a média aritmetica nos pixels:
			for(i=0;i<altura;i++){
				for(j=0;j<largura;j++){  //Processo similar ao descrito acima, a unica diferenca esta relacionada a
					if(i==0){			// aplicacao da media aritmetica nos novos valores juntamente aos anteriores
						if(j==0){ 		// da matriz MX, assim o filtro de Sobel estará pronto com somente uma matriz MX
							//LOCAL DO PIXEL: Extremo superior esquerdo
							temp=(-2*m[i][j+1][1])+(-1*m[i+1][j+1][1]);
							if (temp>255){
								temp=255;	//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;  // <-- Aplicacao e armazenamento da media aritmetica com novos valores
						}else if (j==largura-1){
							    //LOCAL DO PIXEL: Extremo superior direito
								temp=(2*m[i][j-1][1])+(1*m[i+1][j-1][1]);
								if (temp>255){			//O processo descrito anteriormente e' analogo para os demais casos
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}else{
						//LOCAL DO PIXEL: Borda superior
							temp=(2*m[i][j-1][1])+(-2*m[i][j+1][1])+(1*m[i+1][j-1][1])+(-1*m[i+1][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}
					}
					if(j==0){
						if(i==altura-1){
							//LOCAL DO PIXEL: Extremo inferior esquerdo
							temp=(-1*m[i-1][j+1][1])+(-2*m[i][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}else if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: Eorda esquerda
							temp=(-1*m[i-1][j+1][1])+(-2*m[i][j+1][1])+(-1*m[i+1][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}	
					}
					if(i==altura-1){
						if(j=largura-1){
							//LOCAL DO PIXEL: extremo inferior direito
							temp=(1*m[i-1][j-1][1])+(2*m[i][j-1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}else if((j!=largura-1)&&(j!=0)){
								//LOCAL DO PIXEL: borda inferior
								temp=(-1*m[i+1][j+1][1])+(1*m[i+1][j-1][1])+(2*m[i][j-1][1])+(-2*m[i][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}
					}
					if(j==largura-1){
						if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda direita
							temp=(1*m[i-1][j-1][1])+(2*m[i][j-1][1])+(1*m[i+1][j-1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
						}
					}
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da matriz:
						temp=(1*m[i-1][j-1][1])+(-1*m[i-1][j+1][1])+(2*m[i][j-1][1])+(1*m[i+1][j-1][1])+(-2*m[i][j+1][1])+(-1*m[i+1][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= (mx[i][j][1]+temp)/2;
					}
				}
			} 
				for(i=0;i<altura;i++){ //Copiando a matriz de filtro MX para a matriz principal.
					for(j=0;j<largura;j++){
						m[i][j][1]=(mx[i][j][1]);
					}
				}	
		}

		if (strcmp(tipo,"P3")==0) { 	//[FILTRO]  SOBEL (Dectectar bordas) - Imagens Coloridas
			int temp=0;
			for(i=0;i<altura;i++){		//Aplicar o Efeito definido pela primeira Matriz do Sobel em uma matriz MX
				for(j=0;j<largura;j++){		//Mesmo processo de restricoes na aplicacao da matriz descrito anteriormente
					if(i==0){				//Agora o programa armazenara as modificacoes em cada pixel R G B
						if(j==0){ //LOCAL DO PIXEL: extremo superior esquerdo
							for(k=0;k<3;k++){  //<---- Cont usado para aplicar filtro em cada pixel R G B
								temp=(-2*m[i+1][j][k])+(-1*m[i+1][j+1][k]);
								if (temp>255){
									temp=255;	//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp; //<---- Armazenando em cada pixel R G B o filtro aplicado
							}
						}else if (j==largura-1){ //LOCAL DO PIXEL: extremo superior direito
							for(k=0;k<3;k++){
								temp=(-1*m[i+1][j-1][k])+(-2*m[i+1][j][k]);
								if (temp>255){
									temp=255;		//O processo descrito anteriormente e' analogo para os demais casos
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}	
						}
						if ((j!=0 )&& (j!=largura-1)){
							//LOCAL DO PIXEL: borda superior
							for(k=0;k<3;k++){
								temp= (-1*m[i+1][j-1][k])+(-2*m[i+1][j][k])+(-1*m[i+1][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}
					}else{
						if(i==altura-1){
							if(j==0){ //LOCAL DO PIXEL: extremo inferior esquerdo
								for(k=0;k<3;k++){
									temp= (2*(m[i-1][j][k]))+ (1*(m[i-1][j+1][k]));
									if (temp>255){
										temp=255;
									}else if (temp<0){
										temp=0;	
									}
									mx[i][j][k]= temp;
								}
							}else if (j==largura-1){ //LOCAL DO PIXEL: extremo inferior direito
								for(k=0;k<2;k++){
									temp= (1*(m[i-1][j-1][k]))+ (2*(m[i-1][j][k]));
									if (temp>255){
										temp=255;
									}else if (temp<0){
										temp=0;	
									}
									mx[i][j][k]= temp;
								}
							}
							if ((j!=0)&& (j!=largura-1)){
							//LOCAL DO PIXEL: borda inferior
								for(k=0;k<3;k++){
									temp= (1*(m[i-1][j-1][k]))+ (2*(m[i-1][j][k]))+ (1*(m[i-1][j+1][k]));
									if (temp>255){
										temp=255;
									}else if (temp<0){
										temp=0;	
									}
									mx[i][j][k]= temp;
								}
							}
						}	
					}
					if ((j==0) && ((i!=0) && (i!=altura-1))){
					//LOCAL DO PIXEL: pixels da borda esquerda
						for(k=0;k<3;k++){
							temp= (2*(m[i-1][j][k]))+ (1*(m[i-1][j+1][k]))+(-2*m[i+1][j][k])+(-1*m[i+1][j+1][k]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][k]= temp;
						}
					}else if ((j==largura-1) && ((i!=0) && (i!=altura-1) )){
						//LOCAL DO PIXEL: borda direita
						for(k=0;k<3;k++){
							temp= (1*(m[i-1][j-1][k]))+ (2*(m[i-1][j][k]))+ (-1*m[i+1][j-1][k])+(-2*m[i+1][j][k]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][k]= temp;
						}
						
					}
					
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da matriz
						for(k=0;k<3;k++){
							temp= (1*(m[i-1][j-1][k]))+ (2*(m[i-1][j][k]))+ (1*(m[i-1][j+1][k]))+ (-1*m[i+1][j-1][k])+(-2*m[i+1][j][k])+(-1*m[i+1][j+1][k]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][k]= temp;
						}
					}	
			}	
		}  
			//Calculando a segunda etapa (matriz) do Efeito Sobel e realizando a média aritmetica nos pixels:
			for(i=0;i<altura;i++){			//Aplicar o Efeito definido pela segunda Matriz do Sobel em MX
				for(j=0;j<largura;j++){    //Como é a segunda etapa, o programa fara a media aritmetica em cada pixel RGB
					if(i==0){				//Dessa forma, ele poupará memoria RAM do PC do usuario por usar somente 1 matriz
						if(j==0){ 
							//LOCAL DO PIXEL: extremo superior esquerdo
							for(k=0;k<3;k++){
								temp=(-2*m[i][j+1][k])+(-1*m[i+1][j+1][k]);
								if (temp>255){
									temp=255;	//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2; //<----- Media aritmetica em cada pixel RGB usando o novo valor
							}
						}else if (j==largura-1){
							    //LOCAL DO PIXEL: extremo superior direito
							for(k=0;k<3;k++){
								temp=(2*m[i][j-1][k])+(1*m[i+1][j-1][k]);
								if (temp>255){
									temp=255;		//O processo descrito anteriormente e' analogo para os demais casos
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
								}
						}else{
							//LOCAL DO PIXEL: borda superior
							for(k=0;k<3;k++){
								temp=(2*m[i][j-1][k])+(-2*m[i][j+1][k])+(1*m[i+1][j-1][k])+(-1*m[i+1][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}
					}
					if(j==0){
						if(i==altura-1){
							//LOCAL DO PIXEL: extremo inferior esquerdo
							for(k=0;k<3;k++){
								temp=(-1*m[i-1][j+1][k])+(-2*m[i][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}else if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda esquerda
							for(k=0;k<3;k++){
								temp=(-1*m[i-1][j+1][k])+(-2*m[i][j+1][k])+(-1*m[i+1][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}	
					}
					if(i==altura-1){
						if(j=largura-1){
							//LOCAL DO PIXEL: extremo inferior direito
							for(k=0;k<3;k++){
								temp=(1*m[i-1][j-1][k])+(2*m[i][j-1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}else if((j!=largura-1)&&(j!=0)){
							//LOCAL DO PIXEL: borda inferior
							for(k=0;k<3;k++){
								temp=(-1*m[i+1][j+1][k])+(1*m[i+1][j-1][k])+(2*m[i][j-1][k])+(-2*m[i][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}
					}
					if(j==largura-1){
						if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda direita
							for(k=0;k<3;k++){
								temp=(1*m[i-1][j-1][k])+(2*m[i][j-1][k])+(1*m[i+1][j-1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= (mx[i][j][k]+temp)/2;
							}
						}
					}
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da matriz
						for(k=0;k<3;k++){
							temp=(1*m[i-1][j-1][k])+(-1*m[i-1][j+1][k])+(2*m[i][j-1][k])+(1*m[i+1][j-1][k])+(-2*m[i][j+1][k])+(-1*m[i+1][j+1][k]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][k]= (mx[i][j][k]+temp)/2;
						}
					}
				}
			}  
		}
			for(i=0;i<altura;i++){ //Copiando os pixels da matriz MX com o filtro para a matriz original.
				for(j=0;j<largura;j++){
					for(k=0;k<3;k++){
						m[i][j][k]=(mx[i][j][k]);
					}
				}
			} 
					
	}
	if(opcao==7){ 
		if (strcmp(tipo,"P2")==0) {
		//[FILTRO]  SHARPENING (Contraste) - Imagens em Tons de Cinza
			int temp=0;
				for(i=0;i<altura;i++){      //Esse filtro e' mais simples, somente foi aplicado a matriz de filtro e copiado
					for(j=0;j<largura;j++){ //para a matriz MX, mantendo as restricoes das bordas em diferentes casos
						if(i==0){
							if(j==0){ 
								//LOCAL DO PIXEL: extremo superior esquerdo
								temp=(5*m[i][j][1])+(-1*m[i+1][j][1])+(-1*m[i][j+1][1]); //Aplicando matriz de filtro no pixel
								if (temp>255){
									temp=255;//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp; //Armazenando em MX o pixel com o filtro
							}else if (j==largura-1){
							    //LOCAL DO PIXEL: extremo superior direito
								temp=(-1*m[i][j-1][1])+(5*m[i][j][1])+(-1*m[i+1][j][1]);
								if (temp>255){
									temp=255;				//O processo descrito anteriormente e' analogo para os demais casos
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp;
							}else{
								//LOCAL DO PIXEL: borda superior
								temp=(5*m[i][j][1])+(-1*m[i][j-1][1])+(-1*m[i][j+1][1])+(-1*m[i+1][j][1]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][1]= temp;
							}
					}
					if(j==0){
						if(i==altura-1){
							//LOCAL DO PIXEL: extremo inferior esquerdo
							temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}else if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda esquerda
							temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i+1][j][1])+(-1*m[i][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}	
					}
					if(i==altura-1){
						if(j=largura-1){
							//LOCAL DO PIXEL: extremo inferior direito
							temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i][j-1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}else if((j!=largura-1)&&(j!=0)){
							//LOCAL DO PIXEL: borda inferior
							temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i][j-1][1])+(-1*m[i][j+1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}
					}
					if(j==largura-1){
						if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda direita
							temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i+1][j][1])+(-1*m[i][j-1][1]);
							if (temp>255){
								temp=255;
							}else if (temp<0){
								temp=0;	
							}
							mx[i][j][1]= temp;
						}
					}
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da Matriz
						temp=(5*m[i][j][1])+(-1*m[i-1][j][1])+(-1*m[i+1][j][1])+(-1*m[i][j-1][1])+(-1*m[i][j+1][1]);
						if (temp>255){
							temp=255;
						}else if (temp<0){
							temp=0;	
						}
						mx[i][j][1]= temp;
					}
				}
			}
			for(i=0;i<altura;i++){ //Copiando valores da matriz do filtro(MX) para a matriz principal 
				for(j=0;j<largura;j++){
					m[i][j][1]=(mx[i][j][1]);
				}
			}
		}else if (strcmp(tipo,"P3")==0) {
		//[FILTRO]  SHARPENING (Contraste) - Imagens Coloridas
		int temp=0;
			for(i=0;i<altura;i++){
				for(j=0;j<largura;j++){
					if(i==0){
						if(j==0){ 
							//LOCAL DO PIXEL: extremo superior esquerdo
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i+1][j][k])+(-1*m[i][j+1][k]);
								if (temp>255){
									temp=255;	//Verificando se o pixel ultrapassou os limites de valor: (>255) ou (<0).
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;	//Armazenando em MX o pixel com o filtro
							}
						}else if (j==largura-1){
						    //LOCAL DO PIXEL: extremo superior direito
							for(k=0;k<3;k++){
								temp=(-1*m[i][j-1][k])+(5*m[i][j][k])+(-1*m[i+1][j][k]);
								if (temp>255){
									temp=255;			//O processo descrito anteriormente e' analogo para os demais casos
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}else{
							//LOCAL DO PIXEL: borda superior
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i][j-1][k])+(-1*m[i][j+1][k])+(-1*m[i+1][j][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}
					}
					if(j==0){
						if(i==altura-1){
							//LOCAL DO PIXEL: extremo inferior esquerdo
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}else if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda esquerda
								for(k=0;k<3;k++){
									temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i+1][j][k])+(-1*m[i][j+1][k]);
									if (temp>255){
										temp=255;
									}else if (temp<0){
										temp=0;	
									}
									mx[i][j][k]= temp;
								}
						}	
					}
					if(i==altura-1){
						if(j=largura-1){
							//LOCAL DO PIXEL: extremo inferior direito
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i][j-1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}else if((j!=largura-1)&&(j!=0)){
							//LOCAL DO PIXEL: borda inferior
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i][j-1][k])+(-1*m[i][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}
					}
					if(j==largura-1){
						if((i!=0)&&(i!=altura-1)){
							//LOCAL DO PIXEL: borda direita
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i+1][j][k])+(-1*m[i][j-1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
						}
					}
					if ((i!=0)&&(j!=0)&&(j!=largura-1)&&(i!=altura-1)){
						//LOCAL DO PIXEL: Interior da matriz:
							for(k=0;k<3;k++){
								temp=(5*m[i][j][k])+(-1*m[i-1][j][k])+(-1*m[i+1][j][k])+(-1*m[i][j-1][k])+(-1*m[i][j+1][k]);
								if (temp>255){
									temp=255;
								}else if (temp<0){
									temp=0;	
								}
								mx[i][j][k]= temp;
							}
					}
				}
			}
			for(i=0;i<altura;i++){ 
				for(j=0;j<largura;j++){
					for(k=0;k<3;k++){
						m[i][j][k]=(mx[i][j][k]); //Copiando os valores da matriz de filtros(MX) para a matriz principal 
					}
				}
			}
		}
	
	
	}
	if(opcao==8){
		
		if (strcmp(tipo,"P3")==0) {	 
			cout<<"Efeito FEBRE AMARELA selecionado, lembre-se de usar repelente!"<<endl<<endl;
			for(i=0;i<altura;i++){ 
				for(j=0;j<largura;j++){
						m[i][j][0]=240; //Altera R e G para perto do maximo para deixar o tom da matriz amarelado...
						m[i][j][1]=240; 
				}
			}	
		}else if(strcmp(tipo,"P2")==0) {
				cout<<"Efeito criado para fotos coloridas, tente novamente com alguma outra."<<endl;
				return 0;
		}
	}
	if(opcao==0){
		//Encerra o programa 
		return 0;
	}

//*** FIM DO TRATAMENTO DA IMAGEM ***//

//*** GRAVACAO DO ARQUIVO***//
	arqsaida.open("novaimagem.pnm",ios::out);	//Abre arquivo para escrita
	if (!arqsaida) {
		cout << "Nao consegui criar novaimagem.pnm\n";
		return 0;
	}

	arqsaida << tipo << endl;						//tipo
	arqsaida << "# INF110\n";						//comentario
	arqsaida << largura << " " << altura << endl;	//dimensoes
	arqsaida << 255 << endl;						//maior valor
	
	/**** GRAVACAO DE IMAGENS COLORIDAS ****/
	if (strcmp(tipo,"P3")==0) {
		
		
		cout<<"Aplicando o efeito escolhido. Aguarde..."<<endl;
		for(i=0;i<altura;i++){
			for(j=0;j<largura;j++){
				for(k=0;k<3;k++){
					arqsaida << (int)m[i][j][k] << endl;	//Imprimindo a matriz principal
				}
			}
		
		}
	}else if (strcmp(tipo,"P2")==0) {
		/**** GRAVACAO DE IMAGENS EM TONS DE CINZA ****/
		cout<<"Aplicando o efeito escolhido. Aguarde..."<<endl;
			for(i=0;i<altura;i++)
			for(j=0;j<largura;j++)
				arqsaida << (int)m[i][j][1] << endl;	//Imprimindo a matriz principal
	}
	arqsaida.close();		//fecha o arquivo
	cout<<"Tudo pronto! Sua nova imagem foi criada."<<endl<<endl;
	
	cout<<"/**** Obrigado pela preferencia! A equipe MG Software agradece. ****/"<<endl;
	//***************************//
return 0;
//*** FIM DA GRAVACAO DA IMAGEM ***//
}

