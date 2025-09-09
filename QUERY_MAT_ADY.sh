#!/bin/bash
#script para invocar la experimentacion con matady
RESULT_FILE_QUERY=$HOME/RESULTADOS/resultQuery.txt
DATA_SET_NAME=$HOME/DATASET2
WARMUP_FILE=$HOME/DATASET2/warmup_	
sizes=("262144")
#datasets=("DH_1M" "DH_10M" "DH_100M" "DG_1M" "DG_10M" "DG_100M")
datasets=("DH_1M" "DH_10M")
cantPages=("100")

# Función para ejecutar Bitvector_run con argumentos

run_bitvector_NOCACHE() {
  local size="$1"
  local dataset="$2"

#  ./Bitvector_run 2 1 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
 # ./Bitvector_run 2 2 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
 # ./Bitvector_run 2 3 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" 0 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
}

##############################################################################

run_bitvector_LFU() {
  local size="$1"
  local dataset="$2"
  local cantPages="$3"
 
  #./Bitvector_run 2 1 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  #./Bitvector_run 2 2 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  #./Bitvector_run 2 3 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
}


##############################################################################

run_bitvector_LRU() {
  local size="$1"
  local dataset="$2"
  local cantPages="$3"
  
  #./Bitvector_run 2 1 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  #./Bitvector_run 2 2 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  #./Bitvector_run 2 3 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/${dataset}_${size}_q.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 2 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" 0 $cantPages "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"

}

# Iterar a través de los tamaños y archivos
for size in "${sizes[@]}"; do
  for dataset in "${datasets[@]}"; do
    for pages in "${cantPages[@]}"; do
      run_bitvector_LRU "$size" "$dataset" "$pages" # LRU
      #run_bitvector_LFU "$size" "$dataset" "$pages" # LFU
    done
  done
done

# Iterar a través de los tamaños y archivos
#for size in "${sizes[@]}"; do	
 # for dataset in "${datasets[@]}"; do
  #  run_bitvector_NOCACHE "$size" "$dataset" # sin cache
  #done
#done
