#! /usr/bin/env Rscript

# mapper.R - Wordcount program in R
# script for Mapper (R-Hadoop integration)

trimWhiteSpace <- function(line) gsub("(^ +)|( +$)", "", line)
splitIntoCols <- function(line) unlist(strsplit(line, ","))

## **** could wo with a single readLines or in blocks
con <- file("stdin", open = "r")
while (length(row <- readLines(con, n = 1, warn = FALSE)) > 0) {
  row <- trimWhiteSpace(row)
  cols <- splitIntoCols(row)
  ## **** can be done as cat(paste(words, "\t1\n", sep=""), sep="")
  if(length(unlist(strsplit(as.character(cols[2]), ";"))) > 20){
      cat(cols[3],cols[4], "\n" , sep="\t")
  }
}
close(con)
