#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <map>

class Model;

#include <RcppCommon.h>

namespace Rcpp {
  
  
  template<> Model as(SEXP);
  template<> SEXP wrap(const Model&);
}

// [[depends(RcppArmadillo)]]
#include <RcppArmadillo.h>

class Model {
public:
  std::vector<size_t> columns;
  std::vector<std::string, double> beta, residuals;
  arma::mat corr, rss;
  
  double AIC, AICc, BIC, SSE, SSR, SSTO, MSE, MSR;
  
  Model();
  Model(Rcpp::List);
  Model(std::map<std::string, double> );
  Model(SEXP);
  
  operator SEXP();
  bool operator<(const Model&);
  bool operator>(const Model&);
};

#endif /* MODEL_HPP */