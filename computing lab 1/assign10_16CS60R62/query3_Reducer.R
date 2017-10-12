#!/usr/bin/env Rscript

env <- new.env(hash = TRUE)
con <- file("stdin", open = "r")
first_line <- TRUE
prev_line <- ""
title <- c()
id <- c()

while(length(line <- readLines(con, n = 1, warn = FALSE)) > 0){
   line <- unlist(strsplit(line, split = "\t"))
   if(first_line){
      prev_line <- line[1]
      title <- c(title, line[2])
      id <- c(id, line[3])
      sum <- 1
      first_line <- FALSE
      
   }
   else if(prev_line == line[1]){
         title <- c(title, line[2])
         id <- c(id, line[3])
         sum <- sum + 1
   }
   else{
         if(sum > 20){
               for(i in 1:sum){
                  cat(prev_line, title[i], id[i], "\n", sep = "\t")
               }
         }
         title <- c()
         id <- c()
         prev_line <- line[1]
         title <- c(title, line[2])
         id <- c(id, line[3])
         sum <- 1
   }
}
close(con)
