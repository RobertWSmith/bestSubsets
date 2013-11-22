#ifndef OLS_HPP
#define OLS_HPP

// header library for determining the next combination using itertors
#include "combination.hpp"
using boost::next_combination;

#include <vector>
#include <RcppCommon.h>

class Regression;

namespace Rcpp {
  template<> SEXP wrap(const Regression&);
  template<> Regression as(SEXP);
}

#include <RcppArmadillo.h>

//' @name Regression Class
//' 
//' Container for combinatorial seeking of best predictor selection.
//' 
//' @param output \code{arma::colvec} container for output variables for regression
//' @param predictors \code{aram::mat} container for input predictors for regression
//' @param column_iter \code{std::vector<int>} aides combinatorial variable selection
//' 
//' @exportClass
class Regression {
public:
  arma::colvec output;
  arma::mat predictors;
  std::vector<int> column_iter;
  
  Regression();
  Regression(Rcpp::NumericVector output_, Rcpp::NumericMatrix predictors_);
  Regression(SEXP);
  
  operator SEXP();
  Rcpp::NumericMatrix getPredictors();
  Rcpp::NumericVector getOutput();
  void setOutput(Rcpp::NumericVector output_);
  void setPredictors(Rcpp::NumericMatrix predictors_);
  Rcpp::List OLS(std::vector<int> columns);
  Rcpp::List GLS(std::vector<int> columns);
  Rcpp::List bestSubset();
};

#endif /* OLS_HPP */
