#include "model.hpp"

// [[depends(RcppArmadillo)]]
#include <RcppArmadillo.h>

using namespace Rcpp;

Model::Model() :
  AIC(), AICc(), BIC(), SSE(), SSR(), SSTO(), MSE(), MSR() {}
  
Model::Model(std::map<std::string, double> mdl) :
  AIC(mdl["AIC"]), AICc(mdl["AICc"]), BIC(mdl["BIC"]), SSE(mdl["SSE"]), 
  SSR(mdl["SSR"]), SSTO(mdl["SSTO"]), MSE(mdl["MSE"]), MSR(mdl["MSR"]) {}
    
Model::Model(List mdl) :
  AIC(), AICc(), BIC(), SSE(), SSR(), SSTO(), MSE(), MSR() {
  List model(mdl);
  
  AIC = model["AIC"];
  AICc = model["AICc"];
  BIC = model["BIC"];
  SSE = model["SSE"];
  SSR = model["SSR"];
  SSTO = model["SSTO"];
  MSE = model["MSE"];
  MSR = model["MRS"];
  }
  
Model::Model(SEXP mdl) :
  AIC(), AICc(), BIC(), SSE(), SSR(), SSTO(), MSE(), MSR() {
  List model(mdl);
  
  AIC = model["AIC"];
  AICc = model["AICc"];
  BIC = model["BIC"];
  SSE = model["SSE"];
  SSR = model["SSR"];
  SSTO = model["SSTO"];
  MSE = model["MSE"];
  MSR = model["MRS"];
}

Model::operator SEXP() {
  return List::create(
    _["AIC"] = AIC,
    _["AICc"] = AICc,
    _["BIC"] = BIC,
    _["SSE"] = SSE,
    _["SSR"] = SSR,
    _["SSTO"] = SSTO,
    _["MSE"] = MSE,
    _["MSR"] = MSR
    );
}

bool Model::operator<(const Model& mdl) {
  if (AIC < mdl.AIC) return true;
  else if (AICc < mdl.AICc) return true;
  else if (BIC < mdl.BIC) return true;
  else if (SSE < mdl.SSE) return true;
  else return false;
}

bool Model::operator>(const Model& mdl) {
  if (AIC > mdl.AIC) return true;
  else if (AICc > mdl.AICc) return true;
  else if (BIC > mdl.BIC) return true;
  else if (SSE > mdl.SSE) return true;
  else return false;
}

RCPP_EXPOSED_CLASS(Model)

RCPP_MODULE(Model) {
  using namespace Rcpp;
  class_<Model>("Model")
    .constructor()
    .constructor<List>()
    
    .field_readonly("AIC", &Model::AIC)
    .field_readonly("AICc", &Model::AICc)
    .field_readonly("BIC", &Model::BIC)
    .field_readonly("SSE", &Model::SSE)
    .field_readonly("SSR", &Model::SSR)
    .field_readonly("SSTO", &Model::SSTO)
    .field_readonly("MSE", &Model::MSE)
    .field_readonly("MSR", &Model::MSR)
  ;
}

