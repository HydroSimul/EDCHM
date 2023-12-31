% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{evatransActual}
\alias{evatransActual}
\alias{evatransActual_SupplyRatio}
\alias{evatransActual_SupplyPow}
\alias{evatransActual_VIC}
\alias{evatransActual_GR4J}
\alias{evatransActual_UBC}
\alias{evatransActual_LiangLand}
\alias{evatransActual_LiangSoil}
\title{\strong{actuall evapotranspiration}}
\usage{
evatransActual_SupplyRatio(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_sur_k
)

evatransActual_SupplyPow(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_sup_k,
  param_evatrans_sup_gamma
)

evatransActual_VIC(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_vic_gamma
)

evatransActual_GR4J(atmos_potentialEvatrans_mm, water_mm, capacity_mm)

evatransActual_UBC(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_ubc_gamma
)

evatransActual_LiangLand(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_lia_gamma
)

evatransActual_LiangSoil(
  atmos_potentialEvatrans_mm,
  water_mm,
  capacity_mm,
  param_evatrans_lia_B
)
}
\arguments{
\item{atmos_potentialEvatrans_mm}{(mm/m2/TS) \strong{potential / reference} evapotranspiration}

\item{water_mm}{(mm/m2/TS) water volume in \code{soilLy}, \code{groundLy} or intercept of \code{landLy}, same as \code{soil_water_mm}, \code{ground_water_mm} or \code{land_interceptWater_mm}}

\item{capacity_mm}{(mm/m2) water storage capacity in \code{soilLy}, \code{groundLy} or intercept of \code{landLy}, same as \code{soil_capacity_mm}, \code{ground_capacity_mm} or\code{land_interceptCapacity_mm}}

\item{param_evatrans_sur_k}{<0.1, 1> parameter for \code{\link[=evatransActual_SupplyRatio]{evatransActual_SupplyRatio()}}, ratio of potential ET, that is estimated as actually ET}

\item{param_evatrans_sup_k}{<0.1, 1> parameter for \code{\link[=evatransActual_SupplyPow]{evatransActual_SupplyPow()}}, ratio of this method}

\item{param_evatrans_sup_gamma}{<1, 5> parameter for \code{\link[=evatransActual_SupplyPow]{evatransActual_SupplyPow()}}, exponent of this method}

\item{param_evatrans_vic_gamma}{<0.2, 5> parameter for \code{\link[=evatransActual_VIC]{evatransActual_VIC()}}}

\item{param_evatrans_ubc_gamma}{<0.5, 2> parameter for \code{\link[=evatransActual_UBC]{evatransActual_UBC()}}}

\item{param_evatrans_lia_gamma}{<0.4, 1> parameter for \code{\link[=evatransActual_LiangLand]{evatransActual_LiangLand()}}}

\item{param_evatrans_lia_B}{<0.01, 3> parameter for \code{\link[=evatransActual_LiangSoil]{evatransActual_LiangSoil()}}}
}
\value{
actually ET in (mm/m2/TS)
\itemize{
\item evaporation in interception (landLy), \code{land_evatrans_mm}
\item transpiration in root
\item evaporation in soil (soilLy), \code{soil_evatrans_mm}
}
}
\description{
\loadmathjax
Actual ET, or actual evapotranspiration, is a measure of the amount of water that is lost from the land surface through evaporation and transpiration by plants.

Under the concept of the conceptual HM, the actual ET is always calculated by the potential ET \mjseqn{E_p}, which evaluates the meteorological and landuse (vegetation) situations.
The second point to consider is the water availability of the land or soil.

So we can give the function from:

\mjsdeqn{E_a = f_{evatransActual}(D_{atms}, D_{lssg})}

to:

\mjsdeqn{E_a = f_{evatransActual}(E_p, W_{lssg}, ...) = k^* E_p}

where
\itemize{
\item \mjseqn{E_a} is \code{land_evatrans_mm} or \code{soil_evatrans_mm}
\item \mjseqn{E_p} is \code{atmos_potentialEvatrans_mm}
\item \mjseqn{k^*} is estimated ratio.
}

Then the different \code{evatransActual} methods will estimate the ratio \mjseqn{k^*}.

The output density distribution from 7 methods:

\if{html}{\figure{mdl_evatransActual.svg}}
\if{latex}{\figure{mdl_evatransActual.pdf}{options: width=140mm}}
}
\section{\strong{_SupplyRatio}:}{
\if{html}{\figure{mdl_evatransActual_sur.svg}}
\if{latex}{\figure{mdl_evatransActual_sur.pdf}{options: width=140mm}}

The water content (the ratio to the maximal capacity)
is considered as th main factors for the ratio \mjseqn{k^*}.
\mjsdeqn{k^* = k  \frac{W}{C}}
where
\itemize{
\item \mjseqn{W} is water volume in (mm/m2/TS), \code{water_mm}, \code{land_interceptWater_mm}, \code{soil_water_mm}
\item \mjseqn{C} is water capacity in (mm/m2), \code{capacity_mm}, \code{land_interceptCapacity_mm}, \code{soil_capacity_mm}
\item \mjseqn{k} is \code{param_evatrans_sur_k}
}
}

