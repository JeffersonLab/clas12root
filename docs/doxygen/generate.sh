#!/usr/bin/env bash
set -euo pipefail
cd $(dirname ${BASH_SOURCE[0]:-$0})/../..
prefix=share/doc
mkdir -p $prefix
doxygen docs/doxygen/Doxyfile
echo """
Generated documentation webpage:
  file://$(pwd)/$(find $prefix -type f -name index.html)
"""
