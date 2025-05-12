# !/bin/bash

find . -type f -name '*.aux' -delete
find . -type f -name '*.log' -delete
find . -type f -name '*.toc' -delete
find . -type f -name '*.out' -delete
find . -type f -name '*.nav' -delete
find . -type f -name '*.vrb' -delete
find . -type f -name '*.snm' -delete
find . -type f -name '*.xml' -delete
find . -type f -name '*.bbl' -delete
find . -type f -name '*.blg' -delete

rm report-blx.bib