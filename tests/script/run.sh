#!/bin/sh

export LD_PRELOAD="../../libft_malloc.so"
export LD_LIBRARY_PATH="../.."
export LOG_FILE=".log"
$@
