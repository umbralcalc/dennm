---
geometry: margin=2cm
---

# `dennm-torch`: (DEN)sity of (N)on-(M)arkovian phenomena with Libtorch

The computational framework for `dennm-torch` is derived using the formalism from the publicly-available book: [Diffusing Ideas](https://umbralcalc.github.io/diffusing-ideas/). Let's begin by writing the generalised non-Markovian master equation for stochastic phenomena which was given in [this chapter](https://umbralcalc.github.io/diffusing-ideas/empirical_probabilistic_reweighting/chapter.pdf) of the book

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) &= \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}+1}(X\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) P_{({\sf t}+1){\sf t}}(x\vert X',z) .
\end{aligned}
$$

Without loss of generality, we can relate the latest probabilities to those from deeper into the past by chaining conditional probabilities together in a non-Markovian equivalent of the [Chapman-Kolmogorov equation](https://en.wikipedia.org/wiki/Chapman%E2%80%93Kolmogorov_equation)

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) &= \int_{\Omega_{{\sf t}-1}}{\rm d}X''P_{{\sf t}-1}(X''\vert z)\int_{\omega_{{\sf t}}}{\rm d}^nx' P_{{\sf t}({\sf t}-1)}(x'\vert X'',z)P_{({\sf t}+1){\sf t}}(x\vert X',z) \\
&= \int_{\Omega_{{\sf t}-2}}{\rm d}X'''P_{{\sf t}-2}(X'''\vert z)\int_{\omega_{{\sf t}-1}}{\rm d}^nx''P_{({\sf t}-1)({\sf t}-2)}(x''\vert X''',z)\int_{\omega_{{\sf t}}}{\rm d}^nx' P_{{\sf t}({\sf t}-1)}(x'\vert X'',z)P_{({\sf t}+1){\sf t}}(x\vert X',z) \\
&= \dots \\
&= \int_{\Omega_{{\sf t}-{\sf s}}}{\rm d}X'''P_{{\sf t}-{\sf s}}(X'''\vert z)\prod_{{\sf s}'=0}^{{\sf s}-1} \bigg\lbrace \int_{\omega_{{\sf t}-{\sf s}'}}{\rm d}^nx' P_{({\sf t}-{\sf s}')({\sf t}-{\sf s}'-1)}(x'\vert X'',z) \bigg\rbrace P_{({\sf t}+1){\sf t}}(x\vert X',z) .
\end{aligned}
$$

Depending on the temporal correlation structure of the process, the conditional probabilities can be factorised. For example, processes with second or third-order temporal correlations would be described by the following expressions

$$
\begin{aligned}
P_{({\sf t}+1){\sf t}}(x\vert X',z) &= \frac{1}{{\sf t}}\sum_{{\sf t}'=0}^{{\sf t}}\int_{\omega_{{\sf t}'}}{\rm d}^nx' P_{{\sf t}'}(x'\vert z)P_{({\sf t}+1){\sf t}'}(x\vert x',z) \\
P_{({\sf t}+1){\sf t}}(x\vert X',z) &= \frac{1}{{\sf t}}\sum_{{\sf t}'=0}^{{\sf t}}\frac{1}{{\sf t}'}\sum_{{\sf t}'=0}^{{\sf t}'}\int_{\omega_{{\sf t}'}}{\rm d}^nx'\int_{\omega_{{\sf t}''}}{\rm d}^nx'' P_{{\sf t}''}(x''\vert z) P_{{\sf t}'{\sf t}''}(x'\vert x'',z)P_{({\sf t}+1){\sf t}'{\sf t}''}(x\vert x',x'',z) .
\end{aligned}
$$

Let's imagine that $x$ is just a scalar (as opposed to a row vector) for simplicity in the expressions. We can then discretise the 1D space over $x$ into separate $i$-labelled regions such that $[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} = {\cal J}^i_{{\sf t}+1}$, where the probability current ${\cal J}^i_{{\sf t}+1}$ for the factorised processes above would be defined as

$$
\begin{aligned}
{\cal J}^i_{{\sf t}+1} &= - [P]^i_{{\sf t}} + \frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\sum_{i'=0}^N\Delta x[P]^{i'}_{{\sf t}'}[P]^{ii'}_{({\sf t}+1){\sf t}'} \\
{\cal J}^i_{{\sf t}+1} &= - [P]^i_{{\sf t}} + \frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=1}\frac{1}{{\sf t}'-1}\sum^{{\sf t}'-1}_{{\sf t}''=0}\sum_{i'=0}^N\sum_{i''=0}^N\Delta x^2[P]^{i''}_{{\sf t}''}[P]^{i'i''}_{{\sf t}'{\sf t}''}[P]^{ii'i''}_{({\sf t}+1){\sf t}'{\sf t}''} .
\end{aligned}
$$

The $[P]^{ii'i''}_{({\sf t}+1){\sf t}'{\sf t}''}$ tensor, in particular, will have $N^3{\sf t}({\sf t}^2-1)$ elements. Note that the third-order temporal correlations can be evolved by identifying the pairwise conditional probabilities as time-dependent state variables and evolving them according to the following relation

$$
\begin{aligned}
[P]^{ii''}_{({\sf t}+1){\sf t}''} &= \frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=1}\sum_{i'=0}^N\Delta x[P]^{i'i''}_{{\sf t}'{\sf t}''}[P]^{ii'i''}_{({\sf t}+1){\sf t}'{\sf t}''} .
\end{aligned}
$$
