all: manual.pdf

manual.pdf: manual.tex autonomt.tex fjarrstyrt.tex LIPS.tex oversikt.tex
	pdflatex manual.tex && \
	pdflatex manual.tex && \
	pdflatex manual.tex

clean:
	rm -f manual.toc manual.log manual.aux manual.pdf
