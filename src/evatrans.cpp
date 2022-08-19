#include <Rcpp.h>
#include "00utilis.h"
using namespace Rcpp;
// [[Rcpp::interfaces(r, cpp)]]

//' **potential evapotranspiration**
//' @name evatransPotential
//' @param time_step_h <1, 24> (h) time step in hour
//' @param time_dayOfYear_ <1, 366> the number of the day in the year between 1 (1 January) and 365 or 366 (31 December)
//' @param atmos_temperature_Cel (Cel) the average air temperature in the time phase
//' @param atmos_solarRadiat_MJ (MJ/m2/TS) the solar radiation that actually reaches the earths surface
//' @param atmos_netRadiat_MJ	(MJ/m2/TS) the balance between the energy absorbed, reflected and emitted by the earths surface or the difference between the incoming net shortwave (Rns) and the net outgoing longwave (Rnl) radiation
//' @param atmos_vaporPress_hPa (hPa) actual vapour pressure, can be calculated by [atmos_VaporPress()]
//' @param atmos_saturatVaporPress_hPa (hPa) saturation vapour pressure at `atmos_temperature_Cel`, can be calculated by [atmos_SaturatVaporPress()]
//' @param atmos_windSpeed2m_m_s (m/s) wind speed at 2 m above ground surface
//' @param atmos_relativeHumidity_1 <0, 1> relative humidity
//' @param land_latitude_Degree (degree) average latitude
//' @param land_elevation_m (m) average elevation
//' @param land_albedo_1 <0, 1> albedo of the region
//' @param param_evatrans_tur_k <0.6, 1> parameter for [evatransPotential_TurcWendling()], higher value when closer to the sea
//' @return potential evapotranspiration (mm/m2)
//' @export
// [[Rcpp::export]]
NumericVector evatransPotential_TurcWendling(
    NumericVector atmos_temperature_Cel, 
    NumericVector atmos_solarRadiat_MJ, 
    NumericVector time_step_h,
    NumericVector param_evatrans_tur_k 
)
{
  return (atmos_solarRadiat_MJ * 100 + 3.875 * time_step_h * param_evatrans_tur_k) * (atmos_temperature_Cel + 22) / 150 / (atmos_temperature_Cel + 123);
}

//' @rdname evatransPotential
//' @export
// [[Rcpp::export]]
NumericVector evatransPotential_Linacre(
    NumericVector time_dayOfYear_,
    NumericVector atmos_temperature_Cel,
    NumericVector atmos_relativeHumidity_1,
    NumericVector land_latitude_Degree,
    NumericVector land_elevation_m,
    NumericVector land_albedo_1
)
{
  return ((0.75 - land_albedo_1) * 100 * (atmos_temperature_Cel + 0.006 * land_elevation_m) / (100 - land_latitude_Degree) + 3 * 100 * (1 - atmos_relativeHumidity_1)) / (80 - atmos_temperature_Cel);
}

//' @rdname evatransPotential
//' @export
// [[Rcpp::export]]
NumericVector evatransPotential_FAO56(
    NumericVector time_dayOfYear_,
    NumericVector atmos_temperature_Cel, 
    NumericVector atmos_vaporPress_hPa, 
    NumericVector atmos_saturatVaporPress_hPa, 
    NumericVector atmos_netRadiat_MJ,
    NumericVector atmos_windSpeed2m_m_s,
    NumericVector land_latitude_Degree,
    NumericVector land_elevation_m
)
{
  NumericVector Delta_, e_s, e_a, R_n, P_, gamma_, u_2, ET_o;
  
  R_n = atmos_netRadiat_MJ;
  u_2 = atmos_windSpeed2m_m_s;
  e_a = atmos_vaporPress_hPa;
  e_s = atmos_saturatVaporPress_hPa;
  //// Delta_,
  Delta_ = 4098 * (0.6108 * exp(17.27 * atmos_temperature_Cel / (atmos_temperature_Cel + 237.3))) / ((atmos_temperature_Cel + 237.3) * (atmos_temperature_Cel + 237.3)); // Eq.13
  
  //// e_s,
  // e_s = 0.3054 * exp(17.27 * atmos_temperature_max_Cel / (atmos_temperature_max_Cel + 237.3)) + 0.3054 * exp(17.27 * atmos_temperature_min_Cel / (atmos_temperature_min_Cel + 237.3)); // Eq.11, 12
  // 
  // //// e_a,
  // e_a = e_s * atmos_relative_humidity_Perc * 0.01; // Eq.19
  // //// R_n,
  // phi_ = M_PI / 180 * land_latitude_Degree; // Eq.22
  // d_r = 1 + 0.033 * cos(2 * M_PI / 365 * time_dayOfYear_); // Eq.23
  // delta_ = 0.409 * sin(2 * M_PI / 365 * time_dayOfYear_ - 1.39); // Eq.24
  // omega_s = acos(-tan(phi_) * tan(delta_)); // Eq.25
  // //// (24 (60)/M_PI) G_sc = 37.58603, G_sc <- 0.0820 MJ / (m^2 d)
  // R_a = 37.58603 * d_r * (omega_s * sin(phi_) * sin(delta_) + cos(phi_) * cos(delta_) * sin(omega_s)); // Eq.21
  // 
  // R_so = (0.75 + 2e-5 * land_elevation_m) * R_a; // Eq.37
  // R_ns = (1 - alpha_) * solar_radiation_MJ_m2d; // Eq.38
  // R_nl = sigma_ * (pow((atmos_temperature_max_Cel + 273.16), 4) + pow((atmos_temperature_min_Cel + 273.16), 4)) / 2 * (0.34 - 0.14 * sqrt(e_a)) * (1.35 * solar_radiation_MJ_m2d / R_so - 0.35); // Eq.39
  // R_n = R_ns - R_nl; // Eq.40
  
  //// G_,
  // G_ = 0.; // Eq.42
  
  //// gamma_,
  P_ = 101.3 * pow(((293 - 0.0065 * land_elevation_m) / 293), 5.26); // Eq.7
  gamma_ = 0.665e-3 * P_; // Eq.8
  
  //// u_2,
  // u_2 = wind_measure_height_m * 4.87 / (log(67.8 * wind_speed_m_s - 5.42)); // Eq.47
  // u_2 = wind_measure_height_m
  //// TE_o
  ET_o = (0.408 * Delta_ * (R_n - 0.) + gamma_ * 90 * u_2 * (e_s - e_a) / (atmos_temperature_Cel + 273)) / (Delta_ + gamma_ * (1 + 0.34 * u_2));
  return ET_o;
}

