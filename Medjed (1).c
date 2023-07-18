#include<stdio.h>
#include<stdlib.h>
#include<string.h>	//Necessário para facilitar a manipulação de Strings
#include<time.h>	//Necessário para o srand, que gera os números de registro.
#include<ctype.h>	//Possui algumas funções úteis, como isdigit()
#include<stdbool.h>	//Importante para funções booleanas, principalmente as do tipo Check_*
#define CLEAR ("\e[1;1H\e[2J") 		//Dentro de um printf, CLEAR é capaz de limpar a tela do console
typedef struct { //Estrutura básica de um cliente
		char nome[30];
		char senha[15];
		int registro; //Código único que identifica um cliente
		int is_client; //Esse valor indicar se o cliente já está registrado ou não
		float saldo; 
	}user;
	//Para evitar warnings com declarações implicitas, decidimos declarar todas as sub-rotinas no início do programa.
bool Handler_transfer(char* filename, float *deposits, int total, float tempo, user *target, user *eye, user *placeholder);
void Data_deposit_reader(char *filename, float *deposits, int total, user *eye, user *placeholder);		//Declarado na linha 344
void Data_deposit_save(char *filename, float *deposits, int total, float tempo, user *eye, user *placeholder);		//Declarado na linha 321
void Data_deposit_initializer(char *filename);		//Declarado na linha 316
bool Handler_login(int command, char password[15], user *eye, int aux, user *placeholder);		//Declarado na linha 257
user *Data_beholder(char *filename, int *total);	//Declarado na linha 241
void Data_GroundZero();		//Declarado na linha 116
void Data_correct_read(char *filename, int *total);		//Declarado na linha 127
void Data_correct_write(char *filename, int total);		//Declarado na linha 140
void Data_register(char *filename, user data);		//Declarado na linha 151
void Pocket_pause();	//Declarado na linha 161
bool Check_username(char username[30], int is_client);		//Declarado na linha 166
bool Check_password(char password[15], int is_client); 		//Declarado na linha 190
void Check_register(int *registro);		//Declarado na linha 203
int Check_command(char command[30], int num);	//Declarado na linha 216
int main(){
	srand(time(0));
	float tempo;
	float *deposits=calloc(100, sizeof(float));
	int lenght=20;
	int index;	//Index é, provavelmente, a variável mais importante do programa, sendo essencial no "data handling". Sua principal função é contabilizar o número de structs no banco de dados.
	FILE *test;	//É por meio desse ponteiro que o programa lança entradas personalizadas ao novo usuário, chamando Data_GroundZero para criar um arquivo.
	int commandAUX;
	char c;
	char command[30];
	user *t_usuarios;  // Struct que vai armazenar os dados dos usuários
	user *t_placeholder; // Esse Struct só serve para repassar comandos
	user *t_target;
	t_placeholder=calloc(1,sizeof(user));	
	t_usuarios=calloc(lenght,sizeof(user));	//Alocação dinâmica do struct base de registro.
	t_target=calloc(1,sizeof(user));
	test=fopen("MedjedData.bin", "rb");
		if(test==NULL){	//Aqui, verificamos se o arquivo MedjedData, nosso "banco de dados", já está no sistema.
			//Recepção personalizada para novos usuários.
			printf("Looks like it's your first time here. We're glad you've chose to be part of Medjed! The safest Bank ever made\n");
			printf("We strongly recommend that you make an account. That can be done by pressing (2) in our menu.\n");
			Data_GroundZero();
			index=1;
		}
		else{
			Data_correct_read("MedjedData.bin", &index);
			fclose(test);
		}

	while(1){	//O loop em que o menu principal e as principais funcionalidades ocorrem
		user *eye;
		int aux;
		eye=Data_beholder("MedjedData.bin", &aux);
		printf(">> Bank Medjed <<\n");	//Aqui começa o menu principal
		printf("Welcome! How can we help you today?\n");
		printf("1 - Login\n");	//Acessa uma conta já registrada no banco de dados (Sistema rudimentar de login)
		printf("2 - Create a new account\n");	//Cria uma nova conta e registra em MedjedData.bin
		printf("3 - Restricted Area\n");		//Aqui, é possível acessar as informações do banco de dados
		printf("4 - Leave\n");
		printf("Enter your command.\n"); 
		fflush(stdin);	//Limpa o buffer do teclado (Importante para prevenir desastres)
		commandAUX=Check_command(command, 4);
		switch(commandAUX){
		case 1:		//Aqui, começa o sub-menu do login, os dados do usuário logado são armazenados temporariamente em t_placeholder
			printf(CLEAR);
			printf("Please, enter your registration code.\n");	//O login é feito com o código de registro gerado com o srand na função Check_register
			if(Handler_login(t_placeholder[0].registro, t_placeholder[0].senha, eye, aux, t_placeholder)){		//Obviamente, essa função checa se os dados fornecidos estão no banco de dados
				while(1){
					Data_deposit_reader("Deposits.csv", deposits, index, eye, t_placeholder);	//Essa função não funcionaria sem a Data_deposit_initializator.
					//Isso poderia gerar um problema, já que essa outra função está declarada depois, mas é impossível chegar até aqui sem criar uma conta antes, logo não há problemas.
					printf("Welcome %s", t_placeholder[0].nome);
					printf("Balance: %.2f\n", t_placeholder[0].saldo);
					printf("How can we help you today?\n");
					printf("1 - Deposit\n");	//O cliente pode depositar arbitrariamente uma quantia de dinheiro
					printf("2 - Withdraw money\n");		//O cliente pode sacar uma quantia de dinheiro, contanto que ele tenha saldo suficiente
					printf("3 - Transfer\n");	//O cliente pode transferir uma certa quantia.
					printf("4 - Leave\n");
					commandAUX=Check_command(command, 4);
					switch(commandAUX){
						case 1: ;	//Aqui, somos lembrados que Switch-Case é apenas um GOTO disfarçado. sem esse ;, poderia haver problemas.
							printf(CLEAR);
							printf("How much would you like to deposit?\n");
							scanf("%f", &tempo);
							if(tempo<0){
								printf("#ERROR: You cannot deposit negative money!\n");
								while ((c = getchar()) != '\n' && c != EOF) {} 
								break;
							}
							Data_deposit_save("Deposits.csv", deposits, index, tempo, eye, t_placeholder);
							//Data_deposit_save vai salvar o novo valor inserido num arquivo csv chamado Deposits.csv
							fflush(stdin);
							break;
						case 2:
							printf(CLEAR);
							printf("How much would you like to withdraw?\n");
							printf("Current balance: %.2f\n", t_placeholder[0].saldo);
							scanf("%f", &tempo);
							if(tempo<0){
								printf("#ERROR: You cannot withdraw negative money!\n");
								while ((c = getchar()) != '\n' && c != EOF) {} 
								break;
							}
							if(tempo>t_placeholder[0].saldo){	//Logicamente, não é possível ficar com um saldo negativo
								printf("#ERROR: You cannot withdraw more money than what you have!\n");
								while ((c = getchar()) != '\n' && c != EOF) {}	//Novamente, aqui sobra caracteres no buffer e o fflush não funcionou
								break;
							}
							else{
								tempo=-tempo;	//A logica aqui é a mesma da função deposit, mas, dessa vez, será "depositado" um valor negativo.
								Data_deposit_save("Deposits.csv", deposits, index, tempo, eye, t_placeholder);
								printf("Success!\n");
								break;
							}
						case 3:
							for(int i=0;i<aux-1;i++){
								if(t_placeholder[0].registro!=eye[i].registro){
									printf("%d %s", eye[i].registro, eye[i].nome);
								}
							}
							if(index==2){	//Esse if serve para que o usuário não consiga abrir essa janela se não houverem outros registros disponíveis para a transferencia
								//Obviamente, um usuário não pode transferir para ele mesmo	(Na prática, isso é permitido, o programa só não vai listar essa possibilidade)
								printf("#ERROR: No other users found\n");
								break;
							}
							printf("To whom would you like to transfer?\n");
							scanf(" %d", &t_target[0].registro);
							printf("How much would you like to transfer?\n");
							scanf(" %f", &tempo);
							if(Handler_transfer("Deposits.csv", deposits, index, tempo, t_target, eye, t_placeholder)){
								printf("Success!\n");
								printf("Press ENTER to proceed\n");
								Data_deposit_save("Deposits.csv", deposits, index, tempo, eye, t_target);
								//O valor do novo saldo do usuário que recebeu é salvo
								Data_deposit_reader("Deposits.csv", deposits, index, eye, t_target);
								//O programa lê novamente o arquivo, atualizando deposits
								tempo=-tempo;
								Data_deposit_save("Deposits.csv", deposits, index, tempo, eye, t_placeholder);
								//O valor transferido é subtraído do saldo do usuário que o enviou
								printf(CLEAR);
							}
							else{
								while ((c = getchar()) != '\n' && c != EOF) {} 
							}
							break;
						case 4:	
							printf(CLEAR);
							break;
					}
					if(commandAUX==4){
						break;
					}
				}
				break;		//Temos muitos breaks reunidos, pois se trata de um while dentro de um Switch-Case dentro de um Switch-Case
			}
			break;
		case 2:	//Início do processo de registro
			printf(CLEAR);
			printf("We need to collect some information about you.\n");
			printf("This process may take some time\n");
			printf("1 - Proceed to the registration\n");
			printf("2 - Return to the main menu\n");
			if(Check_command(command, 2)==1){
				//printf("Iniciando procedimento de registro...\n");
				printf("Please, enter your full name\n");
				if(Check_username(t_usuarios[index].nome,0)){		//Checa o input de username do usuário (Evita números e caracteres especiais)
					//printf("Nome registrado com sucesso!\n");
					if(Check_password(t_usuarios[index].senha, 0)){	//Checa o input de senha do usuário (Evita senhas maiores que 15 digitos e senhas vazias)
						//printf("Senha registrada com sucesso!\n");
						Check_register(&t_usuarios[index].registro);		//Aqui, o novo usuário recebe seu número de registro único.
							printf("Keep your registration number close at hand. It'll be used to log you in\n");
							t_usuarios[index].saldo=0;
							Data_deposit_initializer("Deposits.csv");
							Pocket_pause();		//O Pocket_pause reduz a poluição da tela.
							printf(CLEAR);
							fflush(stdin);
							Data_register("MedjedData.bin", t_usuarios[index]);		 //Aqui, os dados inseridos são registrados no banco de dados
							index++;
							Data_correct_write("MedjedData.bin", index);		//Essa função é fundamental, voltando àquele conceito de Magic Number.
							//A primeira linha do Arquivo binário indica o número de structs armazenados nele.
							//Index, no início do programa, recebe esse valor e, ao final dessa função, é incrementado e "corrigido" dentro do arquivo.
							break;
						}
					}
				else{
					printf("Returning to the main menu...\n");
					printf(CLEAR);
					break;
				}
			}
			break;
		case 3:
			for(int i=0;i<aux-1;i++){
				printf("Name: %s", eye[i].nome);
				printf("Password: %s", eye[i].senha);
				printf("Registration Number: %d\n", eye[i].registro);
			}
			Pocket_pause();
			printf(CLEAR);
			break;
		case 4: 
			free(t_usuarios);
			free(t_placeholder);
			free(t_target);
			free(eye);
			free(deposits);
			printf("Au revoir!\n");
			return 0;
		}
	}
}	//Aqui é o fim do Main e o início das sub-rotinas
void Data_GroundZero(){ 	//Essa função serve para iniciar o Arquivo (Funciona de modo análogo ao Touch do Linux)
	FILE *Arq;
	int aux=1;
	Arq=fopen("MedjedData.bin", "wb");
	if(Arq==NULL){
		printf("#ERROR: Fatal error in the opening of the file\n");
		exit(1);
	}
	fwrite(&aux, sizeof(int), 1, Arq);
	fclose(Arq);
}
void Data_correct_read(char *filename, int *total){		//Esse par de funções funcionam como o magic number. Total indica o "índice" do array de structs já registrados na memória.
	//Esse índice é sempre repassado para o programa em cada execução, daí o programa consegue resgata os dados registrados previamente
	//Não teríamos conseguido fazer esse sistema funcionar sem a ajuda do monitor (Agradecemos a ele)
	FILE *Arq;
	Arq=fopen(filename, "rb");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);
	}
	fseek(Arq, 0, SEEK_SET);
	fread(total, sizeof(int),1,Arq);	//Se total não fosse passado por referência, fread só aceitaria &total, nesse caso haveria um problema grave. (Aprendemos isso da pior maneira possível)
	fclose(Arq);
}
void Data_correct_write(char *filename, int total){
	FILE *Arq;
	Arq=fopen(filename, "rb+");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);
	}
	fseek(Arq, 0, SEEK_SET);
	fwrite(&total, sizeof(int),1,Arq);
	fclose(Arq);
} 
 void Data_register(char *filename, user data){		//Data_register grava os dados do registro no arquivo Medjed.bin
	FILE *FileOpener;
	FileOpener=fopen(filename, "ab");
	if(FileOpener==NULL){
		printf("#ERROR: The programm couldn't find the file\n");
		exit(1);
	}
	fwrite(&data, sizeof(user),1, FileOpener);
	fclose(FileOpener);
}
void Pocket_pause(){	//Essa sub-rotina pausa momentaneamente o programa.
	char temp[3];
	printf("Press ENTER to proceed\n");
	fgets(temp,3,stdin);
}
bool Check_username(char username[30], int is_client){	//Aqui o username é devidamente checado
	int temp;	//Temp é uma variável temporária
	if(is_client==0){
	while(1){
		fgets(username,30,stdin);
		temp=0;
		for(int i=0;i<strlen(username)-1;i++){
			if(isalpha(username[i])==false && username[i]!=' '){	//Nomes com números ou caracteres especiais são barrados
				temp++;
			}
		}
		if(temp==0){
			printf("Bem vindo %s", username);
			return true;	//A sub-rotina retorna true se o username foi aceito.
		}
		else{
			break;
		}
	}
	printf("#ERROR: Invalid username\n");
	printf("Please, retry!\n");
	Check_username(username,is_client);
	}
}
bool Check_password(char password[15], int is_client){
	while(is_client==0){
		printf("Please, enter a new password\n");
		fgets(password, 16, stdin);
		//Não queremos que o fgets "corte" parte da senha quando mais de 15 caracteres são inseridos
		if(strlen(password)-1>15 || strlen(password)<=1){
			printf("#ERROR: Invalid password! Please, retry\n");
			continue;
		}
		is_client=1; //Se a senha for validada, o cliente está registrado
		return true;
	}
}
void Check_register(int *registro){		//Check_register é uma sub-rotina cuja passagem de parâmetro acontece por referência.
	//Se o parâmetro fosse passado por value, seu valor verdadeiro não seria alterado ao final.
	//O nome e a senha não precisaram da passagem por referência, pois eram strings (Que já são ponteiros)
	//Bem interessante.
	int aux=0;
	while(aux<9999){  //Faremos registros de 5 algarismos;
		aux=rand()%100000;
	}
	*registro=aux;
	printf("Your registration number is: %d\n", *registro);
	//Duas pessoas não podem ter o mesmo número de registro!
}
int Check_command(char command[30], int num){ // Essa função checa se o valor inserido é válido (Foi surpreendentemente difícil de fazer)
	//Num é, basicamente, o tamanho máximo que command pode assumir.
	int temp=0; 
	while(1){
		fgets(command,25,stdin);
		//Tentamos muitas vezes, mas o fflush não colaborou. quando um input ultrapassa 23 caracteres, o programa solta múltiplas mensagens de erro.
		//O scanf deixaria um ENTER "pendurado", o que fatalmente saltaria o input de username
		//Optamos por deixar o fgets, embora o problema das mensagens de erro persista
		fflush(stdin);
		for(int i=0;i<strlen(command)-1;i++){	//Não queremos que ocorra algum erro no programa quando command é um float ou uma string.
			if(isdigit(command[i])==false){	
				temp++;
			}
		}
		if(temp==0 && (atoi(command)>=1 && atoi(command)<=num)){	//Aqui, o atoi de command é limitado até num
			return atoi(command);
		}
		else{
			break;
		}
	}
	printf("#ERROR: Invalid value entered!\n");
	printf("Please, retry!\n");
	Check_command(command, num); 	//Sim, isso é uma função recursiva.
}
user *Data_beholder(char *filename, int *total){ 	//Data_beholder é uma função que lê as structs armazenadas em Medjed.bin e as retorna ao programa.
	//Ela é usada junto com um user *eye, ou seja, um olho que verifica o que está registrado
	FILE *Arq;
	Arq=fopen(filename, "rb");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);
	}
	user *data;
	fread(total, sizeof(int),1,Arq);
	data=calloc(*total, sizeof(user));
	fread(data, sizeof(user), *total, Arq);
	fclose(Arq);
	return data;	//Aqui fica o questionamento, liberar o eye já é o suficiente? Ou data aqui também deveria ser liberada?
}
bool Handler_login(int command, char password[15], user *eye, int aux, user *placeholder){		//Handler_login usa eye e aux para verificar as credenciais do usuário.
	//Os dados de placeholder são redefinidos quando essa função é chamada (Veja a linha em que ela é chamada)
	char temp[6];	//Temp é necessário, pois o registro é um inteiro.
	fgets(temp, 6, stdin);
	int c;
	if(strlen(temp)>=5){
		while ((c = getchar()) != '\n' && c != EOF) {} 		//Isso deve funcionar de modo análogo ao fflush (Que, por alguma razão desconhecida, não funciona de jeito nenhum.)
	}
	printf("Please, enter your password\n");
	fgets(password,15,stdin);
	command=atoi(temp);
	for(int i = 0; i<aux-1;i++){
		if(command==eye[i].registro){	//Compara o valor inserido com os registros no banco de dados
			for(int n=0;n<aux-1;n++){
				if(strcmp(password,eye[i].senha)==0){	//Compara a senha inserida com as senhas dentro do banco de dados
					printf("Success!\n");
					//Pocket_pause();
					placeholder[0].registro=atoi(temp);		//Transforma o registro inserido no placeholder (Struct temporário do usuário logado)
					strcpy(placeholder[0].nome,eye[i].nome);	//Transforma placeholder.nome no nome encontrado no banco de dados;
					return true;
				}
			}
		}
	}
	printf("#ERROR: Invalid username or password\n");
	return false;
}
void Data_deposit_initializer(char *filename){	//Essa função adiciona um valor padrão (Zero, no caso) para o saldo de cada conta criada
	FILE *Arq=fopen(filename, "ab");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);
	}	
	fprintf(Arq, "%d,", 0);		//Devido a nossa imperícia, não conseguimos implementar essa mudança com arquivos binários. Mas isso é -- definitivamente -- um ponto a melhorar no futuro
	//Nosso objetivo, no entanto, era utilizar tudo que foi aprendido ao longo do semestre. Não podíamos deixar Arquivo CSV de fora.
	fclose(Arq);
}
void Data_deposit_save(char *filename, float *deposits, int total, float tempo, user *eye, user *placeholder){		
	//Data_deposit_save serve para editar e salvar os valores armazenados no arquivo csv.
	//deposits é um array inicializado com calloc, cujo espaço alocado foi de 100 inteiros. Portanto, nosso banco de dados só armazena funcionalmente 100 usuários
	FILE *Arq = fopen(filename, "r");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);	
	}
	int p=-1;
	char c;
	for(int i=0; i<total-1; i++){	//primeiramente, deposits armazena todos os valores já inseridos dentro do arquivo csv
		if(eye[i].registro==placeholder[0].registro){	//Aqui, p serve para localizar a posição do valor que será modificado
			p=i;
		}
		fscanf(Arq, "%f", &deposits[i]);
	}	
	fclose(Arq);
	FILE *Arq2 = fopen(filename, "w");	//Observe que Arq2 é um tipo write. Logo, todos os valores anteriores do arquivo csv serão apagados.
	//Isso não é problema, visto que, nas linhas anteriores, deposits recebeu todos os dados.
	//O deposits "modificado", ou seja, aquele cujo valor a ser editado já foi devidamente alterado e salvo no Arquivo.
	//Para o usuário, irá parecer que o arquivo foi simplesmente editado, quando, na realidade, ele foi apagado e refeito
	//Observe que o append não poderia ser usado aqui, visto que isso só clonaria os dados, ao invés de atualiza-los
	if(Arq2==NULL){
		printf("#ERROR: The programm couldn't find the file\n");
		exit(1);
	}
	for(int i=0;i<total-1;i++){
		if(i!=p){
			fprintf(Arq2,"%f,", deposits[i]);	//Aqui, os valores que não serão modificados são reinseridos
		}
		else{
			fprintf(Arq2,"%f,", deposits[i]+tempo);		//Aqui, o valor modificado é inserido
		}
	}
	while ((c = getchar()) != '\n' && c != EOF) {}	//O arquivo csv "sobra" uma vírgula no final, esse comando evita que ela fique no buffer e cause problemas futuros
	fclose(Arq2);
}
void Data_deposit_reader(char *filename, float *deposits, int total, user *eye, user *placeholder){
	//Data_deposit_reader serve para introduzir os dados armazenados de volta ao programa.
	//Essa "introdução" é feita pelo array deposits
	FILE *Arq = fopen(filename, "rt+");
	if(Arq==NULL){
		printf("#ERROR: The programm couldn't read the file\n");
		exit(1);
	}
	int p=-1;
	for(int i=0; i<total-1; i++){
		fscanf(Arq, "%f", &deposits[i]);
		fgetc(Arq);
	}	
	for(int i=0;i<total-1;i++){
		if(placeholder[0].registro==eye[i].registro){
			p=i;
			placeholder[0].saldo=deposits[p];	//O saldo do usuário logado é igual ao saldo armazenado no csv
		}
	} 
	fclose(Arq);
}
bool Handler_transfer(char* filename, float *deposits, int total, float tempo, user *target, user *eye, user *placeholder){
	//Target é o usuário que receberá a transferência
	int p=-1;
	int z=-1;
	for(int i=0; i<total-1;i++){
		if(target[0].registro==eye[i].registro){
			p=target[0].registro;
			z=i;
		}
	}
	if(p==-1){	//Erro para caso o registro inserido não seja encontrado na banco de dados
		printf("#ERROR: Inserted user not found!\n");
		return false;
	}
	if(placeholder[0].saldo<tempo){	//Erro para caso o valor transferido seja maior que o saldo
		printf("#ERROR: You cannot transfer more money than what you have!\n");
		return false;
	}
	if(tempo<0){	//Erro para caso o usuário tente enviar um valor negativo
		printf("#ERROR: You cannot insert negative values!\n");
		return false;
	}
	target[0].saldo=deposits[z];
	return true;	//O programa retorna true se não houver nenhum erro
}
