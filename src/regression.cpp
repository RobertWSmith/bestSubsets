
// header library for determining the next combination using itertors
#include "combination.hpp"
using boost::next_combination;

#include "regression.hpp"
#include "model.hpp"

// [[depends(RcppArmadillo)]]
#include <RcppArmadillo.h>
#include <vector>
#include <array>

#include <map>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Rcpp;

Regression::Regression() : 
  output(), predictors(), column_iter() {}

Regression::Regression(NumericVector output_, NumericMatrix predictors_) :
  output(), predictors(), column_iter() {
    
  setOutput(output_);
  setPredictors(predictors_);
  
}

Regression::Regression(SEXP) :
  output(), predictors(), column_iter() {}

Regression::operator SEXP() {
  return List::create(
    _["output"] = output,
    _["predictors"] = predictors
    );
}

NumericMatrix Regression::getPredictors() {
  return wrap(predictors);
}

NumericVector Regression::getOutput() {
  return wrap(output);
}

void Regression::setOutput(NumericVector output_) {
  output.reset(); // erases any previous data
  arma::colvec temp;
  temp = as<arma::colvec>(output_);
  output = temp;
}

void Regression::setPredictors(NumericMatrix predictors_) {
  predictors.reset(); // erases any previous data
  
  int r = predictors_.nrow();
  
  // concatenate a vector of ones to enable models other than zero-intercept
  arma::colvec ones_vec = arma::ones<arma::colvec>(r); 
  arma::mat temp = as<arma::mat>(predictors_);

  temp.insert_cols(0, ones_vec); 
  predictors = temp; 
  
  column_iter.clear(); // erases any previous data
  for (size_t i=1; i <= predictors.n_cols; i++) column_iter.push_back(i);
}

Model Regression::OLS(std::vector<int> columns) {
  BEGIN_RCPP
  
  arma::uvec cols;
  cols << 0;
  
  for (size_t i = 0; i < columns.size(); i++) {
    cols << columns[i];
  }

  arma::mat preds = predictors.cols(cols);

  int n_obs = preds.n_rows, n_preds = preds.n_cols;
  
  arma::colvec coef = arma::inv(preds.t() * preds) * preds.t() * output;
  arma::colvec resid = output - preds * coef;
  arma::mat rss = resid.t() * resid;
  
  double sse = arma::as_scalar(resid.t() * resid)/(n_obs - n_preds);
  
  // http://www4.ncsu.edu/~shu3/Presentation/AIC.pdf -- slide 7
  double aic = ((n_obs * log( sse / n_obs )) + (2 * (n_preds + 1)));

  
  return List::create(
    _["variables"] = cols,
    _["beta"] = coef,
    _["residuals"] = resid,
    _["RSE"] = resid,
    _["SSE"] = sse,
    _["AIC"] = aic
    );
  END_RCPP
}

Model Regression::GLS(std::vector<int> columns) {
  return List::create(_["nil"] = 0 );
}

Model Regression::bestSubset() {
  BEGIN_RCPP
  
  std::vector<int> temp_cols;
  List best, temp;
  best = List::create(_["aic"] = -100);
  double best_aic, temp_aic;
  
  for (size_t k = 1; k <= predictors.n_cols; k++ ) {
    temp_cols.clear();
    
    do {
      temp_cols.clear();
      temp_cols.insert(temp_cols.begin(), column_iter.begin(), column_iter.begin() + k);
      
      temp = OLS(temp_cols);
      
      best_aic = best["aic"];
      temp_aic = temp["aic"];
      
      if (best_aic <= -1) {
        best = temp;
      } else if (temp_aic < best_aic) {
          best = temp;
      }
    } while(next_combination(column_iter.begin(), column_iter.begin() + k, column_iter.end()));
  }
  
  END_RCPP
}

RCPP_EXPOSED_CLASS(Regression)

RCPP_MODULE(Regression) {
  using namespace Rcpp;
  class_<Regression>("Regression")
    .constructor()
    .constructor<NumericVector, NumericMatrix>()
    
    .property("output", &Regression::getOutput, &Regression::setOutput)
    .property("predictors", &Regression::getPredictors, &Regression::setPredictors)
  
    .method("OLS", &Regression::OLS)
    .method("GLS", &Regression::GLS)
    .method("bestSubset", &Regression::bestSubset)
  ;
}

