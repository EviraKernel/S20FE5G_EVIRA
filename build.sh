#!/bin/bash

export KBUILD_BUILD_USER="Resurrect88"

export KBUILD_BUILD_HOST="EviraRules"

export TOOLCHAIN=clang

export DEVICES=kona

source helper

gen_toolchain

send_msg "⏳ Start build ${DEVICES}..."

START=$(date +"%s")

for i in ${DEVICES//,/ }
do 

	build ${i} -kernel


done

END=$(date +"%s")

DIFF=$(( END - START ))
