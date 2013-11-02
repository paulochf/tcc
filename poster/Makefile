#
# makefile para a compilação do documento
#

BASE_NAME = poster-surf

PARAMS	   = -shell-escape
LATEX      = latex $(PARAMS)
PDFLATEX   = pdflatex $(PARAMS)
BIBTEX     = bibtex
DVIPS      = dvips
PS2PDF     = ps2pdf

pdf: $(BASE_NAME).pdf
ps: $(BASE_NAME).ps

$(BASE_NAME).pdf: $(BASE_NAME).tex
	$(PDFLATEX) $<
#	$(BIBTEX) $(BASE_NAME) #comentar se nao for usar referencias
	$(PDFLATEX) $<
	$(PDFLATEX) $<
	$(PDFLATEX) $<

$(BASE_NAME).ps: $(BASE_NAME).tex
	$(LATEX) $<
#	$(BIBTEX) $(BASE_NAME) #comentar se nao for usar referencias
	$(LATEX) $<
	$(LATEX) $<
	$(LATEX) $<
	$(DVIPS) $(BASE_NAME).dvi
	$(PS2PDF) $(BASE_NAME).ps

.PHONY: clean redo
clean:
	latexmk -c

redo: clean pdf
