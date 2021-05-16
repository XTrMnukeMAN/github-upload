#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "block.h"






char * verificationFichier(char NomFichier[]){
  printf("(NOTPOPSS)\n");
  int fd, nbBloc;
  char *message;
  message =calloc(100,sizeof(char));
  printf("'OKOK'\n" );

  int pd = open(NomFichier,O_RDONLY);
  if(pd == -1){
    strcpy(message,"Le fichier ne peut etre ouvert du à son inexstance ou un probleme ouverture ");
		return message;
  	}
  fd=open(NomFichier,O_RDONLY);

  if(fd==-1){
    strcpy(message,"impossible d'ouvrir le fichier exit(0)");
    close(fd);
    return message;
  }

  nbBloc = lseek(fd,0,SEEK_END);
  // on se place  à la fin du fichier normalement
  // nb d'element dans la structure
  if(nbBloc%sizeof(block)!=0){
    strcpy(message,"[NO] données du fichier incorrect ");
    close(fd);
    return message;
  }
  close(pd);
  close(fd);
  strcpy(message,"[OK]");
  return message;
}

char * getTimeStamp(){
time_t ltime;
time(&ltime);
return ctime(&ltime);
}

int getNbBloc(char NomFichier[]){
  int fd,nbBloc;
  fd=open(NomFichier,O_RDONLY);
  nbBloc=lseek(fd,0,SEEK_END)/sizeof(block); // grace à lseek on sait combien il y a d'octect dans fd '
  close(fd);
  return nbBloc;
}

block getBloc(char NomFichier[],int num_bloc){
  int fd,positionLecture,longueurFiche;
  block bloc;
  fd=open(NomFichier,O_RDONLY);

  if(fd==-1){
    perror("Erreur sur le open");
    close(fd);
  }

  positionLecture=(num_bloc-1)*sizeof(block);
  lseek(fd,positionLecture,SEEK_SET);

  longueurFiche=read(fd,&bloc,sizeof(bloc));
  if(longueurFiche!=sizeof(bloc)){
    perror("Erreur : Lecture de donnée");
    close(fd);
  }

  close(fd);
  return bloc;
}


char *supRetourChariot(char message[]){
    message[strlen(message)-1]='\0';
    return message;
}


int creationBloc(char NomFichier[],block bloc){
  int fd,longueur;

  fd=open(NomFichier,O_WRONLY | O_APPEND,S_IWUSR);
  // autorisation Ecriture + autre seulement pas plus attention au droit donnée au utilisateur ! !!!

  if(fd==-1){
    perror("Erreur du open()");
    close(fd);
    return 1;
  }
  // fd donne la taille n de où commencer le write
  longueur=write(fd,&bloc,sizeof(bloc));
  // arret de la saisie
  if(longueur!=sizeof(bloc)){
    // stop la saisie et ferme le fichier
    perror("Erreur d'écriture");
    close(fd);
    return 2;
  }
  close(fd);
  return 0;
}

void nouvelleTransaction(char NomFichier[]){
  char transaction[256],exit;
  block bloc;
  strcpy(bloc.list_transaction,"");
  strcpy(bloc.TimeStamp,supRetourChariot(getTimeStamp()));
  bloc.nbTransaction=0;
  bloc.nonce=0;

  while(bloc.nbTransaction<10){
    //printf("\033[H\033[J"); clear toute la rappel tp4 de System2...
    printf("\nTransaction à enregistrer : ");
    scanf("%s",transaction);
    strcpy(bloc.hash_precedent,"hash_precedent");
    strcpy(bloc.hash_root,"hash_root");
    strcpy(bloc.hash_courant,"hash_courant");
    strcat(bloc.list_transaction,transaction);
    bloc.nbTransaction++;

    printf("\nFaire une Nouvelle transaction [y/n]  ?\n ");
    scanf("%s",&exit);

    if(exit=='n'){
        break;
    }
  }
  bloc.index=getNbBloc(NomFichier)+1;
  creationBloc(NomFichier,bloc);
}

