setwd("~/A New Perspective on Director Busyness/")
library(Rcpp)

CppFile = "MergeFindSet.cpp"
Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
Rcpp::sourceCpp(CppFile)

MFSet = setRefClass("MFSet",
                           fields = list(Set = "Rcpp_MergeFindSet"),
                           methods = list(
                               initialize = function() {Set <<- new(MergeFindSet)},
                               insert = function(x) {Set$insert(as.character(x))},
                               combine = function(x,y) {Set$combine(as.character(x), as.character(y))},
                               getGroup = function() {return(Set$getGroup())}
                           ))

newMFSet = function() MFSet$new()

b = newMFSet()                        
b$insert(3)
b$insert(4)
b$insert(5)
b$getGroup()
b$combine(4,5)
b$getGroup()
