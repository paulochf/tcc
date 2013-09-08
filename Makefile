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
	rm -f $(BASE_NAME)*.ps $(BASE_NAME)*.dvi *.log \
	      *.aux *.blg *.toc *.brf *.ilg *.ind *.idx\
	      missfont.log $(BASE_NAME)*.bbl $(BASE_NAME)*.pdf $(BASE_NAME)*.out \
		  $(BASE_NAME)*.lof $(BASE_NAME)*.lot 

redo: clean pdf