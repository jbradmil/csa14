FILE* ofile;

void PrintFileHeader() {
  fprintf(ofile, "\\documentclass[11pt, oneside]{article}\n");
  fprintf(ofile, "\\usepackage{geometry}\n");                	
  fprintf(ofile, "\\geometry{letterpaper}\n");           
  fprintf(ofile, "\\usepackage{graphicx}\n");
  fprintf(ofile, "\\usepackage{caption}\n");
  fprintf(ofile, "\\usepackage{subcaption}\n");		
  fprintf(ofile, "\\usepackage{amssymb}\n");
  fprintf(ofile, "\\usepackage{datetime}\n");
  fprintf(ofile, "\n");
  fprintf(ofile, "\\title{Lost-lepton extrapolation studies}\n");
  fprintf(ofile, " \\author{Jack Bradmiller-Feld}\n");
  fprintf(ofile, " \\date{\\today}\n");
  fprintf(ofile, "\n\n\n");
  fprintf(ofile, "\\begin{document}\n");
  fprintf(ofile, "\\maketitle\n");
}

void PrintFileTrailer() {
 
  fprintf(ofile, "\n\n\n");
  fprintf(ofile, "\\end{document}");
}
