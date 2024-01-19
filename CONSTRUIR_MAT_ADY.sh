#!/bin/bash
#script para invocar la experimentacion con matady

# Directorios y archivos
DATA_SET_NAME="$HOME/DATASET2"
sizes=("262144")
datasets=("DH_1M" "DH_10M")
#datasets=("DG_1M")
# Función para ejecutar el comando Bitvector_run
run_bitvector() {
  local dataset="$1"
  local size="$2"
  ./Bitvector_run 2 0 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" 0
}

# Loop sobre los archivos y tamaños
for dataset in "${datasets[@]}"; do
  for size in "${sizes[@]}"; do
    run_bitvector "$dataset" "$size"
  done
done


