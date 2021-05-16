//#define SUCESS 0
//#define FAILURE 1
//#define BLOCK_ERROR 667

typedef struct s_block  {
	int index;
	char TimeStamp[30];
	char hash_precedent[100];
	char hash_root[30];
	char hash_courant[30];
	char list_transaction[100];
	int nbTransaction;
	int nonce;
}block;

int Menu();
void clearConsole();
void nouvelleTransaction(char NomFichier[]);

int getNbBloc(char NomFichier[]);

void consultationBloc(char NomFichier[]);

void afficherTousLesBlocs(char nomFichier[]);

void informationBlockchain(char nomFichier[]);
