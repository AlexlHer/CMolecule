MAKEDIR = build

LEX_INPUT = analyse.l
YACC_INPUT = analyse.y
OTHER_INPUT = Systeme.cpp Systeme.hpp\
	Gestion.hpp\
	Molecule.hpp\
	Reaction.hpp\
	EspeceMoleculaire.cpp EspeceMoleculaire.hpp\
	Case.cpp Case.hpp\
	GestionEntiteCentre.cpp GestionEntiteCentre.hpp\
	GestionPopulationCentre.cpp GestionPopulationCentre.hpp\
	help.hpp\
	main.cpp

LEX_OUTPUT = $(MAKEDIR)/lex.yy.cpp
YACC_OUTPUT = $(MAKEDIR)/y.tab.cpp
OTHER_OUTPUT = $(MAKEDIR)/Systeme.cpp $(MAKEDIR)/Systeme.hpp\
	$(MAKEDIR)/Gestion.hpp\
	$(MAKEDIR)/Molecule.hpp\
	$(MAKEDIR)/Reaction.hpp\
	$(MAKEDIR)/EspeceMoleculaire.cpp $(MAKEDIR)/EspeceMoleculaire.hpp\
	$(MAKEDIR)/Case.cpp $(MAKEDIR)/Case.hpp\
	$(MAKEDIR)/GestionEntiteCentre.cpp $(MAKEDIR)/GestionEntiteCentre.hpp\
	$(MAKEDIR)/GestionPopulationCentre.cpp $(MAKEDIR)/GestionPopulationCentre.hpp\
	$(MAKEDIR)/help.hpp\
	$(MAKEDIR)/main.cpp

GCC_OUTPUT = projet_ter

$(GCC_OUTPUT) : $(LEX_OUTPUT) $(YACC_OUTPUT) $(OTHER_OUTPUT)
	g++ $(MAKEDIR)/* -ll -ly -fopenmp -O2 -o $(GCC_OUTPUT)
	#g++ $(MAKEDIR)/* -ll -ly -fopenmp -ggdb -o $(GCC_OUTPUT)

$(LEX_OUTPUT) : $(LEX_INPUT)
	mkdir -p $(MAKEDIR)
	lex -o $(LEX_OUTPUT) $(LEX_INPUT)

$(YACC_OUTPUT) : $(YACC_INPUT)
	mkdir -p $(MAKEDIR)
	bison -d $(YACC_INPUT) -o $(YACC_OUTPUT)

$(OTHER_OUTPUT) : $(OTHER_INPUT)
	mkdir -p $(MAKEDIR)
	cp $(OTHER_INPUT) $(MAKEDIR)