* Get the APFS structure PDF: https://developer.apple.com/support/downloads/Apple-File-System-Reference.pdf
* Convert the PDF to text.  
```
sudo apt install poppler-utils

pdftotext -y 80 -H 650 -W 1000 -nopgbrk -eol unix apfs.pdf

Refer:
https://stackoverflow.com/questions/27901194/remove-a-page-number-header-and-footer-from-pdf-file
-y 80   -> crop 80 pixels after the top of file (remove header);
-H 650  -> crop 650 pixels after the -y has cropped (remove footer);
-W 1000 -> hight value to crop nothing (need especify something);
```
* Run the shell script and output the structs and the #defines. 
* Code Beautify: https://codebeautify.org/c-formatter-beautifier
