#!/usr/bin/env Rscript

env <- new.env(hash = TRUE)

con <- file("stdin", open = "r")

first_line <- TRUE

prev_line <- ""

while(length(line <- readLines(con, n = 1, warn = FALSE)) > 0){
   if(first_line){
      first_line = FALSE
      prev_line = line
      cat(line, "")
      
   }
   else if(prev_line != line){
         cat(line, "")
   }
}
close(con)
