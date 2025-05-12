LATEXCMD=pdflatex -shell-escape

all: main

main: |
	$(LATEXCMD) report.tex
	bibtex report
	$(LATEXCMD) report.tex
	$(LATEXCMD) report.tex
clean:
	./cleaner.sh