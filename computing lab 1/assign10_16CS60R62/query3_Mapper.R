#! /usr/bin/env Rscript

trimWhiteSpace <- function(line) gsub("(^ +)|( +$)", "", line)
splitIntoCols <- function(line) unlist(strsplit(line, ","))

con <- file("stdin", open = "r")
while (length(row <- readLines(con, n = 1, warn = FALSE)) > 0) {
  row <- trimWhiteSpace(row)
  cols <- splitIntoCols(row)
  if(length(unlist(strsplit(cols[2], ";"))) > 20){
      artists <- strsplit(cols[1], ";")
      for(art in artists){
          cat(art, cols[3], cols[4], "\n", sep = "\t")
      }
  }
}
close(con)
