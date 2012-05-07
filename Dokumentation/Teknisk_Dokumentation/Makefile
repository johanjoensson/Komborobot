all: tekdoc.pdf

tekdoc.pdf: tekdoc.tex produkten.tex inledning.tex \
		teori.tex systemet.tex komm.tex pcmjuk.tex \
		sensor.tex styr.tex slutsats.tex referenser.tex appendix.tex
	pdflatex tekdoc.tex && \
	pdflatex tekdoc.tex && \
	pdflatex tekdoc.tex

clean:
	rm -f tekdoc.toc tekdoc.log tekdoc.aux tekdoc.pdf
