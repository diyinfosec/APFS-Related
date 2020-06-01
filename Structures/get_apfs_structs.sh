#!/bin/bash

#- Author: Ramprasad Ramshankar
#- Alias: diyinfosec
#- Date: 01-Jun-2020
#- Purpose: Reads the text version of Apple's APFS documentation and extracts structures, enums and #defines. 
#- Usecase: Have APFS structs in one place, can be used to build tools that parse/mount an APFS volume. 
#- Language:  bash script

#- APFS Structure documentation: https://developer.apple.com/support/downloads/Apple-File-System-Reference.pdf
#- To convert this to txt: 
#-  sudo apt install poppler-utils
#-  pdftotext Apple-File-System-Reference.pdf (this will create a corresponding .txt file in the working directory)

#!/bin/bash

#- Function to get the enum and struct types in the APFS documentation
function get_apfs_structs()
{
        awk '
        BEGIN { out_str=""; struct_begin="N"; counter=0; lines_to_ignore=0; }
        {
                line=$0

                #- Ignore blank lines
                if(line ~ /^$/)
                {
                        next
                }
                if(lines_to_ignore>0)
                {
                        lines_to_ignore=lines_to_ignore-1;
                        next
                }
                #- Identifying a struct:  Lines that begin with "struct" or "typedef"  and have a {
                if((line ~ /^struct/ || line ~ /^typedef/) && (line ~ /\{/))
                {
                        out_str=line;
                        struct_begin="Y"
                        #printf("Found opening struct \n",out_str);
                        #print(out_str)
                        next
                }
                #- Identifying sub-structure: Any line not having struct keywords (struct/enum) but still have a {
                else if(index(line,"{") !=0)
                {
                        out_str=out_str " " line;
                        counter=counter+1;
                        next
                }
                #- Identifying end of structure
                else if(index(line,"}") !=0)
                {
                        #- If counter is not zero, then it means it is end of a sub-structure.
                        if(counter>0)
                        {
                                out_str=out_str " " line
                                counter=counter-1
                        }
                        else if(counter==0)
                        {
                                out_str=out_str " " line
                                print(out_str)
                                out_str=""
                        }
                        next
                }
                #- Just keep appending to the struct
                else
                {
                        #- Ignore lines that dont have an underscore or an equals
                        if((index(line,"All Rights Reserved") >0))
                        {
                                lines_to_ignore=2;
                                next
                        }
                        else
                        {
                                out_str=out_str " " line
                                #print("out str is ", out_str)
                        }
                }
        } ' $filename
}

#- Function to get the #define from the APFS documentation
function get_apfs_defs()
{
        awk '
        BEGIN { out_str=""; }
        {

        #- Assigning $0 to line
        line=$0

        #- Getting the number of space separated tokens in the line
        num_tokens=split($0,tokens," ");

        #- Getting the last character in the line
        last_char=substr(line,length(line),1)


        #- The input line contains the string "#define"
        if(index(line,"#define")!=0)
        {
                out_str=line

                if(last_char == "\\")
                {
                        include_next_line="Y"
                }

                if(num_tokens ==2)
                {
                        include_next_line="Y"
                }
        }
        #- The input line does not contain the string "#define"
        else
        {
                if(include_next_line == "Y")
                {
                        out_str=out_str " " line
                        if(last_char == "\\")
                        {
                                include_next_line="Y"
                        }
                        else
                        {
                                include_next_line="N"
                                print(out_str)
                        }
                }

        }
        }' $filename
}

#- Testing. 
filename=Apple-File-System-Reference.txt
#get_apfs_defs
get_apfs_structs
