NAME=proj3

$(NAME).pdf: $(NAME).ps
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps
    
$(NAME).ps: $(NAME).dvi
	dvips $(NAME).dvi
  
$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex
  
    
clean:
	-rm $(NAME).dvi $(NAME).aux $(NAME).toc $(NAME).log $(NAME).out $(NAME).ps