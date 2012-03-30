all: tekdoc.pdf

tekdoc.pdf: tekdoc.tex dok.tex helhet.tex inledning.tex \
		integration.tex komm.tex montering.tex pcmjuk.tex \
		protokoll.tex sensor.tex styr.tex systemskiss.tex test.tex
	pdflatex tekdoc.tex && \
	pdflatex tekdoc.tex && \
	pdflatex tekdoc.tex

clean:
	rm -f tekdoc.toc tekdoc.log tekdoc.aux tekdoc.pdf