//' **actuall evapotranspiration**
//' @name evatransActual
//' @description
//' \loadmathjax
//' Under the concept of the conceptional HM, the actually ET is always consider as a part of potential ET:
//' \mjsdeqn{E_a = f E_p}
//' where
//' - \mjseqn{E_a} is `land_evatrans_mm` or `soil_evatrans_mm`
//' - \mjseqn{E_q} is `atmos_potentialEvatrans_mm`
//' - \mjseqn{f} is estimated ratio.
//' Then the different `evatransActual` methods will estimate the ratio \mjseqn{f}.
//' @return actually ET in (mm/m2/TS)
//' - evaporation in interception (landLy), `land_evatrans_mm`
//' - transpiration in root
//' - evaporation in soil (soilLy), `soil_evatrans_mm`
//' @param atmos_potentialEvatrans_mm (mm/m2/TS) **potential / reference** evapotranspiration
//' @param water_mm (mm/m2/TS) water volum in `soilLy` or interceptof `landLy`
//' @param capacity_mm (mm/m2) water storage capacity in `soilLy` or interceptof `landLy`
//' @details
//' - `_SupplyRatio`: the water content (the ratio to the maximal capacity) 
//' is considered as th main factors for the ratio \mjseqn{f}.
//' \mjsdeqn{f = k  \frac{W}{C}}
//' where
//'   - \mjseqn{W} is water volume in (mm/m2/TS), `water_mm`, `land_interceptWater_mm`, `soil_water_mm`
//'   - \mjseqn{C} is water capacity in (mm/m2), `capacity_mm`, `land_interceptCapacity_mm`, `soil_capacity_mm`
//'   - \mjseqn{k} is `param_evatrans_sur_k`
//' @param param_evatrans_sur_k <0.1, 1> parameter for [evatransActual_SupplyRatio()], ratio of potential ET, that is estimated as actually ET  
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_SupplyRatio(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_evatrans_sur_k
)
{
  NumericVector AET, k_;
  
  k_ = water_mm / capacity_mm * param_evatrans_sur_k;
  AET = atmos_potentialEvatrans_mm * k_;
  return ifelse(AET > water_mm, water_mm, AET);
}

//' @rdname evatransActual
//' @details
//' - `_SupplyPow`: the water content (the ratio to the maximal capacity) 
//' is considered as th main factors for the ratio \mjseqn{f}.
//' \mjsdeqn{f = k  \left(\frac{W}{C}\right)^\gamma}
//' where
//'   - \mjseqn{k} is `param_evatrans_sup_k`
//'   - \mjseqn{\gamma} is `param_evatrans_sup_gamma`
//' @param param_evatrans_sup_k <0.1, 1> parameter for [evatransActual_SupplyPow()], ratio of this method
//' @param param_evatrans_sup_gamma <-3, 5> parameter for [evatransActual_SupplyPow()], exponent of this method
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_SupplyPow(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_evatrans_sup_k,
    NumericVector param_evatrans_sup_gamma
)
{
  NumericVector AET, k_;
  
  k_ = param_evatrans_sup_k * vecpow((water_mm / capacity_mm), param_evatrans_sup_gamma);
  AET = atmos_potentialEvatrans_mm * k_;
  return ifelse(AET > water_mm, water_mm, AET);
}

//' @rdname evatransActual
//' @details
//' - `_AcceptPow`: only the potential ET 
//' is considered as th main factors for the ratio \mjseqn{f}.
//' \mjsdeqn{f = k}
//' where
//'   - \mjseqn{k} is `param_evatrans_acr_k`
//' @param param_evatrans_acr_k <0.1, 1> parameter for [evatransActual_AcceptRatio()]
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_AcceptRatio(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector param_evatrans_acr_k
)
{
  NumericVector AET;
  
  AET = atmos_potentialEvatrans_mm * param_evatrans_acr_k;
  
  return ifelse(AET > water_mm, water_mm, AET);
}

