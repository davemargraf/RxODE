% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/rxsympy.R
\name{.rxSymPyJacobian}
\alias{.rxSymPyJacobian}
\title{Calculate the full Jacobian for a model}
\usage{
.rxSymPyJacobian(model)
}
\arguments{
\item{model}{RxODE family of objects}
}
\value{
RxODE syntax for model with Jacobian specified.
}
\description{
This expand the model to calculate the Jacobian.  This requires
rSymPy.
}
\author{
Matthew L. Fidler
}
