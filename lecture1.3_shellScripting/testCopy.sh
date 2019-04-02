#/bin/bash
cp thesis.docx thesis_current.docx
if test $? -ne 0
then 
	echo "copy failed" 1>&2
	exit 1
fi
rm -f thesis.docx
