# Part 2
Second file is encrypted by xoring bit-by-bit strings with secret phrase.
Secret phrase is a concatenation of current date in format yyyyMMdd (i.e. 20001231) and question phrase from Dark Knight marketing company (each word starts from capital letter, no spaces, question symbol at the end).
If the string is shorter than passphrase then string is resized to match passphare, empty symbols are filled with character '?' and then xored.
For user to know the real size of the content first 4 bytes represents size as unsigned int.

