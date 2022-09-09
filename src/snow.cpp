#include "00utilis.h"
// [[Rcpp::interfaces(r, cpp)]]


//' **snow**
//' @name snow
//' @inheritParams all_vari
//' @param param_snow_kus_fE,param_snow_kus_fT parameters for [snowMelt_Kustas()]
//' @return snow_melt_mm (mm/m2) melted snow
//' @export
// [[Rcpp::export]]
NumericVector snowMelt_Kustas(
    NumericVector snow_ice_mm,
    NumericVector atmos_temperature_Cel,
    NumericVector atmos_netRadiat_MJ,
    NumericVector param_snow_kus_fE,
    NumericVector param_snow_kus_fT
)
{
  
  NumericVector snow_melt_mm = atmos_temperature_Cel * param_snow_kus_fT + param_snow_kus_fE * atmos_netRadiat_MJ;
  return ifelse(snow_melt_mm > snow_ice_mm, snow_ice_mm, snow_melt_mm) ;
  
}

//' @rdname snow
//' @param param_snow_fac_Tmelt,param_snow_fac_Tb,param_snow_fac_f parameters for [snowMelt_Factor()]
//' @export
// [[Rcpp::export]]
NumericVector snowMelt_Factor(
    NumericVector snow_ice_mm,
    NumericVector param_snow_fac_Tmelt,
    NumericVector param_snow_fac_Tb,
    NumericVector param_snow_fac_f
)
{
  NumericVector diff_T, snow_melt_mm;
  diff_T = param_snow_fac_Tmelt - param_snow_fac_Tb;
  diff_T = ifelse(diff_T > 0, diff_T, 0);
  
  snow_melt_mm = param_snow_fac_f * diff_T;
  return ifelse(snow_melt_mm > snow_ice_mm, snow_ice_mm, snow_melt_mm) ;
}

