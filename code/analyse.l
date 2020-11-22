%{
	// Auteur : Alexandre l'Heritier

	// Fichier LEX permettant de lire un modèle.
	#include <iostream>
	#include <fstream>
	#include "y.tab.hpp"

	extern std::ifstream *fichier;

	// On redéfini YY_INPUT du lex.yy.cpp pour pouvoir donner un autre fichier que celui du flux d'entrée.
	/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
	 * is returned in "result".
	 */
	#define YY_INPUT(buf, result, max_size)  \
		result = 0; \
		int c = fichier->get(); \
		while (!fichier->eof()) \
		{ \
			buf[result] = c; \
			result++; \
			if (result >= max_size || c == '\n') \
			{ \
				break; \
			} \
			c = fichier->get(); \
		}
%}
%option noyywrap

ident [A-Za-z_][A-Za-z_0-9]*
entier [0-9][0-9]*
flottant [0-9][0-9]*\.[0-9][0-9]*
comment \/\/(.*)

%%

[\n 	] {}

"espece" {
	// std::cout << " ESPECE";
	return ESPECE;
}

"diametre" {
	// std::cout << " DIAMETRE";
	return DIAMETRE;
}

"taille" {
	// std::cout << " TAILLE";
	return TAILLE;
}

"vitesse" {
	// std::cout << " VITESSE";
	return VITESSE;
}

"init" {
	// std::cout << " INIT";
	return INIT;
}

"[" {
	// std::cout << " CO";
	return CO;
}

"]" {
	// std::cout << " CF";
	return CF;
}

"(" {
	// std::cout << " PO";
	return PO;
}

")" {
	// std::cout << " PF";
	return PF;
}

";" {
	// std::cout << " PV";
	return PV;
}

"," {
	// std::cout << " VIRG";
	return VIRG;
}

"=" {
	// std::cout << " EGAL";
	return EGAL;
}

"+" {
	// std::cout << " PLUS";
	return PLUS;
}

"->" {
	// std::cout << " FLE";
	return FLE;
}

"uM" {
	// std::cout << " UM";
	return UM;
}

"mM" {
	// std::cout << " MM";
	return MM;
}

{ident} {
	// std::cout << " IDENT:" << yytext;
	yylval.string = strdup(yytext);
	return IDENT;
}

{entier} {
	// std::cout << " ENTIER:" << yytext;
	yylval.integer = atoi(yytext);
	return ENTIER;
}

{flottant} {
	// std::cout << " FLOTTANT:" << yytext;
	yylval.real = atof(yytext);
	return FLOTTANT;
}

{comment} {
	// std::cout << " COMMENT:" << yytext << std::endl;
	return COMMENT;
}

%%