//' @rdname evatransActual
//' @details
//' - `_AcceptPow`: only the potential ET 
//' is considered as th main factors for the ratio \mjseqn{f}.
//' \mjsdeqn{E_a = k  E_p^\gamma}
//' where
//'   - \mjseqn{k} is `param_evatrans_acp_k`
//'   - \mjseqn{\gamma} is `param_evatrans_acp_gamma`
//' @param param_evatrans_acp_k <0.1, 1> parameter for [evatransActual_AcceptPow()], ratio of this method
//' @param param_evatrans_acp_gamma <-3, 1> parameter for [evatransActual_AcceptPow()], exponent of this method
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_AcceptPow(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector param_evatrans_acp_k,
    NumericVector param_evatrans_acp_gamma
)
{
  NumericVector AET, k_;
  
  k_ = param_evatrans_acp_k * vecpow(atmos_potentialEvatrans_mm, param_evatrans_acp_gamma);
  AET = atmos_potentialEvatrans_mm * k_;
  return ifelse(AET > water_mm, water_mm, AET);
}



//' @rdname evatransActual
//' @param param_evatrans_vic_gamma parameter for [evatransActual_VIC()]
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_VIC(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_evatrans_vic_gamma
)
{
  NumericVector AET, k_;
  
  k_ = 1 - vecpow((1- water_mm / capacity_mm), param_evatrans_vic_gamma);
  AET = atmos_potentialEvatrans_mm * k_;
  return ifelse(AET > water_mm, water_mm, AET);
}




//' @rdname evatransActual
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_GR4J(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector capacity_mm,
    NumericVector water_mm
)
{
  NumericVector AET;
  
  
  AET = water_mm * (2 - water_mm / capacity_mm) * tanh(atmos_potentialEvatrans_mm / capacity_mm) / (1 + (1 - water_mm / capacity_mm) * tanh(atmos_potentialEvatrans_mm / capacity_mm));
  return ifelse(AET > water_mm, water_mm, AET);
  
}

//' @rdname evatransActual
//' @param param_infilt_ubc_P0EGEN parameter for [evatransActual_UBC()]
//' @export
// [[Rcpp::export]]
NumericVector evatransActual_UBC(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_infilt_ubc_P0EGEN
)
{
  NumericVector diff_mm, AET, k_;
  diff_mm = capacity_mm - water_mm;
  
  
  
  k_ = vecpow10(- diff_mm / param_infilt_ubc_P0EGEN);
  AET = atmos_potentialEvatrans_mm * k_;
  return ifelse(AET > water_mm, water_mm, AET);
}

//' @rdname evatransActual
//' @param param_evatrans_lia_gamma parameter for [evatransLand_Liang()]
//' @export
// [[Rcpp::export]]
NumericVector evatransLand_Liang(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_evatrans_lia_gamma
)
{
  NumericVector AET, k_, f_;
  
  k_ = vecpow((water_mm / capacity_mm), param_evatrans_lia_gamma);
  AET = atmos_potentialEvatrans_mm * k_;
  f_ = water_mm / AET;
  f_ = ifelse(f_ > 1, 1, f_);
  AET = f_ * AET;
  return ifelse(AET > water_mm, water_mm, AET);
}


//' @rdname evatransActual
//' @param param_evatrans_lia_B parameter for [evatransSoil_Liang()]
//' @export
// [[Rcpp::export]]
NumericVector evatransSoil_Liang(
    NumericVector atmos_potentialEvatrans_mm,
    NumericVector water_mm,
    NumericVector capacity_mm,
    NumericVector param_evatrans_lia_B
)
{
  NumericVector AET, k_, f_, i_0, i_m, B_1, B_p_1, A_s, A_s_1;
  
  i_m = capacity_mm * (param_evatrans_lia_B + 1);
  
  B_p_1 = (param_evatrans_lia_B + 1);
  B_1 = 1 / param_evatrans_lia_B;
  
  i_0 = i_m * (1 - vecpow(1 - water_mm * B_p_1 / i_m, B_1));
  A_s = 1 - vecpow((1 - i_0 / i_m), param_evatrans_lia_B);
  A_s_1 = (1 - A_s);
  
  k_ = A_s + i_0 / i_m * A_s_1 * (1 + 
    param_evatrans_lia_B / (1+param_evatrans_lia_B) * vecpow(A_s_1, 1 / param_evatrans_lia_B) +
    param_evatrans_lia_B / (2+param_evatrans_lia_B) * vecpow(A_s_1, 2 / param_evatrans_lia_B) +
    param_evatrans_lia_B / (3+param_evatrans_lia_B) * vecpow(A_s_1, 3 / param_evatrans_lia_B));
  
  AET = atmos_potentialEvatrans_mm * k_;
  
  return ifelse(AET > water_mm, water_mm, AET);
}

