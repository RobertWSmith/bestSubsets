#' @name bestSubsets-package
#' @title bestSubsets
#' 
#' 
#' @docType package
NULL

# .onLoad <- function(libname, pkgname) {
#   loadRcppModules()
# }

loadModule("Regression")

#' Is Regression
#' 
#' @param object to confirm if it inherits from the Regression class.
#' @method Regression is
#' @export
is.Regression <- function(obj) {
  return (inherits(obj, "Rcpp_Regression"))
}

#' Linear Regression Object initializer
#' 
#' @param output \code{numeric vector} of outputs to predict with regression
#' @param predictors \code{numeric matrix} of inputs to use for prediction
#' 
#' @method Regression initialize
#' @return \code{OLS} Regression object
#' @export
Regression <- function(output = NULL, predictors = NULL) {
  if (is.null(output)) {
    stopifnot(is.null(predictors))
    
    return (new(.__C__Rcpp_Regression))
  } else {
    stopifnot(is.numeric(output))
    stopifnot(is.matrix(predictors) && is.numeric(predictors))
    
    return(new(.__C__Rcpp_Regression, output_ = output, predictors_ = predictors))
  }
}

#' Ordinary Least Squares Regression
#' 
#' @param \code{Regression} object to confirm
#' @param \code{vector} the columns of the matrix to consider for regression
#' 
#' @return \code{list} of regression diagnostics
#' @rdname LeastSquares
#' @export
ols <- function(reg, preds = NULL) {
  stopifnot(is.Regression(reg))
  
  return (reg$OLS())
}



# # http://gozips.uakron.edu/~rle/CerealData.html
# cereal <- read.table("~/GitHub/bestSubsets/data/cereal.txt", header=T, quote="\"")
# cereal <- cereal[,sapply(cereal, is.numeric)]
# 
# cereal <- sapply(cereal, as.numeric)
# 
# outputs <- as.numeric(cereal[,1])
# inputs <- matrix((cereal[,2:ncol(cereal)]), nrow = 11, ncol = 4, byrow = FALSE)
# 
# 
# obj <- regression(outputs, inputs)
# 
# obj$output
# obj$predictors[]
# 
# obj$LM(1:ncol(inputs))
# 
# op <- cbind(rep(1, length(outputs)), inputs)
# 
# summary(lm(inputs~outputs))



