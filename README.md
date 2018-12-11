# FileArchive.

This program is written in C language.

File compressing and archiving is a method of archiving files in a file and is used

extensively.

It is a file storage program that uses file archives after compressing specified files.

Compression and archive file functions are implemented.

----------------------------------------------------------------------------------------

# How to encoding)

1. Select a file and generate an encoding file using the Huffman coding algorithm.

 -(Command)./MyCompress -c [original file] -o [outputfile]

2. Compressed files are created

3. Save the encoded file to an archive file

 -(Command) ./MyArchive append [encoded file]

4. The compressed file is stored in the archive.

----------------------------------------------------------------------------------------

# How to decoding)

1. You can view a list of files stored in the archive.

2. Extract the compressed file from the archive file.

 -(Command) ./MyArchive extract [encoded file]

3. Unzip the compressed file and revert to the original file.

 -(Command)./MyCompress -x [encoded file] -o [original file]




