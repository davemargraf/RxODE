#include <Rcpp.h>

using namespace Rcpp;

List rxModelVars_(const RObject &obj);
bool rxIs(const RObject &obj, std::string cls);

#define rxModelVars(a) rxModelVars_(a)

bool hasElement(CharacterVector one, std::string what){
  for (unsigned int i = one.size(); i--;){
    if (as<std::string>(one[i]) == what) return true;
  }
  return false;
}

//' Stack a solved object for things like ggplot
//'
//' @param Data is a RxODE object to be stacked.
//'
//' @param vars Variables to include in stacked data; By default this
//'   is all the variables when vars is NULL.
//'
//' @return Stacked data with value and trt, where value is the values
//'   and trt is the state and lhs variables.
//' 
//' @author Matthew Fidler
//[[Rcpp::export]]
List rxStack(List Data, Nullable<CharacterVector> vars=R_NilValue){
  
  List mv = rxModelVars(Data);
  CharacterVector lhs = mv["lhs"];
  CharacterVector state = mv["state"];
  IntegerVector stateIgnore = mv["state.ignore"];
  int nfactor = lhs.size();
  int j, k;
  bool allVars = vars.isNull();
  CharacterVector keep;
  if (!allVars){
    keep = CharacterVector(vars);
    nfactor = 0;
    for (j = 0; j < lhs.size(); ++j){
      if (hasElement(keep, as<std::string>(lhs[j]))) nfactor++;
    }  
  }
  for (j = 0; j < state.size(); ++j){
    if (stateIgnore[j] == 0){
      if (allVars || hasElement(keep, as<std::string>(state[j])))
	nfactor++;
    }
  }
  CharacterVector lvl(nfactor);
  k=0;
  for (j = 0; j < state.size(); ++j){
    if (stateIgnore[j] == 0) {
      if (allVars || hasElement(keep, as<std::string>(state[j]))){
	lvl[k++] = state[j];
      }
    }
  }
  for (j = 0; j < lhs.size(); ++j){
    if (allVars || hasElement(keep, as<std::string>(lhs[j]))){
      lvl[k++] = lhs[j];
    }
  }
  // See if the factors are added to your stacked data frame.
  int ncols=3;
  bool bSimId=Data.containsElementNamed("sim.id");
  if (bSimId) ncols++;
  bool bId=Data.containsElementNamed("id");
  if (bId) ncols++;
  bool bEvid=Data.containsElementNamed("evid");
  if (bEvid) ncols++;
  bool bAmt=Data.containsElementNamed("amt");
  if (bAmt) ncols++;
  List ret;
  
  IntegerVector inSimId;
  IntegerVector outSimId;
  if (bSimId){
    inSimId = Data["sim.id"];
    outSimId = IntegerVector(inSimId.size()*nfactor);
    for (j = nfactor; j--;){
      std::copy(inSimId.begin(),inSimId.end(),outSimId.begin()+j*inSimId.size());
    }
    ret["sim.id"] = outSimId;
  }
  
  IntegerVector inId;
  IntegerVector outId;
  if (bId){
    inId = Data["id"];
    outId = IntegerVector(inId.size()*nfactor);
    for (j = nfactor; j--;){
      std::copy(inId.begin(),inId.end(),outId.begin()+j*inId.size());
    }
    ret["id"] = outId;
  }

  IntegerVector inEvid;
  IntegerVector outEvid;
  if (bEvid){
    inEvid = Data["evid"];
    outEvid = IntegerVector(inEvid.size()*nfactor);
    for (j = nfactor; j--;){
      std::copy(inEvid.begin(),inEvid.end(),outEvid.begin()+j*inEvid.size());
    }
    ret["evid"] = outEvid;
  }

  NumericVector inAmt;
  NumericVector outAmt;
  if (bAmt){
    inAmt = Data["amt"];
    outAmt = NumericVector(inAmt.size()*nfactor);
    for (j = nfactor; j--;){
      std::copy(inAmt.begin(),inAmt.end(),outAmt.begin()+j*inAmt.size());
    }
    if (rxIs(inAmt, "units")){
      outAmt.attr("class") = "units";
      outAmt.attr("units") = inAmt.attr("units");
    }
    ret["amt"] = outAmt;
  }

  NumericVector inTime = Data["time"];
  NumericVector outTime(inTime.size()*nfactor);
  for (j = nfactor; j--;){
    std::copy(inTime.begin(),inTime.end(),outTime.begin()+j*inTime.size());
  }
  if (rxIs(inTime, "units")){
    outTime.attr("class") = "units";
    outTime.attr("units") = inTime.attr("units");
  }
  ret["time"] = outTime;


  NumericVector outValue(inTime.size()*nfactor);
  IntegerVector outLvl(inTime.size()*nfactor);
  NumericVector tmpV;
  for (j = 0; j < nfactor; j++){
    tmpV = Data[as<std::string>(lvl[j])];
    std::copy(tmpV.begin(),tmpV.end(), outValue.begin()+j*inTime.size());
    std::fill_n(outLvl.begin()+j*inTime.size(), inTime.size(), j+1);
  }
  outLvl.attr("class") = "factor";
  outLvl.attr("levels") = lvl;
  ret["value"] = outValue;
  ret["trt"] = outLvl;
  ret.attr("class") = "data.frame";
  ret.attr("row.names") = IntegerVector::create(NA_INTEGER, -inTime.size()*nfactor);
  return ret;
}
