#!/bin/bash

IP_COALESCE="./ip-coalesce"

FIELD_DELIM=":"
IP_RANGE_DELIM=","

while read LINE; do
   FIELD_1=$(echo -n "${LINE}" | cut -f 1 -d ${FIELD_DELIM})
   FIELD_2=$(echo "${LINE}" | cut -f 2 -d ${FIELD_DELIM})

   FIELD_2_COAL=$(echo "${FIELD_2}" | tr ',' ' ' | ${IP_COALESCE} | tr ' ' ',')

   echo "${FIELD_1}${FIELD_DELIM}${FIELD_2_COAL}"
done
