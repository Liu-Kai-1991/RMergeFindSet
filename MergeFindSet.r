library(Rcpp)
setwd("~/Documents/secor/RMergeFindSet/") ## Need to be changed
CppFile = "MergeFindSet.cpp"
Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
Rcpp::sourceCpp(CppFile)

# MFSet is a R class for disjoint-set interface
MFSet = setRefClass("MFSet",
                           fields = list(Set = "Rcpp_MergeFindSet"),
                           methods = list(
                               initialize = function() {Set <<- new(MergeFindSet)},
                               insert = function(x) {Set$insert(as.character(x))},
                               combine = function(x,y) {Set$combine(as.character(x), as.character(y))},
                               getGroup = function() {return(Set$getGroup())}
                           ))

# newMFSet creates a new merge find set instance
newMFSet = function() MFSet$new()

# Example

set = newMFSet() # Initially, there is no group 
set$combine(1,2) # 1, 2 is in a group --> (1,2)
set$combine(1,3) # 1, 3 is in a group --> (1,2,3)
set$combine(4,5) # 4, 5 is in a group --> (1,2,3) (4,5)
set$combine(5,6) # 5, 6 is in a group --> (1,2,3) (4,5,6)
set$combine(10,1) # 10, 1 is in a group --> (1,2,3,10) (4,5,6)
set$insert(11) #11 is a single item --> (1,2,3,10) (4,5,6) (11)
print(set$getGroup())

# Result:
#
# Individual GroupID
# 1         11       1      // (11)
# 2          6       2      // (4,5,6)
# 3          5       2      // (4,5,6)
# 4         10       3      // (1,2,3,10)
# 5          4       2      // (4,5,6)
# 6          3       3      // (1,2,3,10)
# 7          2       3      // (1,2,3,10)
# 8          1       3      // (1,2,3,10)
