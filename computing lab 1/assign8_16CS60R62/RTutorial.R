####################################################
# DataScience Tutorial
####################################################

# Introduction to R
# This is a rapid introduction for Data Scientists

# What is R?
# From Wikipedia:
# R is an open source programming language and software environment for statistical
# computing and graphics. The R language is widely used among statisticians and data
# miners for developing statistical software and data analysis.
# (http://en.wikipedia.org/wiki/R_%28programming_language%29)

# Basic R resources:
# http://www.r-project.org/
# http://cran.r-project.org/doc/contrib/Verzani-SimpleR.pdf

# Download R from:
# http://cran.r-project.org/bin/

# Rstudio
# Rstudio integrates components into an IDE:
#    File Editor
#     Console (command line interpreter)
#	   Plots
#    Help
#    Packages
#
# Download R studio from:  
#  http://www.rstudio.com/ide/download/desktop

# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")


# Use the R console like a calculator
3 + 5
exp(-1^2/2)/sqrt(2*pi)

# assignment
x <- 2
y <- exp(-x^2/2)/sqrt(2*pi)

# Get values of variables
x
y

# It is worthwhile to point out how R is used:
# R is an interpreted language and useful as a scripting language
# R is simple and convenient
# R is a powerful number cruncher and data display facility
# R is command line-oriented
# R is generally not used for large complex programs

# Hello World
# Most language introductions start off by demonstrating a Hello World program
# The simplest Hello World is the string "Hello World" typed into the console:
"Hello World" # The console will respond with "Hello World" if you run this string
hw <- "Hi World"
hw

# User-defined functions
# Hello world as a user-defined function
# Use the function keyword to assign code to a function
hw.f1 <- function()
{
  hw <- "Hello World"
  hw
}

hw.f1()

# A fancier user-defined function for Hello World
hw.f2 <- function(message = "Life \nIs \nGood")
{
  x <- 1
  y <- 1
  for (iter in 1:10)
  {
    plot(1,col='white',frame.plot=F,axes=F,xlab="",ylab="")
    x <- x + runif(1)*0.25 - 0.125
    y <- y + runif(1)*0.25 - 0.125
    text(x,y,labels=message, col=runif(1)*8 + 1, cex=runif(1)*6 + 1, font = runif(1)*4 + 1)
    Sys.sleep(.3)
  }
  plot(1,col='white',frame.plot=F,axes=F,xlab="",ylab="")
  text(1,1,labels=message, col=runif(1)*8 + 1, cex=runif(1)*6 + 1, font = runif(1)*4 + 1)
}

# Try out hw.f2
hw.f2()
hw.f2("Bon Voyage")

# How is code persisted?
# Functions are persisted in R files like this one (DataScience02b.R).
# These files have an R suffix (*.R)

# The most common data structure in R:  the vector
# here x is a scalar:
x <- -1
y <- exp(-x^2/2)/sqrt(2*pi)
x
y

# Ask R:  What is x?
is(x)
length(x)

# Some math with vectors
x <- -1:2
y <- 1:4
x
y
2*x^3
x+y
x-y
x*y
x/y
x^y
outer(x, y, "+") # cross-addition

# here x is a vector:
x <- c(-2, -1.5, -1.0, -0.5, 0, 0.5, 1.0, 1.5, 2)
# y is a vector because x is a vector
y <- exp(-x^2/2)/sqrt(2*pi)
plot(x, y); lines(x, y)

# Use seq to create a sequence in a vector.
# Note how function arguments (from, to, by) can be specified by their name.
x <- seq(from = -2.5, to = 2.5, by = 0.1)
y <- exp(-x^2/2)/sqrt(2*pi)
plot(x, y); lines(x, y) # add a line to the plot

# A user-defined function that produces a gaussian distribution
norm <- function(zRange=2.5, npoints = 50)
{
  x <- seq(from = -zRange, to = zRange, by = 2*zRange/npoints)
  y <- exp(-x^2/2)/sqrt(2*pi)
  return (data.frame(x, y))
}

plot(norm(),bg="yellow", cex=5, pch=21); lines(norm(), col="blue", lwd=2)

# A data frame is a common data structure in R
# A data frame is list of equal sized vectors
# The vectors in a data frame can be of different types

# Digression to matrix algebra
###################################################################
vect = c(1, 0, 1, 0, 1, 0, 1, 0, 1)
m <- matrix(vect, nrow=sqrt(length(vect)))
eigen(m)

###################################################################
# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")

# Sharing Data

