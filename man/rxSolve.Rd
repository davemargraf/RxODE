% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/rxsolve.R
\name{rxControl}
\alias{rxControl}
\alias{rxSolve}
\alias{rxSolve.default}
\alias{update.rxSolve}
\alias{predict.RxODE}
\alias{predict.rxSolve}
\alias{predict.rxEt}
\alias{predict.rxParams}
\alias{simulate.RxODE}
\alias{simulate.rxSolve}
\alias{simulate.rxParams}
\alias{solve.rxSolve}
\alias{solve.RxODE}
\alias{solve.rxParams}
\alias{solve.rxEt}
\title{Solving \& Simulation of a ODE/solved system (and solving options) equation}
\usage{
rxControl(
  scale = NULL,
  method = c("liblsoda", "lsoda", "dop853"),
  transitAbs = NULL,
  atol = 1e-08,
  rtol = 1e-06,
  maxsteps = 70000L,
  hmin = 0L,
  hmax = NA,
  hmaxSd = 0,
  hini = 0,
  maxordn = 12L,
  maxords = 5L,
  ...,
  cores,
  covsInterpolation = c("locf", "linear", "nocb", "midpoint"),
  addCov = FALSE,
  matrix = FALSE,
  sigma = NULL,
  sigmaDf = NULL,
  sigmaLower = -Inf,
  sigmaUpper = Inf,
  nCoresRV = 1L,
  sigmaIsChol = FALSE,
  nDisplayProgress = 10000L,
  amountUnits = NA_character_,
  timeUnits = "hours",
  stiff,
  theta = NULL,
  thetaLower = -Inf,
  thetaUpper = Inf,
  eta = NULL,
  addDosing = FALSE,
  stateTrim = Inf,
  updateObject = FALSE,
  omega = NULL,
  omegaDf = NULL,
  omegaIsChol = FALSE,
  omegaLower = -Inf,
  omegaUpper = Inf,
  nSub = 1L,
  thetaMat = NULL,
  thetaDf = NULL,
  thetaIsChol = FALSE,
  nStud = 1L,
  dfSub = 0,
  dfObs = 0,
  returnType = c("rxSolve", "matrix", "data.frame", "data.frame.TBS", "data.table",
    "tbl", "tibble"),
  seed = NULL,
  nsim = NULL,
  minSS = 10L,
  maxSS = 1000L,
  infSSstep = 12,
  strictSS = TRUE,
  params = NULL,
  events = NULL,
  istateReset = TRUE,
  subsetNonmem = TRUE,
  linLog = FALSE,
  maxAtolRtolFactor = 0.1,
  from = NULL,
  to = NULL,
  by = NULL,
  length.out = NULL,
  iCov = NULL,
  keep = NULL,
  drop = NULL,
  idFactor = TRUE,
  mxhnil = 0,
  hmxi = 0,
  warnIdSort = TRUE,
  warnDrop = TRUE,
  ssAtol = 1e-08,
  ssRtol = 1e-06,
  safeZero = TRUE
)

rxSolve(object, ...)

\method{rxSolve}{default}(object, params = NULL, events = NULL, inits = NULL, ...)

\method{update}{rxSolve}(object, ...)

\method{predict}{RxODE}(object, ...)

\method{predict}{rxSolve}(object, ...)

\method{predict}{rxEt}(object, ...)

\method{predict}{rxParams}(object, ...)

\method{simulate}{RxODE}(object, nsim = 1L, seed = NULL, ...)

\method{simulate}{rxSolve}(object, nsim = 1L, seed = NULL, ...)

\method{simulate}{rxParams}(object, nsim = 1L, seed = NULL, ...)

\method{solve}{rxSolve}(a, b, ...)

\method{solve}{RxODE}(a, b, ...)

\method{solve}{rxParams}(a, b, ...)

\method{solve}{rxEt}(a, b, ...)
}
\arguments{
\item{scale}{a numeric named vector with scaling for ode
parameters of the system.  The names must correspond to the
parameter identifiers in the ODE specification. Each of the
ODE variables will be divided by the scaling factor.  For
example \code{scale=c(center=2)} will divide the center ODE
variable by 2.}

\item{method}{The method for solving ODEs.  Currently this supports:

\itemize{
\item \code{"liblsoda"} thread safe lsoda.  This supports parallel
           thread-based solving, and ignores user Jacobian specification.
\item \code{"lsoda"} -- LSODA solver.  Does not support parallel thread-based
      solving, but allows user Jacobian specification.
\item \code{"dop853"} -- DOP853 solver.  Does not support parallel thread-based
        solving nor user Jacobain specification
}}

\item{transitAbs}{boolean indicating if this is a transit
compartment absorption}

\item{atol}{a numeric absolute tolerance (1e-8 by default) used
by the ODE solver to determine if a good solution has been
achieved;  This is also used in the solved linear model to check
if prior doses do not add anything to the solution.}

\item{rtol}{a numeric relative tolerance (1e-6 by default) used
by the ODE solver to determine if a good solution has been
achieved. This is also used in the solved linear model to check
 if prior doses do not add anything to the solution.}

\item{maxsteps}{maximum number of (internally defined) steps allowed
during one call to the solver. (5000 by default)}

\item{hmin}{The minimum absolute step size allowed. The default
value is 0.}

\item{hmax}{The maximum absolute step size allowed.  When
\code{hmax=NA} (default), uses the average difference (+hmaxSd*sd) in times
and sampling events. When \code{hmax=NULL} RxODE uses the
maximum difference in times in your sampling and events.  The
value 0 is equivalent to infinite maximum absolute step size.}

\item{hmaxSd}{The number of standard deviations of the time
difference to add to hmax. The default is 0}

\item{hini}{The step size to be attempted on the first step. The
default value is determined by the solver (when hini = 0)}

\item{maxordn}{The maximum order to be allowed for the nonstiff
(Adams) method.  The default is 12.  It can be between 1 and
12.}

\item{maxords}{The maximum order to be allowed for the stiff (BDF)
method.  The default value is 5.  This can be between 1 and 5.}

\item{...}{Other arguments including scaling factors for each
compartment.  This includes S# = numeric will scale a compartment
# by a dividing the compartment amount by the scale factor,
like NONMEM.}

\item{cores}{Number of cores used in parallel ODE solving.  This
defaults to the number or system cores determined by
\code{\link{rxCores}} for methods that support parallel
solving (ie thread-safe methods like "liblsoda").}

\item{covsInterpolation}{specifies the interpolation method for
    time-varying covariates. When solving ODEs it often samples
    times outside the sampling time specified in \code{events}.
    When this happens, the time varying covariates are
    interpolated.  Currently this can be:

\itemize{
\item \code{"linear"} interpolation (the default), which interpolates the covariate
    by solving the line between the observed covariates and extrapolating the new
    covariate value.
\item \code{"constant"} -- Last observation carried forward.
\item \code{"NOCB"} -- Next Observation Carried Backward.  This is the same method
      that NONMEM uses.
\item \code{"midpoint"} Last observation carried forward to midpoint; Next observation
  carried backward to midpoint.
}}

\item{addCov}{A boolean indicating if covariates should be added
to the output matrix or data frame. By default this is
disabled.}

\item{matrix}{A boolean indicating if a matrix should be returned
instead of the RxODE's solved object.}

\item{sigma}{Named sigma covariance or Cholesky decomposition of a
covariance matrix.  The names of the columns indicate
parameters that are simulated.  These are simulated for every
observation in the solved system.}

\item{sigmaDf}{Degrees of freedom of the sigma t-distribution.  By
default it is equivalent to \code{Inf}, or a normal distribution.}

\item{sigmaLower}{Lower bounds for simulated unexplained variability (by default -Inf)}

\item{sigmaUpper}{Upper bounds for simulated unexplained variability (by default Inf)}

\item{nCoresRV}{Number of cores used for the simulation of the
sigma variables.  By default this is 1. This uses the package
\code{\link[mvnfast]{rmvn}} and \code{\link[mvnfast]{rmvt}}.
To reproduce the results you need to run on the same platform
with the same number of cores. This is the reason this is set
to be one, regardless of what the number of cores are used in
threaded ODE solving.}

\item{sigmaIsChol}{Boolean indicating if the sigma is in the
Cholesky decomposition instead of a symmetric covariance}

\item{nDisplayProgress}{An integer indicating the minimum number
of c-based solves before a progress bar is shown.  By default
this is 10,000.}

\item{amountUnits}{This supplies the dose units of a data frame
supplied instead of an event table.  This is for importing the
data as an RxODE event table.}

\item{timeUnits}{This supplies the time units of a data frame
supplied instead of an event table.  This is for importing the
data as an RxODE event table.}

\item{stiff}{a logical (\code{TRUE} by default) indicating whether
    the ODE system is stiff or not.

    For stiff ODE systems (\code{stiff = TRUE}), \code{RxODE} uses the
    LSODA (Livermore Solver for Ordinary Differential Equations)
    Fortran package, which implements an automatic method switching
    for stiff and non-stiff problems along the integration
    interval, authored by Hindmarsh and Petzold (2003).

    For non-stiff systems (\code{stiff = FALSE}), \code{RxODE} uses
    DOP853, an explicit Runge-Kutta method of order 8(5, 3) of
    Dormand and Prince as implemented in C by Hairer and Wanner
    (1993).}

\item{theta}{A vector of parameters that will be named THETA[#] and
added to parameters}

\item{thetaLower}{Lower bounds for simulated population parameter variability (by default -Inf)}

\item{thetaUpper}{Upper bounds for simulated population unexplained variability (by default Inf)}

\item{eta}{A vector of parameters that will be named ETA[#] and
added to parameters}

\item{addDosing}{Boolean indicating if the solve should add RxODE
    EVID and related columns.  This will also include dosing
    information and estimates at the doses.  Be default, RxODE
    only includes estimates at the observations. (default
    \code{FALSE}). When \code{addDosing} is \code{NULL}, only
    include \code{EVID=0} on solve and exclude any model-times or
    \code{EVID=2}. If \code{addDosing} is \code{NA} the classic
    \code{RxODE} EVID events. When \code{addDosing} is \code{TRUE}
    add the event information in NONMEM-style format; If
    \code{subsetNonmem=FALSE} RxODE will also extra event types
    (\code{EVID}) for ending infusion and modeled times:

\itemize{

\item \code{EVID=-1} when the modeled rate infusions are turned
off (matches \code{rate=-1})

\item \code{EVID=-2} When the modeled duration infusions are
turned off (matches \code{rate=-2})

\item \code{EVID=-10} When the specified \code{rate} infusions are
turned off (matches \code{rate>0})

\item \code{EVID=-20} When the specified \code{dur} infusions are
turned off (matches \code{dur>0})

\item \code{EVID=101,102,103,...} Modeled time where 101 is the
first model time, 102 is the second etc.

}}

\item{stateTrim}{When amounts/concentrations in one of the states
are above this value, trim them to be this value. By default
Inf.  Also trims to -stateTrim for large negative
amounts/concentrations.  If you want to trim between a range
say `c(0, 2000000)` you may specify 2 values with a lower and
upper range to make sure all state values are in the
reasonable range.}

\item{updateObject}{This is an internally used flag to update the
RxODE solved object (when supplying an RxODE solved object) as
well as returning a new object.  You probably should not
modify it's \code{FALSE} default unless you are willing to
have unexpected results.}

\item{omega}{Estimate of Covariance matrix. When omega is a list,
assume it is a block matrix and convert it to a full matrix
for simulations.}

\item{omegaDf}{The degrees of freedom of a t-distribution for
simulation.  By default this is \code{NULL} which is
equivalent to \code{Inf} degrees, or to simulate from a normal
distribution instead of a t-distribution.}

\item{omegaIsChol}{Indicates if the \code{omega} supplied is a
Cholesky decomposed matrix instead of the traditional
symmetric matrix.}

\item{omegaLower}{Lower bounds for simulated ETAs (by default -Inf)}

\item{omegaUpper}{Upper bounds for simulated ETAs (by default Inf)}

\item{nSub}{Number between subject variabilities (ETAs) simulated for every 
realization of the parameters.}

\item{thetaMat}{Named theta matrix.}

\item{thetaDf}{The degrees of freedom of a t-distribution for
simulation.  By default this is \code{NULL} which is
equivalent to \code{Inf} degrees, or to simulate from a normal
distribution instead of a t-distribution.}

\item{thetaIsChol}{Indicates if the \code{theta} supplied is a
Cholesky decomposed matrix instead of the traditional
symmetric matrix.}

\item{nStud}{Number virtual studies to characterize uncertainty in estimated 
parameters.}

\item{dfSub}{Degrees of freedom to sample the between subject variability matrix from the 
inverse Wishart distribution (scaled) or scaled inverse chi squared distribution.}

\item{dfObs}{Degrees of freedom to sample the unexplained variability matrix from the 
inverse Wishart distribution (scaled) or scaled inverse chi squared distribution.}

\item{returnType}{This tells what type of object is returned.  The currently supported types are:
\itemize{
\item \code{"rxSolve"} (default) will return a reactive data frame
     that can change easily change different pieces of the solve and
     update the data frame.  This is the currently standard solving
     method in RxODE,  is used for \code{rxSolve(object, ...)}, \code{solve(object,...)},
\item \code{"data.frame"} -- returns a plain, non-reactive data
     frame; Currently very slightly faster than \code{returnType="matrix"}
\item \code{"matrix"} -- returns a plain matrix with column names attached
    to the solved object.  This is what is used \code{object$run} as well as \code{object$solve}
\item \code{"data.table"} -- returns a \code{data.table}; The \code{data.table} is
    created by reference (ie \code{setDt()}), which should be fast.
\item \code{"tbl"} or \code{"tibble"} returns a tibble format.
}}

\item{seed}{an object specifying if and how the random number
generator should be initialized}

\item{nsim}{represents the number of simulations.  For RxODE, if
you supply single subject event tables (created with
eventTable)}

\item{minSS}{Minimum number of iterations for a steady-state dose}

\item{maxSS}{Maximum number of iterations for a steady-state dose}

\item{infSSstep}{Step size for determining if a constant infusion
has reached steady state.  By default this is large value,
420.}

\item{strictSS}{Boolean indicating if a strict steady-state is
required. If a strict steady-state is (\code{TRUE}) required
then at least \code{minSS} doses are administered and the
total number of steady states doses will continue until
\code{maxSS} is reached, or \code{atol} and \code{rtol} for
every compartment have been reached.  However, if ODE solving
problems occur after the \code{minSS} has been reached the
whole subject is considered an invalid solve. If
\code{strictSS} is \code{FALSE} then as long as \code{minSS}
has been reached the last good solve before ODE solving
problems occur is considered the steady state, even though
either \code{atol}, \code{rtol} or \code{maxSS} have not
been achieved.}

\item{params}{a numeric named vector with values for every
parameter in the ODE system; the names must correspond to the
parameter identifiers used in the ODE specification;}

\item{events}{an \code{eventTable} object describing the input
(e.g., doses) to the dynamic system and observation sampling
time points (see \code{\link{eventTable}});}

\item{istateReset}{When \code{TRUE}, reset the \code{ISTATE} variable to 1 for
lsoda and liblsoda with doses, like \code{deSolve}; When \code{FALSE}, do
not reset the \code{ISTATE} variable with doses.}

\item{subsetNonmem}{subset to NONMEM compatible EVIDs only.  By default TRUE.}

\item{linLog}{Boolean indicating if linear compartment models be
calculated more accurately in the log-space (slower) By
default this is off (\code{FALSE})}

\item{maxAtolRtolFactor}{The maximum \code{atol}/\code{rtol} that
FOCEi and other routines may adjust to.  By default 0.1}

\item{from}{When there is no observations in the event table,
start observations at this value. By default this is zero.}

\item{to}{When there is no observations in the event table, end
observations at this value. By default this is 24 + maximum
dose time.}

\item{by}{When there are no observations in the event table, this
is the amount to increment for the observations between \code{from}
and \code{to}.}

\item{length.out}{The number of observations to create if there
isn't any observations in the event table. By default this is 200.}

\item{iCov}{A data frame of individual non-time varying covariates
to combine with the \code{params} to form a parameter
data.frame.}

\item{keep}{Columns to keep from either the input dataset or the
\code{iCov} dataset.  With the \code{iCov} dataset, the column
is kept once per line.  For the input dataset, if any records
are added to the data LOCF (Last Observation Carried forward)
imputation is performed.}

\item{drop}{Columns to drop from the output}

\item{idFactor}{This boolean indicates if original ID values
should be maintained. This changes the default sequentially
ordered ID to a factor with the original ID values in the
original dataset.  By default this is enabled.}

\item{mxhnil}{maximum number of messages printed (per problem)
warning that T + H = T on a step (H = step size).  This must
be positive to result in a non-default value.  The default
value is 0 (or infinite).}

\item{hmxi}{inverse of the maximum absolute value of H to be used.
hmxi = 0.0 is allowed and corresponds to an infinite hmax
(default).  hmin and hmxi may be changed at any time, but will
not take effect until the next change of H is considered.
This option is only considered with method=liblsoda.}

\item{warnIdSort}{Warn if the ID is not present and RxODE assumes
the order of the parameters/iCov are the same as the order of
the parameters in the input dataset.}

\item{warnDrop}{Warn if column(s) were supposed to be dropped, but
were not present.}

\item{ssAtol}{Steady state atol convergence factor.  Can be
a vector based on each state.}

\item{ssRtol}{Steady state rtol convergence factor.  Can be a
vector based on each state.}

\item{safeZero}{Use safe zero divide and log routines.  By default
this is turned on but you may turn it off if you wish.}

\item{object}{is a either a RxODE family of objects, or a file-name
with a RxODE model specification, or a string with a RxODE
model specification.}

\item{inits}{a vector of initial values of the state variables
(e.g., amounts in each compartment), and the order in this
vector must be the same as the state variables (e.g., PK/PD
compartments);}

\item{a}{when using \code{solve}, this is equivalent to the
\code{object} argument.  If you specify \code{object} later in
the argument list it overwrites this parameter.}

\item{b}{when using \code{solve}, this is equivalent to the
\code{params} argument.  If you specify \code{params} as a
named argument, this overwrites the output}
}
\value{
An \dQuote{rxSolve} solve object that stores the solved
    value in a matrix with as many rows as there are sampled time
    points and as many columns as system variables (as defined by
    the ODEs and additional assignments in the RxODE model code).
    It also stores information about the call to allow dynamic
    updating of the solved object.

    The operations for the object are similar to a data-frame, but
    expand the \code{$} and \code{[[""]]} access operators and
    assignment operators to resolve based on different parameter
    values, initial conditions, solver parameters, or events (by
    updating the \code{time} variable).

    You can call the \code{\link{eventTable}} methods on the solved
    object to update the event table and resolve the system of
    equations.  % Should be able to use roxygen templates...
}
\description{
This uses RxODE family of objects, file, or model specification to
solve a ODE system.
}
\references{
Hindmarsh, A. C.
\emph{ODEPACK, A Systematized Collection of ODE Solvers}.
Scientific Computing, R. S. Stepleman et al. (Eds.),
North-Holland, Amsterdam, 1983, pp. 55-64.

Petzold, L. R.
\emph{Automatic Selection of Methods for Solving Stiff and Nonstiff
Systems of Ordinary Differential Equations}.
Siam J. Sci. Stat. Comput. 4 (1983), pp. 136-148.

Hairer, E., Norsett, S. P., and Wanner, G.
\emph{Solving ordinary differential equations I, nonstiff problems}.
2nd edition, Springer Series in Computational Mathematics,
Springer-Verlag (1993).
}
\seealso{
\code{\link{RxODE}}
}
\author{
Matthew Fidler, Melissa Hallow and  Wenping Wang
}
