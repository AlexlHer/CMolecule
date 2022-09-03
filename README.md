# CMolecule Arcane Mini-App (CMAMA)
## Par Alexandre l'Heritier
Simulateur stochastique de réactions biochimiques.

TER proposé par Patrick Amar.

Rendu le 10/05/2020.

Projet perso de version Arcane débutée le 22/05/2022 en parallèle au développement de Quicksilver.

## Description

Le principe de ce programme est de simuler les déplacements des molécules et les réactions entre molécule dans une vésicule.

En entrée, on a un fichier qui donne les informations sur les molécules et les réactions possibles entre ces molécules.

En sortie, on a un .csv qui donne la population de chaque type de molécule tous les t temps et un graphique généré par GNUPlot avec le .csv.

```sh
ARCANE_INSTALL_PATH= # your install path.

CMAMA_BUILD_TYPE=Release # or Debug
CMAMA_SOURCE_DIR=CMolecule # your src path
CMAMA_BUILD_DIR=build_qama # build dir

CMAMA_EXE=${CMAMA_BUILD_DIR}/src/CMolecule
CMAMA_ARC=${CMAMA_SOURCE_DIR}/data/test.txt.arc
cd ${CMAMA_BUILD_DIR}

cmake \
  -S ${CMAMA_SOURCE_DIR} \
  -B ${CMAMA_BUILD_DIR} \
  -GNinja
  -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PATH} \
  -DCMAKE_BUILD_TYPE=${CMAMA_BUILD_TYPE}
  
cmake --build ${CMAMA_BUILD_DIR}

# Sequential:
${CMAMA_EXE} ${CMAMA_ARC}

# MPI 4 procs:
mpirun -n 4 ${CMAMA_EXE} ${CMAMA_ARC}

# 2 tasks:
${CMAMA_EXE} -A,T=2 ${CMAMA_ARC}

# MPI 4 procs and 2 tasks/proc:
mpirun -n 4 ${CMAMA_EXE} -A,T=2 ${CMAMA_ARC}
```

Arc input files examples:

```sh
CMAMA_ARC=${CMAMA_SOURCE_DIR}/data/ExampleFull.arc
```