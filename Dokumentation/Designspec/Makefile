all: designspec.pdf

designspec.pdf: designspec.tex dok.tex helhet.tex inledning.tex \
		integration.tex komm.tex montering.tex pcmjuk.tex \
		protokoll.tex sensor.tex styr.tex systemskiss.tex test.tex
	pdflatex designspec.tex && \
	pdflatex designspec.tex && \
	pdflatex designspec.tex

clean:
	rm -f designspec.toc designspec.log designspec.aux designspec.pdf
