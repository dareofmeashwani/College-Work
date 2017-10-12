rm(list = ls())
cat("\014")
myoriginaldata<-read.csv(file.choose(),strip.white =TRUE)
mydata<-myoriginaldata
mydata<- mydata[-c(1,2,7,10:12,15,17,18,20:22)]
a_sortcolumn<-function(mat,n)
{
  mat[rank(-mat[,n]),]<-mat[c(1:nrow(mat)),]
  return(mat)
}
d_sortcolumn<-function(mat,n)
{
  mat[rank(mat[,n]),]<-mat[c(1:nrow(mat)),]
  return(mat)
}
readinteger <- function()
{ 
  n <- readline()
  return(as.integer(n))
}
sop <-function(x,y)
{
  numerator<-0
  sumx<-0
  sumy<-0
  for(i in 1:ncol(x))
  {
    numerator<-numerator+as.numeric(x[,i])*as.numeric(y[,i])
    sumx<-sumx+as.numeric(x[,i])*as.numeric(x[,i])
    sumy<-sumy+as.numeric(y[,i])*as.numeric(y[,i])
  }
  return(numerator/(sqrt(sumx)*sqrt(sumy)))
}
euclidian_distance<-function(x,y)
{
  dis=0
  for(i in 1:ncol(x))
  {
    dis<-dis+(as.numeric(x[,i])-as.numeric(y[,i]))*(as.numeric(x[,i])-as.numeric(y[,i]))
  }
  return(sqrt(dis))
}
jac<-function(d,a)
{
  indexd<-which(correct(myoriginaldata$director_name) == correct(d))
  indexa1<-which(correct(myoriginaldata$actor_1_name) == correct(a))
  indexa2<-which(correct(myoriginaldata$actor_2_name) == correct(a))
  indexa3<-which(correct(myoriginaldata$actor_3_name) == correct(a))
  indexa<-union(union(indexa1,indexa2),indexa3)
  directormovie<-correct(as.character(myoriginaldata[indexd,12]))
  actormovie<-correct(as.character(myoriginaldata[indexa,12]))
  uni<-union(directormovie,actormovie)
  inter<-intersect(directormovie,actormovie)
  return(length(inter)/length(uni))
}
correct<-function(x)
{
  return(gsub(" ", "",tolower(x), fixed = TRUE))
}
rmse <-function(actual, pred)
{
  return(sqrt(mean((actual - pred)^2)))
}
normalize <- function(x) {
  return ((x - min(x)) / (max(x) - min(x))) 
  }
movieprediction<-function()
{
  movielink<-readline(prompt = "Enter Your Favourite Movie Link: ")
  movielist<-myoriginaldata$movie_imdb_link
  index<-which(movielist == movielink)
  selectedmoviefeature<-mydata[index,]
  coslist<-NULL
  euclist<-NULL
  if(nrow(selectedmoviefeature)==1)
  {
    for(i in 1:nrow(mydata))
    {
      temp1<-sop(selectedmoviefeature,mydata[i,])
      temp2<-euclidian_distance(selectedmoviefeature,mydata[i,])
      if(i==1)
      {
        coslist<-cbind(temp1,i)
        euclist<-cbind(temp2,i)
      }
      else
      {
        coslist<-rbind(coslist,cbind(temp1,i))
        euclist<-rbind(euclist,cbind(temp2,i))
      }
    }
    coslist<-a_sortcolumn(coslist,1)
    euclist<-d_sortcolumn(euclist,1)
    print("Recommended Movie Based On Cosine Similarity")
    for (i in 1:5) 
      {
        print(as.character(myoriginaldata[as.numeric(coslist[i+1,2]),12]))
      }
    print("Recommended Movie Based On Euclidean Distance")
    for (i in 1:5) 
    {
      print(as.character(myoriginaldata[as.numeric(euclist[i+1,2]),12]))
    }
  }
  else print("Link Is Not Valid")
}

