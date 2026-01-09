#!/bin/bash
source /esp/esp-idf/export.sh
idf.py set-target esp32-c6
idf.py fullclean
bear -- idf.py build
/bin/bash