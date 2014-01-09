#
# makefile para a compilação do documento
#
pdf:
	latexmk -cd -bibtex -r /home/paulo/Arquivos/tcc/mono/.latexmkrc -silent -f -pdf

.PHONY: clean redo
clean:
	latexmk -c

cleanall: clean
	rm -f tcc.pdf

redo: clean pdf
