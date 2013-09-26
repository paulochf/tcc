add_cus_dep('glo', 'gls', 0, 'run_makeglossaries');
add_cus_dep('acn', 'acr', 0, 'run_makeglossaries');

sub run_makeglossaries {
  if ( $silent ) {
    system "makeglossaries -q '$_[0]'";
  }
  else {
    system "makeglossaries '$_[0]'";
  };
}

# Use xelatex instead of pdflatex
$pdflatex = 'pdflatex -synctex=1 -interaction=nonstopmode --shell-escape %O %S';

# Always create PDFs
$pdf_mode = 1;

# Use Skim.app to preview generated PDFs
#$pdf_previewer = 'open -a Skim.app %S';

# Use continous mode by default
#$preview_continuous_mode = 1;

# output to the directory called output in the current directory
#$out_dir = './output';

# file extensions to remove when cleaning
$clean_ext = 'acn acr alg aux bbl bcf blg brf fdb_latexmk glg glo gls idx ilg ind ist lof log lot out run.xml toc dvi xdy pyg';