# assign a url to variable "url"
url <- "http://archive.ics.uci.edu/ml/machine-learning-databases/blood-transfusion/transfusion.data"
# Download a rectangular dataset
DATAFRAME.obj<-read.csv(url, header=TRUE)

# present an overview of these data

# How many rows and columns are in the dataframe?
nrow(DATAFRAME.obj)
ncol(DATAFRAME.obj)

# See all the data
DATAFRAME.obj

# view the first few rows of the data
head(DATAFRAME.obj)

# view the column names
names(DATAFRAME.obj)

# What do these names mean?

# What are the standard deviation, mean, and median of each vector?

is(DATAFRAME.obj)

# determine the standard deviation, mean, and median for the 1st vector
sd(DATAFRAME.obj[               , 1])
mean(DATAFRAME.obj[, 1])
median(DATAFRAME.obj[, 1])

is(DATAFRAME.obj)
sapply(DATAFRAME.obj, is)

#apply on each vector of dataframe
sapply(DATAFRAME.obj, length)

summary(DATAFRAME.obj)

# determine the standard deviation, mean, and median for each vector
sapply(DATAFRAME.obj, sd)
sapply(DATAFRAME.obj, mean)
sapply(DATAFRAME.obj, median)

# Get a profile of each column
hist(DATAFRAME.obj$Recency, col=rgb(0,1,0,.5)) # hist(DATAFRAME.obj[, 1])
hist(DATAFRAME.obj$Frequency, col=rgb(0,1,0,.5)) # hist(DATAFRAME.obj[, 2])
hist(DATAFRAME.obj$Monetary, col=rgb(0,1,0,.5)) # hist(DATAFRAME.obj[, 3])
hist(DATAFRAME.obj$Time, col=rgb(0,1,0,.5)) # hist(DATAFRAME.obj[, 4])
hist(DATAFRAME.obj$whether, col=rgb(0,1,0,.5)) # hist(DATAFRAME.obj[, 5])

# Correlate columns
plot(DATAFRAME.obj)

############################################################################
# A glimpse into logistic regression using this data
# Predictive Analytics:

# Names are difficult.  They are too long.  
names(DATAFRAME.obj) <- c("Recency", "Frequency", "Monetary", "Time", "whether")
head(DATAFRAME.obj)
# Create a schema
formula <- whether ~ Recency + Frequency + Monetary + Time
# Create a model using the data and an algorithm (logistic regression)
model <- glm(formula=formula, data=DATAFRAME.obj, family="binomial")
# Predict probabilities based on model and data
predictedProbabilities <- predict(model, newdata=DATAFRAME.obj[-5], type="response")

# Compare predictions to actual outcomes
threshold <- 0.5
predictedValues <- as.numeric(predictedProbabilities > threshold)
"Confusion Matrix "
table(predictedValues, DATAFRAME.obj$whether)

############################################################################

# Functional Programmining
# In R, a function can be used as an input to another function

# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")

# functional language
vec_line <- seq(-7, 7)
vec_line
plot(c(-6, 15), c(-8, 8), col="white", bg="white")
points(vec_line, vec_line, col="blue", pch=21, bg="yellow")

# anonymous function:  function(x){exp(-x*x/10)}
vec_gaus <- sapply(vec_line, function(x){exp(-x*x/10)})
vec_gaus <- vec_gaus*(max(vec_line) - min(vec_line)) + min(vec_line)
lines(vec_line, vec_gaus, col="red")

# create a function that requires a function
myFunction <- function(x, y, inputFunction)
{
  inputFunction(x) - inputFunction(y)
}

anotherFunction <- function(x)
{
  10*exp(-x*x/(1 + (x %% 3)))
}

input1 <- seq(-7, 7)
input2 <- seq(-8, 6)
output <- myFunction(input1, input2, anotherFunction)

points(output, col='blue', lty=4, lwd=4)
lines(1:length(output), output, col="green")

############################################################################
# Clear Workspace
rm(list=ls())
# Clear Console:
cat("\014")

# Remove Outlier:
x <- c(1, 2, 2, 3, 3, 3, 4, 4, 5, 99)
highLimit <- mean(x) + 2*sd(x)
lowLimit <- mean(x) - 2*sd(x)
goodFlag <- (x < highLimit) & (x > lowLimit)
x[goodFlag]
x
x <- x[goodFlag]
x

# Replace Outlier: Think of these numbers as the age of ten kindergarteners
x <- c(1, 2, 2, 3, 3, 3, 4, 4, 5, 99)
badFlag <- (x > highLimit) | (x <  lowLimit) # !goodFlag
x[badFlag] <- mean(x)
x

