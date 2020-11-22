%{
	// Auteur : Alexandre l'Heritier
	
	// Fichier YACC permettant de lire un modèle.
	#include <iostream>
	#include <vector>
	#include "Systeme.hpp"

	int yylex(void);
	void yyerror(const char *);

	Systeme systeme;

%}
// On met ces deux lignes dans le .hpp du yacc généré, sinon impossible d'utiliser Systeme.
%code requires {
	#include "Systeme.hpp"
	Systeme recupSysteme();
}

%token ESPECE DIAMETRE TAILLE VITESSE INIT COMMENT
%token CO CF PO PF VIRG PV EGAL PLUS FLE UM MM
%token IDENT ENTIER FLOTTANT

%union { char *string; float real; int integer; }

%type<string> IDENT sr
%type<integer> ENTIER initunit
%type<real> FLOTTANT prob

%start axiome

%%

axiome: espece diametre listechoses {}
	;

espece: ESPECE listesp {}
	| COMMENT espece
	;

listesp: IDENT VIRG listesp { 
		// std::cout << "YACC:AEM1 : " << $1 << std::endl;

		// Lecture des espèces moléculaires.
		if(systeme.addEspeceMoleculaire($1)) {
			yyerror("Erreur AEM1");
			return 1;
		}
	} 
	| IDENT PV { 
		// std::cout << std::endl << "YACC:AEM2 : " << $1 << std::endl;
		if(systeme.addEspeceMoleculaire($1)) {
			yyerror("Erreur AEM2");
			return 1;
		}
	}
	;

diametre: DIAMETRE EGAL ENTIER PV {
		// std::cout << std::endl << "YACC:SDV : " << $3;

		// Lecture du diamètre de la vésicule.
		if(systeme.setDiametreVesicule($3)) {
			yyerror("Erreur SDV");
			return 1;
		}
	}
	| COMMENT diametre
	;

listechoses: chose
	| chose listechoses
	;

chose: taille
	| vitesse
	| init
	| reaction
	| COMMENT
	;

taille: TAILLE PO IDENT PF EGAL ENTIER PV { 
		// std::cout << std::endl << "YACC:STEM : " << $3 << " : " << $6 << std::endl;

		// On lie la taille au nom de l'espèce moléculaire.
		if(systeme.setTailleEspeceMoleculaire($3, $6)) {
			yyerror("Erreur STEM");
			return 1;
		}
	}
	;

vitesse: VITESSE PO IDENT PF EGAL FLOTTANT PV { 
		// std::cout << std::endl << "YACC:SVEM : " << $3 << " : " << $6 << std::endl;
		// On lie la vitesse au nom de l'espèce moléculaire.
		if(systeme.setVitesseEspeceMoleculaire($3, $6)) {
			yyerror("Erreur SVEM");
			return 1;
		}
	}
	;

init: INIT PO IDENT PF EGAL ENTIER initunit PV {
		// std::cout << std::endl << "YACC:SIEM1 : " << $3 << " : " << $6 << std::endl;
		// On lie le nombre d'individu au nom de l'espèce moléculaire.
		if(systeme.setInitEspeceMoleculaire($3, (float)$6, $7)) {
			yyerror("Erreur SIEM1");
			return 1;
		}
	}
	| INIT PO IDENT PF EGAL FLOTTANT initunit PV {
		// std::cout << std::endl << "YACC:SIEM2 : " << $3 << " : " << $6 << std::endl;
		if(systeme.setInitEspeceMoleculaire($3, $6, $7)) {
			yyerror("Erreur SIEM2");
			return 1;
		}
	}
	;

initunit: UM { $$ = 0; }
	| MM { $$ = 1; }
	| { $$ = 2; }
	;

reaction: IDENT sr FLE IDENT sr prob PV { 
		// std::cout << std::endl << "YACC:AR : " << $1 << " : " << $2 << " : " << $4 << " : " << $5 << " : " << $6 << std::endl;
		// On ajoute la réaction.
		if(systeme.addReaction($1, $2, $4, $5, $6)) {
			yyerror("Erreur AR");
			return 1;
		}
	}
	;

sr: PLUS IDENT { $$ = $2; }
	| { $$ = (char*)""; }
	;

prob: CO FLOTTANT CF { $$ = $2; }
	;

%%
void yyerror(const char * erreur)
{
	// On écrit le code de l'erreur.
	std::cout << "Erreur YACC : " << erreur << std::endl;
}

// Fonction permettant de récupérer le système généré.
Systeme recupSysteme()
{
	return systeme;
}