jaccard_similarity<-function()
{
  director_actor1 <- myoriginaldata[,c("director_name", "actor_1_name")]
  director_actor2 <- myoriginaldata[,c("director_name", "actor_2_name")]
  director_actor3 <- myoriginaldata[,c("director_name", "actor_3_name")]
  colnames(director_actor1)[2] <- "actor_name"
  colnames(director_actor2)[2] <- "actor_name"
  colnames(director_actor3)[2] <- "actor_name"
  director_actor_duo <- rbind(director_actor1, director_actor2, director_actor3)
  director_actor_duo<-unique(director_actor_duo)
  duolist<-NULL
  for (i in 1:nrow(director_actor_duo)) 
    {
    d<-as.character(director_actor_duo[i,1])
    a<-as.character(director_actor_duo[i,2])
    temp<-jac(d,a)
    c<-cbind(d,a)
    temp2<-cbind(temp,c)
    if(i==1)
    {
      duolist<-temp2
    }
    else
    {
      duolist<-rbind(duolist,temp2)
    }
  }
  duolist<-duolist[rev(order(duolist[,1])),]
  print("top 5 director actor duo")
  for (i in 1:5) 
  {
    print(i)
    print(as.character(duolist[i,2]))
    print(as.character(duolist[i,3]))
  }
}
linearregression<-function()
{
  normalizedata<-mydata
  partition<-as.numeric(floor(nrow(normalizedata)*.7))
  trainingdata<-normalizedata[1:partition,]
  testdata<-normalizedata[(partition+1):nrow(normalizedata),]
  originalscore<-testdata$imdb_score
  testdata$imdb_score<-NULL
  formula <- imdb_score ~ num_critic_for_reviews+duration+director_facebook_likes+actor_3_facebook_likes+actor_1_facebook_likes+gross+num_voted_users+cast_total_facebook_likes+facenumber_in_poster+num_user_for_reviews+budget+title_year+actor_2_facebook_likes+aspect_ratio+movie_facebook_likes
  model <- lm(formula=formula, data=trainingdata,na.action = na.omit)
  print(summary(model))
  predictresult<-predict(model, newdata=testdata, type="response")
  plot(originalscore,predictresult,col="black")
  print("ROOT MEAN SQUARE ERROR :")
  print(as.numeric(rmse(mydata$imdb_score[-which(! complete.cases(mydata))],c(fitted(model)))))
}
KNN<-function()
{
  tempdata<- myoriginaldata[-c(1,2,7,11,12,15,17,18,20:22)]
  tempdata<-na.omit(tempdata)
  tempdata$genres <-unlist( lapply(strsplit(as.character(tempdata$genres), "\\|"), "[", 1),recursive = TRUE, use.names = TRUE)
  genre<-tempdata[,7]
  normalizedata<-tempdata[,-7]
  normalizedata<- as.data.frame(lapply(normalizedata[], normalize))
  partition<-as.numeric(floor(nrow(normalizedata)*.7))
  trainingdata<-normalizedata[1:partition,]
  testdata<-normalizedata[(partition+1):nrow(normalizedata),]
  traininglabel<-genre[1:partition]
  testlabel<-genre[(partition+1):length(genre)]
  library(class)
  fit<- knn(train =trainingdata, test = testdata,cl = traininglabel, floor(sqrt(nrow(normalizedata))))
  library(gmodels)
  table<-CrossTable(x = testlabel, y = fit, prop.chisq=FALSE)
  table<-as.data.frame(table$t,stringsAsFactors = FALSE)
  dia<-subset(table,as.character(table[,1])==as.character(table[,2]))
  total<-sum(as.numeric(table[,3]))
  diasum<-sum(as.numeric(dia[,3]))
  print("Accuracy :")
  print(diasum/total)
}
while(TRUE) 
{
  print('   Enter THe choice:')
  print('       0 for Exit')
  print('       1 for Movie prediction')
  print('       2 for Jaccard Similarity')
  print('       3 for Linear Regression')
  print('       4 for KNN')
  choice<-readinteger()
  if (choice==0)
  {
    break
  }
  else if (choice==1)
  {
    movieprediction()
  }
  else if (choice==2)
  {
    jaccard_similarity()
  }
  else if (choice==3)
  {
    linearregression()
  }
  else if (choice==4)
  {
    KNN()
  }
  else
  {
    print("YOu Entered The Invalid Choice")
  }
}