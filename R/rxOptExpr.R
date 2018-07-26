.rxOptFn <- function(fn){
    force(fn);
    function(...){
        ret <- paste0(fn, "(", paste(unlist(list(...)), collapse=", "), ")")
        if (!is.null(.rxOptEnv$.rep[[ret]])){
            return(.rxOptEnv$.rep[[ret]])
        }
        if(is.null(.rxOptEnv$.list[[ret]])){
            .rxOptEnv$.list[[ret]] <- 1L;
        } else {
            .rxOptEnv$.list[[ret]] <- .rxOptEnv$.list[[ret]] + 1L;
        }
        return(ret)
    }
}
.rxOptBin <- function(sep) {
    force(sep)
    function(e1, e2) {
        if (missing(e2)){
            if (sep == "+"){
                ret <- paste0(e1)
            } else {
                ret <- paste0(gsub(" ", "", sep), e1)
            }
        } else {
            ret <- paste0(e1, sep, e2)
        }
        if (!is.null(.rxOptEnv$.rep[[ret]])){
            return(.rxOptEnv$.rep[[ret]])
        }
        if(is.null(.rxOptEnv$.list[[ret]])){
            .rxOptEnv$.list[[ret]] <- 1L;
        } else {
            .rxOptEnv$.list[[ret]] <- .rxOptEnv$.list[[ret]] + 1L;
        }
        return(ret)
    }
}
.rxOptEnv <- new.env(parent = emptyenv())
.rxOptEnv$"^" <- .rxOptBin("^")
.rxOptEnv$"**" <- .rxOptBin("^")

.rxOptEnv[["*"]] <- .rxOptBin("*");

.rxOptEnv[["/"]] <- .rxOptBin("/");

.rxOptEnv[["+"]] <- .rxOptBin("+");

.rxOptEnv[["-"]] <- .rxOptBin("-");

.rxOptEnv$"[" <- function(name, val){
    n <- toupper(name)
    err <- "RxODE only supports THETA[#] and ETA[#] numbers."
    if (any(n == c("THETA", "ETA")) && is.numeric(val)){
        if (round(val) == val && val > 0){
            return(sprintf("%s[%s]", n, val));
        } else {
            stop(err);
        }
    } else {
        stop(err)
    }
}
.rxOptEnv$"[" <- function(name, val){
    n <- toupper(name)
    err <- "RxODE only supports THETA[#] and ETA[#] numbers."
    if (any(n == c("THETA", "ETA")) && is.numeric(val)){
        if (round(val) == val && val > 0){
            return(sprintf("%s[%s]", n, val));
        } else {
            stop(err);
        }
    } else {
        stop(err)
    }
}

.rxOptEnv$"(" <- unaryOp("(", ")")

.rxOptEnv$.list <- list();
.rxOptEnv$.rep <- list();

.rxOptGetEnv <- function(expr){
    ## Known functions
    .calls <- allCalls(expr)
    .callList <- setNames(lapply(.calls, .rxOptFn), .calls)
    .callEnv <- list2env(.callList);
    .currEnv <- cloneEnv(.rxOptEnv, .callEnv);
    .names <- allNames(expr)
    .n1 <- .names;
    .n2 <-.names;
    .symbolList <- setNames(as.list(.n2), .n1);
    .symbolEnv <- list2env(.symbolList, parent=.currEnv);
    return(.symbolEnv)
}
.rxOptExpr <- function(x){
    return(eval(x, .rxOptGetEnv(x)))
}
##' Optimize RxODE for computer evaluation
##'
##' This optimizes RxODE code for computer evaluation by only
##' calculating redundant expressions once.
##'
##' @param x RxODE model that can be access by rxNorm
##' @return Optimized RxODE model text.  The order and type lhs and
##'     state variables is maintained while the evaluation is sped up.
##'     While parameters names are maintained, their order may be
##'     modified.
##' @author Matthew L. Fidler
##' @export
rxOptExpr <- function(x){
    .rxOptEnv$.list <- list();
    .rxOptEnv$.rep <- list();
    .lines <- strsplit(rxNorm(x), "\n")[[1]];
    .f <- function(line){
        .silent <- (regexpr("[~]", line) != -1)
        .l2 <- strsplit(line, "[=~]")[[1]]
        .ret <- eval(parse(text=sprintf(".rxOptExpr(quote(%s))", gsub(";$", "",.l2[2]))));
        if (.silent){
            return(paste0(.l2[1], " ~ ", .ret))
        } else {
            return(paste0(.l2[1], " = ", .ret))
        }
    }
    .ret <- sapply(.lines, .f)
    .rxOptEnv$.list <- .rxOptEnv$.list[which(unlist(.rxOptEnv$.list) > 1L)];
    .exprs <- names(.rxOptEnv$.list)[order(nchar(names(.rxOptEnv$.list)))];
    .exprs <- .exprs[regexpr(rex::rex(start, regNum, end), .exprs, perl=TRUE) == -1]
    .exprs <- .exprs[regexpr(rex::rex(start, or("THETA[", "ETA["), any_numbers, "]", end), .exprs, perl=TRUE) == -1]

    for (i in seq(1, length(.exprs) - 1)){
        .exprs[-seq(1, i)] <- gsub(rex::rex(.exprs[i]), sprintf("rx_expr_%03d", i), .exprs[-seq(1, i)])
    }
    .rxOptEnv$.rep <- setNames(as.list(sprintf("rx_expr_%03d", seq_along(.exprs))), .exprs)
    .ret <- c(paste(sprintf("rx_expr_%03d ~", seq_along(.exprs)), .exprs),
              sapply(.lines, .f))
    return(paste(.ret, collapse="\n"))
}