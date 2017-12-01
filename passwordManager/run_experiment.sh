#!/bin/bash

mkdir -p data/
OUTPUT_FILE_ENCLAVE="data/password_manager_throughput_$(date +"%Y%m%d%H%M").csv"
OUTPUT_FILE_REF="data/password_manager_ref_$(date +"%Y%m%d%H%M").csv"

sudo ./password_manager_without_enclave | tee $OUTPUT_FILE_REF

sudo ./password_manager | tee $OUTPUT_FILE_ENCLAVE
