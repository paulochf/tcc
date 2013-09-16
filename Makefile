#
# makefile para a compilação do documento
#

BASE_NAME = tcc

PARAMS	  = -shell-escape
LATEX     = latex $(PARAMS)
PDFLATEX  = pdflatex $(PARAMS)
BIBTEX    = bibtex
DVIPS     = dvips
PS2PDF    = ps2pdf

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

clean:
	rm -f *.acn *.acr *.alg *.aux *.bbl *.blg *.dvi *.fdb_latexmk \
		  *.glg *.glo *.gls *.idx *.ilg *.ind *.ist *.lof *.log \
		  *.lot *.maf *.mtc *.mtc1 *.out *.ps *.synctex.gz *.toc *.pdf

redo: clean pdf
