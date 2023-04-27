#!/bin/bash

# ./build_champsim.sh tage no no no no lru 1
# ./build_champsim.sh hashed_perceptron no no no no lru 1
# ./build_champsim.sh tage_gshare_tour no no no no lru 1
# ./build_champsim.sh tage_hashed_tour no no no no lru 1
# ./build_champsim.sh tage_hashed_bimod_tour no no no no lru 1

./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 10 10 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 10 10 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 10 10 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 10 10 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz


./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage-no-no-no-no-lru-1core 30 30 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh hashed_perceptron-no-no-no-no-lru-1core 30 30 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage_gshare_tour-no-no-no-no-lru-1core 30 30 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz

./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-low-60K-1186B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-109B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-1246B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-137B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-1463B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-267B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 cadical-med-30K-831B.champsimtrace.xz
./run_champsim.sh tage_hashed_bimod_tour-no-no-no-no-lru-1core 30 30 kissat-mab-hywalk-low-30K-299B.champsimtrace.xz