void consultationBloc(char NomFichier[]){
    int nbBloc=getNbBloc(NomFichier),numBloc;

    block bloc;

    do
    {
        printf("\n  Quel bloc ouvrir ? [1-%d]  : ",nbBloc); // numero ok avec nb_fich
        scanf("%d",&numBloc);


        if(numBloc !=0 && numBloc > nbBloc){
            printf("Saisie invalide : fin de la fonction\n");
            break;
        }
          bloc = getBloc(NomFichier,numBloc);

        printf("\n   FICHE\n   index   n* : %d\n   Timestamp  n* : %s\n hash_precedent n* : %s\n  hash_root n* : %s\n  hash_current n* : %s\n  list_transaction n* : %s\n   nb_Transaction n* : %d\n  nonce  n* : %d\n",bloc.index,bloc.TimeStamp,bloc.hash_precedent,bloc.hash_root,bloc.hash_courant,bloc.list_transaction,bloc.nbTransaction,bloc.nonce);
    }while(0!=numBloc);
    printf("\033[H\033[J"); // clear la console
}

void afficherTousLesBlocs(char nomFichier[]){
    block bloc;
    printf("Index | Index | TimeStamp             | hash_precedent  | hash_root  | hash_courant  | ");
    for(int a =1; a<getNbBloc(nomFichier)+1;a++){
        bloc=getBloc(nomFichier,a);
        printf("%5d |  %s  |  %s  |  %s  |  %s  |  %s  |  %5d  | %5d \n",bloc.index, bloc.TimeStamp,bloc.hash_precedent, bloc.hash_root, bloc.hash_courant, bloc.list_transaction, bloc.nbTransaction, bloc.nonce);
    }
}



void informationBlockchain(char nomFichier[])
{
  printf("             =============  Information Blockchain  =============\n");
  printf("             Intégrité du fichier  : %s\n",verificationFichier(nomFichier));
  printf("             Fichier sauvegarde de la Blockchain :  %s\n",nomFichier);
  printf("             Nombre de blocs  :  %d\n",getNbBloc(nomFichier));
  printf("             Nombre de Transaction par block : %d\n",10);
  printf("...\n");
  printf("\n");
  printf("\n");
  printf("\n");
}


int menu()
{
	printf("\n");
	int choix;
	printf("\n");
	printf("                      Menu          \n");
	printf("          1 ====> Informations blockchain\n");
	printf("          2 ====> Faire une Transaction \n");
	printf("          3 ====> Consultation d'un bloc \n");
	printf("          4 ====> Afficher tous les blocs \n");
	printf("          0 ====> Quittez le programme \n");
	printf("\n");
	printf("\n");
	printf("          Navigation : ");
	scanf("%d",&choix);
	return choix;
}

void clearConsole(){
	printf("\033[H\033[J");
	// va clear la console
}
int main(){
	printf("    1     \n");
	char nomFichier[100];

	strcpy(nomFichier,"blockchain.txt");

	printf("\033[H\033[J"); // va clear la console

	informationBlockchain(nomFichier);


	while(1){
		switch(menu()){
			case 1:
				printf("\033[H\033[J"); // va clear la console
				informationBlockchain(nomFichier);
				break;

			case 2:
				printf("\033[H\033[J"); // va clear la console
				printf("     5    \n");
				nouvelleTransaction(nomFichier);
				break;

			case 3 :
				printf("\033[H\033[J"); // va clear la console
				printf("     6    \n");
				consultationBloc(nomFichier);
				break;

			case 4 :
				printf("\033[H\033[J"); // va clear la console
				printf("     7    \n");
				afficherTousLesBlocs(nomFichier);
				break;

			default:
				printf(" OUT \n ");
				return 0;
		}
	}

	return 0;
}
