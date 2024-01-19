#!/bin/bash
# script para invocar la experimentación con matady
RESULT_FILE_QUERY=$HOME/RESULTADOS/resultQuery.txt
DATA_SET_NAME=$HOME/DATASET2
WARMUP_FILE=$HOME/DATASET2/warmup_
sizes=("262144")
datasets=("DH_1M" "DH_10M" "DH_100M" "DG_1M" "DG_10M" "DG_100M")
cantPages=("10" "50" "100" "500" "1000")
k_values=("2" "4" "8") # Nuevos valores de k

# Función para ejecutar Bitvector_run con argumentos
run_bitvector_NOCACHE() {
  local size="$1"
  local dataset="$2"
  local k_value="$3"  # Nuevo parámetro k
  ./Bitvector_run 1 1 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 2 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 3 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 0 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" "$k_value" 0 "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"

}

run_bitvector_LFU() {
  local size="$1"
  local dataset="$2"
  local cantPages="$3"
  local k_value="$4"  # Nuevo parámetro k
  ./Bitvector_run 1 1 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 2 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 3 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 1 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"

}

run_bitvector_LRU() {
  local size="$1"
  local dataset="$2"
  local cantPages="$3"
  local k_value="$4"  # Nuevo parámetro k
  ./Bitvector_run 1 1 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 2 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 3 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "$DATA_SET_NAME/${dataset}_${size}_q.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_100.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_1000.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"
  ./Bitvector_run 1 4 2 100 "$DATA_SET_NAME/${dataset}_${size}.txt" "${DATA_SET_NAME}/queryWindow_262144_10000.txt" "$k_value" "$cantPages" "$WARMUP_FILE${size}.txt" >> "$RESULT_FILE_QUERY"

}

# Iterar a través de los tamaños, archivos y páginas para LFU y LRU
for size in "${sizes[@]}"; do
  for dataset in "${datasets[@]}"; do
    for pages in "${cantPages[@]}"; do
      for k in "${k_values[@]}"; do
        run_bitvector_LFU "$size" "$dataset" "$pages" "$k" # LFU
        run_bitvector_LRU "$size" "$dataset" "$pages" "$k" # LRU
      done
    done
  done
done

# Ejecutar run_bitvector_NOCACHE por separado
for size in "${sizes[@]}"; do
  for dataset in "${datasets[@]}"; do
    for k in "${k_values[@]}"; do
      run_bitvector_NOCACHE "$size" "$dataset" "$k" # sin cache
    done
  done
done