\section{\strong{_SupplyPow}:}{
\if{html}{\figure{mdl_evatransActual_sup.svg}}
\if{latex}{\figure{mdl_evatransActual_sup.pdf}{options: width=140mm}}

The water content (the ratio to the maximal capacity)
is considered as th main factors for the ratio \mjseqn{k^*}.
\mjsdeqn{k^* = k  \left(\frac{W}{C}\right)^\gamma}
where
\itemize{
\item \mjseqn{k} is \code{param_evatrans_sup_k}
\item \mjseqn{\gamma} is \code{param_evatrans_sup_gamma}
}
}

\section{\strong{_VIC} \insertCite{VIC_Wood_1992}{EDCHM}:}{
\if{html}{\figure{mdl_evatransActual_vic.svg}}
\if{latex}{\figure{mdl_evatransActual_vic.pdf}{options: width=140mm}}

This method is similar with \code{\link[=evatransActual_SupplyPow]{evatransActual_SupplyPow()}}, estimate the water content in the storage.
\mjsdeqn{k^* = 1-\left(1-\frac{W}{C}\right)^{\gamma}}
where
\itemize{
\item \mjseqn{\gamma} is \code{param_evatrans_vic_gamma}
}
}

\section{\strong{_GR4J} \insertCite{GR4J_Perrin_2003}{EDCHM}:}{
\if{html}{\figure{mdl_evatransActual_gr4.svg}}
\if{latex}{\figure{mdl_evatransActual_gr4.pdf}{options: width=140mm}}

It is a little different than other method, it estimate not the ratio \mjseqn{f},
rather dieectly a equation with potential ET and water content.
And it need \strong{no parameter}.
\mjsdeqn{E_a = \frac{W\left(2-\frac{W}{C}\right)\tanh \left(\frac{E_p}{C}\right)}{1 + \left(1-\frac{W}{C}\right)\tanh \left(\frac{E_p}{C}\right)}}
}

\section{\strong{_UBC} \insertCite{UBC_Quick_1977}{EDCHM}:}{
\if{html}{\figure{mdl_evatransActual_ubc.svg}}
\if{latex}{\figure{mdl_evatransActual_ubc.pdf}{options: width=140mm}}

It estimates the water content in the storage.
(This is a little different than original, the parameter \code{P0AGEN} is replaced by \mjseqn{\frac{C}{\gamma}}.)
\mjsdeqn{k^* = 10^{\gamma \frac{W-C}{C}}}
where
\itemize{
\item \mjseqn{\gamma} is \code{param_evatrans_ubc_gamma}
}
}

\section{\strong{_LiangLand} \insertCite{VIC2_Liang_1994}{EDCHM}:}{
\if{html}{\figure{mdl_evatransActual_lia.svg}}
\if{latex}{\figure{mdl_evatransActual_lia.pdf}{options: width=140mm}}

It is also a similar method like \code{\link[=evatransActual_SupplyPow]{evatransActual_SupplyPow()}},
but it will estimate the supply ability agian, whwn the water is still not enough.
\mjsdeqn{E_a^* = \left(\frac{W}{C}\right)^\gamma E_p}
\mjsdeqn{E_a = \min \left(1, \frac{W}{E_a^*}\right) E_a^*}
where
\itemize{
\item \mjseqn{E_l^*} is the first estimated actuall ET
\item \mjseqn{E_l} is actuall ET from land, \code{land_evatrans_mm}
\item \mjseqn{\gamma} is \code{param_evatrans_lia_gamma}
}
}

\section{\strong{_LiangSoil} \insertCite{VIC2_Liang_1994}{EDCHM}:}{
\if{html}{\figure{mdl_evatransActual_sia.svg}}
\if{latex}{\figure{mdl_evatransActual_sia.pdf}{options: width=140mm}}

It estimates the water content in the storage.
(This is a little different than original, the parameter \code{P0AGEN} is replaced by \mjseqn{\frac{C}{\gamma}}.)
\mjsdeqn{k^* = \int_{0}^{A_{s}} {\rm d} A + \int_{A_{s}}^{1} \frac{i_{0}}{i_{m} [1-(1-A)^{1 / B} ]} {\rm d} A }
where
\itemize{
\item \mjseqn{B} is \code{param_evatrans_lia_B}
\item \mjseqn{A} is fraction of area
}

\figure{liang_evatransSoil.png}
}

\references{
\insertAllCited{}
}
