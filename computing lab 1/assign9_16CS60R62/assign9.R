cat("\014")
rm(list=ls())
clustering<-function()
{
rm(list=ls())
#install.packages("ggplot2")
library(ggplot2)
myoriginaldata <- read.csv(file=file.choose(), header=TRUE, sep=";")
mydata1<-myoriginaldata
mydata1$Time<-NULL
mydata1$Date<-NULL
mydata1[,] <- sapply(mydata1[,], as.numeric)
mydata1<-na.omit(mydata1)
mydata2<-myoriginaldata[c(7:9)]
mydata2<-na.omit(mydata2)
mydata2[,] <- sapply(mydata2[,], as.numeric)
status<-NULL
klist<-NULL
for (i in 5:8) {
  t1<-system.time(fit1 <- kmeans(mydata1, i))
  t2<-system.time(fit2 <- kmeans(mydata2, i))
  t1<-as.numeric(t1[3])
  t2<-as.numeric(t2[3])
  acc1<-fit1$betweenss/fit1$totss
  acc2<-fit2$betweenss/fit2$totss
  temp1<-cbind(t1,acc1)
  temp2<-cbind(t2,acc2)
  if(i==5)
  {
    status<-cbind(temp1,temp2)
    klist<-c(i)
  }
  else
  {
      status<-rbind(status,cbind(temp1,temp2))
      klist<-c(klist,i)
  }
}
png("1.png")
plot(klist,status[,1],xlab ="K",ylab = "Running Time (with 3 to 9 parameters)")
dev.off()
png("2.png")
plot(klist,status[,2],xlab ="K",ylab = "Accuracy (with 3 to 9 parameters)")
dev.off()
png("3.png")
plot(klist,status[,3],xlab ="K",ylab = "Running Time (with 7 to 9 parameters)")
dev.off()
png("4.png")
plot(klist,status[,4],xlab ="K",ylab = "Accuracy (with 7 to 9 parameters)")
dev.off()
}

classification<-function()
{
#install.packages("e1071")
library("e1071")
rm(list=ls())
myoriginaldata <- read.csv(file=file.choose(), header=TRUE, sep=";")
mydata<-myoriginaldata
mydata$job=NULL
mydata$education=NULL
mydata$marital=NULL
mydata$default=NULL
mydata$housing=NULL
mydata$loan=NULL
mydata$contact=NULL
mydata$month=NULL
mydata$poutcome=NULL
##########################################
indexes <-sample(1:nrow(mydata), size=0.7*nrow(mydata))
trainingdata<-mydata[indexes,]
testdata<-mydata[-indexes,]
write.csv(trainingdata,file="training_linear.csv")
write.csv(testdata,file="test_linear.csv")
testwithouty<-testdata
y <- testwithouty$y
testwithouty$y<-NULL
svm_modal_linear <- svm(y ~ ., data = trainingdata, kernel = "linear",cost = 1, gamma = 0.05)
summary(svm_modal_linear)
t<-system.time(predicted_result_linear <- predict(svm_modal_linear,testwithouty))
confusiontable_linear<-data.frame(table(predicted_result_linear,y))
accuracy_linear<-(sum(confusiontable_linear[1,3],confusiontable_linear[4,3]))/sum(confusiontable_linear[,3])
print("SVM Linear Accuracy")
print(accuracy_linear)
###################################################
indexes <-sample(1:nrow(mydata), size=0.7*nrow(mydata))
trainingdata<-mydata[indexes,]
testdata<-mydata[-indexes,]
write.csv(trainingdata,file="training_radial.csv")
write.csv(testdata,file="test_radial.csv")
testwithouty<-testdata
y <- testwithouty$y
testwithouty$y<-NULL
svm_modal_radial <- svm(y ~ ., data = trainingdata, kernel = "radial",cost = 1, gamma = 0.05)
summary(svm_modal_radial)
t<-system.time(predicted_result_radial <- predict(svm_modal_radial,testwithouty))
confusiontable_radial<-data.frame(table(predicted_result_radial,y))
accuracy_radial<-(sum(confusiontable_radial[1,3],confusiontable_radial[4,3]))/sum(confusiontable_radial[,3])
print("SVM Radial Accuracy")
print(accuracy_radial)
}
readinteger <- function()
{ 
  n <- readline()
  return(as.integer(n))
}
while(TRUE) 
{
  print('   Enter THe choice:')
  print('       0 for Exit')
  print('       1 for classification')
  print('       2 for Clustering')
  choice<-readinteger()
  if (choice==0)
  {
    break
  }
  else if (choice==1)
  {
    classification()
  }
  else if (choice==2)
  {
    clustering()
  }
  else
  {
    print("YOu Entered The Invalid Choice")
  }
}